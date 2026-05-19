#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Blob : public Resource
	{
	public:
		NX_ENGINE_API Blob(NxFr::StringView Path);
		NX_ENGINE_API ~Blob();

		NX_ENGINE_API NxFr::Buffer& GetBytes() { return Data; }

	protected:
		NX_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Unload() override;

	private:
		NxFr::Buffer Data;
	};
}
