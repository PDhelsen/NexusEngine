#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class StylesPanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, StylePanel)

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnEnable() override;
		NX_ENGINE_API void OnDisable() override;
		NX_ENGINE_API void OnGui(float TimeStep) override;

	private:
		GUI::Menu Menu;
		GUI::Style Style;
	};
}

