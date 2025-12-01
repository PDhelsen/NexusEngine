#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Table : public Asset
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Table)

		NEXUS_ENGINE_API YAML::Node& GetRoot() { return Data; }

	private:
		NEXUS_ENGINE_API void OnLoad(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;

	private:
		YAML::Node Data;
	};
}
