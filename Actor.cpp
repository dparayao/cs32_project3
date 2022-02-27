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

bool Actor::isDamageable()
{
    return ifDamageable;
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

void FlowerBlock::releaseGoodie()
{
    Flower* holdF = new Flower(f_world, getX(), getY()+8);
    f_world->addActor(holdF);
}

void StarBlock::releaseGoodie()
{
    Star* holdS = new Star(s_world, getX(), getY()+8);
    s_world->addActor(holdS);
}

void movingActors::movement()
{
    if(!(m_world->objectBlockingAt(getX(), getY()-2)))
    {
        moveTo(getX(), getY()-2);
    }
    
    if(getDirection() == 0)
    {
        int rightHitbox = getX() + SPRITE_WIDTH - 1;
        if(m_world->objectBlockingAt(rightHitbox+2, getY()))
        {
            meetsBlock();
            return;
        }
        else
        {
            moveTo(getX()+2, getY());
        }
    }
    else if(getDirection() == 180)
    {
        if(m_world->objectBlockingAt(getX()-2, getY()))
        {
            meetsBlock();
            return;
        }
        else
        {
            moveTo(getX()-2, getY());
        }
    }
}

void Goodie::doSomething()
{
    int checkX = getX();
    
    if(getDirection() == 0)
    {
        checkX += SPRITE_WIDTH - 1;
    }
    
    if(g_world->isPeachAt(checkX, getY()))
    {
        g_world->playSound(SOUND_PLAYER_POWERUP);
        givePowerUp();
        //set peach's hit points to 2
        setAliveStatus(false);
        return;
    }
    else
    {
        movement();
    }
}

void Goodie::meetsBlock()
{
    if(getDirection() == 0)
    {
        setDirection(180);
    }
    else if(getDirection() == 180)
    {
        setDirection(0);
    }
}

void Mushroom::givePowerUp()
{
    m_world->givePeach()->setHP(2);
    //increase player's score by 75 points
    m_world->increaseScore(75);
    //tell peach object it has jump power
    m_world->givePeach()->setPower(1);
    return;
}

void Flower::givePowerUp()
{
    f_world->givePeach()->setHP(2);
    //increase player's score by 50 points
    f_world->increaseScore(50);
    //tell peach object it has shoot power
    f_world->givePeach()->setPower(2);
    return;
}

void Star::givePowerUp()
{
    s_world->givePeach()->setHP(2);
    //increase player's score by 100 points
    s_world->increaseScore(100);
    //tell peach object it has star power for 150 game ticks
    s_world->givePeach()->setPower(3);
    s_world->givePeach()->setStarPower(150);
    return;
}

void Projectile::doSomething()
{
    int checkX = getX();
    
    if(getDirection() == 0)
    {
        checkX += SPRITE_WIDTH - 1;
    }
    
    Actor* whosThere = p_world->damageableObjectAt(checkX, getY());
    
    if(whosThere != nullptr)
    {
        whosThere->getsHit();
        setAliveStatus(false);
        return;
    }
    else
    {
        movement();
    }

}

void Projectile::meetsBlock()
{
    setAliveStatus(false);
    return;
}

void movingEnemy::doSomething()
{
    //if enemy is not alive, return immediately
    if(isAlive()==false)
    {
        return;
    }
    
    //checks if overlaps w peach - if it does, bonks and returns
    int checkX = getX();
    
    if(getDirection() == 0)
    {
        checkX += SPRITE_WIDTH - 1;
    }
    
    if(e_world->isPeachAt(checkX, getY()))
    {
        e_world->givePeach()->bonk();
        return;
    }
    
    //checks to see if it can move without getting blocked
    if(getDirection() == 0)
    {
        if(e_world->objectBlockingAt(checkX + 1, getY()) || !e_world->objectBlockingAt(checkX+1, getY()-1))
        {
            setDirection(180);
        }
    }
    else if(getDirection() == 180)
    {
        if(e_world->objectBlockingAt(checkX - 1, getY()) || !e_world->objectBlockingAt(checkX-1, getY()-1))
        {
            setDirection(0);
        }
    }
    
    //attempts to move
    if(getDirection() == 0)
    {
        if(e_world->objectBlockingAt(checkX + 1, getY()))
        {
            return;
        }
        else
        {
            moveTo(getX() + 1, getY());
        }
    }
    else if(getDirection() == 180)
    {
        if(e_world->objectBlockingAt(checkX-1, getY()))
        {
            return;
        }
        else
        {
            moveTo(getX() - 1, getY());
        }
    }
    
    return;
}

void movingEnemy::getsHit()
{
    e_world->increaseScore(100);
    setAliveStatus(false);
    afterHit();
}

void movingEnemy::bonk()
{
    if(e_world->givePeach()->ifStarPower() == true)
    {
        e_world->playSound(SOUND_PLAYER_KICK);
        e_world->increaseScore(100);
        setAliveStatus(false);
    }
}

void Koopa::afterHit()
{
    //add shell to beginning of vector so it won't be iterated during current tick
    Shell *holdShell = new Shell(k_world, getX(), getY(), getDirection());
    k_world->addActorToFront(holdShell);
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
    if(isAlive()==false)
    {
        return;
    }
     
    //checking if she is invisible
    if(ifInvincible == true)
    {
        starPowerTimeLeft--;
    }
    if(starPowerTimeLeft == 0)
    {
        ifInvincible = false;
    }
    
    //checking if she is temp invincible
    //checking if she is invisible
    if(ifTempInvincible == true)
    {
        tempInTimeLeft--;
    }
    if(tempInTimeLeft == 0)
    {
        ifTempInvincible = false;
    }
    
    //checking if she is in recharge mode
    if(time_to_recharge_before_next_fire > 0)
    {
        time_to_recharge_before_next_fire--;
    }
    
    //check if she overlaps another object
    int checkX = getX();
    if(getDirection() == 0)
    {
        checkX += SPRITE_WIDTH - 1;
    }
    Actor* who = p_world->objectAt(checkX, getY());
    if(who != nullptr)
    {
        who->bonk();
    }
    
    //if she has jump distance left, jumps
    if(remaining_jump_distance > 0)
    {
        jump();
    }
    else
    {
        //if not, falls if needed
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
    
    //checking/initiating movement
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
                    if(ifJumpPower() == true)
                    {
                        remaining_jump_distance = 12;
                    }
                    else
                    {
                        remaining_jump_distance = 8;
                    }
                }
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if(ifShootPower() == true)
                {
                    if(time_to_recharge_before_next_fire <= 0)
                    {
                        p_world->playSound(SOUND_PLAYER_FIRE);
                        time_to_recharge_before_next_fire = 8;
                        int fireX = getX();
                        if(getDirection() == 0)
                        {
                            fireX += 4;
                        }
                        else if(getDirection() == 180)
                        {
                            fireX -= 4;
                        }
                        PeachFireball* holdPF = new PeachFireball(p_world, fireX, getY(), getDirection());
                        p_world->addActor(holdPF);
                    }
                }
                break;
            }
        }
    }
    
    return;
}

int Peach::ifShootPower()
{
    return shootPower;
}

int Peach::ifJumpPower()
{
    return jumpPower;
}

int Peach::ifStarPower()
{
    return starPower;
}

void Peach::bonk()
{
    if(ifInvincible == true || ifTempInvincible == true)
    {
        return;
    }
    else
    {
        hp--;
        tempInTimeLeft = 10;
        shootPower = false;
        jumpPower = false;
        
        if(hp >= 1)
        {
            p_world->playSound(SOUND_PLAYER_HURT);
        }
        else if(hp == 0)
        {
            setAliveStatus(false);
        }
    }
}

void Peach::setPower(int power)
{
    //1 means mushroom power
    //2 means shooting power
    //3 means star power
    switch(power)
    {
        case 1:
            jumpPower = true;
            ifTempInvincible = true;
            break;
        case 2:
            shootPower = true;
            ifTempInvincible = true;
            break;
        case 3:
            starPower = true;
            ifInvincible = true;
            break;
    }
}

void Peach::setStarPower(int time)
{
    starPowerTimeLeft = time;
}

void Peach::setHP(int newHP)
{
    hp = newHP;
}
