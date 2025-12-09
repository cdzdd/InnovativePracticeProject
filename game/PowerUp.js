export class PowerUp {
  constructor(x, y, type) {
    this.x = x;
    this.y = y;
    this.type = type; // 'speed', 'shield', 'xp'
    this.width = 32;
    this.height = 32;
    this.img = new Image();
    this.img.src = `assets/powerup/icon_powerup_${type}.png`;
    this.speed = 2;
  }

  move(canvas) {
    this.y += this.speed;
    if (this.y > canvas.height) this.y = -this.height;
  }

  draw(ctx) {
    if (this.img.complete && this.img.naturalWidth > 0) {
      ctx.drawImage(this.img, this.x, this.y, this.width, this.height);
    }
  }
}
