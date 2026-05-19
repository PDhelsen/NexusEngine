#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API Prefab : public Asset
	{
	public:
		NX_ASSET_DECLARATION(Prefab)

		NxFr::Handle<GameObject> GetRoot() const;
		void SetRoot(NxFr::Handle<GameObject> Instance);

	protected:
		void OnGui(float TimeStep) override;
		void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		void OnUnload() override;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
