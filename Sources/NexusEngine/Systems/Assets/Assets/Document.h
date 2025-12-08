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
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;

		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> GetDependencies() override { return NxFr::Array<NxFr::GUID>(); };

	private:
		NxFr::String Data;
	};
}
