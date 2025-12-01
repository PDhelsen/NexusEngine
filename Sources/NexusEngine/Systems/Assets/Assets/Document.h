#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Document : public Asset
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Document)

		NEXUS_ENGINE_API NxFr::String& GetText() { return Data; }
		NEXUS_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	private:
		NEXUS_ENGINE_API void OnLoad(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;

	private:
		NxFr::String Data;
	};
}
