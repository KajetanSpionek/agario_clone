function myTimer()
{
    player.dx_ = Math.random() * 2 - 1;
    player.dy_ = Math.random() * 2 - 1;
    var message = "move:";
    message += player.dx_;
    message += ",";
    message += player.dy_;
    if (gameStart == true) websocket.send(message);
    if (gameStart == true) consoleDisplay("Sent");


}
               
setInterval(function(){myTimer()},200);