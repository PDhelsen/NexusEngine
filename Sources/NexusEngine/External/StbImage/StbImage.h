#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace StbImage
	{
		void Save(NxFr::StringView Path, void* Pixels, NxFr::Vector2i Resolution, int Channels);
		void* Load(NxFr::StringView Path, NxFr::Vector2i& Resolution, int& Channels);
		void Unload(void* Pixels);
	}
}
