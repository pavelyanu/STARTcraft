#include <bwem.h>

#include "AIModule.h"
#include "Utils.h"
#include "MapTools.h"

#include <iostream>


AIModule::AIModule()
{
    frameCounter = 0;
}

// Called when the bot starts!
void AIModule::onStart()
{
	BWAPI::Broodwar->setLocalSpeed(0);
    //BWAPI::Broodwar->setFrameSkip(0);
    auto& theMap = BWEM::Map::Instance();
    theMap.Initialize(BWAPI::BroodwarPtr);
    theMap.EnableAutomaticPathAnalysis();
    if (bool startingLocationsOK = theMap.FindBasesForStartingLocations())
    {
        std::cout << "Starting locations initialized\n";
    }
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
    manager.OnStart();
    manager.maptools = &maptools;
    planner.manager = &manager;
    planner.OnStart();
    maptools.onStart();
}

// Called whenever the game ends and tells you if you won or not
void AIModule::onEnd(bool isWinner) 
{
    std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void AIModule::onFrame()
{
    Utils::TrainWorkers(30);
    drawDebugInformation();
    if (frameCounter < 0)
    {
		planner.OnFrame();
        manager.OnFrame();
        frameCounter += 30;
    }
    frameCounter--;
    maptools.onFrame();
}

void AIModule::drawDebugInformation()
{
    Utils::DrawUnitCommands();
    Utils::DrawUnitIDs();
    manager.DrawRoles();
    Utils::DrawExpansionBase();
}

void AIModule::onUnitDestroy(BWAPI::Unit unit)
{
	
}

void AIModule::onSendText(std::string text) 
{ 
    if (text == "/map") maptools.toggleDraw();
}

void AIModule::onUnitCreate(BWAPI::Unit unit)
{ 
	
}

void AIModule::onUnitComplete(BWAPI::Unit unit)
{
    planner.OnUnitComplete(unit);
    manager.OnUnitComplete(unit);
}

void AIModule::onUnitShow(BWAPI::Unit unit)
{ 
	
}

void AIModule::onUnitHide(BWAPI::Unit unit)
{ 
	
}

void AIModule::onUnitRenegade(BWAPI::Unit unit)
{ 
	
}

void AIModule::onUnitMorph(BWAPI::Unit unit)
{

}