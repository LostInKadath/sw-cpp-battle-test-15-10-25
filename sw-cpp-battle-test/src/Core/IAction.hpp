#pragma once

namespace sw::core
{
	struct Unit;
	struct World;

	struct IAction
	{
		virtual ~IAction() = default;

		virtual bool tryAct(Unit&, World&) = 0;
	};
}
