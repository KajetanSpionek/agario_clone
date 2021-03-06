/*!
*\file      player.js
*\author    Wojciech Przybysz, Kajetan Spionek
*           Implementation of HTML5 canvas and draw functions
*/

/* Event Listeners */

canvas.addEventListener('mouseover', function(evt) {
    raf = window.requestAnimationFrame(update);
});

/* Animation handle */

window.requestAnimFrame = (function() {
    return  window.requestAnimationFrame       ||
            window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame    ||
            window.msRequestAnimationFrame     ||
            function( callback ) {
            window.setTimeout(callback, 1000 / 60);
            };
})();

window.cancelAnimFrame = (function(handle) {
    return  window.cancelAnimationFrame     ||
            window.mozCancelAnimationFrame;
})();

/* Draw Functions */

/// Draws circle on x,y position
function drawCircle(x_center,y_center,radius,color) {
    context.strokeStyle = 'black';
    var start_angle = 0;
    var end_angle = 2*Math.PI;
    context.beginPath();    
    context.arc(x_center-deltaX,y_center-deltaY,scl*radius,start_angle,end_angle);
	context.fillStyle = color;
    context.fill();
    context.lineWidth = 2;
    context.stroke();
}

/// Draws circle within fx,fy distance from center of player's screen
function drawCircleFx(fx_center,fy_center,radius,color) {
	context.strokeStyle = 'black';
    var start_angle = 0;
    var end_angle = 2*Math.PI;
    context.beginPath();    
    context.arc(player.x_+fx_center-deltaX,player.y_+fy_center-deltaY,scl*radius,start_angle,end_angle);
	context.fillStyle = color;
    context.fill();
    context.lineWidth = 2;
    context.stroke();
}

/// Redraws grid
function reDrawGrid() {
    context.fillStyle = '#F0FBFF';
    context.strokeStyle = '#BFBFBF';
    context.lineWidth = 1;
    constY = 50*scl - ((deltaY + (canvas.height/2)) % 50*scl);
    constX = 50*scl - ((deltaX + (canvas.width/2)) % 50*scl);


    for (i=0; i < canvas.height; i += 50*scl) {
        context.beginPath();
        context.moveTo(0,constY + i);
        context.lineTo(canvas.width,constY + i);
        context.stroke();
    }


    for (i=0; i < canvas.width; i += 50*scl) {
        context.beginPath();
        context.moveTo(constX + i,0);
        context.lineTo(constX + i,canvas.height);
        context.stroke();
    }
}

/// Draws objects on map (player, balls and food)
function reDrawCanvas() {    
    for (var i in foods) {
        calculateFixedPos();
        foods[i].show();
    }

    for (var i in balls) {
        balls[i].show();
    }
    
    if (playerSet == 1) player.show();
}

/// Displays death screen on canvas
function deathScreen() {

    context.fillStyle = 'rgba(31, 31, 31, 0.8)';
    context.fillRect(0, 0, canvas.width, canvas.height);
    context.textAlign = 'center';
    context.fillStyle = '#FFFFFF';
    context.font = 'bold 50px sans-serif';
    context.fillText('You got eaten!', canvas.width / 2, canvas.height * 0.4);
    context.font = 'bold 30px sans-serif';
    
    if (canvas.width > 300 && canvas.height > 500) {
        context.fillText('Food eaten: ' + deathStats[0], canvas.width / 2, canvas.height * 0.55);
        context.fillText('Balls eaten: ' +  deathStats[1], canvas.width / 2, canvas.height * 0.60);
        context.fillText('Final mass: ' +  deathStats[2], canvas.width / 2, canvas.height * 0.65);
    }
}
