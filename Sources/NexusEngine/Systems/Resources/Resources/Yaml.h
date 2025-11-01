#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Yaml : public Resource
	{
	public:
		NEXUS_ENGINE_API Yaml(NxFr::StringView Path, bool Loaded);
		NEXUS_ENGINE_API ~Yaml();

		NEXUS_ENGINE_API YAML::Node& GetRoot() { return Data; }

	protected:
		NEXUS_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

	private:
		YAML::Node Data;
	};
}
