package com.sxt;

import java.awt.*;

public class Bg {
    void paintSelf(Graphics g , int fishLevel){
        g.drawImage(GameUtils.bgimg,0,0,null);
        switch (GameWin.state){
            case 0:
                GameUtils.drawWord(g,"开始",Color.red,80,700,500);
                break;
            case 1:
                GameUtils.drawWord(g,"积分"+GameUtils.count,Color.ORANGE,50,200,120);
                GameUtils.drawWord(g,"难度"+GameUtils.level,Color.ORANGE,50,600,120);
                GameUtils.drawWord(g,"等级"+fishLevel,Color.ORANGE,50,1000,120);
                // 显示当前激活的增益效果
                int effectY = 160;
                if (GameUtils.hasShield) {
                    long remaining = (GameUtils.shieldEndTime - System.currentTimeMillis()) / 1000;
                    GameUtils.drawWord(g, "护盾:" + remaining + "秒", Color.CYAN, 30, 200, effectY);
                    effectY += 40;
                }
                if (GameUtils.hasSpeedBoost) {
                    long remaining = (GameUtils.speedBoostEndTime - System.currentTimeMillis()) / 1000;
                    GameUtils.drawWord(g, "加速:" + remaining + "秒", Color.YELLOW, 30, 200, effectY);
                }
                break;
            case 2:
                GameUtils.drawWord(g,"积分"+GameUtils.count,Color.ORANGE,50,200,120);
                GameUtils.drawWord(g,"难度"+GameUtils.level,Color.ORANGE,50,600,120);
                GameUtils.drawWord(g,"等级"+fishLevel,Color.ORANGE,50,1000,120);
                GameUtils.drawWord(g,"失败",Color.red,80,700,500);
                break;
            case 3:
                GameUtils.drawWord(g,"积分"+GameUtils.count,Color.ORANGE,50,200,120);
                GameUtils.drawWord(g,"难度"+GameUtils.level,Color.ORANGE,50,600,120);
                GameUtils.drawWord(g,"等级"+fishLevel,Color.ORANGE,50,1000,120);
                GameUtils.drawWord(g ,"胜利",Color.red,80,700,500);
                break;
            case 4:
                GameUtils.drawWord(g,"积分"+GameUtils.count,Color.ORANGE,50,200,120);
                GameUtils.drawWord(g,"难度"+GameUtils.level,Color.ORANGE,50,600,120);
                GameUtils.drawWord(g,"等级"+fishLevel,Color.ORANGE,50,1000,120);
                default:

        }
    }
}
