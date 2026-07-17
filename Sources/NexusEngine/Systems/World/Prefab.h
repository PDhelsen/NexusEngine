#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API Prefab : public Asset
	{
		friend class WorldSystem;

	public:
		inline static const NxFr::String Extension = "prefab";

		NX_OBJECT(Prefab)

		NxFr::Handle<GameObject> GetRoot() const;
		void SetRoot(NxFr::Handle<GameObject> Instance);

	protected:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::Handle<GameObject> Root;
	};
}
