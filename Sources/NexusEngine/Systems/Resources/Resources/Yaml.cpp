#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Yaml.h"

namespace NxEn
{
	Yaml::Yaml(NxFr::StringView Path)
		: Resource(Path), Data()
	{
	}

	Yaml::~Yaml()
	{
	}

	void Yaml::OnSave(NxFr::StringView FilePath) const
	{
		NxFr::Yaml::SerializeAndSave(Data, FilePath);
	}

	void Yaml::OnLoad(NxFr::StringView FilePath)
	{
		Data = NxFr::Yaml::LoadAndDeserialize(FilePath);
	}

	void Yaml::OnUnload()
	{
		Data = NxFr::Yaml::Node();
	}
}
