#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class NX_ENGINE_API Document : public Asset
	{
	public:
		NX_ASSET_DECLARATION(Document)

		NxFr::String& GetText() { return Data; }
		uint64 GetCount() { return Data.GetCount(); }

	protected:
		void OnDraw() override;
		void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		void OnUnload() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const override;

	private:
		NxFr::String Data;
	};
}
