#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Text : public Resource
	{
	public:
		Text(NxFr::StringView Path);
		~Text();

		NxFr::String& GetText() { return Data; }

	protected:
		void Save(NxFr::StringView FilePath) override;
		void Load(NxFr::StringView FilePath) override;
		void Unload() override;

	private:
		NxFr::String Data;
	};
}
