#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"

namespace NxEn
{
	class NX_ENGINE_API StylesPanel : public GUI::Panel
	{
	public:
		NX_OBJECT(StylesPanel)

		StylesPanel();
		~StylesPanel();

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

