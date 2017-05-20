class Food {
  constructor(id,x,y) {

    this.x_ = x;
    this.y_ = y;
    this.id_ = id;
    this.r_ = 6; 
  }

  show() {
      drawCircle(this.x_, this.y_,7, 0);
  }

}

class Player {
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

  update() {

    context.clearRect(0, 0, canvas.width, canvas.height);
    
  }

}