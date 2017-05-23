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