#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct WorldObjectReferences
	{
		static WorldObjectReferences* GetReferences();
		static NxFr::GUID Resolve(NxFr::GUID Id);

		WorldObjectReferences();
		~WorldObjectReferences();

		NxFr::Dictionary<NxFr::GUID, NxFr::GUID> Ids;
	};
}

