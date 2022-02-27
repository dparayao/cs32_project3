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
    void doSomething() = 0;
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
    //give power up function, called by above function, will be pure virtual
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
    void doSomething();
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
    };
    virtual ~Peach() {};
    
    //gameplay functions
    void doSomething();
    void jump();
    
    //accessors
    int ifShootPower();
    int ifJumpPower();
    int ifStarPower();
    
    //mutators
    void setPower(int power);
private:
    StudentWorld* p_world;
    int hp;
    int remaining_jump_distance;
    bool shootPower;
    bool jumpPower;
    bool starPower;
    //determines if peach is in recharge mode
    int time_to_recharge_before_next_fire;
};


#endif // ACTOR_H_
