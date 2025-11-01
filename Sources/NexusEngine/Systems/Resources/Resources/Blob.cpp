#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Blob.h"

namespace NxEn
{
	Blob::Blob(NxFr::StringView Path)
		: Resource(Path), Data()
	{
	}

	Blob::~Blob()
	{
	}

	void Blob::Load(NxFr::StringView FilePath)
	{
		Resource::Load(FilePath);

		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Read, false);
		Data = File.ReadByte();
		File.Close();
	}

	void Blob::Unload()
	{
		Data = NxFr::Buffer();

		Resource::Unload();
	}
}
