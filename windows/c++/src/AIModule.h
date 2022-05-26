#pragma once

#include "Planner.h"

#include <BWAPI.h>

class AIModule
{

public:

    AIModule();

    void drawDebugInformation();
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);

	Planner planner;
	int frameCounter;
};