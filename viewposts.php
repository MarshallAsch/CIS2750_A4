<?php

    $userID = "";
    $streamName = "";

    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // if allready loged in get the stream and the userID
        if (isset($_POST["__USERID_B"]) && isset($_POST["__STREAM_B"]))
        {
            $userID = $_POST["__USERID_B"];
            $streamName = $_POST["__STREAM_B"];
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
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css"> 
    </head>
    
    <body onload='load(<?php echo "\"$userID\", \"$streamName\""; ?>)'>

        <?php 
            // Load the login form 
            $array = array();
            exec("./genHTML viewPost.wpml", $array);
            foreach ($array as $value)
            {
                eval("?>".$value);
                echo "\n";
            }
        ?>

        <!-- inject the page body here -->
        <div id="posts"></div>

        <?php
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
        <script type="text/javascript" src="viewScripts.js"></script>

        <!-- script for form generating the page and the AJAX calls -->
        <script type="text/javascript" src="script.js"></script> 
     
    </body>
</html>
