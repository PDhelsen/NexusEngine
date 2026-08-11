#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Dock : public Element
		{
			struct Item
			{
				NxFr::String Name;
				ImGuiDir Direction;
				float Ratio;
			};

		public:
			enum class Mode
			{
				None, FirstTime, Always
			};

			NX_OBJECT(Dock)

			Dock();
			virtual ~Dock();

			virtual void Draw() override;

			void DockElement(Element* Instance, ImGuiDir Direction = ImGuiDir::ImGuiDir_None, float Ratio = 0.0f, bool Force = false);
			void ClearElements();

			void SetMode(Mode LayoutMode) { Layout = LayoutMode; }

		protected:
			virtual void OnInitialize() override;

		private:
			NxFr::List<Item> Items;
			Mode Layout;
			bool Relayout;
		};
	}
}
