#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/StbImage/StbImage.h"

#include "stbimage/stb_image.h"
#include "stbimage/stb_image_resize2.h"
#include "stbimage/stb_image_write.h"

namespace NxEn
{
	namespace StbImage
	{
		void Save(NxFr::StringView Path, void* Pixels, NxFr::Vector2i Resolution, int Channels)
		{
			stbi_write_png(Path.C(), Resolution.x, Resolution.y, Channels, Pixels, Resolution.x * Channels);
		}

		void* Load(NxFr::StringView Path, NxFr::Vector2i& Resolution, int& Channels)
		{
			return stbi_load(Path.C(), &Resolution.x, &Resolution.y, &Channels, 0);
		}

		void Unload(void* Pixels)
		{
			stbi_image_free(Pixels);
		}
	}
}
