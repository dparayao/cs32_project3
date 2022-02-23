#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//class declarations for (base) Actor class
class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int startDir, bool s_status, int depth = 0, double size = 1.0) : GraphObject(imageID, startX, startY, startDir, depth = 0, size = 1.0)
    {
        status = s_status;
    };
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    bool returnStatus();
private:
     bool status;
};

//class declarations for Block class
class Block: public Actor
{
public:
    Block(int startX, int startY) : Actor(IID_BLOCK, startX, startY, true, 0, 2, 1.0)
    {
        hasReleased = false;
    };
    virtual ~Block() {};
    void doSomething();
private:
    //hasReleased says if the block has released a goodie or not
    int hasReleased;
};

//class declarations for peach class
class Peach: public Actor
{
public:
    Peach(StudentWorld* sw, int startX, int startY) : Actor(IID_PEACH, startX, startY, true, 0, 0, 1.0)
    {
        p_world = sw;
        hp = 1;
    };
    virtual ~Peach() {};
    void doSomething();
private:
    StudentWorld* p_world;
    int hp;
};


#endif // ACTOR_H_
