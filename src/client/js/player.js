/*
File: player.js
Author: Kajetan Śpionek, Wojciech Przybysz
Discription: Implementation of player class
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
    // Player x velocity (assigned by server)
    this.vx_ = 4;
    // Player y velocity (assigned by server)
    this.vy_ = 4;
  }

  // Display player on screen
  show() {
      drawCircle(this.x_, this.y_,this.r_,this.color_);
  }
}

/*
To do:
-> Singleton?
*/