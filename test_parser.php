<?php

if (strpos($argv[0], 'test_parser') !== false)
{
	error_reporting(0);
	test_parser($argv[1]);
}

function test_parser($file)
{
	$tests = file($file);

	$descriptorspec = array(
		0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
		1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
		2 => array("file", "/tmp/error-output.txt", "w") // stderr is a file to write to
	);
	
	$results = array('OKAY' => 0, 'FAIL' => 0);
	$format = array('OKAY' => '[32m', 'FAIL' => '[31m', 'NORM' => '[m');

	foreach ($tests as $test)
	{
		$test_parts = explode('|', $test);
		$teste = trim($test_parts[2]);
		$ts_exp = trim($test_parts[0]);
		$ps_exp = trim($test_parts[1]);
		$process = proc_open("./tester-parse-string \"$teste\"", $descriptorspec, $pipes);

		fwrite($pipes[0], $teste);
		fclose($pipes[0]);

		$result = explode('|', trim(fgets($pipes[1])));
		fclose($pipes[1]);

		$return_value = proc_close($process);

		$ts_res = trim($result[0]);
		$ps_res = trim($result[1]);
		
		if ($ps_exp == $ps_res) {
			echo $format['OKAY'] . "OKAY" . $format['NORM'];
			echo " | " . $ps_res . " | " . $teste;
			if ($return_value) {
				echo " errors=", $return_value;
			}
			echo "\n";
			$results['OKAY']++;
		} else {
			echo $format['FAIL'] . "FAIL";
			echo " | " . $ps_res . " | " . $teste;
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
