#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API Scene : public Asset
	{
	public:
		NX_OBJECT(Scene)

		NxFr::Handle<GameObject> GetRoot() const;

	protected:
		void OnInitialize() override;

		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
