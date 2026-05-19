#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Yaml : public Resource
	{
	public:
		Yaml(NxFr::StringView Path);
		~Yaml();

		YAML::Node& GetRoot() { return Data; }

	protected:
		void Save(NxFr::StringView FilePath) override;
		void Load(NxFr::StringView FilePath) override;
		void Unload() override;

	private:
		YAML::Node Data;
	};
}
