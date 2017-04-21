var io = require('../node_modules/socket.io-client');
var global = require('./global');
var Canvas = require('./canvas.js');
var global = require('./global');

var usernameInput = document.getElementById('usernameInput');
var socket;
var reason;


function validNick() {
    var regex = /^\w*$/;
    return regex.exec(usernameInput.nodeValue) !== null;
}

function startGame(type) {
    global.playerName = usernameInput.value.replace(/(<([^>]+)>)/ig, '').substring(0, 25);
    global.playerType = type;
    
    global.screenWidth = window.innerWidth;
    global.screenHeight = window.innerHeight;
    
    document.getElementById('loginArea').style.maxHeight = '0px';
    document.getElementById('gameArea').style.opacity = 1;
    if (!socket) {
        socket = io({query: "type=" + type});
        setupSocket(socket);
    }
    socket.emit('respawn');
    window.chat.socket = socket;
    window.chat.registerFunctions();
    window.canvas.socket = socket;  
}

window.onload = function () {
    var btn = document.getElementById('startButton');
    
    btn.onclick = function () {
        if (validNick()) {
            nickErrorText.style.opacity = 0;
            startGame('player');
        } else {
            nickErrorText.style.opacity = 1;
        }
    }
}

