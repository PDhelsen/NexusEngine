#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Table : public Asset
	{
	public:
		NEXUS_ASSET_DECLARATION(NEXUS_ENGINE_API, Table)

		NEXUS_ENGINE_API YAML::Node& GetRoot() { return Data; }

	protected:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		YAML::Node Data;
	};
}
