export class Background {
  constructor() {
    this.img = new Image();
    this.img.src = 'assets/sea.png';
  }

  draw(ctx, canvas) {
    if (this.img.complete && this.img.naturalWidth > 0) {
      ctx.drawImage(this.img, 0, 0, canvas.width, canvas.height);
    }
  }

  drawHUD(ctx, score, level) {
    ctx.font = '24px Arial';
    ctx.fillStyle = '#fff';
    ctx.fillText(`分数: ${score}`, 24, 36);
    ctx.fillText(`等级: ${level}`, 24, 72);
  }
}
