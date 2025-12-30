#pragma once

#include "NexusEngine/Systems/World/Component/Component.h"

namespace NxEn
{
	class Tags : public Component
	{
	public:
		NEXUS_COMPONENT_DECLARATION(NEXUS_ENGINE_API, Tags)

		NEXUS_ENGINE_API Tags();
		NEXUS_ENGINE_API ~Tags();

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;

	private:
		NxFr::Set<NxFr::String> Items;
	};
}

