#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Style.h"
#include "NexusEngine/Systems/GUI/Utils.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Draw
		{
			NX_ENGINE_API void Label(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API void Label(NxFr::StringView Data);
			NX_ENGINE_API void Text(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API void Text(NxFr::StringView Data);
			NX_ENGINE_API bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual);
			NX_ENGINE_API bool Input(NxFr::String& Data, NxFr::StringView Id, uint64 Flag = 0);
			NX_ENGINE_API bool Button(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API bool Button(NxFr::StringView Data, NxFr::Vector2f Size = NxFr::Vector2f::Zero);
		}

		template<typename T>
		struct Drawer
		{
			static void Property(const T& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Buffer, &DrawerStyle);
			}

			static bool Field(T& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
				bool Result = Draw::Input(Buffer, ImGuiId, &DrawerStyle);

				NxFr::StringUtility::FromString<T>(Buffer, Data, "");
				return Result;
			}
		};
	}
}
