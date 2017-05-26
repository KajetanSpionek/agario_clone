/*
File: ball.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Implementation of ball class
*/

class Ball {
  // Ball constructor 
  constructor(id,x,y,r,color) {
    // Ball x position (assigned by server)
    this.x_ = x;
    // Ball y position (assigned by server)
    this.y_ = y;
    // Ball unique id (assigned by server)
    this.id_ = id;
    // Food radius (calculated on server)
    this.r_ = r;
    // Food color (assigned by server)
    this.color_ = color;
    // X distance from player's screen middle position
    this.fx;
    // Y distance from player's screen middle position
    this.fy;
  }

  // Display ball on screen
  show() {
      drawCircleFx(this.fx_, this.fy_,this.r_,this.color_);
  }
}