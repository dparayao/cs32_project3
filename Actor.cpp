#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::isAlive()
{
    if(ifAlive == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Actor::isBlocking()
{
    return ifBlocking;
}

void Actor::setAliveStatus(bool status)
{
    ifAlive = status;
}

void baseBlock::doSomething()
{
    return;
    
    //doesn't do anything bc it's a block ?
}

void baseBlock::bonk()
{
    b_world->playSound(SOUND_PLAYER_BONK);
}

void GoodieBlock::bonk()
{
    if(getGoodieStatus() == true)
    {
        g_world->playSound(SOUND_POWERUP_APPEARS);
        releaseGoodie();
        setGoodieStatus(false);
    }
    else
    {
        g_world->playSound(SOUND_PLAYER_BONK);
    }
}

bool GoodieBlock::getGoodieStatus()
{
    return hasGoodie;
}

void GoodieBlock::setGoodieStatus(bool status)
{
    hasGoodie = status;
}

void MushroomBlock::releaseGoodie()
{
    Mushroom* holdM = new Mushroom(m_world, getX(), getY()+8);
    m_world->addActor(holdM);
}

void Goodie::doSomething()
{
    if(g_world->isPeachAt(getX(), getY()))
    {
        g_world->playSound(SOUND_PLAYER_POWERUP);
        givePowerUp();
        return;
    }
    else
    {
        if(!(g_world->objectBlockingAt(getX(), getY()-2)))
        {
            moveTo(getX(), getY()-2);
        }
        
        if(getDirection() == 0)
        {
            int rightHitbox = getX() + SPRITE_WIDTH - 1;
            if(g_world->objectBlockingAt(rightHitbox+2, getY()))
            {
                setDirection(180);
                return;
            }
            else
            {
                moveTo(getX()+2, getY());
            }
        }
        else if(getDirection() == 180)
        {
            if(g_world->objectBlockingAt(getX()-2, getY()))
            {
                setDirection(0);
                return;
            }
            else
            {
                moveTo(getX()-2, getY());
            }
        }
    }
}

void Mushroom::givePowerUp()
{
    setAliveStatus(false);
    m_world->playSound(SOUND_PLAYER_POWERUP);
    return;
}

void Peach::jump()
{
    int topHitbox = getY() + SPRITE_HEIGHT - 1;
    
    Actor* whosThere = p_world->objectBlockingAt(getX(), topHitbox + 4);
    
    if(whosThere == nullptr)
    {
        moveTo(getX(), getY()+4);
        remaining_jump_distance--;
    }
    else
    {
        whosThere->bonk();
        remaining_jump_distance = 0;
    }
    
}

void Peach::doSomething()
{
    //if peach is not alive, return immediately
    /*TO-DO: has issue, defaults to false
    if(isAlive()==false)
    {
        cout << "not alive" << endl;
    }
     */
    
    if(remaining_jump_distance > 0)
    {
        jump();
    }
    else
    {
        Actor* ifBlock = nullptr;
        
        for(int i = 0; i <= 3; i++)
        {
            ifBlock = p_world->objectBlockingAt(getX(), getY()-i);
            
            if(ifBlock != nullptr)
            {
                break;
            }
        }
        
        if(ifBlock == nullptr)
        {
            moveTo(getX(), getY()-4);
        }
    }
    
    int whatDir = 0;
    
    //checking if/what keystroke was pressed
    if(p_world->getKey(whatDir))
    {
        Actor* whosThere;
        
        switch(whatDir)
        {
            case KEY_PRESS_LEFT:
            {
                whosThere = p_world->objectBlockingAt(getX()-4, getY());
                //no hit box from left bc x is taken from left side
                //int leftHitbox = getX() - SPRITE_WIDTH + 1;
                setDirection(180);
                if(whosThere == nullptr)
                {
                    moveTo(getX()-4, getY());
                }
                else
                {
                    whosThere->bonk();
                }
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                int rightHitbox = getX() + SPRITE_WIDTH - 1;
                setDirection(0);
                whosThere = p_world->objectBlockingAt(rightHitbox+4, getY());
                
                if(whosThere == nullptr)
                {
                    moveTo(getX()+4, getY());
                }
                else
                {
                    whosThere->bonk();
                }
                break;
            }
            case KEY_PRESS_UP:
            {
                //check if there is an object that would block movement one pixel below her
                whosThere = p_world->objectBlockingAt(getX(), getY() - 1);
                
                if(whosThere != nullptr)
                {
                    remaining_jump_distance = 8;
                }
                break;
            }
        }
    }
    
    return;
}
