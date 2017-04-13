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

error_reporting(4095);

$global = array('OKAY' => 0, 'FAIL' => 0);
$format = array('OKAY' => '[32m', 'FAIL' => '[31m', 'NORM' => '[m');

$tests = array(
	'test_parser' => '.parse',
	'test_create' => '.ts',
	'test_render' => '.render',
	'test_render_zoneinfo' => '.render',
	'test_parse_format' => '.parseformat',
);

foreach ($tests as $test => $ext)
{
	echo "$test=>$ext\n";
	require_once("tests/{$test}.php");
	foreach (new DirectoryIterator(dirname(__FILE__) . '/files') as $it)
	{
		if (fnmatch("*$ext", $it->getFilename()))
		{
			echo $it->getFilename(), "\n";
			$result = $test($it->getPathname());
			$global['FAIL'] += $result['FAIL'];
			$global['OKAY'] += $result['OKAY'];
			echo "\n";
		}
	}

}
echo "\n";
echo "Tests executed: " . array_sum($global) . "\n";
echo "Tests failed:   " . $format['FAIL'] . $global['FAIL'] . $format['NORM'] . "\n";
echo "Tests passed:   " . $format['OKAY'] . $global['OKAY'] . $format['NORM'] . "\n";

?>
