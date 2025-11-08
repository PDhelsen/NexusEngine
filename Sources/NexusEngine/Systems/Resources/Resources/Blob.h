#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Blob : public Resource
	{
	public:
		NEXUS_ENGINE_API Blob(NxFr::StringView Path);
		NEXUS_ENGINE_API ~Blob();

		NEXUS_ENGINE_API NxFr::Buffer& GetBytes() { return Data; }

	protected:
		NEXUS_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

	private:
		NxFr::Buffer Data;
	};
}
