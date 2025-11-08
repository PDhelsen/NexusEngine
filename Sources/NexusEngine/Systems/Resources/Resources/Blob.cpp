#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Blob.h"

namespace NxEn
{
	Blob::Blob(NxFr::StringView Path, bool Loaded)
		: Resource(Path), Data()
	{
	}

	Blob::~Blob()
	{
	}

	void Blob::Save(NxFr::StringView FilePath)
	{
		Resource::Save(FilePath);

		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Write, false);
		File.WriteByte(Data);
		File.Close();
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
