#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Document : public Asset
	{
	public:
		NEXUS_ASSET_DECLARATION(NEXUS_ENGINE_API, Document)

		NEXUS_ENGINE_API NxFr::String& GetText() { return Data; }
		NEXUS_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	protected:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView ContentFilePath) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFilePath) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NxFr::String Data;
	};
}
