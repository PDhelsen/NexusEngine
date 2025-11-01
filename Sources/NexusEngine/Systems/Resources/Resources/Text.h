#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Text : public Resource
	{
	public:
		NEXUS_ENGINE_API Text(NxFr::StringView Path);
		NEXUS_ENGINE_API ~Text();

		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

		NEXUS_ENGINE_API NxFr::StringView GetText() { return Data; }

	private:
		NxFr::String Data;
	};
}
