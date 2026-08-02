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

			NxFr::Vector2f Center(float WidgetSize, float Count, bool IncludePadding)
			{
				NxFr::Vector2f WindowWidth = ImGui::GetWindowSize();
				NxFr::Vector2f Spacing = ImGui::GetStyle().ItemSpacing;
				NxFr::Vector2f Padding = ImGui::GetStyle().FramePadding;
				return (WindowWidth - WidgetSize - Spacing * Count - (IncludePadding ? Padding : NxFr::Vector2f::Zero)) * 0.5f;
			}

			float Center(NxFr::StringView Text)
			{
				float WindowWidth = ImGui::GetWindowSize().x;
				float TextWidth = ImGui::CalcTextSize(Text.C()).x;
				return (WindowWidth - TextWidth) * 0.5f;
			}

			NxFr::String GenerateId(NxFr::StringView Label, NxFr::StringView Id)
			{
				return "##" + (!Id.IsEmpty() ? Id : Label);
			}

			bool IsPanelActive()
			{
				return
					(ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))) ||
					(ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsAnyItemActive());
			}

			Style Copy(const Style* Original)
			{
				Style Copy;

				if (Original)
				{
					Copy = *Original;
				}
				else
				{
					Copy.Reset();
					Copy.Label = GUI::Styles::WidthLabel();
				}

				return Copy;
			}

			void Push(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				ImGui::PushStyleColor(ImGuiCol_Button, Instance->Color);
				ImGui::PushStyleColor(ImGuiCol_Text, Instance->ColorText);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, Instance->ColorBackground);
				ImGui::PushStyleColor(ImGuiCol_Border, Instance->ColorBorder);

				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Instance->Alpha);
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, Instance->Align);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Instance->Spacing);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Instance->Padding);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Instance->Rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, Instance->Border);

				ImGui::SetWindowFontScale(Instance->Font);
			}

			void Pop(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				ImGui::PopStyleColor(4);
				ImGui::PopStyleVar(6);
				ImGui::SetWindowFontScale(1);
			}

			void SetPosition(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				if (Instance->Position.x >= 0.0f)
				{
					ImGui::SetCursorPosX(Instance->Position.x);
				}
				if (Instance->Position.y >= 0.0f)
				{
					ImGui::SetCursorPosY(Instance->Position.y);
				}
			}

			void SetWidth(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				ImGui::SetNextItemWidth(Instance->Size.x >= 0.0f ? Instance->Size.x : ImGui::GetContentRegionAvail().x);
			}

			void SetWidth(const Style* Instance, NxFr::StringView Text, bool Label)
			{
				if (!Instance)
				{
					return;
				}

				float Size = ImGui::CalcTextSize(Text.C()).x;
				float Position = ImGui::GetCursorPosX() - Size;
				float Offset = Label ? Instance->Label : 0;
				ImGui::SetCursorPosX(Position + (Offset > 0.0f ? Offset : Offset == 0.0f ? Size : ImGui::GetContentRegionAvail().x));
			}
		}

		namespace Draw
		{
			void Label(NxFr::StringView Data, const Style* Visual)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				Utils::Push(Visual);
				Utils::SetPosition(Visual);

				Label(Data);

				Utils::SetWidth(Visual, Data, true);
				Utils::Pop(Visual);
			}

			void Label(NxFr::StringView Data)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				ImGui::AlignTextToFramePadding();
				ImGui::Text(Data.C());
				ImGui::SameLine();
			}

			void Text(NxFr::StringView Data, const Style* Visual)
			{
				Utils::Push(Visual);
				Utils::SetPosition(Visual);

				Text(Data);

				Utils::Pop(Visual);
			}

			void Text(NxFr::StringView Data)
			{
				ImGui::Text(Data.C());
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual)
			{
				Utils::Push(Visual);
				Utils::SetPosition(Visual);
				Utils::SetWidth(Visual);
				uint64 Flag = Visual ? Visual->Flag : ImGuiInputTextFlags_EnterReturnsTrue;

				bool Result = Input(Data, Id, Flag);

				Utils::Pop(Visual);

				return Result;
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, uint64 Flag)
			{
				Data.Reserve(128);
				bool Result = ImGui::InputText(Id.C(), Data.Characters(), Data.GetCapacity(), Flag);
				if (Result)
				{
					Data.Validate();
				}

				return Result;
			}

			bool Button(NxFr::StringView Data, const Style* Visual)
			{
				Utils::Push(Visual);
				Utils::SetPosition(Visual);
				Utils::SetWidth(Visual);
				NxFr::Vector2f Size = Visual ? Visual->Size : NxFr::Vector2f::Zero;

				bool Result = Button(Data, Size);

				Utils::Pop(Visual);

				return Result;
			}

			bool Button(NxFr::StringView Data, NxFr::Vector2f Size)
			{
				return ImGui::Button(Data.C(), Size);
			}
		}
	}
}
