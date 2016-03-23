<?php
// Create connection
//mysql_connect("localhost","root","root") or die ("Keine Verbindung moeglich");
//mysql_select_db("logging") or die ("Die Datenbank existiert nicht.");
mysql_connect("ygg2dbnew","dznevm1","aigait4HShe0eitu") or die ("Keine Verbindung moeglich");
//mysql_connect("dznevm1","ygg2dbnew","aigait4HShe0eitu") or die ("Keine Verbindung moeglich");
mysql_select_db("dzne_arduino_logging") or die ("Die Datenbank existiert nicht.");
$sql = "SELECT DATE, TEMP, HUMID, LI, VIBEV, VIBSCO FROM arduino_log_exp ORDER BY UNIX_TIMESTAMP(DATE)";
$query = mysql_query($sql)OR die(mysql_error()); 
while($row = mysql_fetch_array($query)) {
	$result_TEMP[] = array(strtotime($row['DATE'])*1000,$row['TEMP'],);
	$result_HUMID[] = array(strtotime($row['DATE'])*1000,$row['HUMID']);
	$result_LI[] = array(strtotime($row['DATE'])*1000,$row['LI']*10);
	$result_VIBEV[] = array(strtotime($row['DATE'])*1000,$row['VIBEV']/100);
	$result_VIBSCO[] = array(strtotime($row['DATE'])*1000,$row['VIBSCO']/100);
	}
//print_r($result); 
$all_in[] = array($result_TEMP,$result_HUMID, $result_LI, $result_VIBEV, $result_VIBSCO); 
echo json_encode($all_in, JSON_NUMERIC_CHECK);

	