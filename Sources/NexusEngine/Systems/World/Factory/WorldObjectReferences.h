#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct WorldObjectReferences
	{
	public:
		static NxFr::Context<WorldObjectReferences>& GetContexts();
		static NxFr::GUID Resolve(NxFr::GUID Id);

		NxFr::Dictionary<NxFr::GUID, NxFr::GUID> Ids;
	};
}

