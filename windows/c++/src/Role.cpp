#include "Role.h"

#include <BWAPI.h>
#include <bwem.h>

#include <memory>
#include <iostream>

#include "Command.h"
#include "Utils.h"
#include "MapTools.h"

Role::Role()
{
}

Role::~Role()
{
}

void Role::PopCommand()
{
	commands.pop_back();
}

Command* Role::GetCurrentCommand()
{
	if (command)
	{
		return command.get();
	}
	else
	{
		return nullptr;
	}
}

void Role::ClearCommands()
{
	commands.clear();
}

void Role::PopNext()
{
	command = std::move(commands.back());
	commands.pop_back();
}

/*
--------------------Worker--------------------
*/

Worker::Worker(BWAPI::Unit _self)
{
	self = _self;
	command = nullptr;
	PushCommand(std::move(std::make_unique<Gather>(self)));
}

Worker::Worker(BWAPI::Unit _self, BWAPI::Unit target)
{
	self = _self;
	command = nullptr;
	PushCommand(std::move(std::make_unique<Gather>(self, target)));
}

void Worker::OnFrame()
{
	if (command == nullptr)
	{
		if (commands.empty())
		{
			PushCommand(std::move(std::make_unique<Gather>(self)));
		}
		else
		{
			PopNext();
		}
	}
	else if (command.get()->IsDone())
	{
		if (commands.empty())
		{
			PushCommand(std::move(std::make_unique<Gather>(self)));
		}
		else
		{
			PopNext();
		}
	}
	else if (!command.get()->IsDone())
	{
		bool result = command.get()->Execute();
	}
	

}

Worker::~Worker()
{
}

/*
--------------------Guard--------------------
*/

Guard::Guard(BWAPI::Unit _self)
{
	self = _self;
}

Guard::Guard(BWAPI::Unit _self, BWAPI::Position)
{
}

void Guard::OnFrame()
{
	if ((command == nullptr || command.get()->IsDone()) && !commands.empty())
	{
		PopNext();
	}
	else if (commands.empty())
	{
		auto& map = BWEM::Map::Instance();
		auto area = map.GetArea(BWAPI::Broodwar->self()->getStartLocation());
		auto chokePoints = area->ChokePoints();
		int time = BWAPI::Broodwar->elapsedTime();
		auto choke_point = chokePoints[time % chokePoints.size()];
		PushCommand(std::move(std::make_unique<Hold>(self)));
		PushCommand(std::move(std::make_unique<Move>(self, BWAPI::Position(choke_point->Center()))));
	}
	else if (!command.get()->IsDone())
	{
		command.get()->Execute();
	}
	auto enemy = BWAPI::Broodwar->enemy();
	auto enemy_unit = BWAPI::Broodwar->getClosestUnit(
		self->getPosition(), 
		BWAPI::Filter::GetPlayer == enemy,
		self->getType().sightRange() * 3
	);
	if (enemy_unit != nullptr && enemy_unit->exists())
	{
		if (!attacking)
		{
			PushCommand(std::move(std::make_unique<Attack>(self, enemy_unit)));
			attacking = true;
		}
	}
	else
	{
		attacking = false;
	}
}

Guard::~Guard()
{

}

/*
--------------------Observer--------------------
*/

Observer::Observer(BWAPI::Unit _self, MapTools* _maptools)
{
	self = _self;
	maptools = _maptools;
	frameCount = 0;
	MoveToNextDestination();
}

void Observer::OnFrame()
{
	if (!command)
	{ 
		MoveToNextDestination();
	}
	else if (command.get()->IsDone() || frameCount < 0)
	{
		MoveToNextDestination();
		frameCount += 20;
	}

	command.get()->Execute();
	frameCount--;
}

void Observer::MoveToNextDestination()
{
	unexplored = maptools->getUnexploredReachableTiles(self->getPosition());
	if (unexplored.empty())
	{
		PushCommand(std::move(std::make_unique<Hold>(self)));
	}
	else
	{
		std::map<int, BWAPI::Position> positions;
		for (auto& pos : unexplored)
		{
			int key = pos.getApproxDistance(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
			positions[key] = pos;
		}
		PushCommand(std::move(std::make_unique<Move>(self, positions.begin()->second)));
	}
}

Observer::~Observer()
{
}

/*
--------------------Spy--------------------
*/

Spy::Spy(BWAPI::Unit _self, MapTools* _maptools)
{
	self = _self;
	startPositions = BWAPI::Broodwar->getStartLocations();
	foundEnemyBase = false;
	MoveToNextDestination();
	maptools = _maptools;
}

void Spy::OnFrame()
{
	auto enemy = BWAPI::Broodwar->enemy();
	auto enemy_unit = BWAPI::Broodwar->getClosestUnit(
		self->getPosition(),
		BWAPI::Filter::GetPlayer == enemy && !BWAPI::Filter::IsCloaked,
		self->getType().sightRange()
	);
	int distance = self->getPosition().getApproxDistance(BWAPI::Position(destination));
	if (enemy_unit != nullptr && enemy_unit->exists() && distance < self->getType().sightRange() * 3)
	{
		foundEnemyBase = true;
		maptools->SetEnemyBase(destination);
	}

	if ((command == nullptr || command.get()->IsDone()) && !commands.empty())
	{
		PopNext();
	}
	else if (commands.empty())
	{
		if (!command)
		{
			if (foundEnemyBase)
			{
				PushCommand(std::move(std::make_unique<Hold>(self)));
			}
			else
			{
				MoveToNextDestination();
			}
		}
	}
	else if (!command.get()->IsDone())
	{
		bool result = command.get()->Execute();
	}
}

void Spy::MoveToNextDestination()
{
	if (startPositions.empty())
	{
		startPositions = BWAPI::Broodwar->getStartLocations();
	}
	destination = startPositions.at(0);
	startPositions.pop_front();
	PushCommand(std::move(std::make_unique<Move>(self, destination)));
}

Spy::~Spy()
{
}

/*
--------------------Attacker--------------------
*/

Attacker::Attacker(BWAPI::Unit _self, MapTools* _maptools)
{
	self = _self;
	maptools = _maptools;
	attacking = false;
PushCommand(std::move(std::make_unique<Move>(self, maptools->GetEnemyBase())));
}

void Attacker::OnFrame()
{	
auto enemy = BWAPI::Broodwar->enemy();
auto enemy_unit = BWAPI::Broodwar->getClosestUnit(
		self->getPosition(),
		BWAPI::Filter::GetPlayer == enemy && !BWAPI::Filter::IsCloaked,
		self->getType().sightRange() * 3
	);
	if (enemy_unit != nullptr && enemy_unit->exists())
	{
			PushCommand(std::move(std::make_unique<Attack>(self, enemy_unit)));
			command.get()->Execute();
			return;
	}

	if (command == nullptr)
	{
		if (commands.empty())
		{
			PushCommand(std::move(std::make_unique<Move>(self, maptools->GetEnemyBase())));
		}
		else
		{
			PopNext();
		}
	}
	else if (command.get()->IsDone())
	{
		if (commands.empty())
		{
			PushCommand(std::move(std::make_unique<Move>(self, maptools->GetEnemyBase())));
		}
		else
		{
			PopNext();
		}
	}
	if (!command.get()->IsDone())
	{
		bool result = command.get()->Execute();
	}


}

Attacker::~Attacker()
{
}
