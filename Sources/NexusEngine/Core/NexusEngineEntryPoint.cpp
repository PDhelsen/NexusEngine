#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineEntryPoint.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEn
{
	namespace EntryPoint
	{
		void NxEn::EntryPoint::Initialize()
		{
			NxFr::Initialize();
		}

		void NxEn::EntryPoint::Shutdown()
		{
			NxFr::Shutdown();
		}
	}
}
