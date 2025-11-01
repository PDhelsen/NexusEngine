#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Blob : public Resource
	{
	public:
		NEXUS_ENGINE_API Blob(NxFr::StringView Path);
		NEXUS_ENGINE_API ~Blob();

		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

		NEXUS_ENGINE_API NxFr::BufferView GetBytes() { return Data; }

	private:
		NxFr::Buffer Data;
	};
}
