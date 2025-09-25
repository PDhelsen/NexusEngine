#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Utils.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Utils
		{
			NxFr::Vector2f Fill(NxFr::Vector2f Offset, float Count, bool IncludePadding, bool Window)
			{
				NxFr::Vector2f ContentArea = ImGui::GetContentRegionAvail();
				NxFr::Vector2f WindowArea = NxFr::Vector2f(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				NxFr::Vector2f Total = Window ? WindowArea : ContentArea;
				NxFr::Vector2f Spacing = ImGui::GetStyle().ItemSpacing;
				NxFr::Vector2f Padding = ImGui::GetStyle().FramePadding;
				return Total - Offset - Spacing * Count - (IncludePadding ? Padding : NxFr::Vector2f::Zero);
			}
		}
	}
}
