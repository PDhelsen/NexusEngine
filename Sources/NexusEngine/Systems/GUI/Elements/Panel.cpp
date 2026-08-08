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

			if (DockId != 0)
			{
				ImGui::SetNextWindowDockID(DockId, ImGuiCond_FirstUseEver);
			}

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
