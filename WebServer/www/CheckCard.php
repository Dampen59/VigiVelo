<?php

include ("Configuration.php"); // On inclus le fichier de configuration

date_default_timezone_set("Europe/Paris");

$CodeInput = $_GET["code"]; // www.site.fr/CheckCard.php?code=
$CodeInput = addslashes($CodeInput); // On protège la variable pour éviter des injections SQL

mysql_connect($Serveur,$Utilisateur,$Password) or die ("Erreur de connexion a MySql"); // Connexion au serveur MySql

mysql_select_db($Database) or die ("Impossible de se connecter a la BDD"); // Selection de la base de données

$VerifCardQuery = "SELECT * FROM cartes WHERE UID = '$CodeInput'";
$ExecuteVerifCardQuery = mysql_query($VerifCardQuery) or die ("Impossible d'executer la requete SQL");


if (mysql_num_rows($ExecuteVerifCardQuery) > 0) {


while ($row = mysql_fetch_array($ExecuteVerifCardQuery)) {
   
   if ($row['isEnabled'] == 1) {
	   echo "{CARTE OK}";
	   	$dateheure = date("d-m-Y H:i:s");
		$AddHisto = "INSERT INTO historique (UID, DateHeure) VALUES ('$CodeInput','$dateheure')";
		$ExecuteHisto = mysql_query($AddHisto) or die ("Impossible d'ajouter a l'historique");
   } else {
	   echo "{CARTE NON}";
   }
}

} else {
	echo "{CARTE NON}";
}

?>