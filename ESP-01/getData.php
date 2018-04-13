<?php

// It reads a json formatted text file and outputs it.

    echo' {
        "cols": [
        {"label":"Time","type":"string"},
        {"label":"Temperature C","type":"number"}
        ],
        "rows": [
    ';
    
    $filename = "log_001.csv";
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
