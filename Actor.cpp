#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::returnStatus()
{
    return status;
}

void Block::doSomething()
{
    return;
    
    //doesn't do anything bc it's a block ?
}

void Peach::doSomething()
{
    int whatDir = 0;
    
    if(p_world->getKey(whatDir))
    {
        switch(whatDir)
        {
            case KEY_PRESS_LEFT:
            {
                //no hit box from left bc x is taken from left side
                //int leftHitbox = getX() - SPRITE_WIDTH + 1;
                setDirection(180);
                if(!(p_world->objectBlockingAt(getX()-4, getY())))
                {
                    moveTo(getX()-4, getY());
                }
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                int rightHitbox = getX() + SPRITE_WIDTH - 1;
                setDirection(0);
                if(!(p_world->objectBlockingAt(rightHitbox+4, getY())))
                {
                    moveTo(getX()+4, getY());
                }
                break;
            }
        }
    }
    
    return;
}
