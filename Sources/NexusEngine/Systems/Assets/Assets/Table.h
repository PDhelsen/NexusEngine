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
		void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnUnload() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const override;

	private:
		YAML::Node Data;
	};
}
