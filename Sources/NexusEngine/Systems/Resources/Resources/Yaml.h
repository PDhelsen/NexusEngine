#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Yaml : public Resource
	{
	public:
		Yaml(NxFr::StringView Path);
		~Yaml();

		YAML::Node& GetRoot() { return Data; }
		const YAML::Node& GetRoot() const { return Data; }

	protected:
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		YAML::Node Data;
	};
}
