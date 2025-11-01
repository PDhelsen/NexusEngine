#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Yaml : public Resource
	{
	public:
		NEXUS_ENGINE_API Yaml(NxFr::StringView Path);
		NEXUS_ENGINE_API ~Yaml();

		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

		NEXUS_ENGINE_API const YAML::Node& GetRoot() const { return Data; }

	private:
		YAML::Node Data;
	};
}
