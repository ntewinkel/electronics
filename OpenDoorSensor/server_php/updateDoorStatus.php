<?php
//
//  Store a door status to a file, in JSON format
//  Also keeps a timestamp of when the last update was made.
//  Allows for multiple doors to be monitored
//
//  Example of file content:
//  {"001":{"status":"OPEN","timestamp":"2018-02-16 10:54:42pm","opentime":"2:35"}
//   "002":{"status":"CLOSED","timestamp":"2018-02-16 10:54:06pm","opentime":"1:58"}}
//
//

$fileName = "doorstatus.json";

// Get parameters from URL
$secret = $_GET["secret"];

if ($secret != "somethingsecret") {  // this is very basic security. use your own secret password here
   die("Yup.");
}

$doorID = $_GET["doorID"];
$doorStatus = $_GET["status"];
$opentime = $_GET["opentime"];  // opentime is how long the door was most recently open for

date_default_timezone_set("America/Vancouver");
$timestamp = date("Y-m-d h:i:sa");

$statusUpdate["timestamp"] = $timestamp;
$statusUpdate["doorStatus"] = $doorStatus;
$statusUpdate["opentime"] = $opentime;

// step 1: update status file

if (!file_exists($fileName)) {
  $statusfile = fopen($fileName, "a") or die("Unable to open file to record status");
  fclose($statusfile);
}

$currentValuesStr = file_get_contents($fileName);

$jsonValues = json_decode($currentValuesStr, true);
$jsonValues[$doorID] = $statusUpdate;

$updatedValuesStr = json_encode($jsonValues);

file_put_contents($fileName, $updatedValuesStr);

// step 2: add to log file
$logfileName = "log_door" . $doorID . ".csv";
$needs_header = !file_exists($logfileName);

$logfile = fopen($logfileName, "a") or die("Unable to open file for logging");

if ($needs_header) {
  $update = "Time Stamp,Door Status,Open Time\n";
  fwrite($logfile, $update);
}

$update = $timestamp . "," . $doorStatus . "," . $opentime . "\n";
fwrite($logfile, $update);
fclose($logfile);


// send text message via email
$to = "2505551234@txt.bell.ca";  // use your own email-to-text address here, or just straight email it.
$from = "doorstatus@yourserver.com";  // use any "from" address here
$headers = "From:" . $from;

if ($doorStatus == "OPEN" && $opentime != "") {
	// if OPEN and opentime > 0, this means the door has been open too long, which is an alert situation
	$subject = "Warning";
	$message = "The door has been OPEN for " . $opentime . "\n";
	echo "Alert email sent<br>";
}
else {
	$subject = "Info";
	$message = "The door is now " . $doorStatus . "\n";
	if ($opentime != "") {
		$message .= "It was open for " . $opentime . "\n";
	}
	echo "Info email sent<br>";
}

$message .= "See the log file: http://www.yourserver.com/yourdoorstatusfolder/doorLogSummary.php\n";  // keep your doorstatus folder secret too

mail($to,$subject,$message,$headers);

echo "success";

?>
