#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Image : public Resource
	{
	public:
		NEXUS_ENGINE_API Image(NxFr::StringView Path);
		NEXUS_ENGINE_API ~Image();

		NEXUS_ENGINE_API NxFr::Vector2i GetResolution() { return Resolution; }
		NEXUS_ENGINE_API int32 GetChannels() { return Channels; }
		NEXUS_ENGINE_API void* GetPixels() { return Data; }

	protected:
		NEXUS_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NEXUS_ENGINE_API void Unload() override;

	private:
		NxFr::Vector2i Resolution;
		int32 Channels;
		void* Data;
	};
}
