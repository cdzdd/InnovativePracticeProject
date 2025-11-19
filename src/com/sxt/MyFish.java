package com.sxt;

import java.awt.*;

public class MyFish {
    //图片
    Image img = GameUtils.MyFishimg_L;
    //坐标
    int x = 700;
    int y = 500;
    int width = 50;
    int height = 50;
    //移动速度
    int speed = 20;
    //等级
    int level = 1;




    void logic(){
        checkPowerUpStatus();

        int currentSpeed = speed;
        if (GameUtils.hasSpeedBoost) {
            currentSpeed = speed * 2; // 加速效果
        }
        if (GameUtils.UP){
            y = y-speed;
        }
        if (GameUtils.DOWN){
            y = y+speed;
        }
        if (GameUtils.LEFT){
            x =x-speed;
            img = GameUtils.MyFishimg_L;
        }
        if (GameUtils.RIGHT){
            x = x+speed;
            img = GameUtils.MyFishimg_R;
        }
    }

    private void checkPowerUpStatus() {
        long currentTime = System.currentTimeMillis();
        if (GameUtils.hasShield && currentTime > GameUtils.shieldEndTime) {
            GameUtils.hasShield = false;
        }
        if (GameUtils.hasSpeedBoost && currentTime > GameUtils.speedBoostEndTime) {
            GameUtils.hasSpeedBoost = false;
        }
    }

    // 绘制自身的方法（添加护盾效果）
    public void paintSelf(Graphics g){
        logic();
        g.drawImage(img, x, y, width + GameUtils.count, height + GameUtils.count, null);

        // 如果有护盾，绘制护盾效果
        if (GameUtils.hasShield) {
            g.setColor(new Color(0, 255, 255, 100)); // 半透明蓝色
            g.fillOval(x - 10, y - 10, width + GameUtils.count + 20, height + GameUtils.count + 20);
        }
    }
    //获取自身矩形的方法,用于碰撞检测'
    public Rectangle getRec(){
        return new Rectangle(x,y,width+GameUtils.count,height+GameUtils.count);
    }
}
