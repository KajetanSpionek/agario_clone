class Ball {
  constructor(id,x,y,r,color) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = r;
    this.color_ = color;
    this.fx;
    this.fy;
  }

  show() {
      drawCircleFx(this.fx_, this.fy_,this.r_,this.color_);
  }

}