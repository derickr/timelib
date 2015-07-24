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

if (strpos($argv[0], 'test_diff') !== false)
{
    error_reporting(0);
    test_diff($argv[1]);
}

function test_diff($file)
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
        $test_parts = explode(': DIFF: ', $test);
        $test_name = trim($test_parts[0]);
        $test_dates_and_response = explode(' = ', $test_parts[1]);
        $test_response = trim($test_dates_and_response[1]);
        $test_dates = explode(' - ', $test_dates_and_response[0]);
        $test_start = trim($test_dates[0]);
        $test_end = trim($test_dates[1]);

        $process = proc_open("./tests/tester-diff \"$test_start\" \"$test_end\"", $descriptorspec, $pipes);

        // fwrite($pipes[0], $teste);
        fclose($pipes[0]);

        $result_line1 = explode('|', trim(fgets($pipes[1])));
        $result_line2 = explode('|', trim(fgets($pipes[1])));
        $result_diff = trim(fgets($pipes[1]));
        fclose($pipes[1]);

        $return_value = proc_close($process);

        $found_diff = convert_result_to_expected_diff($result_diff);
        
        if ($found_diff == $test_response) {
            echo $format['OKAY'] . "OKAY" . $format['NORM'];
            echo " | " . $test_start . " | " . $test_end . " | " . $found_diff;
            if ($return_value) {
                echo " errors=", $return_value;
            }
            echo "\n";
            $results['OKAY']++;
        } else {
            echo $format['FAIL'] . "FAIL";
            echo " | " . $test_start . " | " . $test_end . " | " . $found_diff . " (" . $result_diff . ")";
            if ($return_value) {
                echo " errors=", $return_value;
            }
            echo "\n";
            echo "EXP  = " . $test_response . "\n";
            echo $format['NORM'];
            $results['FAIL']++;
        }
    }
    
    return $results;
}

function convert_result_to_expected_diff($result)
{
    $expectedDiff = '';
    if (preg_match('/^([0-9]+)Y\s+([0-9]+)M\s+([0-9]+)D\s+\/\s+([0-9]+)H\s+([0-9]+)M\s+([0-9]+)S\s+\(days:\s([0-9]{1,})\)(\sinverted)?$/', $result, $matches)) {
        $expectedDiff = sprintf(
            '**P%s%dY%dM%dDT%dH%dM%dS**',
            (isset($matches[8]) && trim($matches[8]))?'+':'-',
            $matches[1],
            $matches[2],
            $matches[3],
            $matches[4],
            $matches[5],
            $matches[6]
        );
    }
    return $expectedDiff;
}

?>
