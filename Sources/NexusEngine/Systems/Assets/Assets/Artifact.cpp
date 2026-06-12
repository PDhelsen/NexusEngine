#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Artifact.h"

namespace NxEn
{
	NX_ASSET(Artifact)

	void Artifact::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<uint64>::Property(Data.GetCount(), "Size");
	}

	void Artifact::OnClone(const Object& Other)
	{
		const Artifact& Instance = static_cast<const Artifact&>(Other);

		Data = Instance.Data;
	}

	void Artifact::OnSave(NxFr::StringView Path) const
	{
		NxFr::File File = NxFr::File(Path);
		File.Create(true);
		File.WriteByte(Data);
		File.Close();
	}

	void Artifact::OnLoad(NxFr::StringView Path)
	{
		NxFr::File File = NxFr::File(Path);
		File.Open(NxFr::File::Mode::Read);
		Data = File.ReadByte();
		File.Close();
	}

	void Artifact::OnUnload()
	{
		Data = NxFr::Buffer();
	}
}
