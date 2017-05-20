class Food {
  constructor(id,x,y) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = 6;
  }

  show() {
      drawCircle(this.x_, this.y_,7);
  }

}

class Player {
  constructor(id,x,y,r) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = r;
  }

  show() {
      drawCircle(this.x_, this.y_,this.r_);
  }

}