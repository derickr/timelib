<?php

if (strpos($argv[0], 'test_create') !== false)
{
	error_reporting(0);
	test_create($argv[1]);
}

function test_create($file)
{
	$tests = file($file);
	unset($tests[0]);

	$descriptorspec = array(
		0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
		1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
		2 => array("file", "/tmp/error-output.txt", "w") // stderr is a file to write to
	);
	
	$results = array('OKAY' => 0, 'FAIL' => 0);
	$format = array('OKAY' => '[32m', 'FAIL' => '[31m', 'NORM' => '[m');

	foreach ($tests as $test)
	{
		if (trim($test) === '') {
			echo "\n";
			continue;
		}
		$test_parts = explode('|', $test);
		$teste = trim($test_parts[1]);
		$ts_exp = trim($test_parts[0]);
		$base = trim($test_parts[2]);
		$tz = trim($test_parts[3]);
		$process = proc_open("./tests/tester-create-ts \"$teste\" \"$base\" \"$tz\"", $descriptorspec, $pipes);

		fwrite($pipes[0], $teste);
		fclose($pipes[0]);

		$result = explode('|', trim(fgets($pipes[1])));
		$ts_res = trim($result[0]);
		$ps_res = trim($result[1]);
		
		$teste .= " ( $base, $tz)\n";
		if ($ts_exp == $ts_res) {
			echo $format['OKAY'] . "OKAY" . $format['NORM'];
			echo " | " . $ts_res . " | " . $teste;
			$results['OKAY']++;
		} else {
			echo $format['FAIL'] . "FAIL";
			echo " | " . $ts_res . " | " . $teste;
			echo "EXP  = " . $ts_exp, "\n";
			echo $format['NORM'];
			$results['FAIL']++;
		}
		fclose($pipes[1]);

		$return_value = proc_close($process);
	}
	
	return $results;
}

?>
