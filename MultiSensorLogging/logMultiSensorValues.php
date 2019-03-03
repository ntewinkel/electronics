<?php
//
//  Stores a sensor's values to a log file, in CSV format,
//  with a timestamp
//
// This script looks for a single sensorID with multiple values in json format in a "values" parameter
// example usage:
// http://localhost/sensors/logMultiSensorValues?sensorID=007&values={%22inside%20temp%22:22.5,%22outside%20temp%22:15.1}
//
// note the url-encoding.
//	%22 = "
//	%20 = space
//


// Get parameters from URL
$sensorID = $_GET["sensorID"];

// only log for sensors starting with 00, to avoid abuse
if (strlen($sensorID) != 3 || substr( $sensorID, 0, 2 ) != "00") {
	die("Invalid sensor ID for logging.");
}

// values is a chunk of data
$sensorValuesStr = $_GET["values"];
$sensorValues = json_decode($sensorValuesStr, true);
$titles = array_keys($sensorValues);

date_default_timezone_set("America/Vancouver");
$timestamp = date("Y-m-d h:i:sa");

$filename = "log_" . $sensorID . ".csv";

if (!file_exists($filename)) {
    $logfile = fopen($filename, "a") or die("Unable to open file for logging.");
    $update = "time," . join(",",$titles) . "\n";
    fwrite($logfile, $update);
    fclose($logfile);
}

$logfile = fopen($filename, "a") or die("Unable to open file for logging");
$update = $timestamp;

for($x = 0; $x < count($titles); $x++) {
    $update = $update . "," . $sensorValues[$titles[$x]];
}

$update = $update . "\n";

fwrite($logfile, $update);
fclose($logfile);
    
echo "success.";

?>
