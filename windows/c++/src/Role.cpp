#include "Role.h"

#include <BWAPI.h>
#include <bwem.h>

#include <memory>

#include "Command.h"
#include "Utils.h"

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

const Command* Role::GetCurrentCommand()
{
	return commands.back().get();
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
}

Worker::Worker(BWAPI::Unit _self, BWAPI::Unit target)
{
	self = _self;
	command = nullptr;
	PushCommand(std::move(std::make_unique<Gather>(self, target)));
}

void Worker::OnFrame()
{
	if ((command == nullptr || command.get()->IsDone()) && !commands.empty())
	{
		PopNext();
	}
	else if (commands.empty())
	{
		PushCommand(std::move(std::make_unique<Gather>(self)));
	}
	else if (!command.get()->IsDone())
	{
		command.get()->Execute();
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
		self->getType().sightRange()
	);
	if (enemy_unit != nullptr && enemy_unit->exists())
	{
		if (!attacking)
		{
			BWAPI::Broodwar->setLocalSpeed(111);
			PushCommand(std::move(std::make_unique<Attack>(self, enemy_unit)));
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
--------------------Spy--------------------
*/

Spy::Spy(BWAPI::Unit _self)
{
	self = _self;
}

void Spy::OnFrame()
{
}

Spy::~Spy()
{
}

/*
--------------------Observer--------------------
*/

Observer::Observer(BWAPI::Unit _self)
{
	self = _self;
}

void Observer::OnFrame()
{
}

Observer::~Observer()
{
}

/*
--------------------Attacker--------------------
*/

Attacker::Attacker(BWAPI::Unit _self)
{
	self = _self;
}

void Attacker::OnFrame()
{
}

Attacker::~Attacker()
{
}
