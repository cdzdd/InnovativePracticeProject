package com.sxt;//道具

import java.awt.*;

// 道具类型枚举
enum PowerUpType {
    SPEED, SHIELD, XP
}

public class PowerUp {
    // 道具属性
    int x, y; // 位置
    int width = 40; // 宽度
    int height = 40; // 高度
    int speed = 2; // 下落速度
    PowerUpType type; // 道具类型
    Image img; // 道具图片

    public PowerUp(int x, int y, PowerUpType type) {
        this.x = x;
        this.y = y;
        this.type = type;
        // 初始化对应类型的图片
        switch (type) {
            case SPEED:
                img = Toolkit.getDefaultToolkit().createImage("images/powerup/icon_powerup_speed.jpg");
                break;
            case SHIELD:
                img = Toolkit.getDefaultToolkit().createImage("images/powerup/icon_powerup_shield.jpg");
                break;
            case XP:
                img = Toolkit.getDefaultToolkit().createImage("images/powerup/icon_powerup_xp.jpg");
                break;
        }
    }

    // 绘制道具
    public void paintSelf(Graphics g) {
        g.drawImage(img, x, y, width, height, null);
    }

    // 移动道具
    public void move() {
        y += speed;
    }

    // 获取道具碰撞范围
    public Rectangle getRect() {
        return new Rectangle(x, y, width, height);
    }
}