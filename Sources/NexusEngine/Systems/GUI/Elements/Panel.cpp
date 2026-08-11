#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"

namespace NxEn
{
	namespace GUI
	{
		Panel::Panel()
		{
		}

		Panel::~Panel()
		{
		}

		void Panel::Draw()
		{
			if (!IsEnabled())
			{
				return;
			}

			bool IsOpen = true;

			if (ImGui::Begin(GetNamedId().C(), &IsOpen, GetImGuiFlags()))
			{
				OnDraw();
			}
			ImGui::End();

			if (!IsOpen)
			{
				Close();
			}
		}

		void Panel::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(ElementFlags::HideInsteadOfClose, true);
			SetImGuiFlag(ImGuiWindowFlags_NoCollapse, true);
		}
	}
}
