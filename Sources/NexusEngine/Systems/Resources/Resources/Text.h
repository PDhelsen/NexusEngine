#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Text : public Resource
	{
	public:
		NEXUS_ENGINE_API Text(NxFr::StringView Path, bool Loaded);
		NEXUS_ENGINE_API ~Text();

		NEXUS_ENGINE_API NxFr::String& GetText() { return Data; }

	protected:
		NEXUS_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

	private:
		NxFr::String Data;
	};
}
