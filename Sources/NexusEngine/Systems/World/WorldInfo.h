#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct WorldObject
	{
		NxFr::GUID Id;
		uint64 Index;
		NxFr::Handle<Object> Handle;
	};
}

