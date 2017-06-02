  //var x_pos_state = document.getElementById("x_pos");
    //var y_pos_state = document.getElementById("y_pos");
    //var mass_state = document.getElementById("mass_pos");


// Import stuff





    var canvas = document.getElementById('myCanvas');
    var context = canvas.getContext('2d');
    
    var raf;

    var maximalUpdateDelay = 25; // ms
	   var updateTimeout, now;


    var foods = [];
    var player = -1;
    var balls = [];
    var mousePos;

    var scl = 1;
    var send_interval = 1;
    var send_interval_period = 2;

    // Board margin
    var boardMargin = 0;

    // Whole map size
    var gameBoardX;
    var gameBoardY;

    // Vector to left top corner from (0,0) pos
    var deltaX;
    var deltaY;


    window.addEventListener('resize', resizeCanvas, false);       
           resizeCanvas();	
    
    
    function getRandomColor() 
    {
        var letters = '0123456789ABCDEF';
        var color = '#';
        for (var i = 0; i < 6; i++ ) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    }
    
    function drawCircle(x_center,y_center,radius,color)
    {
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

    function drawCircleFx(fx_center,fy_center,radius,color)
   {
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
    
    function onError(evt)
    { 
        state.className = "fail";
        state.innerHTML = "Communication error";
    }  

    function gameLoop() {

        raf = window.requestAnimationFrame(update);

    }

function getMousePos(canvas, evt) {
        var rect = canvas.getBoundingClientRect();
        return {
          x: evt.clientX - rect.left,
          y: evt.clientY - rect.top
        };
}

canvas.addEventListener('mousemove', function(evt) {
        mousePos = getMousePos(canvas, evt);
});

canvas.addEventListener('mouseover', function(e) {
  	raf = window.requestAnimationFrame(update);
});


canvas.addEventListener('mouseout', function(e) {
  window.cancelAnimationFrame(raf);
});

function reDrawCanvas() {

	

	for (var i in foods) {
		calculateFixedPos();
		foods[i].show();
	}

	

	for (var i in balls)
		balls[i].show();

	player.show();
}

function reDrawGrid() {

context.fillStyle = '#F0FBFF';
context.strokeStyle = '#BFBFBF';
context.lineWidth = 1;
constY = 50*scl - ((deltaY + (canvas.height/2)) % 50*scl);
constX = 50*scl - ((deltaX + (canvas.width/2)) % 50*scl);

//log.innerHTML = '<li class="message">' + deltaX + "</li>" + log.innerHTML;


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

context.lineWidth = 3;
context.strokeStyle = 'black';
context.beginPath();
context.moveTo(- deltaX,- deltaY);
context.lineTo(- deltaX + gameBoardX,- deltaY);
context.stroke();

context.beginPath();
context.moveTo(- deltaX,- deltaY);
context.lineTo(- deltaX,- deltaY + gameBoardY);
context.stroke();

context.beginPath();
context.moveTo(- deltaX + gameBoardX,- deltaY);
context.lineTo(- deltaX + gameBoardX,- deltaY + gameBoardY);
context.stroke();

context.beginPath();
context.moveTo(- deltaX,- deltaY + gameBoardY);
context.lineTo(- deltaX + gameBoardX,- deltaY + gameBoardY);
context.stroke();


}

function update() {
 
	// New movement 

	var dx = mousePos.x - canvas.width/2;
  	var dy = mousePos.y - canvas.height/2;
  	var v_len = Math.abs(Math.sqrt( (dy*dy) + (dx*dx) ));

  	var scaleX = Math.abs(dx / v_len);
  	var scaleY = Math.abs(dy / v_len);

  	

  	if (mousePos.x > canvas.width/2 && player.x_ < gameBoardX - boardMargin) {
  		deltaX += scaleX *player.vx_;
  		player.x_ += scaleX * player.vx_;
  	}
  	else if (mousePos.x < canvas.width/2 && player.x_ > 0 + boardMargin) {
  		deltaX -= scaleX * player.vx_;
  		player.x_ -= scaleX * player.vx_;
  	}
  	else {
		deltaX = deltaX;
  		player.x_ = player.x_;

  	}

  	if (mousePos.y > canvas.height/2 && player.y_ < gameBoardY - boardMargin) {
  		deltaY += scaleY * player.vy_;
  		player.y_ += scaleY * player.vy_;

  	}
  	else if (mousePos.y < canvas.height/2 && player.y_ > 0 + boardMargin) {
  		deltaY -= scaleY * player.vy_;
  		player.y_ -= scaleY * player.vy_;
  	}
  	else {
		deltaY = deltaY;
  		player.y_ = player.y_;

  	}

  	scl = 1 - (player.r_ / 400);
  	if (scl < 0.3) scl = 0.3;

	context.clearRect(0,0,canvas.width,canvas.height);
	reDrawGrid();
	reDrawCanvas();
	sendPos();
	raf = window.requestAnimationFrame(update);
     
    
}


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

function calculateFixedPos() {

	for (var i in foods) {

		foods[i].fx_ = (foods[i].x_ - player.x_)*scl;
		foods[i].fy_ = (foods[i].y_ - player.y_)*scl;
	}

	for (var i in balls) {

		balls[i].fx_ = (balls[i].x_ - player.x_)*scl;
		balls[i].fy_ = (balls[i].y_ - player.y_)*scl;
	}
}

function gameOver() {

	balls = [];
	foods = [];
	player = [];
	context.clearRect(0,0,canvas.width,canvas.height);

}


function resizeCanvas() {
            canvas.width = window.innerWidth;
            canvas.height = window.innerHeight;

            deltaX = player.x_ - canvas.width/2;
            deltaY = player.y_ - canvas.height/2;
        }




