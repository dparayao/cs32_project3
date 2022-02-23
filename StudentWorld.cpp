#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
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
                        mainChar = new Peach(this, i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        break;
                    case Level::block:
                        Block *hold = new Block(i*SPRITE_HEIGHT, j*SPRITE_WIDTH);
                        actorList.push_back(hold);
                        break;
                }
            }
        }
    }
    
    //for each coordinate, check what object is there and store it
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //asks peach to do something
    mainChar->doSomething();
    
    //for each actor, in the world, ask the actor to do something
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        if((*it)->returnStatus() == true)
        {
            (*it)->doSomething();
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //deletes peach
    delete mainChar;
    mainChar = nullptr;
    
    //deletes actors
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        delete (*it);
        it = actorList.erase(it);
    }
}

bool StudentWorld::objectBlockingAt(int x, int y)
{
    //iterates through actor list
    for(list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++)
    {
        int a_x;
        int a_y;
        
        //checks for actor's coordinates
        a_x = (*it)->getX();
        a_y = (*it)->getY();
        
        //if it matches, return true
        if(x <= (a_x + SPRITE_WIDTH - 1) && x > a_x)
        {
            //if(y <= (a_y + SPRITE_WIDTH - 1) && y >= a_y)
            if(y == a_y)
            {
                return true;
            }
        }
    }
    
    //if it doesn't find an object at that location, return false
    return false;
}
