#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		Scope::Scope(NxFr::StringId Id)
			: Id(Id), Instance(nullptr)
		{
			const Style& IdInstance = *Style::GetStyles().TryGet(Id);
			IdInstance.Push();
			IdInstance.SetPosition();
			IdInstance.SetWidth();
		}

		Scope::Scope(const Style* Instance)
			: Id(0), Instance(Instance)
		{
			if (!Instance)
			{
				return;
			}

			Instance->Push();
			Instance->SetPosition();
			Instance->SetWidth();
		}

		Scope::~Scope()
		{
			if (Id.IsValid())
			{
				Style::GetStyles().TryGet(Id)->Pop();
			}
			else if (Instance)
			{
				Instance->Pop();
			}
		}

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
			}

			return Copy;
		}

		Style::Style()
			:
			StylePreset(Preset::Text),
			Position(-NxFr::Vector2f::One), Width(0.0f), WidthLabel(0.0f), Height(0.0f),
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
			ImGuiStyle& Style = ImGui::GetStyle();

			Position = -NxFr::Vector2f::One;
			Width = -1.0f;
			WidthLabel = -1.0f;
			Height = 0.0f;
			Color = Style.Colors[21];
			ColorText = Style.Colors[0];
			ColorBackground = StylePreset == Preset::Panel ? Style.Colors[2] : Style.Colors[7];
			ColorBorder = Style.Colors[5];
			Alpha = Style.Alpha;
			Align = Style.ButtonTextAlign;
			Spacing = Style.ItemSpacing;
			Padding = StylePreset == Preset::Panel ? Style.WindowPadding : Style.FramePadding;
			Rounding = StylePreset == Preset::Panel ? Style.WindowRounding : Style.FrameRounding;
			Border = StylePreset == Preset::Panel ? Style.WindowBorderSize : Style.FrameBorderSize;
			Font = Style.FontScaleMain;
			Flag = ImGuiInputTextFlags_EnterReturnsTrue;
		}

		void Style::Push() const
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Color);
			ImGui::PushStyleColor(ImGuiCol_Text, ColorText);
			ImGui::PushStyleColor(StylePreset == Preset::Panel ? ImGuiCol_WindowBg : ImGuiCol_FrameBg, ColorBackground);
			ImGui::PushStyleColor(ImGuiCol_Border, ColorBorder);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Alpha);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, Align);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Spacing);
			ImGui::PushStyleVar(StylePreset == Preset::Panel ? ImGuiStyleVar_WindowPadding : ImGuiStyleVar_FramePadding, Padding);
			ImGui::PushStyleVar(StylePreset == Preset::Panel ? ImGuiStyleVar_WindowRounding : ImGuiStyleVar_FrameRounding, Rounding);
			ImGui::PushStyleVar(StylePreset == Preset::Panel ? ImGuiStyleVar_WindowBorderSize : ImGuiStyleVar_FrameBorderSize, Border);

			ImGui::SetWindowFontScale(Font);
		}

		void Style::Pop() const
		{
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(6);
			ImGui::SetWindowFontScale(1);
		}

		void Style::SetPosition() const
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

		void Style::SetWidth() const
		{
			ImGui::SetNextItemWidth(Width >= 0.0f ? Width : ImGui::GetContentRegionAvail().x);
		}

		void Style::SetWidthLabel(NxFr::StringView Label) const
		{
			float Size = ImGui::CalcTextSize(Label.C()).x;
			float Position = ImGui::GetCursorPosX() - Size;
			ImGui::SetCursorPosX(Position + (WidthLabel > 0.0f ? WidthLabel : WidthLabel == 0.0f ? Size : Styles::WidthLabel()));
		}
	}
}
