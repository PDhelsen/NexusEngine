#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		Style::Scope::Scope(NxFr::StringId Id)
			: Id(Id)
		{
			GetStyle(Id).Push();
		}

		Style::Scope::~Scope()
		{
			GetStyle(Id).Pop();
		}

		NxFr::Dictionary<NxFr::StringId, float>& Style::GetVars()
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Dictionary<NxFr::StringId, float> Vars;
			return Vars;
		}

		float& Style::GetVar(NxFr::StringId Id)
		{
			return GetVars()[Id];
		}

		void Style::RegisterVar(NxFr::StringId Id, float Instance)
		{
			GetVars().AppendOrAssign(Id, Instance);
		}

		void Style::UnregisterVar(NxFr::StringId Id)
		{
			GetVars().Remove(Id);
		}

		NxFr::Dictionary<NxFr::StringId, NxFr::Color>& Style::GetColors()
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Dictionary<NxFr::StringId, NxFr::Color> Colors;
			return Colors;
		}

		NxFr::Color& Style::GetColor(NxFr::StringId Id)
		{
			return GetColors()[Id];
		}

		void Style::RegisterColor(NxFr::StringId Id, NxFr::Color Instance)
		{
			GetColors().AppendOrAssign(Id, Instance);
		}

		void Style::UnregisterColor(NxFr::StringId Id)
		{
			GetColors().Remove(Id);
		}

		NxFr::Dictionary<NxFr::StringId, Style>& Style::GetStyles()
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Dictionary<NxFr::StringId, Style> Styles;
			return Styles;
		}

		Style& Style::GetStyle(NxFr::StringId Id)
		{
			return GetStyles()[Id];
		}

		void Style::RegisterStyle(NxFr::StringId Id, const Style& Instance)
		{
			GetStyles().AppendOrAssign(Id, Instance);
		}

		void Style::UnregisterStyle(NxFr::StringId Id)
		{
			GetStyles().Remove(Id);
		}

		void Style::Reset()
		{
			ImGuiStyle& Style = ImGui::GetStyle();

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
			Font = Style.FontSizeBase;
		}

		void Style::Push()
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

		void Style::Pop()
		{
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(6);
			ImGui::SetWindowFontScale(1);
		}
	}
}
