
var userID;
var postsList = [];
var currentShowing = -1;
var numPosts;
var streamSelected;
var sortOption = 0;



function Post(author, stream, date, viewed, text) {

    var d = new Date(date);

    this.author = author;
    this.stream = stream;
    this.date = date;
    this.dateStr = d.toDateString()+" "+d.toLocaleTimeString();
    this.dateVal = d.valueOf();                                 //for sorting
    this.viewed = viewed;
    this.text = text;
}

Post.prototype.markRead = function() {
    
    if (this.viewed == "False")
    {
        var post = this;
        AJAX("type=mark1&userID="+userID+"&streamName="+this.stream, function(data){
            if (data["status"] == "true")
            {
                post.viewed = true;
            }
        });
    }
};

Post.prototype.print = function() {
    

    var display = document.getElementById('posts'); //table to be added to

    display.innerHTML = "";

    //create the elements
    var author = document.createElement("span");
    var stream = document.createElement("span");
    var date = document.createElement("span");
    var text = document.createElement("p");
    var bold = document.createElement("b");

    author.style.fontSize = "2em";
    stream.style.fontSize = "1.5em";
    date.style.fontSize = "1.5em";

    author.innerHTML = "Author: " + this.author + "<br>";
    stream.innerHTML = "Stream: " + this.stream + "<br>";
    date.innerHTML = "Date: " + this.dateStr + "<br>";

    text.innerHTML = (currentShowing+1).toString() + "/" + (numPosts).toString() + "<br>";
    bold.appendChild(author);
    bold.appendChild(stream);
    bold.appendChild(date);
    text.appendChild(bold);

    text.innerHTML += this.text;
    display.appendChild(text);
};


function compDate(a, b)
{
    return a.dateVal - b.dateVal;
}

function compStream(a, b)
{
    return a.stream.localeCompare(b.stream);
}

function compAuthor(a, b)
{
    return a.author.localeCompare(b.author);
}

function sort()
{
    var sortStr = ["Sort by date", "Sort by author", "Sort by stream"];

    sortOption = (sortOption+1) % 3;

    if (sortOption == 1)
    {
        postsList = postsList.sort(compAuthor);
    }
    else if (sortOption == 2)
    {
        postsList = postsList.sort(compStream);
    }
    else
    {
        postsList = postsList.sort(compDate);
    }


    document.getElementById("srt_btn").innerHTML = sortStr[(sortOption+1) % 3];
    currentShowing = 0;
    displayPost();
}


function markAll()
{
    if (sortOption != 0) {
        return;
    }

    streams = [];
    streamssub = [];
    for (var i = 0; i < postsList.length; i++) {
        streams.push(postsList[i].stream);
    }

    for (var i = 0; i < streams.length; i++) {
        if (streamssub.indexOf(streams[i]) == -1)
        {
            streamssub.push(streams[i]);

            AJAX("type=markAll&userID="+userID+"&streamName="+streams[i], function(data){ return; });
        }
    }

    for (var i = 0; i < postsList.length; i++) {
        postsList[i].viewed = true;
    }

}


function displayPost()
{
    postsList[currentShowing].print();

    // disable the previous button
    if (currentShowing == 0) {
        document.getElementById("prev_btn").disabled = true;
    }
    else {
        document.getElementById("prev_btn").disabled = false;
    }

    // disable the next button
    if (currentShowing == numPosts-1) {
        document.getElementById("next_btn").disabled = true;
    }
    else {
        document.getElementById("next_btn").disabled = false;
    }

    postsList[currentShowing].markRead();
}

function next()
{
    if (currentShowing < numPosts-1)
    {
        document.getElementById("prev_btn").disabled = false;
        currentShowing++;
        displayPost();
    }
    else{
        document.getElementById("next_btn").disabled = true;
    }
}

function previous()
{
    if (currentShowing > 0)
    {
        currentShowing--;
        displayPost();
    }
}


function AJAX(request, callback) {
    var xmlhttp;

    if (window.XMLHttpRequest) {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {
        // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {

            var responce;
            try {
                responce = JSON.parse(this.responseText);

            }
            catch (err) {
                alert(err);
                return;
            }

            callback(responce["data"]);
        }
        else if (this.readyState == 3) {

        }
    };
    xmlhttp.open("POST", "viewBackend.php", true);
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");

    xmlhttp.send(request);
}

function refresh()
{
    load(userID, streamSelected);
}

function load(user, stream)
{
    userID = user;
    streamSelected = stream;
    AJAX("type=load&userID="+userID+"&streamName="+stream, viewCallback);
}

function viewCallback(posts)
{
    postsList = []
    numPosts = posts.length;
    for (var i = 0; i < numPosts; i++) {
        var post = new Post(posts[i]["author"], posts[i]["stream"], posts[i]["date"], posts[i]["viewed"], posts[i]["text"]);
        postsList.push(post);

        if (currentShowing == -1 && post.viewed == "False")
        {
            currentShowing = i;
        }
    }
    if (currentShowing == -1)
    {
        currentShowing = numPosts-1;
    }

    displayPost();
}


