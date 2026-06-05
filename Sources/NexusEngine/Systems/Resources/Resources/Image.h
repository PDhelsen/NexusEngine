#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Image : public Resource
	{
	public:
		Image(NxFr::StringView Path);
		~Image();

		NxFr::Vector2i GetResolution() const { return Resolution; }
		int32 GetChannels() const { return Channels; }
		void* GetPixels() const { return Data; }

	protected:
		void Save(NxFr::StringView FilePath) override;
		void Load(NxFr::StringView FilePath) override;
		void Unload() override;

	private:
		NxFr::Vector2i Resolution;
		int32 Channels;
		void* Data;
	};
}
