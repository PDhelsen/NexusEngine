#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API Scene : public Asset
	{
	public:
		NX_ASSET_DECLARATION(Scene)

		NxFr::Handle<GameObject> GetRoot() const;

	protected:
		void OnInitialize() override;

		void OnDraw() override;
		void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		void OnUnload() override;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
