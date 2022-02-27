#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Peach;
class Block;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //finding objects at
    Actor* objectAt(int x, int y);
    Actor* objectBlockingAt(int x, int y);
    Actor* damageableObjectAt(int x, int y);
    bool isPeachAt(int x, int y);
    
    //accessors
    Peach* givePeach();
    bool getLevelStatus();
    bool getGameStatus();
    
    //mutators
    void addActor(Actor* holdActor);
    void addActorToFront(Actor* holdActor);
    void addText(string text);
    void writePowers();
    void changeLevelStatus(bool status);
    void changeGameStatus(bool status);
    
    //helpful
    int returnRandomDir();
private:
    list<Actor*> actorList;
    int numActors;
    Peach* mainChar;
    ostringstream gameText;
    string whatPowers = "";
    bool ifFinishLevel = false;
    bool ifFinishGame = false;
};

#endif // STUDENTWORLD_H_
