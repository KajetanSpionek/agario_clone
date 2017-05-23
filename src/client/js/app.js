class Food {
  constructor(id,x,y,color) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = 10; 
    this.color_ = color;
  }

  show() {
      drawCircle(this.x_, this.y_,this.r_,this.color_);
  }

}

class Player {
  constructor(id,x,y,r,color) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = r;
    this.color_ = color;
    this.vx_ = 4;
    this.vy_ = 4;
  }

  show() {
      drawCircle(this.x_, this.y_,this.r_,this.color_);
  }

}