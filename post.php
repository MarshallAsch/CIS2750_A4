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

            // get the list of streams
            $streamList = array();
            exec("./auth \"$userID\" \"\"", $streamList);
        }
        else if (isset($_POST["userID"]) && isset($_POST["oldstream"]))
        {
            //then the user is submitting a post 
            $postFileName = tempnam("tmp", "post");
            $postFile = fopen($postFileName, "w");

            $userID = $_POST["userID"];
            $streamName = $_POST["oldstream"];
            $postMsg = $_POST["post"];
            $postStream = $_POST["stream"];
            
            fwrite($postFile, $postMsg);
            fclose($postFile);

            // submit the post
            exec("./post \"$userID\" \"$postStream\" \"$postFileName\"");

            // get the list of streams
            $streamList = array();
            exec("./auth \"$userID\" \"\"", $streamList);

            // remove the file 
            unlink($postFile);
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
            exec("./genHTML post.wpml", $array);
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
