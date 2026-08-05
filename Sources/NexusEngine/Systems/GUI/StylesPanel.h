#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class NX_ENGINE_API StylesPanel : public GUI::Panel
	{
	public:
		NX_OBJECT(StylePanel)

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		GUI::Menu Menu;
	};
}

