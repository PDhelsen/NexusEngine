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

	void Artifact::OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		NxFr::File File = NxFr::File(ContentFsPath);
		File.Create(true);
		File.WriteByte(Data);
		File.Close();
	}

	void Artifact::OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		NxFr::File File = NxFr::File(ContentFsPath);
		File.Open(NxFr::File::Mode::Read);
		Data = File.ReadByte();
		File.Close();
	}

	void Artifact::OnUnload()
	{
		Data = NxFr::Buffer();
	}

	void Artifact::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const
	{
	}
}
