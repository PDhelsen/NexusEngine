#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Artifact : public Asset
	{
	public:
		NX_OBJECT(Artifact)

		NxFr::Buffer& GetBytes() { return Data; }
		uint64 GetCount() { return Data.GetCount(); }

	private:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::Buffer Data;
	};
}
