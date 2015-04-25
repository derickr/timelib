<?php
$zone = $argv[1];
$dataf = 'code/data/'. $zone;

// obtain data from tz file
$fdata = file_get_contents( $dataf, false, NULL, 20 );


// process extra info
$f = file( 'code/zone.tab' );

$cc = '??';
$lat = $long = 0;
$desc = '';

foreach ( $f as $line )
{
	$line = trim( $line );
	if ( strlen( $line ) < 5 || $line[0] == '#' )
	{
		continue;
	}
	$parts = explode( "\t", $line );
	if ( $parts[2] == $zone )
	{
		// format lang/lat
		if ( strlen( $parts[1] ) == 11 )
		{
			sscanf( $parts[1], '%c%2d%2d%c%3d%2d', $xSign, $xH, $xM, $ySign, $yH, $yM );
			$xS = $yS = 0;
		}
		else
		{
			sscanf( $parts[1], '%c%2d%2d%2d%c%3d%2d%2d', $xSign, $xH, $xM, $xS, $ySign, $yH, $yM, $yS );
		}
		$lat = $xH + ( $xM / 60 ) + ( $xS / 3600 );
		$long = $yH + ( $yM / 60 ) + ( $yS / 3600 );
		$lat = $xSign == '+' ? $lat : -$lat;
		$long = $ySign == '+' ? $long : -$long;

		$cc   = $parts[0];
		$desc = isset( $parts[3] ) ? $parts[3] : '';

		break;
	}
}
//printf( '{ "%2s", %d, %d, "%s" },' . "\n",
//	$cc, $lat * 100000, $long * 100000, addslashes( $desc ) );
echo pack( 'a4ca2a13a*NNNa*', "PHP2", $cc != '??' || $zone == 'UTC', $cc, '', $fdata, ($lat + 90) * 100000, ($long + 180) * 100000, strlen( $desc ), $desc );
?>
