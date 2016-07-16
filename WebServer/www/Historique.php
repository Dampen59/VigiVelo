<?php

include ("Configuration.php"); // On inclus le fichier de configuration

$CodeInput = $_GET["uid"]; // www.site.fr/CheckCard.php?code=
$CodeInput = addslashes($CodeInput); // On protège la variable pour éviter des injections SQL

mysql_connect($Serveur,$Utilisateur,$Password) or die ("Erreur de connexion a MySql"); // Connexion au serveur MySql

mysql_select_db($Database) or die ("Impossible de se connecter a la BDD"); // Selection de la base de données

$VerifCardQuery = "SELECT DateHeure FROM historique WHERE UID = '$CodeInput'";
$ExecuteVerifCardQuery = mysql_query($VerifCardQuery) or die ("Impossible d'executer la requete SQL");

	while($row = mysql_fetch_assoc($ExecuteVerifCardQuery)) 
	{
		$rows[] = $row;
	}
	
	print(json_encode($rows, JSON_NUMERIC_CHECK));

?>