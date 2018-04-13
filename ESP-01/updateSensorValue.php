<?php
//
//  Store a sensor value to a file, in JSON format
//  Also keeps a timestamp of when the last update was made.
//
//  Example of file content:
//  {"001":{"value":"17","timestamp":"2018-02-16 10:54:42pm"}
//   "002":{"value":"77","timestamp":"2018-02-16 10:54:06pm"}}
//
//

$fileName = "sensorValues.json";

// Get parameters from URL
$sensorID = $_GET["sensorID"];
$sensorValue = $_GET["value"];

date_default_timezone_set("America/Vancouver");
$timestamp = date("Y-m-d h:i:sa");

$sensorUpdate["value"] = $sensorValue;
    $sensorUpdate["timestamp"] = $timestamp;

$currentValuesStr = file_get_contents($fileName);

$jsonValues = json_decode($currentValuesStr, true);
$jsonValues[$sensorID] = $sensorUpdate;

$updatedValuesStr = json_encode($jsonValues);

file_put_contents($fileName, $updatedValuesStr);

if (strlen($sensorID) == 3 && substr( $sensorID, 0, 2 ) === "00") {
    // only log for sensors starting with 00, to avoid abuse
    $logfile = fopen("log_" . $sensorID . ".csv", "a") or die("Unable to open file for logging");
    $update = $timestamp . "," . $sensorValue . "\n";
    fwrite($logfile, $update);
    fclose($logfile);
}
    
echo "success";

?>
