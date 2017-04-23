var io = require('../node_modules/socket.io-client');

var socket;

function startGame() {
// connects players to server
// takes player's nickname
}

window.onload = function () {
  // display's nick form and play button - when screen load's on clinets side
}

var foods = [];
var players = [];
var user;

window.canvas = new Canvas();
var c = window.canvas.cv;

  // Recived socket handle
function setupSocket(socket) {

    // Disconnect handle
    socket.on('disconnect', function() {
      // Close socket
    });

    // Error handle
    socket.on('connection_error', function() {
      // Close socket
    });

    // Connection handle
    socket.on('connection_init', function() {
      // First connection initialization
      // Transfering basic parameters (?)
      // Setting basic parameters (?)
    });

    // Recieve data about food & players from server
    socket.on('update', function(playersData, foodData) {
      // Update foods[], players[], user
    });

    // Death msg - recieve information when eaten by another player
    socket.on('death', function() {
      // Recieve information when eaten by another player
    });
}

// Drawing functions

function drawCircle(x, y, r) {
  // Draw circle in specific directions
}

function drawFood(foods){
  // Draws all food on user's map
}

function drawPlayers(players){
  // Draws all players on user's map
}

function drawMap() {
  // Draws map when game started
}

// Game loop - handles whole game process
function gameLoop(){

  // if new game - draw board, food and players

  // Player's send cordinates (Heartbeat)

  // if player died - end game
  // if player disconnected - end game

}
