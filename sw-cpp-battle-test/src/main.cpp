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

#include <Core/Simulation.hpp>
#include <Core/SimulationObserver.hpp>

#include <fstream>
#include <iostream>
#include <optional>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
		throw std::runtime_error("Error: No file specified in command line argument");

	std::ifstream file(argv[1]);
	if (!file)
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	
	EventLog eventLog;
	io::CommandParser parser;

	sw::simulation::SimulationObserver observer(eventLog);

	sw::core::Simulation simulation(observer);

	parser.add<io::CreateMap>([&](auto command) {
		simulation.createMap(command.width, command.height);
	});

	parser.add<io::SpawnSwordsman>([&](auto command) {
		simulation.spawnSwordsman(command.unitId, command.x, command.y, command.hp, command.strength);
	});

	parser.add<io::SpawnHunter>([&](auto command) {
		simulation.spawnHunter(command.unitId, command.x, command.y, command.hp, command.agility, command.strength, command.range);
	});

	parser.add<io::March>([&](auto command) {
		simulation.marchUnit(command.unitId, command.targetX, command.targetY);
	});

	parser.parse(file);

	simulation.run();

	return 0;
}
