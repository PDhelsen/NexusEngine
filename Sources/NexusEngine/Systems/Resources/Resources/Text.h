#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class Text : public Resource
	{
	public:
		NX_ENGINE_API Text(NxFr::StringView Path);
		NX_ENGINE_API ~Text();

		NX_ENGINE_API NxFr::String& GetText() { return Data; }

	protected:
		NX_ENGINE_API void Save(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Load(NxFr::StringView FilePath) override;
		NX_ENGINE_API void Unload() override;

	private:
		NxFr::String Data;
	};
}
