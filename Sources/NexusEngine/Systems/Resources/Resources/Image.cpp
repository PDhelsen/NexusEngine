#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Image.h"

#include "NexusEngine/External/StbImage.h"

namespace NxEn
{
	Image::Image(NxFr::StringView Path)
		: Resource(Path), Resolution(), Channels(0), Data(nullptr)
	{
	}

	Image::~Image()
	{
	}

	void Image::Save(NxFr::StringView FilePath)
	{
		Resource::Save(FilePath);
	}

	void Image::Load(NxFr::StringView FilePath)
	{
		Resource::Load(FilePath);

		Data = stbi_load(FilePath.C(), &Resolution.x, &Resolution.y, &Channels, 0);
	}

	void Image::Unload()
	{
		stbi_image_free(Data);

		Resource::Unload();
	}
}
