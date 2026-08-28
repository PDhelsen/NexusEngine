#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"
#include "NexusEngine/Systems/GUI/Elements/Dock.h"

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
			Dock& GetDock() { return Dock; }

		protected:
			void OnInitialize() override;
			void OnShutdown() override;
			void OnEnable() override;
			void OnDisable() override;
			void OnDraw() override;

		private:
			Menu Menu;
			Dock Dock;
		};
	}
}

