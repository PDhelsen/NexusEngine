#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/StbImage/StbImage.h"

#include "stbimage/StbImage.h"

namespace NxEn
{
	namespace StbImage
	{
		void* Load(NxFr::StringView Path, NxFr::Vector2i& Resolution, int& Channels)
		{
			return stbi_load(Path.C(), &Resolution.x, &Resolution.y, &Channels, 0);
		}

		void Free(void* Pixels)
		{
			stbi_image_free(Pixels);
		}
	}
}
