#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Table : public Asset
	{
	public:
		NX_ASSET_DECLARATION(Table)

		YAML::Node& GetRoot() { return Data; }

	protected:
		void OnGui(float TimeStep) override;
		void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnUnload() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		YAML::Node Data;
	};
}
