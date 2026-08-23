#include "IO/System/TypeRegistry.hpp"

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>

#include <Core/World.hpp>

#include <Features/Units/Swordsman.hpp>

#include <fstream>
#include <iostream>
#include <optional>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}
	
	EventLog eventLog;
	io::CommandParser parser;

	std::optional<core::World> world;

	parser.add<io::CreateMap>([&](auto command) {
		world.emplace(command.width, command.height);
		eventLog.log(0, io::MapCreated{command.width, command.height});
	});

	parser.add<io::SpawnSwordsman>([&](auto command) {
		auto unit = features::units::Swordsman::create(command.unitId, command.hp, command.strength);
		world->spawnUnit(std::move(unit), {command.x, command.y});
		eventLog.log(0, io::UnitSpawned{command.unitId, "Swordsman", command.x, command.y});
	});
	parser.add<io::SpawnHunter>([&](auto command) {
	//	auto unit = features::units::Hunter::create(command.unitId, command.hp, command.strength);
	//	world->spawnUnit(std::move(unit), {command.x, command.y});
		eventLog.log(0, io::UnitSpawned{command.unitId, "Hunter", command.x, command.y});
	});
	parser.add<io::March>([&](auto command) {

		eventLog.log(1, io::MarchStarted{1, 0, 0, 9, 0});
		printDebug(std::cout, command);
	});

	parser.parse(file);

	return 0;
}
