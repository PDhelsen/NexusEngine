#pragma once

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Globals
	{
		NEXUS_ENGINE_API void SetErrorCode(uint8 Code);
		NEXUS_ENGINE_API uint8 GetErrorCode();
	}
}
