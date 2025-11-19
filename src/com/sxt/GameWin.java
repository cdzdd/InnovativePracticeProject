package com.sxt;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class GameWin extends JFrame {

    /** 游戏状态 0未开始,1游戏中,2通关失败,3通关成功,4暂停,5重新开始*/
    static int state = 0;

    Image offScreenImage;
    int width = 1440;
    int height = 900;
    double random;
    int time = 0;

    //背景
    Bg bg = new Bg();

    //敌方鱼类
    Enamy enamy;
    //是否生成boss
    boolean isboss = false;

    //boss类
    Enamy boss;

    //我方鱼类
    MyFish myFish = new MyFish();

    public void launch(){
        this.setVisible(true);
        this.setSize(width,height);
        this.setLocationRelativeTo(null);
        this.setTitle("大鱼吃小鱼");
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        this.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (e.getButton()==1&&state==0){
                    state=1;
                    repaint();
                }
                if (e.getButton()==1&&(state==2||state==3)){
                    reGame();
                    state = 1;
                }
            }
        });

        //键盘移动
        this.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                super.keyPressed(e);
                //WASD
                if (e.getKeyCode()==87){
                    GameUtils.UP = true;
                }
                if (e.getKeyCode()==83){
                    GameUtils.DOWN = true;
                }
                if (e.getKeyCode()==65){
                    GameUtils.LEFT = true;
                }
                if (e.getKeyCode()==68){
                    GameUtils.RIGHT = true;
                }
                if (e.getKeyCode()==32){
                    switch (state){
                        case 1:
                            state = 4;
                            break;
                        case 4:
                            state =1;
                            break;
                    }
                }
            }

            @Override
            public void keyReleased(KeyEvent e){
                super.keyReleased(e);
                if (e.getKeyCode()==87){
                    GameUtils.UP = false;
                }
                if (e.getKeyCode()==83){
                    GameUtils.DOWN = false;
                }
                if (e.getKeyCode()==65){
                    GameUtils.LEFT = false;
                }
                if (e.getKeyCode()==68){
                    GameUtils.RIGHT = false;
                }
            }
        });

        while (true){
            repaint();
            time++;
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void paint(Graphics g) {
        if (offScreenImage == null) {
            offScreenImage = createImage(width,height);
        }
        Graphics gImage = offScreenImage.getGraphics();
        bg.paintSelf(gImage , myFish.level);

        switch (state){
            case 0:
                // 开始界面
                GameUtils.drawWord(gImage, "点击开始游戏", Color.red, 80, 500, 450);
                break;
            case 1:
                logic();
                myFish.paintSelf(gImage);
                for (Enamy enamy:GameUtils.EnamyList) {
                    enamy.paintSelf(gImage);
                }
                // 绘制道具
                for (PowerUp powerUp : GameUtils.PowerUpList) {
                    powerUp.paintSelf(gImage);
                }
                if (isboss && boss != null){
                    boss.x = boss.x + boss.dir * boss.speed;
                    boss.paintSelf(gImage);
                }
                break;
            case 2:
                // 游戏失败
                GameUtils.drawWord(gImage, "游戏失败!", Color.red, 80, 600, 400);
                GameUtils.drawWord(gImage, "点击重新开始", Color.orange, 40, 580, 500);
                break;
            case 3:
                // 游戏胜利
                GameUtils.drawWord(gImage, "游戏胜利!", Color.green, 80, 600, 400);
                GameUtils.drawWord(gImage, "点击重新开始", Color.orange, 40, 580, 500);
                break;
            case 4:
                // 暂停
                GameUtils.drawWord(gImage, "游戏暂停", Color.blue, 80, 600, 400);
                GameUtils.drawWord(gImage, "按空格继续", Color.orange, 40, 600, 500);
                break;
            default:
        }

        g.drawImage(offScreenImage,0,0,null);
    }

    void logic(){
        //关卡难度
        checkShieldExpiration();
        if (GameUtils.count < 5){
            GameUtils.level = 0;
            myFish.level = 1;
        } else if (GameUtils.count <= 15){
            GameUtils.level = 1;
            myFish.level = 1;
        } else if (GameUtils.count <= 50){
            GameUtils.level = 2;
            myFish.level = 2;
        } else if (GameUtils.count <= 150){
            GameUtils.level = 3;
            myFish.level = 3;
        } else if (GameUtils.count <= 200){
            GameUtils.level = 4;
            myFish.level = 3;
        } else if (GameUtils.count > 200){
            state = 3;
        }

        random = Math.random();

        // 道具生成逻辑（每3-5秒有10%几率生成道具）
        if (time % 50 == 0 && Math.random() < 0.5) {
            generatePowerUp();
        }

        // 道具移动和碰撞检测
        for (int i = GameUtils.PowerUpList.size() - 1; i >= 0; i--) {
            PowerUp powerUp = GameUtils.PowerUpList.get(i);
            powerUp.move();

            // 检测与玩家的碰撞
            if (myFish.getRec().intersects(powerUp.getRect())) {
                applyPowerUpEffect(powerUp);
                GameUtils.PowerUpList.remove(i);
            }

            // 移除超出屏幕的道具
            if (powerUp.y > height) {
                GameUtils.PowerUpList.remove(i);
            }
        }

        // 生成敌人的逻辑
        switch (GameUtils.level) {
            case 4:
                if (time % 60 == 0 && random > 0.5 && !isboss){
                    boss = new Enamy_Boss();
                    isboss = true;
                }
                // 注意：这里没有break，会继续执行下面的case
            case 3:
                if (time % 30 == 0 && random > 0.5){
                    if (random > 0.5){
                        enamy = new Enamy_3_L();
                    } else {
                        enamy = new Enamy_3_R();
                    }
                    GameUtils.EnamyList.add(enamy);
                }
                // 注意：这里没有break，会继续执行下面的case
            case 2:
                if (time % 20 == 0 && random > 0.5){
                    if (random > 0.5){
                        enamy = new Enamy_2_L();
                    } else {
                        enamy = new Enamy_2_R();
                    }
                    GameUtils.EnamyList.add(enamy);
                }
                // 注意：这里没有break，会继续执行下面的case
            case 1:
                if (time % 20 == 0 && random > 0.5){
                    if (random > 0.5){
                        enamy = new Enamy_2_L();
                    } else {
                        enamy = new Enamy_2_R();
                    }
                    GameUtils.EnamyList.add(enamy);
                }
                // 注意：这里没有break，会继续执行下面的case
            case 0:
                if (time % 10 == 0 && random > 0.5) {
                    if (random > 0.5) {
                        enamy = new Enamy_1_L();
                    } else {
                        enamy = new Enamy_1_R();
                    }
                    GameUtils.EnamyList.add(enamy);
                }
                break;
            default:
        }

        // 碰撞检测
        for (Enamy enamy : GameUtils.EnamyList) {
            enamy.x = enamy.x + enamy.dir * enamy.speed;

            // 我方鱼与敌方鱼碰撞检测
            if (myFish.getRec().intersects(enamy.getRec())) {
                if (GameUtils.hasShield && System.currentTimeMillis() <= GameUtils.shieldEndTime) {
                    // 有有效护盾时免疫伤害，护盾消失，敌人被移除
                    GameUtils.hasShield = false;
                    enamy.x = -200;
                    enamy.y = -200;
                    GameUtils.count += enamy.count; // 吃掉敌人还能得分！
                } else if (myFish.level >= enamy.type) {
                    // 吃掉敌人
                    enamy.x = -200;
                    enamy.y = -200;
                    GameUtils.count = GameUtils.count + enamy.count;
                } else {
                    // 被敌人吃掉
                    state = 2;
                }
                // 在 logic() 方法的碰撞检测部分
                if (GameUtils.hasShield) {
                    GameUtils.hasShield = false;
                    System.out.println("护盾被消耗！");
                }
            }
        }

        // Boss碰撞检测
        if (isboss && boss != null) {
            if (boss.getRec().intersects(myFish.getRec())) {
                if (GameUtils.hasShield && System.currentTimeMillis() <= GameUtils.shieldEndTime) {
                    // 有有效护盾时免疫Boss伤害，护盾消失
                    GameUtils.hasShield = false;
                } else {
                    state = 2;
                }
                // 在 logic() 方法的碰撞检测部分
                if (GameUtils.hasShield) {
                    GameUtils.hasShield = false;
                    System.out.println("护盾被消耗！");
                }
            }
        }
    }

    void checkShieldExpiration() {
        if (GameUtils.hasShield && System.currentTimeMillis() > GameUtils.shieldEndTime) {
            GameUtils.hasShield = false;
        }
    }
    // 生成道具
    void generatePowerUp() {
        int x = (int)(Math.random() * (width - 50));
        PowerUpType type;
        double rand = Math.random();

        if (rand < 0.33) {
            type = PowerUpType.SPEED;
        } else if (rand < 0.66) {
            type = PowerUpType.SHIELD;
        } else {
            type = PowerUpType.XP;
        }

        PowerUp powerUp = new PowerUp(x, -50, type);
        GameUtils.PowerUpList.add(powerUp);
    }

    // 应用道具效果
    void applyPowerUpEffect(PowerUp powerUp) {
        long currentTime = System.currentTimeMillis();

        switch (powerUp.type) {
            case SPEED:
                GameUtils.hasSpeedBoost = true;
                GameUtils.speedBoostEndTime = currentTime + GameUtils.SPEED_BOOST_DURATION;
                break;
            case SHIELD:
                GameUtils.hasShield = true;
                GameUtils.shieldEndTime = currentTime + GameUtils.SHIELD_DURATION;
                System.out.println("获得护盾！持续时间：" + GameUtils.SHIELD_DURATION + "ms");
                break;
            case XP:
                // 直接增加积分
                GameUtils.count += 10;
                break;
        }
    }

    //重新开始
    void reGame(){
        GameUtils.EnamyList.clear();
        GameUtils.PowerUpList.clear(); // 清空道具列表
        GameUtils.hasShield = false; // 重置护盾状态
        GameUtils.hasSpeedBoost = false; // 重置加速状态
        time = 0;
        myFish.level = 1;
        GameUtils.count = 0;
        myFish.x = 700;
        myFish.y = 500;
        myFish.width = 50;
        myFish.height = 50;
        boss = null;
        isboss = false;
    }

    public static void main(String[] args) {
        GameWin gameWin = new GameWin();
        gameWin.launch();
    }
}