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
		NxFr::Yaml::SerializeFile(Data, FilePath);
	}

	void Yaml::OnLoad(NxFr::StringView FilePath)
	{
		Data = NxFr::Yaml::DeserializeFile(FilePath);
	}

	void Yaml::OnUnload()
	{
		Data = YAML::Node();
	}
}
