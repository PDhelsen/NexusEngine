#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Artifact : public Asset
	{
	public:
		NEXUS_ASSET_DECLARATION(NEXUS_ENGINE_API, Artifact)

		NEXUS_ENGINE_API NxFr::Buffer& GetBytes() { return Data; }
		NEXUS_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	private:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NxFr::Buffer Data;
	};
}
