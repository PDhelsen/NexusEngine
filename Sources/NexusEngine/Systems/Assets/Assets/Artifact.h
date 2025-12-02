#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Artifact : public Asset
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Artifact)

		NEXUS_ENGINE_API NxFr::Buffer& GetBytes() { return Data; }
		NEXUS_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	private:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;

	private:
		NxFr::Buffer Data;
	};
}
