#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class WorldObjectType : uint8
	{
		None = 0,

		GameObject = 1 << 0,
		Behaviour = 1 << 1,
		Component = 1 << 2,

		All = GameObject | Behaviour | Component
	};

	struct WorldObject
	{
		NxFr::GUID Id;
		WorldObjectType Type;
		uint64 Index;
		NxFr::Handle<Object> Handle;
	};
}

NX_FLAG(NxEn::WorldObjectType, uint8)
