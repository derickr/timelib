<?php
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Derick Rethans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

if (strpos($argv[0], 'test_parse_format') !== false)
{
	error_reporting(0);
	test_parse_format($argv[1]);
}

$c = 0;

function test_parse_format($file)
{
	$tests = file($file);
	$base = pathinfo($file, PATHINFO_FILENAME);

	$descriptorspec = array(
		0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
		1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
		2 => array("file", "/tmp/error-output.txt", "w") // stderr is a file to write to
	);

	$results = array('OKAY' => 0, 'FAIL' => 0);
	$format = array('OKAY' => '[32m', 'FAIL' => '[31m', 'NORM' => '[m');

	foreach ($tests as $test)
	{
		if (trim( $test) == '') {
			continue;
		}
		$abbr = $offset = $isDst = $tzid = false;
		$relY = $relM = $relD = $relH = $relI = $relS = $relDOW = $relDT = false;
		$test_parts = explode('|', $test);
		$testName = sprintf("%s_%02d", strtr($base, '-', '_'), $c);
		$c++;
		//preg_match("@^TS:.*\| (-?\d+)-(-?\d+)-(-?\d+) 0?(.*):0?(.*):0?([^ ]*)( 0.(\d+))?( ([A-Z][A-Za-z_/]+[a-z]))?( ([A-Z]+) ([0-9-]+|UTC))?( \(DST\))?(\s+(\d)Y\s+(\d)M\s+(\d)D /\s+(\d)H\s+(\d)M\s+(\d)S / (\d)\.(\d))?\s+\| (.*) \| (.*)@", $test, $match);
		$format = trim($match[24]);
		$input  = trim($match[25]);

		$res = preg_match(
			"@^TS:.*\| " .
			"(-?\d+)-(-?\d+)-(-?\d+) 0?(.*):0?(.*):0?([^ ]*)" .
			"( 0.(\d+))?" .
			"( ([A-Z][A-Za-z_/-]+[a-z]))?" .
			"( ([A-Z]+) ([0-9-]+|UTC))?( \(DST\))?" .
			"(\s+([0-9-]+)Y\s+([0-9-]+)M\s+([0-9-]+)D /\s+([0-9-]+)H\s+([0-9-]+)M\s+([0-9-]+)S( / ([0-9-]+)\.(\d))?)?" .
			"\s+\| (.*)@", $test, $match);

		if (!$res) {
			echo "/*\n * {$base}:\n * {$test} */\n\n";
			continue;
		}

		$input  = trim($match[25]);

		$y = sprintf('%4d', $match[1]);
		$m = sprintf('%2d', $match[2]);
		$d = sprintf('%2d', $match[3]);
		$h = sprintf('%2d', $match[4]);
		$i = sprintf('%2d', $match[5]);
		$s = sprintf('%2d', $match[6]);
		$us = isset( $match[7] ) && strlen($match[7]) ? sprintf('%06d', $match[8]) : false;
		if (isset( $match[9] ) && strlen($match[9])) {
			$tzid = trim( $match[9] );
		}
		if (isset( $match[11] ) && strlen($match[11])) {
			$abbr = trim( $match[12] );
			$offset = ltrim( $match[13], ' 0' );
		}
		if (isset( $match[14] ) && strlen($match[14]) )
		{
			$isDst = true;
		}
		if (isset( $match[15] ) && strlen($match[15]) )
		{
			$relY = $match[16];
			$relM = $match[17];
			$relD = $match[18];
			$relH = $match[19];
			$relI = $match[20];
			$relS = $match[21];
			$relDOW = $match[23];
			$relDT  = $match[24];
		}

		//$output = "TEST(parse_date_from_format, {$testName})\n{\n";
		//$output .= "\ttest_parse(\"{$input}\", \"{$format}\");\n";
		$output = "TEST(parse_date, {$testName})\n{\n";
		$output .= "\ttest_parse(\"{$input}\");\n";

		if ( $y != '-99999' || $m != '-99999' || $d != '-99999' )
		{
			$output .= <<<ENDR
	LONGS_EQUAL({$y}, t->y);
	LONGS_EQUAL({$m}, t->m);
	LONGS_EQUAL({$d}, t->d);

ENDR;
		}

		if ( $h != '-99999' || $i != '-99999' || $s != '-99999' )
		{
			$output .= <<<ENDR
	LONGS_EQUAL({$h}, t->h);
	LONGS_EQUAL({$i}, t->i);
	LONGS_EQUAL({$s}, t->s);

ENDR;
		}

		if ( !!$us )
		{
			$output .= "\tLONGS_EQUAL({$us}, t->us);\n";
		}
		if ( !!$abbr && $abbr != 'GMT' )
		{
			$output .= "\tSTRCMP_EQUAL(\"{$abbr}\", t->tz_abbr);\n";
		}
		if ( !!$offset )
		{
			$output .= sprintf("\tLONGS_EQUAL(%d, t->z);\n", $offset);
		}
		if ( $isDst )
		{
			$output .= sprintf("\tLONGS_EQUAL(1, t->dst);\n");
		}
		if ( !!$tzid )
		{
			$output .= "\tSTRCMP_EQUAL(\"{$tzid}\", t->tz_info->name);\n";
		}

		if ($relY || $relM || $relD || $relH || $relI || $relS)
		{
			$output .= <<<ENDR
	LONGS_EQUAL({$relY}, t->relative.y);
	LONGS_EQUAL({$relM}, t->relative.m);
	LONGS_EQUAL({$relD}, t->relative.d);
	LONGS_EQUAL({$relH}, t->relative.h);
	LONGS_EQUAL({$relI}, t->relative.i);
	LONGS_EQUAL({$relS}, t->relative.s);

ENDR;
		}

		if ($relDOW || $relDT)
		{
			$output .= <<<ENDR
	LONGS_EQUAL({$relDOW}, t->relative.weekday);
	LONGS_EQUAL({$relDT}, t->relative.weekday_behavior);

ENDR;
		}

		echo str_replace("-99999", "TIMELIB_UNSET", $output), "}\n\n";
		continue;

		$testform = trim($test_parts[2]);
		$teste = trim($test_parts[3]);
		$ts_exp = trim($test_parts[0]);
		$ps_exp = trim($test_parts[1]);
		$process = proc_open("./tests/tester-parse-string-by-format \"$testform\" \"$teste\"", $descriptorspec, $pipes);

		fwrite($pipes[0], $teste);
		fclose($pipes[0]);

		$result = explode('|', trim(fgets($pipes[1])));
		fclose($pipes[1]);

		$return_value = proc_close($process);

		$ts_res = trim($result[0]);
		$ps_res = trim($result[1]);

		if ($ps_exp == $ps_res) {
			echo $format['OKAY'] . "OKAY" . $format['NORM'];
			echo " | " . $ps_res . " | " . $testform . " | " . $teste;
			if ($return_value) {
				echo " errors=", $return_value;
			}
			echo "\n";
			$results['OKAY']++;
		} else {
			echo $format['FAIL'] . "FAIL";
			echo " | " . $ps_res . " | " . $testform . " | " . $teste;
			if ($return_value) {
				echo " errors=", $return_value;
			}
			echo "\n";
			echo "EXP  = " . $ps_exp . "\n";
			echo $format['NORM'];
			$results['FAIL']++;
		}

	}

	return $results;
}

?>
