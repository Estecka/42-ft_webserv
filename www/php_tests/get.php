<h1>GET Inspector</h1>


<form method="GET" action="">
	<label for="name">Name:</label>
	<input type="text" placeholder="Type here" id="name" name="name" value="John"/>
	<br/>
	<label for="surname">Surname:</label>
	<input type="text" placeholder="Type here" id="surname" name="surname" value="Smith"/>
	<br/>
	<label for="profession">Profession:</label>
	<input type="text" placeholder="Type here" id="profession" name="profession" value="Clone"/>
	<br/>
	<input type="submit" value="Get"/>
</form>

<?php
	var_dump($_GET);
?>