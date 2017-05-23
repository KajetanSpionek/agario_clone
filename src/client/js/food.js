class Food {
  constructor(id,x,y,color) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = 10; 
    this.color_ = color;
    this.fx_;
    this.fy_;
  }

  show() {
      drawCircleFx(this.fx_, this.fy_,this.r_,this.color_);
  }

}