#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Table : public Asset
	{
	public:
		NX_OBJECT(Table)

		YAML::Node& GetRoot() { return Data; }

	protected:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		YAML::Node Data;
	};
}
