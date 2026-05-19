#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	class Document : public Asset
	{
	public:
		NX_ASSET_DECLARATION(NX_ENGINE_API, Document)

		NX_ENGINE_API NxFr::String& GetText() { return Data; }
		NX_ENGINE_API uint64 GetCount() { return Data.GetCount(); }

	protected:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) override;
		NX_ENGINE_API void OnUnload() override;
		NX_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NxFr::String Data;
	};
}
