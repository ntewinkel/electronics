<?php
//
//  Show summary of csv file for a door sensor
//

$doorID = $_GET["doorID"];

if (!$doorID) {
	$doorID = "1";
}

$logfileName = "log_door" . $doorID . ".csv";

echo "
<!DOCTYPE html>
<html>

<head>
<title>Door Status For Door " . $doorID . " </title>
<style>
    table, th, td {
    border: 1px solid black;
    }
    th, td {
    padding: 15px;
    }
</style>
</head>
<body>
<br>
<h3>Here is the status log for door " . $doorID . ":<br><br></h3>
<table>";

$f = fopen($logfileName, "r");

$headerline = true;

while (($line = fgetcsv($f)) !== false) {
        echo "<tr>";
        foreach ($line as $cell) {
        	    echo $headerline ? "<th>" : "<td>";
                echo htmlspecialchars($cell);
                echo $headerline ? "</th>" : "</td>";
        }
        echo "</tr>\n";
        $headerline = false;
}

fclose($f);

echo "\n</table></body></html>";

?>
