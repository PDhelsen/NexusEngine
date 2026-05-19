#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Artifact : public Asset
	{
	public:
		NX_ASSET_DECLARATION(NX_ENGINE_API, Artifact)

		NX_ENGINE_API NxFr::Buffer& GetBytes() { return Data; }
		NX_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	private:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnUnload() override;
		NX_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NxFr::Buffer Data;
	};
}
