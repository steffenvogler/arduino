<?php
// werden keine Daten per "?TEMP=......" übergeben, wird der aktuellste Wert aus der Datenbank angezeigt
//

//Datenbank-Verbindung herstellen
//--------------------------------
include("db.php");

// GET mit Prüfung (durch Aufruf von "http://XXXXX.de/arduino_push_data.php?TEMP=21.90&key=XXXXXXXXX")
//-----------------
if ((isset($_GET['TEMP'])) and (($_GET['key']) == "test")) {	// Wenn 'TEMP' übergeben wurde und key stimmt...
	$TEMP = ($_GET['TEMP']);
	$HUMID = ($_GET['HUMID']);
	$LI = ($_GET['LI']);
	$VIBEV = ($_GET['VIBEV']);
	$VIBSCO = ($_GET['VIBSCO']);
	echo $TEMP;
	$eintragen = mysql_query("INSERT INTO arduino_log_exp (TEMP,HUMID,LI,VIBEV,VIBSCO,DATE)	VALUES ($TEMP,$HUMID,$LI,$VIBEV,$VIBSCO, NOW())");	// TEMP real übergeben, DATE = automatischer SQL-Befehl (NOW)
		
} else {

	$ergebnis = mysql_query("SELECT * FROM arduino_log_exp ORDER BY id DESC LIMIT 1");	 //nur letzten Datensatz
	while($row = mysql_fetch_object($ergebnis))
	{
		echo "Latest entry in database: <br><br>";
		echo "ID \t\t\t","<b>","<font color = 'red'>",$row->ID,"</b><br>";
		echo "<font color = 'black'>","Temp \t\t","<b>","<font color = 'red'>",$row->TEMP,"</b><br>";
		echo "<font color = 'black'>","Humid \t\t","<b>","<font color = 'red'>",$row->HUMID,"</b><br>";
		echo "<font color = 'black'>","Light-Intensity (approx 140 a.u. is DAY and >700 is NIGHT)  \t\t","<b>","<font color = 'red'>",$row->LI,"</b><br>";
		echo "<font color = 'black'>","Vibration Events per Time Intervall (30min currently)  \t\t","<b>","<font color = 'red'>",$row->VIBEV,"</b><br>";
		echo "<font color = 'black'>","Vibration Score (arbitary unit; high score means either few large vibrations OR many small)  \t\t","<b>","<font color = 'red'>",$row->VIBSCO,"</b><br>";
		echo "<font color = 'black'>","Datum / Uhrzeit \t","<b>","<font color = 'red'>",$row->DATE,"</b><br>";
		
	}
}

?>