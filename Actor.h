#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Goodie;

//class declarations for (base) Actor class
class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int startDir, bool s_status, int depth, double size, bool ifBlocks, bool ifDamage) : GraphObject(imageID, startX, startY, startDir, depth, size)
    {
        ifAlive = s_status;
        ifBlocking = ifBlocks;
        ifDamageable = ifDamage;
    };
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    virtual void bonk() {};
    virtual void getsHit() {};
    
    //accessors
    bool isAlive();
    bool isBlocking();
    bool isDamageable();
    
    //mutators
    void setAliveStatus(bool status);
private:
    bool ifAlive;
    bool ifBlocking;
    bool ifDamageable;
};

//class declarations for Block class
class baseBlock: public Actor
{
public:
    baseBlock(StudentWorld* sw, int imgId, int startX, int startY) : Actor(imgId, startX, startY, 0, true, 2, 1.0, true, false)
    {
        b_world = sw;
    };
    virtual ~baseBlock() {};
    virtual void bonk();
    void doSomething();
private:
    StudentWorld* b_world;
};

class Block: public baseBlock
{
public:
    Block(StudentWorld* sw, int startX, int startY) : baseBlock(sw, IID_BLOCK, startX, startY)
    {
        b_world = sw;
        hasGoodie = false;
    }
    virtual ~Block() {};
private:
    StudentWorld* b_world;
    bool hasGoodie;
};

class GoodieBlock: public Block
{
public:
    GoodieBlock(StudentWorld* sw, int startX, int startY) : Block(sw, startX, startY)
    {
        g_world = sw;
        hasGoodie = true;
    }
    virtual ~GoodieBlock() {};
    virtual void releaseGoodie() = 0;
    void bonk();
    void setGoodieStatus(bool status);
    bool getGoodieStatus();
private:
    StudentWorld* g_world;
    bool hasGoodie;
};

class MushroomBlock: public GoodieBlock
{
public:
    MushroomBlock(StudentWorld* sw, int startX, int startY) : GoodieBlock(sw, startX, startY)
    {
        m_world = sw;
    }
    virtual ~MushroomBlock() {};
    void releaseGoodie();
private:
    StudentWorld* m_world;
};

class FlowerBlock: public GoodieBlock
{
public:
    FlowerBlock(StudentWorld* sw, int startX, int startY) : GoodieBlock(sw, startX, startY)
    {
        f_world = sw;
    };
    virtual ~FlowerBlock() {};
    void releaseGoodie();
private:
    StudentWorld* f_world;
};

class StarBlock: public GoodieBlock
{
public:
    StarBlock(StudentWorld* sw, int startX, int startY) : GoodieBlock(sw, startX, startY)
    {
        s_world = sw;
    };
    virtual ~StarBlock() {};
    void releaseGoodie();
private:
    StudentWorld* s_world;
};

class Pipe: public baseBlock
{
public:
    Pipe(StudentWorld* sw, int startX, int startY) : baseBlock(sw, IID_PIPE, startX, startY)
    {
        p_world = sw;
    };
    virtual ~Pipe() {};
private:
    StudentWorld* p_world;
};

class movingActors: public Actor
{
public:
    movingActors(StudentWorld* sw, int imgID, int startX, int startY, int startDir): Actor(imgID, startX, startY, startDir, true, 1, 1.0, false, false)
    {
        m_world = sw;
    };
    virtual ~movingActors() {};
    void movement();
    virtual void meetsBlock() = 0;
private:
    StudentWorld* m_world;
};

class Goodie: public movingActors
{
public:
    Goodie(StudentWorld* sw, int imgID, int startX, int startY): movingActors(sw, imgID, startX, startY, 0)
    {
        g_world = sw;
    };
    virtual ~Goodie() {};
    void doSomething();
    virtual void givePowerUp() = 0;
    void meetsBlock();
private:
    StudentWorld* g_world;
};

class Mushroom: public Goodie
{
public:
    Mushroom(StudentWorld* sw, int startX, int startY): Goodie(sw, IID_MUSHROOM, startX, startY)
    {
        m_world = sw;
    };
    virtual ~Mushroom() {};
    void givePowerUp();
private:
    StudentWorld* m_world;
};

class Flower: public Goodie
{
public:
    Flower(StudentWorld* sw, int startX, int startY): Goodie(sw, IID_FLOWER, startX, startY)
    {
        f_world = sw;
    };
    virtual ~Flower() {};
    void givePowerUp();
private:
    StudentWorld* f_world;
};

class Star: public Goodie
{
public:
    Star(StudentWorld* sw, int startX, int startY): Goodie(sw, IID_STAR, startX, startY)
    {
        s_world = sw;
    };
    virtual ~Star() {};
    void givePowerUp();
private:
    StudentWorld* s_world;
};

class Projectile: public movingActors
{
public:
    Projectile(StudentWorld* sw, int imgID, int startX, int startY, int startDir): movingActors(sw, imgID, startX, startY, startDir)
    {
        p_world = sw;
    };
    virtual ~Projectile() {};
    virtual void doSomething();
    void meetsBlock();
private:
    StudentWorld* p_world;
};

class PeachFireball: public Projectile
{
public:
    PeachFireball(StudentWorld* sw, int startX, int startY, int startDir): Projectile(sw, IID_PEACH_FIRE, startX, startY, startDir)
    {
        p_world = sw;
    };
    virtual ~PeachFireball() {};
private:
    StudentWorld* p_world;
};

class Shell: public Projectile
{
public:
    Shell(StudentWorld* sw, int startX, int startY, int startDir): Projectile(sw, IID_SHELL, startX, startY, startDir)
    {
        s_world = sw;
    };
    virtual ~Shell() {};
private:
    StudentWorld* s_world;
};

class PiranhaFireball: public Projectile
{
public:
    PiranhaFireball(StudentWorld* sw, int startX, int startY, int startDir): Projectile(sw, IID_PIRANHA_FIRE, startX, startY, startDir)
    {
        p_world = sw;
    };
    virtual ~PiranhaFireball() {};
    void doSomething();
private:
    StudentWorld* p_world;
};

class movingEnemy: public Actor
{
public:
    movingEnemy(StudentWorld* sw, int imgID, int startX, int startY, int startDir): Actor(imgID, startX, startY, startDir, true, 0, 1.0, false, true)
    {
        e_world = sw;
    }
    virtual ~movingEnemy() {};
    void bonk();
    void doSomething();
    virtual void attemptMove();
    void getsHit();
    virtual void afterHit() {};
private:
    StudentWorld* e_world;
};

class Koopa: public movingEnemy
{
public:
    Koopa(StudentWorld* sw, int startX, int startY, int startDir): movingEnemy(sw, IID_KOOPA, startX, startY, startDir)
    {
        k_world = sw;
    };
    virtual ~Koopa() {};
    void afterHit();
private:
    StudentWorld* k_world;
};

class Goomba: public movingEnemy
{
public:
    Goomba(StudentWorld* sw, int startX, int startY, int startDir): movingEnemy(sw, IID_GOOMBA, startX, startY, startDir)
    {
        g_world = sw;
    };
    virtual ~Goomba() {};
private:
    StudentWorld* g_world;
};

class Piranha: public movingEnemy
{
public:
    Piranha(StudentWorld* sw, int startX, int startY, int startDir): movingEnemy(sw, IID_PIRANHA, startX, startY, startDir)
    {
        p_world = sw;
        firingDelay = 0;
    };
    virtual ~Piranha() {};
    void attemptMove();
private:
    StudentWorld* p_world;
    int firingDelay;
};

class Flag: public Actor
{
public:
    Flag(StudentWorld* sw, int imgID, int startX, int startY): Actor(imgID, startX, startY, 0, true, 1, 1.0, false, false)
    {
        f_world = sw;
    };
private:
    StudentWorld* f_world;
    void doSomething();
    virtual void informGame();
};

class Mario: public Flag
{
public:
    Mario(StudentWorld* sw, int startX, int startY): Flag(sw, IID_MARIO, startX, startY)
    {
        m_world = sw;
    };
private:
    StudentWorld* m_world;
    void informGame();
};

//class declarations for peach class
class Peach: public Actor
{
public:
    Peach(StudentWorld* sw, int startX, int startY) : Actor(IID_PEACH, startX, startY, 0, true, 0, 1.0, false, true)
    {
        p_world = sw;
        hp = 1;
        remaining_jump_distance = 0;
        shootPower = false;
        jumpPower = false;
        starPower = false;
        ifInvincible = false;
        ifTempInvincible = false;
        starPowerTimeLeft = 0;
        tempInTimeLeft = 0;
    };
    virtual ~Peach() {};
    
    //gameplay functions
    void doSomething();
    void jump();
    void bonk();
    
    //accessors
    bool ifShootPower();
    bool ifJumpPower();
    bool ifStarPower();
    
    //mutators
    void setPower(int power);
    void setStarPower(int time);
    void setHP(int newHP);
private:
    StudentWorld* p_world;
    int hp;
    int remaining_jump_distance;
    bool shootPower;
    bool jumpPower;
    bool starPower;
    bool ifInvincible;
    bool ifTempInvincible;
    //determines if peach is in recharge mode
    int time_to_recharge_before_next_fire;
    //determines time left for star power
    int starPowerTimeLeft;
    int tempInTimeLeft;
};


#endif // ACTOR_H_
