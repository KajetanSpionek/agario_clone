/*!
*\file      connection.js
*\author    Wojciech Przybysz, Kajetan Spionek 
*           Handles connection with server; Contains in/out frames implementations. 
*/

//connected = document.getElementById("connected");
//state = document.getElementById("status");

if (window.WebSocket === undefined)
    {
        //state.innerHTML = "sockets not supported";
        //state.className = "fail";
    }
    else
    {
        if (typeof String.prototype.startsWith != "function")
        {
            String.prototype.startsWith = function (str)
            {
                return this.indexOf(str) == 0;
            };
        }
    
        window.addEventListener("load", onLoad, false);
    }

    function onLoad() {
        var wsUri = "ws://127.0.0.1:7777";  
        websocket = new WebSocket(wsUri);
        websocket.onopen = function(evt) { onOpen(evt) };
        websocket.onclose = function(evt) { onClose(evt) };
        websocket.onmessage = function(evt) { onMessage(evt) };
        websocket.onerror = function(evt) { onError(evt) };

        var btn = document.getElementById('startButton');
        var nickErrorText = document.querySelector('#startMenu .input-error');

        document.getElementById('gameArea').style.opacity = 0;
        document.getElementById('startMenu').style.opacity = 1;
        
        // Button click detection
        btn.onclick = function () { 
            if (isNickValid()) {
                nickErrorText.style.opacity = 0;  
                sendPlayerName();
            } else {
                nickErrorText.style.opacity = 1;
            }
        };
    }
  
    function onOpen(evt) {
        //state.className = "success";
        // state.innerHTML = "Connected to server";    
    }
  
    function onClose(evt) {
        //state.className = "fail";
        //state.innerHTML = "Not connected";
        //connected.innerHTML = "0";        
    }
  

    /* Recieved messages */

    function onMessage(evt) {
        var message = evt.data;
    
        // Connected - number of connected user's
        if (message.startsWith("connected:")) {
            message = message.slice("connected:".length); 
        }   

        // New food frame - ID, x, y
        else if (message.startsWith("newFood:")) {
            message = message.slice("newFood:,".length);
            message = message.split(" ");

            if (foods.indexOf(message[0]) == -1) { // bugged
                var len = foods.length;
                var color = getRandomColor();
                foods[len] = new Food(parseInt(message[0]),parseInt(message[1]), parseInt(message[2]), color);
                foods[len].show(); //Display ball
            }
        }
        
        // New ball frame - ID, x, y, r, color
        else if (message.startsWith("newBall:")) {
            message = message.slice("newBall:,".length);
            message = message.split(" ");
           
            if (balls.indexOf(message[0]) == -1 && player.id_ != message[0]) { //bugged?
                var len = balls.length;
                balls[len] = new Ball(parseInt(message[0]),parseInt(message[1]), parseInt(message[2]), parseInt(message[3]),message[4]);
                balls[len].show();
            }
        } 

        // All balls frame - (ID, x, y, r, color) times number of balls
        else if (message.startsWith("gameStateBall")) {
            message = message.slice("gameStateBall:,".length);
            message = message.split(" ");

            if (message.length > 4 ) {
              for( i = 0; i < message.length; i+=5){
                  if (balls.indexOf(message[i]) == -1) { //bugged?
                      var len = balls.length;
                      balls[len] = new Ball(parseInt(message[i]),parseInt(message[i+1]), parseInt(message[i+2]),parseInt(message[i+3]),message[i+4]);
                      balls[len].show(); 
                    }    
                } 
            }
        }
        
        // All food frame - (ID, x, y) times number of food
        else if (message.startsWith("gameStateFood:")) {
            message = message.slice("gameStateFood:,".length);
            message = message.split(" ");
            
            for( i = 0; i < message.length; i+=3) {    
                if (foods.indexOf(message[i]) == -1) { // bugged
                    var len = foods.length;
                    var color = getRandomColor();
                    foods[len] = new Food(parseInt(message[i]),parseInt(message[i+1]), parseInt(message[i+2]), color);
                    foods[len].show();
                }
            }  
        }

        // Frame with player details (ID, x, y, r, color) 
        else if (message.startsWith("newPlayerBall:")) {
            message = message.slice("newPlayerBall:,".length);
            message = message.split(" ");
            player = new Player( parseInt(message[0]),  parseInt(message[1]),  parseInt(message[2]),  parseInt(message[3]),  message[4]); 

            deltaX = player.x_ - canvas.width/2;
            deltaY = player.y_ - canvas.height/2;

            gameBoardY = 2990;
            gameBoardX = 2990;

            player.show();
        }

        // Frame with deleted ball details - ID)
        else if (message.startsWith("deleteBall:")) {
            message = message.slice("deleteBall:,".length);
            message = message.split(" ");
            var index = -1;

            for (i in balls) {
                if (balls[i].id_ == message[0]) index = i;
            } 
            
            if (index != -1) balls.splice(index,1);
            
            reDrawCanvas();
        }

        // Frame with deleted food details - ID)
        else if (message.startsWith("deleteFood:")) {
            message = message.slice("deleteFood:,".length);
            message = message.split(" ");
 
            var index = -1;

            for (i in foods) {
              if (foods[i].id_ == message[0]) index = i;
            }

            if (index != -1) foods.splice(index,1);

            reDrawCanvas();
        } 

        // Ball update frame - ID, x, y, r
        else if (message.startsWith("ballUpdate:")) {
            message = message.slice("ballUpdate:,".length);
            message = message.split(" ");
            
            var id = parseInt(message[0]);

            if (id != player.id_) {
              for(i in balls) {
                if (balls[i].id_ == id) {
                  balls[i].x_ = parseInt(message[1]);
                  balls[i].y_ = parseInt(message[2]);
                  balls[i].r_ = parseInt(message[3]);
                }
              }
            }
            else if (id == player.id_) {
                player.r_ = parseInt(message[3]);
                player.x_ = parseInt(message[1]);
                player.y_ = parseInt(message[2]);

                deltaX = player.x_ - canvas.width/2;
                deltaY = player.y_ - canvas.height/2;
            }
        } 

        // End of game frame - Food eaten, Balls eaten, Mass
        else if (message.startsWith("endOfGame:")) {
            message = message.slice("endOfGame:,".length);
            message = message.split(" "); 
            var foodEaten = parseInt(message[0]);
            var ballsEaten = parseInt(message[1]);
            var mass = parseInt(message[2]);         
            gameOver(foodEaten, ballsEaten, mass);
        }  

        // New player nick validation gram
        else if (message.startsWith("newPlayerValidNick")) {
            message = message.slice("newPlayerValidNick:".length);
            if (message == "OK") {
                sendPlayerStatus(1);
                startGame();
            }
        }  

        // Game board size frame - x,y - not implemented yet
        else if (message.startsWith("gameBoardSize")) {
            message = message.splice("gameBoardSize:".length);
            gameBoardX = parseInt(message[0]);
            gameBOardY = parseInt(message[1]);
        }  
    }

    /* Sent messages */

// Mouse position (normalised)
function sendPos() {
 
    var x = player.dx_;
    var y = player.dy_;

    if ( (x <= 1) && (y <= 1) && (x >= -1) && (y >= -1) ){
        var message = "move:";
        message += x;
        message += ",";
        message += y;
        websocket.send(message);
    }
}

// Player name frame (handshake)
function sendPlayerName() {

    var message = "newPlayerName:";
    message += document.getElementById('playerNameInput').value;
    websocket.send(message);
    consoleDisplay  (message);
}

// Player status frame (handshake)
 function sendPlayerStatus(state) {

    var message = "newPlayerStatus:";

    if (state == 1) message += "rdy";
    else            message += "nrdy";

    message += ",";
    message += document.getElementById('playerNameInput').value;

    websocket.send(message);
    consoleDisplay  (message);

    }