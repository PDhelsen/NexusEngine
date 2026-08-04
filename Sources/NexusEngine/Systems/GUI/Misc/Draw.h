#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Draw.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"
#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEn
{
	namespace GUI
	{
		template<>
		struct Drawer<NxFr::String>
		{
			static void Property(const NxFr::String& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Draw::Text(Data);
			}

			static bool Field(NxFr::String& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Utils::SetSize(Visual);
				bool Result = Draw::Input(Data, ImGuiId, Visual ? Visual->Flag : ImGuiInputTextFlags_EnterReturnsTrue);

				return Result;
			}
		};

		template<>
		struct Drawer<NxFr::StringView>
		{
			static void Property(const NxFr::StringView& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Draw::Text(Data);
			}

			static bool Field(NxFr::StringView& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NX_ASSERT(false, Default, "Unsupported");
				return false;
			}
		};

		template<>
		struct Drawer<const char*>
		{
			static void Property(const char*& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Draw::Text(Data);
			}

			static bool Field(const char*& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NX_ASSERT(false, Default, "Unsupported");
				return false;
			}
		};

		template<>
		struct Drawer<bool>
		{
			static void Property(const bool& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId("", Label);

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				ImGui::BeginDisabled();
				bool Copy = Data;
				Draw::CheckBox(ImGuiId, Copy);
				ImGui::EndDisabled();
			}

			static bool Field(bool& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				bool Result = Draw::CheckBox(ImGuiId, Data);

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::Array<T>>
		{
			static void Property(const NxFr::Array<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + ImGui::CalcTextSize("Count").x + ImGui::GetStyle().ItemSpacing.x;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr, &ItemStyle);
				}
			}

			static bool Field(NxFr::Array<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				bool Result = false;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				Draw::Label("Count");
				Utils::SetSize(NxFr::Vector2f(Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 2).x, 0));
				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				if (Draw::Input(Count, ImGuiId, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					NxFr::ContainerUtility::Resize(Data, NxFr::StringUtility::FromString<uint64>(Count));
					Result = true;
				}
				ImGui::SameLine();
				if (Draw::Button("Add" + ImGuiId, NxFr::Vector2f(ButtonSize, 0)))
				{
					NxFr::ContainerUtility::Resize(Data, Data.GetCount() + 1);
					Result = true;
				}
				ImGui::SameLine();
				if (Draw::Button("Remove" + ImGuiId, NxFr::Vector2f(ButtonSize, 0)))
				{
					NxFr::ContainerUtility::Resize(Data, Data.GetCount() - 1);
					Result = true;
				}

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + ImGui::CalcTextSize("Count").x + ImGui::GetStyle().ItemSpacing.x;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Result |= GUI::Drawer<T>::Field(Data[Index], IndexStr, ImGuiId + IndexStr, &ItemStyle);
				}

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::List<T>>
		{
			static void Property(const NxFr::List<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + ImGui::CalcTextSize("Count").x + ImGui::GetStyle().ItemSpacing.x;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr, &ItemStyle);
				}
			}

			static bool Field(NxFr::List<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				bool Result = false;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				Draw::Label("Count");
				Utils::SetSize(NxFr::Vector2f(Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 2).x, 0));
				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				if (Draw::Input(Count, ImGuiId, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					NxFr::ContainerUtility::Resize(Data, NxFr::StringUtility::FromString<uint64>(Count));
					Result = true;
				}
				ImGui::SameLine();
				if (Draw::Button("Add" + ImGuiId, NxFr::Vector2f(ButtonSize, 0)))
				{
					Data.AppendConstruct();
					Result = true;
				}
				ImGui::SameLine();
				if (Draw::Button("Remove" + ImGuiId, NxFr::Vector2f(ButtonSize, 0)))
				{
					Data.RemoveLast();
					Result = true;
				}

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + ImGui::CalcTextSize("Count").x + ImGui::GetStyle().ItemSpacing.x;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Result |= GUI::Drawer<T>::Field(Data[Index], IndexStr, ImGuiId + IndexStr, &ItemStyle);
				}

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::Set<T>>
		{
			static void Property(const NxFr::Set<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + Styles::WidthLabel();
				for (const auto& It : Data)
				{
					GUI::Drawer<T>::Property(It, "", &ItemStyle);
				}
			}

			static bool Field(NxFr::Set<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				bool Result = false;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				Draw::Label("Count");
				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);
				ImGui::SameLine();
				Utils::SetPosition(NxFr::Vector2f(ImGui::GetCursorPosX() + Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 1).x, -1.0f));
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
					{
						Data.Append(NxFr::StringUtility::FromString<T>(Value));
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
					{
						Data.Remove(NxFr::StringUtility::FromString<T>(Value));
					});
				}

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + Styles::WidthLabel();
				for (const auto& It : Data)
				{
					Result |= GUI::Drawer<T>::Property(It, "", &ItemStyle);
				}

				return false;
			}
		};

		template<typename K, typename T>
		struct Drawer<NxFr::Dictionary<K, T>>
		{
			static void Property(const NxFr::Dictionary<K, T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + Styles::WidthLabel();
				for (const auto& It : Data)
				{
					NxFr::String KeyStr = NxFr::StringUtility::ToString(It.Key);
					GUI::Drawer<T>::Property(It.Value, KeyStr, &ItemStyle);
				}
			}

			static bool Field(NxFr::Dictionary<K, T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				bool Result = false;

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);

				float Position = ImGui::GetCursorPosX();

				Draw::Label("Count");
				NxFr::String Count = NxFr::StringUtility::ToString(Data.GetCount());
				Draw::Text(Count);
				ImGui::SameLine();
				Utils::SetPosition(NxFr::Vector2f(ImGui::GetCursorPosX() + Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 1).x, -1.0f));
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
					{
						Data.AppendConstruct(NxFr::StringUtility::FromString<K>(Value));
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
					{
						Data.Remove(NxFr::StringUtility::FromString<K>(Value));
					});
				}

				Style ItemStyle = Visual ? *Visual : Styles::Default();
				ItemStyle.Position = NxFr::Vector2f(Position, -1);
				ItemStyle.Size = -NxFr::Vector2f::One;
				ItemStyle.Label = Position + Styles::WidthLabel();
				for (auto& It : Data)
				{
					NxFr::String KeyStr = NxFr::StringUtility::ToString(It.Key);
					GUI::Drawer<T>::Field(It.Value, KeyStr, ImGuiId + KeyStr, &ItemStyle);
				}

				return false;
			}
		};
	}
}
