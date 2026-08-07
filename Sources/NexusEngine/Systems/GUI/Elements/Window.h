#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Window : public NxEn::GUI::Element
		{
		public:
			NX_OBJECT(Window)

			Window();
			virtual ~Window();

			virtual void Draw() override;

			Menu& GetMenu() { return Menu; }
			NxFr::GUID GetDock() { return DockId; }

		protected:
			void OnInitialize() override;
			void OnShutdown() override;

		private:
			Menu Menu;
			NxFr::GUID DockId;
		};
	}
}

