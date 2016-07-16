<?php

include ("Configuration.php"); // On inclus le fichier de configuration

$NomInput = $_GET["nom"]; // www.site.fr/CheckCard.php?code=
$NomInput = addslashes($NomInput); // On protège la variable pour éviter des injections SQL

$PrenomInput = $_GET["prenom"]; // www.site.fr/CheckCard.php?code=
$PrenomInput = addslashes($PrenomInput); // On protège la variable pour éviter des injections SQL

mysql_connect($Serveur,$Utilisateur,$Password) or die ("Erreur de connexion a MySql"); // Connexion au serveur MySql

mysql_select_db($Database) or die ("Impossible de se connecter a la BDD"); // Selection de la base de données

$VerifCardQuery = "SELECT * FROM cartes WHERE Nom = '$NomInput' AND Prenom = '$PrenomInput'";
$ExecuteVerifCardQuery = mysql_query($VerifCardQuery) or die ("Impossible d'executer la requete SQL");


if (mysql_num_rows($ExecuteVerifCardQuery) > 0) {


while ($row = mysql_fetch_array($ExecuteVerifCardQuery)) {
   
echo $row['UID'];
}
} else {
	echo "NOT-FOUND";
}

?>