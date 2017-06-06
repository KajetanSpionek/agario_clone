/*!
*\file      app.js
*\author    Wojciech Przybysz, Kajetan Spionek 
*           Main application file
*/

// Canvas variable
var canvas = document.getElementById('cvs');
// Canvas context variable
var context = canvas.getContext('2d');
    
// Animation handle
var raf;

// Array with recieved data - array with other players
var balls = [];
// Array with recieved data - array with food
var foods = [];

// Mouse handle
var mousePos;

// Declaration of player's variable
var player = 0;

// Map scale variable (handles zooming out when getting bigger)
var scl = 1;

// Board margin
var boardMargin = 0;

// Whole map size - X
var gameBoardX;
// Whole map size - Y
var gameBoardY;

// Vector to left top corner from (0,0) gameBoard corner - X
var deltaX;
// Vector to left top corner from (0,0) gameBoard corner - X
var deltaY;

// Game status variables - true if game started 
var gameStart = false;
// Game status variables - true if player died
var gameDied = false;
// Game status variables - true if frame with player's info recieved
var playerSet = false;
// Game status variables - game statistics (food eaten, balls eaten and mass)
var deathStats = [0, 0, 0];

// Resize handle
window.addEventListener('resize', resizeCanvas, false); 

// Adjust canvas to player's screen size
resizeCanvas();     

/// Returns random color
function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++ ) {
        color += letters[Math.floor(Math.random() * 16)];
    }
        return color;
}

/// Validates nickname
function isNickValid() {
    var regex = /^\w*$/;
    if (regex.exec(playerNameInput.value) !== null) {
        return 1;
    }
    else return 0;
}
    
/// Error handle 
function onError(evt) { 
    document.getElementById('input-error').innerHTML = "Connection error";
    document.getElementById('input-error').style.opacity = 1;
    gameStart = false;
}  

/// Mouse handle
function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

/// Display update (Animation)
function update() {
    if (gameStart == true && gameDied == false) {

        // Animation handle
        raf = window.requestAnimationFrame(update); 
        // Scale to (-1,1) mouse positions (from center of player's screen)
        //player.dx_ = (mousePos.x - canvas.width/2) / canvas.width * 2;
        //player.dy_ = (mousePos.y - canvas.height/2) / canvas.height * 2;

        // Zooming out while getting fatter ball 
        scl = 1 - (player.r_ / 500);
        if (scl < 0.3) scl = 0.3;

        // Clear canvas
        context.clearRect(0,0,canvas.width,canvas.height);
        // Draw grid and borders
        reDrawGrid();
        // Draws objects on map (player, balls and food)
        reDrawCanvas();
        // Send current mouse position to server
        //sendPos();   
    }
}

/// Calculate object's fx,fy distance from player's center of screen
function calculateFixedPos() {
    //  Food fixed position
    for (var i in foods) {
		    foods[i].fx_ = (foods[i].x_ - player.x_)*scl;
		    foods[i].fy_ = (foods[i].y_ - player.y_)*scl;
    }
    // Ball fixed position
    for (var i in balls) {
		  balls[i].fx_ = (balls[i].x_ - player.x_)*scl;
		  balls[i].fy_ = (balls[i].y_ - player.y_)*scl;
    }
}

/// GameOver handle
function gameOver() {
    gameStart = false;
    gameDied = true;
    deathScreen();
}

/// StartGame handle
function startGame() {
         
    document.getElementById('startMenu').style.opacity = 0;
    document.getElementById('startMenuWrapper').style.maxHeight = '0px';
    document.getElementById('gameArea').style.opacity = 1;
    
    gameStart = true;
    gameDied = false;
}

/// Resize canvas handle
function resizeCanvas() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    deltaX = player.x_ - canvas.width/2;
    deltaY = player.y_ - canvas.height/2;

    if (gameDied == true) {
        reDrawGrid();
        reDrawCanvas();
        deathScreen();
    }
}



function startWorker() {
   // First check whether Web Workers are supported
   if (typeof(Worker)!=="undefined"){
      // Check whether Web Worker has been created. If not, create a new Web Worker based on the Javascript file simple-timer.js
        
         worker = new Worker("js/worker.js"); 
         worker.postMessage('startBot');

         worker.addEventListener('message', function(evt) {

            var x = Math.random() / 2 - 0.25;
            var y = Math.random() / 2 - 0.25;
            var message = "move:";

            if (gameStart == true && gameDied == false && playerSet == true) {
                    if ( (player.dx_ + x <= 1) && (player.dy_ + y <= 1) && (player.dx_ + x >= -1) && (player.dy_ + y >= -1) ) 

                        if (player.x_ > gameBoardX-10 || player.x_ < 10 || player.y_ > gameBoardY-10 || player.y_ < 10) {

                            if (player.x_ > gameBoardX-10 || player.x_ < 10) player.dx_ = -player.dx_;
                            if (player.y_ > gameBoardY-10 || player.y_ < 10) player.dy_ = -player.dy_;

                            message += player.dx_;
                            message += ",";
                            message += player.dy_;
                            websocket.send(message);

                        }

                        else {
                            player.dx_ += x;
                            player.dy_ += y;
                            message += player.dx_;
                            message += ",";
                            message += player.dy_;
                            websocket.send(message);
                        }
                }

        });
      
   }
}