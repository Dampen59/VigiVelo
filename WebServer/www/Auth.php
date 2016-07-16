<?php

include ("Configuration.php"); // On inclus le fichier de configuration

$Usernamee = $_GET["user"]; // www.site.fr/CheckCard.php?code=
$UsernameInput = addslashes($Usernamee); // On protège la variable pour éviter des injections SQL

$Passworde = $_GET["pass"]; // www.site.fr/CheckCard.php?code=
$PasswordInput = addslashes($Passworde); // On protège la variable pour éviter des injections SQL

mysql_connect($Serveur,$Utilisateur,$Password) or die ("Erreur de connexion a MySql"); // Connexion au serveur MySql

mysql_select_db($Database) or die ("Impossible de se connecter a la BDD"); // Selection de la base de données

$VerifCardQuery = "SELECT * FROM users WHERE username = '$UsernameInput' AND password = '$PasswordInput'";
$ExecuteVerifCardQuery = mysql_query($VerifCardQuery) or die ("Impossible d'executer la requete SQL");


if (mysql_num_rows($ExecuteVerifCardQuery) > 0) {

	echo ("OK");
	
} else {
	echo "NON";
}

?>