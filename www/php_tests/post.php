<h1>Post Inspector</h1>

<h2>Form</h2>

<form method="POST" action="">
	<input name="Sacred Text" type="text" value="Some random text" placeholder="Type here"/>
	<br/>
	<label for="Sacred Color">A pretty color: </label> <input name="Sacred Color" type="color" value="#ffaadd"/>
	<br/>
	<textarea name="Prophecy of Legends">Seul link pourra vaincre Ganon.</textarea>
	<input type="submit" value="Post"/>
</form>


<h2>Result</h2>

<?php
	var_dump($_POST);
?>