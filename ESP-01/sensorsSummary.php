<?php
//
//  Show summary of store sensor information
//

$fileName = "sensorValues.json";
    
$currentValuesStr = file_get_contents($fileName);
    
$jsonValues = json_decode($currentValuesStr, true);
    
$sensor1 = $jsonValues["001"];
$sensor2 = $jsonValues["002"];

echo "
<!DOCTYPE html>
<html>

<head>
<title>Summary of Sensors</title>
<style>
    table, th, td {
    border: 1px solid black;
    }
    th, td {
    padding: 15px;
    }
</style>
<meta http-equiv=\"refresh\" content=\"2\">
</head>
<body>
<br><br>
Here is a current summary of the sensors:<br><br>
<table>
<tr>
<th>Sensor</th>
<th>Value</th>
<th>Timestamp</th>
</tr>
";
    
echo "<tr><td align=\"center\">001</td><td align=\"center\">" . $sensor1["value"] . "</td><td align=\"center\">" . $sensor1["timestamp"] . "</td></tr>";
echo "<tr><td align=\"center\">002</td><td align=\"center\">" . $sensor2["value"] . "</td><td align=\"center\">" . $sensor2["timestamp"] . "</td></tr>";

echo "
</table>
</body>
</html>
";
?>
