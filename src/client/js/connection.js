/*
File: connection.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Handles connection with server; Contains in/out frames implementations.
*/

//connected = document.getElementById("connected");
log = document.getElementById("log");
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

    function onLoad()
    {
        var wsUri = "ws://127.0.0.1:7777";  
    
        websocket = new WebSocket(wsUri);
        websocket.onopen = function(evt) { onOpen(evt) };
        websocket.onclose = function(evt) { onClose(evt) };
        websocket.onmessage = function(evt) { onMessage(evt) };
        websocket.onerror = function(evt) { onError(evt) };

    // Load screen with Ball
    var btn = document.getElementById('startButton');
    var nickErrorText = document.querySelector('#startMenu .input-error');

    document.getElementById('gameArea').style.opacity = 0;
    document.getElementById('startMenu').style.opacity = 1;
    document.getElementById('dataWindow').style.opacity = 1;

    btn.onclick = function () {
            


        if (isNickValid()) {
            nickErrorText.style.opacity = 0;  
            startGame();
        } else {

            nickErrorText.style.opacity = 1;
        }



        };

    gameStatus = "init";

    }
  
    function onOpen(evt)
    {
        //state.className = "success";
       // state.innerHTML = "Connected to server";    
    }
  
    function onClose(evt)
    {
        //state.className = "fail";
        //state.innerHTML = "Not connected";
        //connected.innerHTML = "0";        
    }
  
    function onMessage(evt)
    {
  
        var message = evt.data;
    
        if (message.startsWith("log:")) {
            message = message.slice("log:".length);
            //log.innerHTML = '<li class="message">' + message + "</li>" + log.innerHTML; 
        }
        else if (message.startsWith("connected:")) {
            message = message.slice("connected:".length);
            //connected.innerHTML = message;  
        }   

        else if (message.startsWith("newFood:")) {
            message = message.slice("newFood:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "newFood:" + message + "</li>" + log.innerHTML; 

            if (foods.indexOf(message[0]) == -1) {

                    var len = foods.length;
                    var color = getRandomColor();
                    foods[len] = new Food(parseInt(message[0]),parseInt(message[1]), parseInt(message[2]), color);
                    foods[len].show(); //Display ball
                }
            

        }
        
        else if (message.startsWith("newBall:")) {
            message = message.slice("newBall:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "newBall:" + message + "</li>" + log.innerHTML;

            if (balls.indexOf(message[0]) == -1 && player.id_ != message[0]) {

                    var len = balls.length;
                    balls[len] = new Ball(parseInt(message[0]),parseInt(message[1]), parseInt(message[2]), parseInt(message[3]),message[4]);
                    balls[len].show(); //Display player
                }
  
        } 

        else if (message.startsWith("gameStateBall")) {
            message = message.slice("gameStateBall:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "gameStateBall:" + message + "</li>" + log.innerHTML;
            if (message.length > 4 ) {
              for( i = 0; i < message.length; i+=5){
                  if (balls.indexOf(message[i]) == -1) { 
                      var len = balls.length;
                      balls[len] = new Ball(parseInt(message[i]),parseInt(message[i+1]), parseInt(message[i+2]),parseInt(message[i+3]),message[i+4]);
                      balls[len].show(); 
                  }    
              } 
          }

        }
        
        else if (message.startsWith("gameStateFood:")) {
            message = message.slice("gameStateFood:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "gameStateFood:" + message + "</li>" + log.innerHTML;

            // Display food
            for( i = 0; i < message.length; i+=3) {    
                //Check if object already exists
                // \/ line belove doent work!
                if (foods.indexOf(message[i]) == -1) {
                    var len = foods.length;
                    var color = getRandomColor();
                    foods[len] = new Food(parseInt(message[i]),parseInt(message[i+1]), parseInt(message[i+2]), color);
                    foods[len].show(); //Display ball
                }
            }  

        }

        else if (message.startsWith("newPlayerBall:")) {
            message = message.slice("newPlayerBall:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "newPlayerBall:" + message + "</li>" + log.innerHTML;
            player = new Player( parseInt(message[0]),  parseInt(message[1]),  parseInt(message[2]),  parseInt(message[3]),  message[4]); 

            deltaX = player.x_ - canvas.width/2;
            deltaY = player.y_ - canvas.height/2;

            gameBoardY = 2990;
            gameBoardX = 2990;


            player.show();
        }


        else if (message.startsWith("deleteBall:")) {
            message = message.slice("deleteBall:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "deleteBall:" + message + "</li>" + log.innerHTML;
            var index = -1;

            for (i in balls) {
              if (balls[i].id_ == message[0]) index = i;
            } 
            
        if (index != -1) balls.splice(index,1);
        reDrawCanvas();

        }

        else if (message.startsWith("deleteFood:")) {
            message = message.slice("deleteFood:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "deleteFood:" + message + "</li>" + log.innerHTML;
        
            var index = -1;

            for (i in foods) {
              if (foods[i].id_ == message[0]) index = i;
            } 
        if (index != -1) foods.splice(index,1);
        reDrawCanvas();

        }   

        else if (message.startsWith("ballUpdate:")) {
            message = message.slice("ballUpdate:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "ballUpdate: " + message + "</li>" + log.innerHTML;
            
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

              // Wyswietlanie siebie
            }
        } 

        else if (message.startsWith("endOfGame:")) {
            message = message.slice("endOfGame:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "endOfGame " + message + "</li>" + log.innerHTML;
            
            gameOver();
        }  

        else if (message.startsWith("newPlayerServer:")) {
            message = message.splice("newPlayerServer:,".length);
        }  

        else if (message.startsWith("gameBoardSize")) {
            message = message.splice("gameBoardSize:,".length);
            gameBoardX = parseInt(message[0]);
            gameBOardY = parseInt(message[1]);

        }
   
    }

    function sendPos() {
 
        var x = player.dx_;
        var y = player.dy_;

        if ( (x <= 1) && (y <= 1) && (x >= -1) && (y >= -1) ){
            var message = "move:";
            message += x;
            message += ",";
            message += y;
            //log.innerHTML = '<li class="message">' + " " + message + "</li>" + log.innerHTML;
            websocket.send(message);
        }
    }

    function sendPlayerName() {

        var message = "newPlayerName:";
        // add player's name
        websocket.sned(message);
    }

    function sendPlaterStatus(state) {

        var message = "newPlayerStatus:";

        if (state == 1) message += "rdy";
        else            message += "nrdy";

        websocket.send(message);

    }