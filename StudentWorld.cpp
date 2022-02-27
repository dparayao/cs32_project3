#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    mainChar = nullptr;
    numActors = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //int curLevel = getLevel();
    //load in level
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    
    gameText << " Lives: " << getLives() << " Level: " << getLevel() << " Points: " << getScore();
    
    setGameStatText(gameText.str());
    
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find level01.txt data file" << endl;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "level01.txt is improperly formatted" << endl;
    }
    else if (result == Level::load_success)
    {
    cerr << "Successfully loaded level" << endl; Level::GridEntry ge;
        for(int i = 0; i < GRID_HEIGHT; i++)
        {
            for(int j = 0; j < GRID_WIDTH; j++)
            {
                ge = lev.getContentsOf(i, j);
                switch(ge)
                {
                    case Level::empty:
                        break;
                    case Level::peach:
                    {
                        mainChar = new Peach(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        break;
                    }
                    case Level::block:
                    {
                        Block *holdBlock = new Block(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(holdBlock);
                        break;
                    }
                    case Level::pipe:
                    {
                        Pipe *holdPipe = new Pipe(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(holdPipe);
                        break;
                    }
                    case Level::mushroom_goodie_block:
                    {
                        MushroomBlock *holdMB = new MushroomBlock(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(holdMB);
                        break;
                    }
                    case Level::flower_goodie_block:
                    {
                        FlowerBlock *holdFB = new FlowerBlock(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(holdFB);
                        break;
                    }
                    case Level::star_goodie_block:
                    {
                        StarBlock *holdSB = new StarBlock(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(holdSB);
                        break;
                    }
                    case Level::koopa:
                    {
                        Koopa *holdKoopa = new Koopa(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH, returnRandomDir());
                        actorList.push_back(holdKoopa);
                        break;
                    }
                    case Level::goomba:
                    {
                        Goomba *holdGoomba = new Goomba(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH, returnRandomDir());
                        actorList.push_back(holdGoomba);
                        break;
                    }
                }
            }
        }
    }
    
    //for each coordinate, check what object is there and store it
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    gameText.str("");
    writePowers();
    gameText << " Lives: " << getLives() << " Level: " << getLevel() << " Points: " << getScore() << " " << whatPowers;
    setGameStatText(gameText.str());
    
    //asks peach to do something
    mainChar->doSomething();
    
    //for each actor, in the world, ask the actor to do something
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end();)
    {
        if((*it)->isAlive() == true)
        {
            (*it)->doSomething();
            it++;
        }
        else if((*it)->isAlive() == false)
        {
            delete *it;
            it = actorList.erase(it);
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //TO DO: FIX DESTRUCTORS
    //deletes peach
    delete mainChar;
    mainChar = nullptr;
    
    //deletes actors
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        delete (*it);
        //it = actorList.erase(it);
    }
    actorList.clear();
}

Actor* StudentWorld::objectAt(int x, int y)
{
    //iterates through actor list
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        int a_x;
        int a_y;
        
        //checks for actor's coordinates
        a_x = (*it)->getX();
        a_y = (*it)->getY();
        
        //if it matches, return the actor who's hitbox overlaps
        //if(x <= (a_x + SPRITE_WIDTH - 1) && x > a_x)
        if(x <= (a_x + SPRITE_WIDTH - 1) && x >= a_x)
        {
            if(y <= (a_y + SPRITE_WIDTH - 1) && y >= a_y)
            {
                return *it;
            }
        }
    }

    //if it doesn't find an object at that location, return false
    return nullptr;
}

Actor* StudentWorld::objectBlockingAt(int x, int y)
{
    //iterates through actor list
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        int a_x;
        int a_y;
        
        //checks for actor's coordinates
        a_x = (*it)->getX();
        a_y = (*it)->getY();
        
        //if it matches, return the actor who's hitbox overlaps
        //if(x <= (a_x + SPRITE_WIDTH - 1) && x > a_x)
        if(x <= (a_x + SPRITE_WIDTH - 1) && x >= a_x)
        {
            if(y <= (a_y + SPRITE_WIDTH - 1) && y >= a_y)
            {
                if((*it)->isBlocking()==true)
                {
                    return *it;
                }
            }
        }
    }

    //if it doesn't find an object at that location, return false
    return nullptr;
}

Actor* StudentWorld::damageableObjectAt(int x, int y)
{
    //iterates through actor list
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        int a_x;
        int a_y;
        
        //checks for actor's coordinates
        a_x = (*it)->getX();
        a_y = (*it)->getY();
        
        //if it matches, return the actor who's hitbox overlaps
        //if(x <= (a_x + SPRITE_WIDTH - 1) && x > a_x)
        if(x <= (a_x + SPRITE_WIDTH - 1) && x >= a_x)
        {
            if(y <= (a_y + SPRITE_WIDTH - 1) && y >= a_y)
            {
                if((*it)->isDamageable()==true)
                {
                    return *it;
                }
            }
        }
    }

    //if it doesn't find an object at that location, return false
    return nullptr;
}

bool StudentWorld::isPeachAt(int x, int y)
{
    int p_x;
    int p_y;
    
    p_x = mainChar->getX();
    p_y = mainChar->getY();
    
    if(x <= (p_x + SPRITE_WIDTH - 1) && x >= p_x)
    {
        if(y <= (p_y + SPRITE_WIDTH - 1) && y >= p_y)
        {
            return true;
        }
    }
    
    return false;
}

Peach* StudentWorld::givePeach()
{
    return mainChar;
}

void StudentWorld::addActor(Actor *holdActor)
{
    actorList.push_back(holdActor);
}

void StudentWorld::addActorToFront(Actor* holdActor)
{
    actorList.push_front(holdActor);
}

void StudentWorld::addText(string text)
{
    gameText << text;
    
}

void StudentWorld::writePowers()
{
    whatPowers = "";
    
    if(mainChar->ifStarPower())
    {
        whatPowers += "StarPower! ";
    }
    if(mainChar->ifJumpPower())
    {
        whatPowers += "JumpPower! ";
    }
    if(mainChar->ifShootPower())
    {
        whatPowers += "ShootPower! ";
    }
}

int StudentWorld::returnRandomDir()
{
    int chooseDir = randInt(0, 1);
    int actualDir;
    if(chooseDir == 0)
    {
        actualDir = 0;
    }
    else
    {
        actualDir = 180;
    }
    
    return actualDir;
}
