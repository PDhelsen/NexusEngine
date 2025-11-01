#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Yaml.h"

namespace NxEn
{
	Yaml::Yaml(NxFr::StringView Path, bool Loaded)
		: Resource(Path, Loaded), Data()
	{
	}

	Yaml::~Yaml()
	{
	}

	void Yaml::Save(NxFr::StringView FilePath)
	{
		Resource::Save(FilePath);

		NxFr::Yaml::SerializeFile(Data, FilePath);
	}

	void Yaml::Load(NxFr::StringView FilePath)
	{
		Resource::Load(FilePath);

		Data = NxFr::Yaml::DeserializeFile(FilePath);
	}

	void Yaml::Unload()
	{
		Data = YAML::Node();

		Resource::Unload();
	}
}
