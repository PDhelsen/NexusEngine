#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Image : public Resource
	{
	public:
		NX_ENGINE_API Image(NxFr::StringView Path);
		NX_ENGINE_API ~Image();

		NX_ENGINE_API NxFr::Vector2i GetResolution() { return Resolution; }
		NX_ENGINE_API int32 GetChannels() { return Channels; }
		NX_ENGINE_API void* GetPixels() { return Data; }

	protected:
		NX_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Unload() override;

	private:
		NxFr::Vector2i Resolution;
		int32 Channels;
		void* Data;
	};
}
