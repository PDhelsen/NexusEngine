#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Utils.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Utils
		{
			bool IsPanelActive()
			{
				return
					(ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))) ||
					(ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsAnyItemActive());
			}

			NxFr::String GenerateId(NxFr::StringView Id, NxFr::StringView Label)
			{
				return "##" + (!Id.IsEmpty() ? Id : Label);
			}

			void PushStyle(const Style* Instance)
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

			void PopStyle(const Style* Instance)
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

				SetPosition(Instance->Position);
			}

			void SetPosition(NxFr::Vector2f Position)
			{
				if (Position.x >= 0.0f)
				{
					ImGui::SetCursorPosX(Position.x);
				}
				if (Position.y >= 0.0f)
				{
					ImGui::SetCursorPosY(Position.y);
				}
			}

			void SetSize(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				SetSize(Instance->Size);
			}

			void SetSize(NxFr::Vector2f Size)
			{
				ImGui::SetNextItemWidth(Size.x >= 0.0f ? Size.x : ImGui::GetContentRegionAvail().x);
			}

			void OffsetLabel(const Style* Instance)
			{
				if (!Instance)
				{
					return;
				}

				OffsetLabel(Instance->Label);
			}

			void OffsetLabel(float Label)
			{
				SetPosition(NxFr::Vector2f(Label, -1));
			}

			NxFr::Vector2f Fill(NxFr::Vector2f Reserved, uint64 Space)
			{
				NxFr::Vector2f Available = ImGui::GetContentRegionAvail();
				NxFr::Vector2f Spacing = ImGui::GetStyle().ItemSpacing;
				return Available - Reserved - Spacing * (float)Space;
			}

			NxFr::Vector2f Center(NxFr::StringView Text)
			{
				NxFr::Vector2f Position = ImGui::GetCursorPos();
				NxFr::Vector2f Available = ImGui::GetContentRegionAvail();
				NxFr::Vector2f Size = ImGui::CalcTextSize(Text.C());
				return Position + (Available - Size) * 0.5f;
			}

			NxFr::Vector2f Center(NxFr::Vector2f Size, uint64 Space)
			{
				NxFr::Vector2f Position = ImGui::GetCursorPos();
				NxFr::Vector2f Available = ImGui::GetContentRegionAvail();
				NxFr::Vector2f Spacing = ImGui::GetStyle().ItemSpacing;
				return Position + (Available - Size - Spacing * (float)Space) * 0.5f;
			}
		}
	}
}
