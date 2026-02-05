#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#include "NexusEngine/Systems/GUI/GUISystem.h"

namespace NxEn
{
	template<typename T>
	union Reference;

	namespace GUI
	{
		template<typename T>
		struct Drawer<Reference<T>>
		{
			static void Property(const Reference<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Data ? NxFr::StringUtility::ToString(Data->GetId()) : "0", &DrawerStyle);
				ImGui::SameLine();
				Draw::Text(Data ? Data->GetName() : "", &DrawerStyle);
			}

			static bool Field(Reference<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);
				NxFr::String Value = Data ? NxFr::StringUtility::ToString(Data->GetId()) : "0";

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				DrawerStyle.Width = 150.0f;
				DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
				bool Result = Draw::Input(Value, ImGuiId, &DrawerStyle);
				ImGui::SameLine();
				Draw::Text(Data ? Data->GetName() : "", &DrawerStyle);

				if (Result)
				{
					Data = NxFr::StringUtility::FromString<NxFr::GUID>(Value);
				}

				return Result;
			}
		};
	}
}
