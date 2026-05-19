#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Table : public Asset
	{
	public:
		NX_ASSET_DECLARATION(NX_ENGINE_API, Table)

		NX_ENGINE_API YAML::Node& GetRoot() { return Data; }

	protected:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnUnload() override;
		NX_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		YAML::Node Data;
	};
}
