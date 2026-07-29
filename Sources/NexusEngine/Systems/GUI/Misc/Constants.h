#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Styles
		{
			inline static const float& WidthButton() { return *Style::GetVars().TryGet("WidthButton"_Sid); }
			inline static const float& WidthLabel() { return *Style::GetVars().TryGet("WidthLabel"_Sid); }
			inline static const float& WidthInpuText() { return *Style::GetVars().TryGet("WidthInpuText"_Sid); }

			inline static const NxFr::Color& Red() { return *Style::GetColors().TryGet("Red"_Sid); }
			inline static const NxFr::Color& Yellow() { return *Style::GetColors().TryGet("Yellow"_Sid); }
			inline static const NxFr::Color& Green() { return *Style::GetColors().TryGet("Green"_Sid); }
			inline static const NxFr::Color& Cyan() { return *Style::GetColors().TryGet("Cyan"_Sid); }
			inline static const NxFr::Color& Blue() { return *Style::GetColors().TryGet("Blue"_Sid); }
			inline static const NxFr::Color& Magenta() { return *Style::GetColors().TryGet("Magenta"_Sid); }

			inline static const Style& Text_Normal() { return *Style::GetStyles().TryGet("Text_Normal"_Sid); }
			inline static const Style& Text_Title() { return *Style::GetStyles().TryGet("Text_Title"_Sid); }
			inline static const Style& Button_Normal() { return *Style::GetStyles().TryGet("Button_Normal"_Sid); }
			inline static const Style& Button_Pressed() { return *Style::GetStyles().TryGet("Button_Pressed"_Sid); }
			inline static const Style& Info() { return *Style::GetStyles().TryGet("Info"_Sid); }
			inline static const Style& Warning() { return *Style::GetStyles().TryGet("Warning"_Sid); }
			inline static const Style& Error() { return *Style::GetStyles().TryGet("Error"_Sid); }
			inline static const Style& Fatal() { return *Style::GetStyles().TryGet("Fatal"_Sid); }
		}
	}
}
