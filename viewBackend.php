<?php

if ($_SERVER["REQUEST_METHOD"] == "POST" )
{
	$userID = $_POST["userID"];
    $streamName = $_POST["streamName"];

    if ($_POST["type"] == "load")
    {
	    exec("./view.py \"$userID\" \"$streamName\"", $posts);

	    foreach ($posts as $value)
	    {
	        echo $value;
	    }
	}
	else if ($_POST["type"] == "mark1")
    {
	    exec("./markOne.py \"$userID\" \"$streamName\"", $data);

	    foreach ($data as $value)
	    {
	        echo $value;
	    }
	}
	else if ($_POST["type"] == "markAll")
    {
	    exec("./markAll.py \"$userID\" \"$streamName\"", $data);

	    foreach ($data as $value)
	    {
	        echo $value;
	    }
	}
	else
	{
		echo "{\"data\": \"ERROR, unknown option\"}";
	}

}