<?php

// It reads a json formatted text file and outputs it.

$sensorID = $_GET["sensorID"];

$filename = "log_" . $sensorID . ".csv";
$handle = fopen($filename, "r");

if ($handle) {
    echo'{"cols": [';

    // get the titles row
    if (($line1 = fgets($handle)) !== false) {
        $line1 = str_replace(array("\n", "\r"), '', $line1);
        $columnTitles = explode(",", $line1);
        echo '{"label":"'. $columnTitles[0] .'","type":"string"},';

        for($x = 1; $x < count($columnTitles); $x++) {
            echo '{"label":"' . $columnTitles[$x] . '","type":"number"},';
        }
    }

    echo '],';
    echo'"rows": [';

    while (($line = fgets($handle)) !== false) {
        $line = str_replace(array("\n", "\r"), '', $line);
        $columns = explode(",", $line);
        echo '{"c":[';

        for($x = 0; $x < count($columns); $x++) {
            echo '{"v":"' . $columns[$x] . '"},';
        }

        echo ']},';
    }
    
    fclose($handle);

    echo ']}';
}
else {
    // error opening the file.
    echo "<tr><td align=\"center\">error reading log file.</td>";
}

?>
