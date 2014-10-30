/* This is a generated file, do not modify */
<?php
	$index = include 'timezonedb.idx.php';
	$dta = file_get_contents('timezonedb.dta');
	$dta_l = strlen($dta);
	$j = 0;

	echo "const unsigned char timelib_timezone_db_data_builtin[$dta_l] = {\n";
	for ($i = 0; $i < $dta_l; $i++) {
		if (($key = array_search($i, $index)) !== false) {
			echo "\n\n/* $key */\n";
			$j = 0;
		}
		printf("0x%02X, ", ord($dta[$i]));
		if ($j % 16 == 15) {
			echo "\n";
		}
		$j++;
	}
	echo "};\n";
?>
