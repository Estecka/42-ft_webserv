<?php
$timeout = 30;
if (isset($_GET['timeout']) && is_numeric($_GET['timeout']))
	$timeout = $_GET['timeout'];

sleep($timeout);

echo "<h1>It's time to waste someone else's time.</h1>";
echo "<h2>(But they won't know about it.)</h2>";

echo "I am contempt with my mischief</br>";
exit;

?>
