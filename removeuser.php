<?php

    $userID = "";
    $streamName = "";

    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        /* if it was directed here from a button */
        if (isset($_POST["__USERID_B"]))
        {
            $userID = $_POST["__USERID_B"];
            $streamName = $_POST["__STREAM_B"];
        }

        /* check if the user is removing a user from stream list */
        if (isset($_POST["userID"]) && isset($_POST["streamList"]))
        {
            $newUser = $_POST["userID"];
            $streamList = $_POST["streamList"];
            exec("./removeauthor \"$newUser\" \"$streamList\"");
        }
    }
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="description" content="">
        <meta name="author" content="Marshall Asch">
        
        <!-- Title -->
        <title>CIS2750 | A3 </title>
    
        <!-- Include top style docs -->
        <link href="style.css" rel="stylesheet">      
    </head>
    
    <body>

        <?php 
            $array = array();
            exec("./genHTML removeUser.wpml", $array);
            foreach ($array as $value)
            {
                eval("?>".$value);
                echo "\n";
            }
            
            // Load the list of menu buttons that are on each page
            $array = array();
            exec("./genHTML menuButtons.wpml", $array);
            foreach ($array as $value)
            {
                eval("?>".$value);
                echo "\n";
            }
        ?>

        <!-- script for form submition and the dark button -->
        <script type="text/javascript" src="script.js"></script> 
        
    </body>
</html>