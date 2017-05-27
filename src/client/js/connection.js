/*
File: connection.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Handles connection with server; Contains in/out frames implementations.
*/

connected = document.getElementById("connected");
log = document.getElementById("log");
state = document.getElementById("status");




if (window.WebSocket === undefined)
    {
        state.innerHTML = "sockets not supported";
        state.className = "fail";
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
    }
  
    function onOpen(evt)
    {
        state.className = "success";
       // state.innerHTML = "Connected to server";    
    }
  
    function onClose(evt)
    {
        state.className = "fail";
        state.innerHTML = "Not connected";
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
            message = message.slice("gameStateBall:,".length);
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
            gameLoop();

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
            }
        } 

        else if (message.startsWith("endOfGame:")) {
            message = message.slice("endOfGame:,".length);
            message = message.split(" ");
            //log.innerHTML = '<li class="message">' + "endOfGame " + message + "</li>" + log.innerHTML;
            
            gameOver();
        }      
   
    }

    function sendPos() {
      //x_pos_state.innerHTML = player.x_;
      //y_pos_state.innerHTML = player.y_;
      //mass_state.innerHTML = player.r_;
        var x = Math.floor(player.x_);
        var y = Math.floor(player.y_);
        if ( (x < 3000) && (y < 3000)){
            var message = "move:";
            message += Math.floor(player.x_);
            message += ",";
            message += Math.floor(player.y_);
            if (send_interval == send_interval_period) {
                websocket.send(message);
                send_interval == 0;
            }
            else {
                send_interval+=1;
            }
         
        }
    }