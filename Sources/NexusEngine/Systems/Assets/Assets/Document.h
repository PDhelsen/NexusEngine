#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Document : public Asset
	{
	public:
		NX_OBJECT(Document)

		NxFr::String& GetText() { return Data; }
		uint64 GetCount() { return Data.GetCount(); }

	protected:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::String Data;
	};
}
