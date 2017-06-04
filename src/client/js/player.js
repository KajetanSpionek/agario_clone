/*!
*\file      player.js 
*\author    Wojciech Przybysz, Kajetan Spionek
*           Implementation of player class 
*/

class Player {
  // Player constructor 
  constructor(id,x,y,r,color) {
    // Player x position
    this.x_ = x;
    // Flayer y position
    this.y_ = y;
    // Flayer id (assigned by server)
    this.id_ = id;
    // Player radius (assigned by server)
    this.r_ = r;
    // Player color (randomized by server)
    this.color_ = color;
    // Normalized mouse horizontal position
    this.dx_ = 0;
    // Normalized mouse vertical position
    this.dy_ = 0;
    // Player's name
    this.name_ = "TestName";
  }

  // Display player on screen
  show() {
      drawCircle(this.x_, this.y_,this.r_,this.color_);
  }
}
