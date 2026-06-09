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

	void Blob::OnSave(NxFr::StringView FilePath) const
	{
		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Write, false);
		File.WriteByte(Data);
		File.Close();
	}

	void Blob::OnLoad(NxFr::StringView FilePath)
	{
		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Read, false);
		Data = File.ReadByte();
		File.Close();
	}

	void Blob::OnUnload()
	{
		Data = NxFr::Buffer();
	}
}
