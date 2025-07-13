#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Command
	{
	public:
		NEXUS_ENGINE_API Command(NxFr::StringId Id, NxFr::Delegate<void()> Callback);
		NEXUS_ENGINE_API ~Command();

		NEXUS_ENGINE_API void Invoke();

		NEXUS_ENGINE_API NxFr::StringId GetId() const { return Id; }

	private:
		NxFr::StringId Id;
		NxFr::Delegate<void()> Callback;
	};
}
