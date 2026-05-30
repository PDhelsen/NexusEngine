#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class ObjectFlags : uint8
	{
		None = 0,

		Initialized = 1 << 0,
		Enabled = 1 << 1,
		EnabledInHierarchy = 1 << 2,
		Tickable = 1 << 3,
	};
}

NX_FLAG(NxEn::ObjectFlags, uint8)
