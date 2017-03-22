<?php

    $userID = "";
    $streamName = "";

    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check to see if it is a login 
        if (isset($_POST["userID"]))
        {
            $userID = $_POST["userID"];

            exec("./auth \"$userID\" \"\"", $streamList);
        }

        /* if it was directed here from a button */
        if (isset($_POST["__USERID_B"]))
        {
            $userID = $_POST["__USERID_B"];
            $streamName = $_POST["__STREAM_B"];

            $streamList = array();
            exec("./auth \"$userID\" \"\"", $streamList);
            /* make sure it is valid */
        }
    }
    else
    {
        /* if username not given then redirect to login page */
        header("Location: login.php");
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
            exec("./genHTML selectStream.wpml", $array);
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