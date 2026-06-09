#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
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
		void* GetPixels() { return Data; }
		const void* GetPixels() const { return Data; }

	protected:
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::Vector2i Resolution;
		int32 Channels;
		void* Data;
	};
}
