export class MyFish {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.width = 64;
    this.height = 48;
    this.speed = 3;
    this.level = 1;
    this.dir = 'right';
    this.imgLeft = null;
    this.imgRight = null;
    this.loadImages();
  }

  loadImages() {
    this.imgLeft = new Image();
    this.imgLeft.src = 'assets/myFish/myfish_left.png';
    this.imgRight = new Image();
    this.imgRight.src = 'assets/myFish/myfish_right.png';
  }

  move(input, canvas) {
    if (input.left) { this.x -= this.speed; this.dir = 'left'; }
    if (input.right) { this.x += this.speed; this.dir = 'right'; }
    if (input.up) { this.y -= this.speed; }
    if (input.down) { this.y += this.speed; }
    // 边界限制
    this.x = Math.max(0, Math.min(canvas.width - this.width, this.x));
    this.y = Math.max(0, Math.min(canvas.height - this.height, this.y));
  }

  draw(ctx) {
    const img = this.dir === 'left' ? this.imgLeft : this.imgRight;
    if (img.complete && img.naturalWidth > 0) {
      ctx.drawImage(img, this.x, this.y, this.width, this.height);
    }
  }
}
