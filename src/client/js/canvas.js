 /*
File: player.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Implementation of HTML5 canvas and draw functions
*/   

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