/*
File: food.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Implementation of food class
*/

class Food {
  // Food constructor 
  constructor(id,x,y,color) {
    // Food x position (assigned by server)
    this.x_ = x;
    // Food y position (assigned by server)
    this.y_ = y;
    // Food unique id (assigned by server)
    this.id_ = id;
    // Food radius 
    this.r_ = 10; 
    // Food color (randomized by client)
    this.color_ = color;
    // X distance from player's screen middle position
    this.fx_;
    // Y distance from player's screen middle position
    this.fy_;
  }

  // Display food on screen
  show() {
      drawCircleFx(this.fx_, this.fy_,this.r_,this.color_);
  }
}