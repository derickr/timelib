<?php

error_reporting(4095);

$global = array('OKAY' => 0, 'FAIL' => 0);
$format = array('OKAY' => '[32m', 'FAIL' => '[31m', 'NORM' => '[m');

$tests = array(
	'test_parser' => '.parse',
	'test_create' => '.ts',
	'test_render' => '.render',
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
