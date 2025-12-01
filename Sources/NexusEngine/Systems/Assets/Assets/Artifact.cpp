#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Artifact.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Artifact)

	void Artifact::OnLoad(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::File File = NxFr::File(Content);
		File.Open(NxFr::File::Mode::Read);
		Data = File.ReadByte();
		File.Close();
	}

	void Artifact::OnUnload()
	{
		Data = NxFr::Buffer();
	}

	void Artifact::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::File File = NxFr::File(Content);
		File.Create(true);
		File.WriteByte(Data);
		File.Close();
	}
}
