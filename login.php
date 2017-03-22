<?php

    $userID = "";
    $streamName = "";

    $status = 0;
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // if allready loged in get the stream and the userID
        if (isset($_POST["__USERID_B"]) && isset($_POST["__STREAM_B"]))
        {
            $userID = $_POST["__USERID_B"];
            $streamName = $_POST["__STREAM_B"];
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
            // Load the login form 
            $array = array();
            exec("./genHTML login.wpml", $array);
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
