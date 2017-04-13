//try{
//    alert("Hello User");
//}
//catch(err){
//    document.getElementById("test").innerHTML = err.message;
//}

function logIn(){
    var msg, x;
    
    msg = document.getElementById("msg");
    msg.innerHTML = "";
    
    x = document.getElementById("username").value;
    try{
        if( x == "") throw "Insert username";
        //check if username is valid - it is server side
        //if username is valid connect to the game by websocket
        if( !isNaN(x) ) throw "Username must contain letter of an alphabet";
    } 
    catch(err) {
        msg.innerHTML = "Error: " + err;
    }
    finally {
        document.getElementById("username").value = "";    
    }
}
    