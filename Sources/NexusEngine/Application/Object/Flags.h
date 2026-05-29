#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class ObjectFlags : uint8
	{
		None = 0,

		Initialized = 1 << 0,
		Enabled = 1 << 1,
		Tickable = 1 << 2,

		Flag_1 = 1 << 3,
		Flag_2 = 1 << 4,
		Flag_3 = 1 << 5,
		Flag_4 = 1 << 6,
		Flag_5 = 1 << 7,
	};
}

NX_FLAG(NxEn::ObjectFlags, uint8)
