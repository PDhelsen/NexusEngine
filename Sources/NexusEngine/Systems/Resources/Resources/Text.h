#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Text : public Resource
	{
	public:
		Text(NxFr::StringView Path);
		~Text();

		NxFr::String& GetText() { return Data; }
		const NxFr::String& GetText() const { return Data; }

	protected:
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::String Data;
	};
}
