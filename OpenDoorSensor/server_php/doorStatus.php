<?php
//
//  Show summary of stored door sensor information
//

$fileName = "doorstatus.json";
    
$currentValuesStr = file_get_contents($fileName);
    
$jsonValues = json_decode($currentValuesStr, true);

$doorID = $_GET["doorID"];

if (!$doorID) {
	$doorID = "1";
}

$doorValues = $jsonValues[$doorID];

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
<h3>Here is the last status update for door " . $doorID . ":<br><br></h3>
<table>
<tr>
<th>Time Stamp</th>
<th>Door Status</th>
<th>Open Time</th>
</tr>
";
    
echo "<tr><td align=\"center\">" . $doorValues["timestamp"] . "</td><td align=\"center\">" . $doorValues["doorStatus"] . "</td><td align=\"center\">" . $doorValues["opentime"] . "</td></tr>";

echo "
</table>
</body>
</html>
";
?>
