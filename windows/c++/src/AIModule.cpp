#include "AIModule.h"
#include "Utils.h"
#include <bwem.h>

AIModule::AIModule()
{
    frameCounter = 0;
}

// Called when the bot starts!
void AIModule::onStart()
{
	//BWAPI::Broodwar->setLocalSpeed(5);
    //BWAPI::Broodwar->setFrameSkip(0);
    BWEM::Map::Instance().Initialize(BWAPI::BroodwarPtr);
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
    planner.OnStart();
}

// Called whenever the game ends and tells you if you won or not
void AIModule::onEnd(bool isWinner) 
{
    std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void AIModule::onFrame()
{
    Utils::TrainWorkers(20);
    drawDebugInformation();
    if (frameCounter < 0)
    {
		planner.OnFrame();
        frameCounter += 20;
    }
    frameCounter--;
}

void AIModule::drawDebugInformation()
{
    Utils::DrawUnitCommands();
}

void AIModule::onUnitDestroy(BWAPI::Unit unit)
{
	
}

void AIModule::onSendText(std::string text) 
{ 

}

void AIModule::onUnitCreate(BWAPI::Unit unit)
{ 
	
}

void AIModule::onUnitComplete(BWAPI::Unit unit)
{
    planner.OnUnitComplete(unit);
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