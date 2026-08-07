#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Style.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Styles
		{
			inline static const NxFr::Vector2f Spacing() { return ImGui::GetStyle().ItemSpacing; }

			inline static const float& WidthButton() { return *Style::GetVars().TryGet("WidthButton"_Sid); }
			inline static const float& WidthLabel() { return *Style::GetVars().TryGet("WidthLabel"_Sid); }
			inline static const float& WidthInpuText() { return *Style::GetVars().TryGet("WidthInpuText"_Sid); }

			inline static const NxFr::Color& Red() { return *Style::GetColors().TryGet("Red"_Sid); }
			inline static const NxFr::Color& Yellow() { return *Style::GetColors().TryGet("Yellow"_Sid); }
			inline static const NxFr::Color& Green() { return *Style::GetColors().TryGet("Green"_Sid); }
			inline static const NxFr::Color& Cyan() { return *Style::GetColors().TryGet("Cyan"_Sid); }
			inline static const NxFr::Color& Blue() { return *Style::GetColors().TryGet("Blue"_Sid); }
			inline static const NxFr::Color& Magenta() { return *Style::GetColors().TryGet("Magenta"_Sid); }

			inline static const Style& Default() { return *Style::GetStyles().TryGet("Default"_Sid); }
			inline static const Style& TextNormal() { return *Style::GetStyles().TryGet("TextNormal"_Sid); }
			inline static const Style& TextTitle() { return *Style::GetStyles().TryGet("TextTitle"_Sid); }
			inline static const Style& ButtonNormal() { return *Style::GetStyles().TryGet("ButtonNormal"_Sid); }
			inline static const Style& ButtonPressed() { return *Style::GetStyles().TryGet("ButtonPressed"_Sid); }
			inline static const Style& Info() { return *Style::GetStyles().TryGet("Info"_Sid); }
			inline static const Style& Warning() { return *Style::GetStyles().TryGet("Warning"_Sid); }
			inline static const Style& Error() { return *Style::GetStyles().TryGet("Error"_Sid); }
			inline static const Style& Fatal() { return *Style::GetStyles().TryGet("Fatal"_Sid); }
		}
	}
}
