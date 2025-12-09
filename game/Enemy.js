export class Enemy {
  constructor(x, y, type, dir) {
    this.x = x;
    this.y = y;
    this.type = type; // 1,2,3,boss
    this.dir = dir; // 'left' or 'right'
    this.speed = type === 'boss' ? 2 : 2 + type;
    this.width = type === 'boss' ? 128 : 48 + type * 8;
    this.height = type === 'boss' ? 96 : 32 + type * 8;
    this.img = null;
    this.loadImage();
  }

  loadImage() {
    let name = '';
    if (this.type === 'boss') {
      name = 'boss.png';
    } else {
      name = `fish${this.type}_${this.dir === 'left' ? 'l' : 'r'}.png`;
    }
    this.img = new Image();
    this.img.src = `assets/enemyFish/${name}`;
  }

  move(canvas) {
    if (this.dir === 'left') {
      this.x -= this.speed;
      if (this.x < -this.width) this.x = canvas.width;
    } else {
      this.x += this.speed;
      if (this.x > canvas.width) this.x = -this.width;
    }
  }

  draw(ctx) {
    if (this.img.complete && this.img.naturalWidth > 0) {
      ctx.drawImage(this.img, this.x, this.y, this.width, this.height);
    }
  }
}
