#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Yaml : public Resource
	{
	public:
		NX_ENGINE_API Yaml(NxFr::StringView Path);
		NX_ENGINE_API ~Yaml();

		NX_ENGINE_API YAML::Node& GetRoot() { return Data; }

	protected:
		NX_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Unload() override;

	private:
		YAML::Node Data;
	};
}
