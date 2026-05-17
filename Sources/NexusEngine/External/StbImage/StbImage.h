#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace StbImage
	{
		void* Load(NxFr::StringView Path, NxFr::Vector2i& Resolution, int& Channels);
		void Free(void* Pixels);
	}
}
