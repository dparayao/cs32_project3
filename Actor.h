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
    Actor(int imageID, int startX, int startY, int startDir, bool s_status, int depth, double size, bool ifBlocks) : GraphObject(imageID, startX, startY, startDir, depth, size)
    {
        ifAlive = s_status;
        ifBlocking = ifBlocks;
    };
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    virtual void bonk() {};
    bool isAlive();
    bool isBlocking();
    //TO DO:
    //if damageable object function
    void setAliveStatus(bool status);
private:
    bool ifAlive;
    bool ifBlocking;
};

//class declarations for Block class
class baseBlock: public Actor
{
public:
    baseBlock(StudentWorld* sw, int imgId, int startX, int startY) : Actor(imgId, startX, startY, 0, true, 2, 1.0, true)
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

class Pipe: public baseBlock
{
public:
    Pipe(StudentWorld* sw, int startX, int startY) : baseBlock(sw, IID_PIPE, startX, startY)
    {
        p_world = sw;
    }
    virtual ~Pipe() {};
private:
    StudentWorld* p_world;
};

class Goodie: public Actor
{
public:
    Goodie(StudentWorld* sw, int imgID, int startX, int startY): Actor(imgID, startX, startY, 0, true, 1, 1.0, false)
    {
        g_world = sw;
    };
    virtual ~Goodie() {};
    void doSomething();
    //TO DO: give power up function, called by above function, will be pure virtual
    virtual void givePowerUp() = 0;
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

//class declarations for peach class
class Peach: public Actor
{
public:
    Peach(StudentWorld* sw, int startX, int startY) : Actor(IID_PEACH, startX, startY, 0, true, 0, 1.0, false)
    {
        p_world = sw;
        hp = 1;
        remaining_jump_distance = 0;
    };
    virtual ~Peach() {};
    void doSomething();
    void jump();
private:
    StudentWorld* p_world;
    int hp;
    int remaining_jump_distance;
};


#endif // ACTOR_H_
