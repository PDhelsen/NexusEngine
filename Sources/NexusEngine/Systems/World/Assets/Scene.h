#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class Scene : public Asset
	{
	public:
		NX_ASSET_DECLARATION(NX_ENGINE_API, Scene)

		NX_ENGINE_API NxFr::Handle<GameObject> GetRoot() const;

	protected:
		NX_ENGINE_API void OnInitialize() override;

		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView Content) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView Content) override;
		NX_ENGINE_API void OnUnload() override;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
