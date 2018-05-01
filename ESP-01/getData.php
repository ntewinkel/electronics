<?php

// It reads a json formatted text file and outputs it.

$sensorID = $_GET["sensorID"];

echo' {
    "cols": [
    {"label":"Time","type":"string"},
    {"label":"Sensor ' . $sensorID . '","type":"number"}
    ],
    "rows": [
';

$filename = "log_" . $sensorID . ".csv";
$handle = fopen($filename, "r");
if ($handle) {
    while (($line = fgets($handle)) !== false) {
        // process the line read.
        $columns = explode(",", $line);
        echo '{"c":[{"v":"' . $columns[0] . '"},{"v":' . $columns[1] . '}]},';
    }
    
    fclose($handle);
} else {
    // error opening the file.
    echo "<tr><td align=\"center\">error reading log file.</td>";
}

echo ']}';

?>
