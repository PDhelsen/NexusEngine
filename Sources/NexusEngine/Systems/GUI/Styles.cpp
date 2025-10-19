#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		Scope::Scope(NxFr::StringId Id)
			: Id(Id), Instance(nullptr)
		{
			const Style& IdInstance = Style::GetStyle(Id);
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
				Style::GetStyle(Id).Pop();
			}
			else if (Instance)
			{
				Instance->Pop();
			}
		}

		const Style Style::Default;

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
			Width = 0.0f;
			WidthLabel = 0.0f;
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
			Flag = 0;
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
			ImGui::SetCursorPosX(Position + (WidthLabel > 0.0f ? WidthLabel : WidthLabel == 0.0f ? Size : GetVar(IdWidthLabel)));
		}
	}
}
