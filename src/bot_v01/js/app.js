/*!
*\file      app.js
*\author    Wojciech Przybysz, Kajetan Spionek 
*           Main application file
*/

// Canvas variables
var canvas = document.getElementById('cvs');
var context = canvas.getContext('2d');
    
// Animation handle
var raf;

// Array's with data
var balls = [];
var foods = [];

// Mouse handle
var mousePos;

// Declaration of player's variable
var player = 0;

// Map scale variable
var scl = 1;

// Board margin
var boardMargin = 0;

// Whole map size
var gameBoardX;
var gameBoardY;

// Vector to left top corner from (0,0) gameBoard corner
var deltaX;
var deltaY;

// Game status variables
var gameStart = false;
var gameDied = false;

// Resize handle
window.addEventListener('resize', resizeCanvas, false); 

// Adjust canvas to player's screen size
resizeCanvas();     

// Returns random color
function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++ ) {
        color += letters[Math.floor(Math.random() * 16)];
    }
        return color;
}

// Valid nick chcek
function isNickValid() {
    var regex = /^\w*$/;
    if (regex.exec(playerNameInput.value) !== null) {
        return 1;
    }
    else return 0;
}
    
// Error handle - doesn't work   
function onError(evt) { 
    state.className = "fail";
    state.innerHTML = "Communication error";
    gameStart = false; 
}  

// Mouse handle
function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

// Display update (Animation)
function update() {
    if (gameStart == true && gameDied == false) {

        // Animation handle
        raf = window.requestAnimationFrame(update); 
        // Scale to (-1,1) mouse positions (from center of player's screen)
        //player.dx_ = (mousePos.x - canvas.width/2) / canvas.width * 2;
        //player.dy_ = (mousePos.y - canvas.height/2) / canvas.height * 2;
        //randomMouse();
        //startWorker();

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

// Calculate object's fx,fy distance from player's center of screen
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

// GameOver handle - doesnt work yet
function gameOver(foodEaten, ballsEaten, mass) {
    gameStart = false;
    gameDied = true;
    alert("GameOver\n\nFood eaten: " + foodEaten + "\nBalls eaten: " + ballsEaten + "\nMass: " + mass);
    //document.location.href="/";
}

// StartGame handle - doesnt work yet
function startGame() {
         
    document.getElementById('startMenu').style.opacity = 0;
    document.getElementById('startMenuWrapper').style.maxHeight = '0px';
    document.getElementById('gameArea').style.opacity = 1;
    
    gameStart = true;
    gameDied = false;

    startWorker();
}

// Resize canvas handle
function resizeCanvas() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    deltaX = player.x_ - canvas.width/2;
    deltaY = player.y_ - canvas.height/2;
}

// Debug console
var consoleDisplay = function(args) {
    if (console && console.log) {
        console.log(args);
    }
};



function startWorker() {
   // First check whether Web Workers are supported
   if (typeof(Worker)!=="undefined"){
      // Check whether Web Worker has been created. If not, create a new Web Worker based on the Javascript file simple-timer.js
      if (w==null) {
         w = new Worker("worker.js");
      }
     
      else {
        consoleDisplay("No workin my friend");
      }
   }
}


