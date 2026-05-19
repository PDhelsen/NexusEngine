#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Artifact : public Asset
	{
	public:
		NX_ASSET_DECLARATION(Artifact)

		NxFr::Buffer& GetBytes() { return Data; }
		uint64 GetCount() { return Data.GetCount(); }

	private:
		void OnGui(float TimeStep) override;
		void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnUnload() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NxFr::Buffer Data;
	};
}
