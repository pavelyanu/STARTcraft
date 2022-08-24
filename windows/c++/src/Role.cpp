#include "Role.h"

#include <BWAPI.h>
#include <bwem.h>

#include "Command.h"
#include "Utils.h"

#include <memory>

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

void Role::PushNext()
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
	PushCommand(Gather(self, target));
}

void Worker::OnFrame()
{
	if (commands.empty())
	{
		auto minerals = BWAPI::Broodwar->getMinerals();
		auto mineral = Utils::GetClosestUnitTo(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()), minerals);
		auto command = Gather(self, mineral);
		PushCommand(command);
	}
	if (command == nullptr || command.get()->IsDone()) PushNext();
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
	if (commands.empty())
	{
		auto& map = BWEM::Map::Instance();
		auto area = map.GetArea(BWAPI::Broodwar->self()->getStartLocation());
		auto choke_point = area->ChokePoints()[0];
		PushCommand(Hold(self));
		PushCommand(Move(self, BWAPI::Position(choke_point->Center())));
	}
	if (command == nullptr || command.get()->IsDone()) PushNext();
	auto enemy = BWAPI::Broodwar->enemy();
	auto unit = BWAPI::Broodwar->getClosestUnit(self->getPosition(), BWAPI::Filter::GetPlayer == enemy, 50);
	if (unit != nullptr && unit->exists())
	{
		PushCommand(Attack(self, unit));
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
