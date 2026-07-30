#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		NxFr::Registry<float>& Style::GetVars()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<float> Vars;
			return Vars;
		}

		NxFr::Registry<NxFr::Color>& Style::GetColors()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<NxFr::Color> Colors;
			return Colors;
		}

		NxFr::Registry<Style>& Style::GetStyles()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<Style> Styles;
			return Styles;
		}

		Style Style::Copy(const Style* Original)
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

		void Style::Push(const Style* Instance)
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

		void Style::Pop(const Style* Instance)
		{
			if (!Instance)
			{
				return;
			}

			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(6);
			ImGui::SetWindowFontScale(1);
		}

		void Style::SetPosition(const Style* Instance)
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

		void Style::SetWidth(const Style* Instance)
		{
			if (!Instance)
			{
				return;
			}

			ImGui::SetNextItemWidth(Instance->Size.x >= 0.0f ? Instance->Size.x : ImGui::GetContentRegionAvail().x);
		}

		void Style::SetWidth(const Style* Instance, NxFr::StringView Text, bool Label)
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

		Style::Style()
			:
			Position(-NxFr::Vector2f::One), Size(NxFr::Vector2f::Zero), Label(0.0f),
			Color(NxFr::Colors::White), ColorText(NxFr::Colors::White), ColorBackground(NxFr::Colors::Black), ColorBorder(NxFr::Colors::Gray), Alpha(1.0f),
			Align(NxFr::Vector2f(0.5f, 0.5f)), Spacing(NxFr::Vector2f(8.0f, 4.0f)), Padding(NxFr::Vector2f(4.0f , 3.0f)), Rounding(0.0f), Border(0.0f),
			Font(1.0f),
			Flag(0)
		{
		}

		Style::~Style()
		{
		}

		void Style::Reset()
		{
			*this = GUI::Styles::Default();
		}
	}
}
