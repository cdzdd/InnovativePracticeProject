export class Game {
  constructor(canvas, ctx) {
    this.canvas = canvas;
    this.ctx = ctx;
    this.lastTime = 0;
    this.running = false;
      this.input = { left: false, right: false, up: false, down: false };
      this.bg = new Background();
      this.myFish = new MyFish(canvas.width / 2 - 32, canvas.height / 2 - 24);
      this.enemies = [];
      this.powerUps = [];
      this.levelIndex = 0;
      this.level = Levels[this.levelIndex];
      this.score = 0;
      this.spawnTimer = 0;
      this.powerUpTimer = 0;
      this.gameOver = false;
      this.win = false;
      this.initInput();
  }

  start() {
    this.running = true;
    window.requestAnimationFrame(this.loop.bind(this));
  }

  loop(timestamp) {
    if (!this.running) return;
    const delta = timestamp - this.lastTime;
    this.lastTime = timestamp;
    this.update(delta);
    this.render();
    window.requestAnimationFrame(this.loop.bind(this));
  }


    update(delta) {
      if (this.gameOver || this.win) return;
      // 玩家移动
      this.myFish.move(this.input, this.canvas);
      // 敌鱼移动
      for (const enemy of this.enemies) {
        enemy.move(this.canvas);
      }
      // 道具移动
      for (const p of this.powerUps) {
        p.move(this.canvas);
      }
      // 敌鱼生成
      this.spawnTimer += delta;
      if (this.spawnTimer > 1200) {
        this.spawnEnemy();
        this.spawnTimer = 0;
      }
      // 道具生成
      this.powerUpTimer += delta;
      if (this.powerUpTimer > 4000) {
        this.spawnPowerUp();
        this.powerUpTimer = 0;
      }
      // 碰撞检测
      this.checkCollisions();
      // 关卡推进
      if (this.score >= this.level.score) {
        this.levelIndex++;
        if (this.levelIndex >= Levels.length) {
          this.win = true;
        } else {
          this.level = Levels[this.levelIndex];
          this.enemies = [];
          this.powerUps = [];
          this.myFish.level++;
        }
      }
    }

    render() {
      this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
      this.bg.draw(this.ctx, this.canvas);
      // 敌鱼
      for (const enemy of this.enemies) {
        enemy.draw(this.ctx);
      }
      // 道具
      for (const p of this.powerUps) {
        p.draw(this.ctx);
      }
      // 玩家鱼
      this.myFish.draw(this.ctx);
      // HUD
      this.bg.drawHUD(this.ctx, this.score, this.myFish.level);
      // 结束/胜利提示
      if (this.gameOver) {
        this.ctx.font = '48px Arial';
        this.ctx.fillStyle = '#f44';
        this.ctx.fillText('游戏失败', this.canvas.width/2-100, this.canvas.height/2);
      }
      if (this.win) {
        this.ctx.font = '48px Arial';
        this.ctx.fillStyle = '#4f4';
        this.ctx.fillText('胜利通关！', this.canvas.width/2-120, this.canvas.height/2);
      }
    }

    spawnEnemy() {
      // 随机类型和方向
      const types = this.level.enemyTypes;
      const type = types[Math.floor(Math.random()*types.length)];
      const dir = Math.random() > 0.5 ? 'left' : 'right';
      const y = Math.random() * (this.canvas.height - 80) + 20;
      const x = dir === 'left' ? this.canvas.width : -80;
      this.enemies.push(new Enemy(x, y, type, dir));
    }

    spawnPowerUp() {
      const types = ['speed','shield','xp'];
      const type = types[Math.floor(Math.random()*types.length)];
      const x = Math.random() * (this.canvas.width - 32);
      const y = -32;
      this.powerUps.push(new PowerUp(x, y, type));
    }

    checkCollisions() {
      // 玩家与敌鱼
      for (let i = this.enemies.length-1; i >=0; i--) {
        const e = this.enemies[i];
        if (this.rectCollide(this.myFish, e)) {
          if (e.type === 'boss' || e.type > this.myFish.level) {
            this.gameOver = true;
          } else {
            this.score += 5 * (e.type === 'boss' ? 10 : e.type);
            this.enemies.splice(i,1);
          }
        }
      }
      // 玩家与道具
      for (let i = this.powerUps.length-1; i>=0; i--) {
        const p = this.powerUps[i];
        if (this.rectCollide(this.myFish, p)) {
          if (p.type === 'speed') {
            this.myFish.speed += 2;
            setTimeout(()=>{this.myFish.speed -=2;}, 3000);
          } else if (p.type === 'shield') {
            // 简化：获得一次免死机会
            this.myFish.shield = true;
          } else if (p.type === 'xp') {
            this.myFish.level++;
          }
          this.powerUps.splice(i,1);
        }
      }
    }

    rectCollide(a, b) {
      return a.x < b.x + b.width && a.x + a.width > b.x &&
             a.y < b.y + b.height && a.y + a.height > b.y;
    }

    initInput() {
      window.addEventListener('keydown', e => {
        if (e.code === 'ArrowLeft' || e.code === 'KeyA') this.input.left = true;
        if (e.code === 'ArrowRight' || e.code === 'KeyD') this.input.right = true;
        if (e.code === 'ArrowUp' || e.code === 'KeyW') this.input.up = true;
        if (e.code === 'ArrowDown' || e.code === 'KeyS') this.input.down = true;
      });
      window.addEventListener('keyup', e => {
        if (e.code === 'ArrowLeft' || e.code === 'KeyA') this.input.left = false;
        if (e.code === 'ArrowRight' || e.code === 'KeyD') this.input.right = false;
        if (e.code === 'ArrowUp' || e.code === 'KeyW') this.input.up = false;
        if (e.code === 'ArrowDown' || e.code === 'KeyS') this.input.down = false;
      });
    }
  }

