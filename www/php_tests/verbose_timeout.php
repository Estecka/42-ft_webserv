<?php

header("Status: 200 OK");
echo "<h1>It's time to waste someone else's time.</h1>";
flush();

$timeout = 30;
if (isset($_GET['timeout']) && is_numeric($_GET['timeout']))
	$timeout = $_GET['timeout'];

for ($i=0; $i<$timeout; $i++){
	echo "The time is : ".date("H\h i\m s\s")."<br/>";
	flush();
	sleep(1);
}

echo "I am contempt with my mischief</br>";
exit;

?>