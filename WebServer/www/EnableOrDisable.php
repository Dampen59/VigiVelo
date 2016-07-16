<?php

include ("Configuration.php"); // On inclus le fichier de configuration

$UIDInput = $_GET["uid"]; // www.site.fr/CheckCard.php?code=
$UIDInput = addslashes($UIDInput); // On protège la variable pour éviter des injections SQL

$BoolInput = $_GET["disable"]; // www.site.fr/CheckCard.php?code=
$BoolInput = addslashes($BoolInput); // On protège la variable pour éviter des injections SQL

mysql_connect($Serveur,$Utilisateur,$Password) or die ("Erreur de connexion a MySql"); // Connexion au serveur MySql

mysql_select_db($Database) or die ("Impossible de se connecter a la BDD"); // Selection de la base de données

$VerifCardQuery = "SELECT * FROM cartes WHERE UID = '$UIDInput'";
$ExecuteVerifCardQuery = mysql_query($VerifCardQuery) or die ("Impossible d'executer la requete SQL");


if (mysql_num_rows($ExecuteVerifCardQuery) > 0) {

while ($row = mysql_fetch_array($ExecuteVerifCardQuery)) {
   
if ($BoolInput == 0) {
	$ActivateCardQuery = "UPDATE cartes SET isEnabled = 1 WHERE UID = '$UIDInput'";
	$ExecActivateCardQuery = mysql_query($ActivateCardQuery) or die ("Erreur activation");
} else {
	$DesactivateCardQuery = "UPDATE cartes SET isEnabled = 0 WHERE UID = '$UIDInput'";
	$ExecDesactivateCardQuery = mysql_query($DesactivateCardQuery) or die ("Erreur desactivation");
}
}

} else {
	echo "NOT-FOUND";
}

?>