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

	protected:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;

	private:
		NxFr::String Data;
	};
}
