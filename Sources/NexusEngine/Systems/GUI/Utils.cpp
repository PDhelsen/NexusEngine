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
		}

		namespace Draw
		{
			void Label(NxFr::StringView Data, const Style* Visual)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				Scope _ = Visual;

				Label(Data);

				if (Visual)
				{
					Visual->SetWidthLabel(Data);
				}
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
				Scope _ = Visual;

				Text(Data);
			}

			void Text(NxFr::StringView Data)
			{
				ImGui::Text(Data.C());
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual)
			{
				Scope _ = Visual;
				uint64 Flag = ImGuiInputTextFlags_EnterReturnsTrue;

				if (Visual)
				{
					Visual->SetWidth();
					Flag = Visual->Flag;
				}

				return Input(Data, Id, Flag);
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
				Scope _ = Visual;

				NxFr::Vector2f Size = NxFr::Vector2f::Zero;

				if (Visual)
				{
					Size.x = Visual->Width;
				}

				return Button(Data, Size);
			}

			bool Button(NxFr::StringView Data, NxFr::Vector2f Size)
			{
				return ImGui::Button(Data.C(), Size);
			}
		}
	}
}
