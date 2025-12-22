#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class Prefab : public Asset
	{
	public:
		NEXUS_ASSET_DECLARATION(NEXUS_ENGINE_API, Prefab)

		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetRoot() const;
		NEXUS_ENGINE_API void SetRoot(NxFr::Handle<GameObject> Instance);

	protected:
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NEXUS_ENGINE_API void OnUnload() override;

		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> GetDependencies() override;
		NEXUS_ENGINE_API void GetDependencies(NxFr::Handle<GameObject> Instance, NxFr::Set<NxFr::GUID>& Result) const;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
