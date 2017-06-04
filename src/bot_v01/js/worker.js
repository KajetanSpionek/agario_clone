function myTimer()
{
    player.dx_ = Math.random() * 2 - 1;
    player.dy_ = Math.random() * 2 - 1;
    var message = "move:";
    message += player.dx_;
    message += ",";
    message += player.dy_;
    if (gameStart == true && playerSet == true) {
    	websocket.send(message);
    	consoleDisplay(message);
    }


}
               
setInterval(function(){myTimer()},200);