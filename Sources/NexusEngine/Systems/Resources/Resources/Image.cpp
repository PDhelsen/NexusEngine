#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Image.h"

#include "NexusEngine/External/StbImage/StbImage.h"

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

		Data = StbImage::Load(FilePath, Resolution, Channels);
	}

	void Image::Unload()
	{
		StbImage::Free(Data);

		Resource::Unload();
	}
}
