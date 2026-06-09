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

	void Image::OnSave(NxFr::StringView FilePath) const
	{

	}

	void Image::OnLoad(NxFr::StringView FilePath)
	{
		Data = StbImage::Load(FilePath, Resolution, Channels);
	}

	void Image::OnUnload()
	{
		StbImage::Free(Data);
	}
}
