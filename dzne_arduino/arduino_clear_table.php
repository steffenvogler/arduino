<?php
// werden keine Daten per "?TEMP=......" �bergeben, wird der aktuellste Wert aus der Datenbank angezeigt
//

//Datenbank-Verbindung herstellen
//--------------------------------
include("db.php");

// GET mit Pr�fung (durch Aufruf von "http://XXXXX.de/arduino_push_data.php?TEMP=21.90&key=XXXXXXXXX")
//-----------------
mysql_query( "TRUNCATE TABLE arduino_log_exp" );
?>