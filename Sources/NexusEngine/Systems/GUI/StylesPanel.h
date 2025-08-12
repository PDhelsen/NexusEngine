#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class StylesPanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, StylePanel)

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		NEXUS_ENGINE_API virtual void* FetchDefaultTarget() const;

		NxFr::Dictionary<NxFr::StringId, GUI::Style>* GetStyles() const { return reinterpret_cast<NxFr::Dictionary<NxFr::StringId, GUI::Style>*>(Target); }
	};
}

