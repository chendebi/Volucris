#ifndef __volucris_event_h__
#define __volucris_event_h__

#include <Engine/Core/TypesHelp.h>
#include <vector>
#include <string>
#include <Engine/Core/Point.h>

namespace volucris
{
	struct Event
	{

	};

	struct DropEvent
	{
		Point pos;
		std::vector<std::string> files;
	};
}

#endif // !__volucris_event_h__
