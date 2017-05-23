class Ball {
  constructor(id,x,y,r,color) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = r;
    this.color_ = color;
  }

  show() {
      drawCircle(this.x_, this.y_,this.r_,this.color_);
  }

}