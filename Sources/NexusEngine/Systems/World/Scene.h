#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API Scene : public Asset
	{
		friend class WorldSystem;

	public:
		inline static const NxFr::String Extension = "scene";

		NX_OBJECT(Scene)

		NxFr::Handle<GameObject> GetRoot() const { return Root; }

	protected:
		void OnInitialize() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const override;

	private:
		void SetRoot(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::Handle<GameObject> Root;
	};
}
