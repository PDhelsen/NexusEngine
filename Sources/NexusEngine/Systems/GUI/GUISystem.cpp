#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	const NxFr::StringView SavedConfigName = "imgui";
	const NxFr::StringView SavedStyleName = "style";
	const NxFr::StringView ConfigExtension = ".ini";
	const NxFr::StringView StyleExtension = ".yaml";
	const NxFr::StringView ImGuiFolder = "imgui";
#if NEXUS_EDITOR
	const NxFr::StringView Suffix = "_editor";
#else
	const NxFr::StringView Suffix = "_app";
#endif

	static NxFr::Dictionary<NxFr::StringId, GUI::Panel*>& GetPanels()
	{
		static NxFr::Dictionary<NxFr::StringId, GUI::Panel*> Panels;
		return Panels;
	}

	GUI::Panel* GUISystem::GetPanel(NxFr::StringId Id)
	{
		return GetPanels()[Id];
	}

	void GUISystem::RegisterPanel(GUI::Panel* Instance)
	{
		GetPanels().Append(Instance->GetObjectType(), Instance);
	}

	void GUISystem::UnregisterPanel(GUI::Panel* Instance)
	{
		GetPanels().Remove(Instance->GetObjectType());
	}

	GUISystem::GUISystem()
		: Elements(), Styles()
	{

	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::RegisterElement(GUI::Element* Element)
	{
		Elements.Append(Element);
	}

	void GUISystem::UnregisterElement(GUI::Element* Element)
	{
		Elements.Remove(Element);
	}

	void GUISystem::LoadConfig(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedConfigName, ConfigExtension);
		ImGui::LoadIniSettingsFromDisk(Path.C());
	}

	void GUISystem::SaveConfig(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedConfigName, ConfigExtension);
		ImGui::SaveIniSettingsToDisk(Path.C());
	}

	void GUISystem::LoadTheme(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedStyleName, StyleExtension);
		if (!NxFr::File(Path).Exists())
		{
			return;
		}

		YAML::Node Data = NxFr::Yaml::DeserializeFile(Path);

		LoadThemeImGui(Data["ImGui"]);
		LoadThemeNexus(Data["Nexus"]);
	}

	void GUISystem::SaveTheme(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedStyleName, StyleExtension);

		YAML::Emitter Data;
		Data << YAML::BeginMap;
		Data << YAML::Key << "ImGui" << YAML::Value;
		SaveThemeImGui(Data);
		Data << YAML::Key << "Nexus" << YAML::Value;
		SaveThemeNexus(Data);
		Data << YAML::EndMap;

		NxFr::Yaml::SerializeFile(Data, Path);
	}

	void GUISystem::AppendStyle(NxFr::StringId Id, const GUI::Style& Style)
	{
		Styles.Append(Id, Style);
	}

	void GUISystem::AssignStyle(NxFr::StringId Id, const GUI::Style& Style)
	{
		Styles[Id] = Style;
	}

	void GUISystem::RemoveStyle(NxFr::StringId Id)
	{
		Styles.Remove(Id);
	}

	const GUI::Style& GUISystem::GetStyle(NxFr::StringId Id)
	{
		return Styles[Id];
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		Imgui::Initialize();
		LoadConfig();
		LoadTheme();
	}

	void GUISystem::OnShutdown()
	{
		SaveTheme();
		SaveConfig();
		Imgui::Shutdown();
		
		System::OnShutdown();
	}

	void GUISystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Imgui::Frame();

		for (auto& Element : Elements)
		{
			if (Element->IsManual())
			{
				continue;
			}

			Element->Tick(TimeStep);
		}

		Imgui::Render();
	}

	NxFr::Path GUISystem::GetSettingsPath(NxFr::StringView Name, NxFr::StringView Saved, NxFr::StringView Extension)
	{
		NxFr::Path Path = NxFr::Path("");
		if (!Name.IsEmpty())
		{
			Path = NxFr::Path(NxFr::Paths::Configs.ToView()) + ImGuiFolder + (Name + Extension);
		}
		else
		{
			Path = NxFr::Path(NxFr::Paths::Saved.ToView()) + ImGuiFolder + (Saved + Suffix + Extension);
		}

		NxFr::Directory(Path.GetDirectoryPath()).Create();
		return Path;
	}

	void GUISystem::LoadThemeImGui(const YAML::Node& Node)
	{
		ImGuiStyle& Style = ImGui::GetStyle();

		Style.FontSizeBase = Node["FontSizeBase"].as<float>();
		Style.FontScaleMain = Node["FontScaleMain"].as<float>();
		Style.FontScaleDpi = Node["FontScaleDpi"].as<float>();
		Style.Alpha = Node["Alpha"].as<float>();
		Style.DisabledAlpha = Node["DisabledAlpha"].as<float>();
		Style.WindowPadding = Node["WindowPadding"].as<NxFr::Vector2f>();
		Style.WindowRounding = Node["WindowRounding"].as<float>();
		Style.WindowBorderSize = Node["WindowBorderSize"].as<float>();
		Style.WindowBorderHoverPadding = Node["WindowBorderHoverPadding"].as<float>();
		Style.WindowMinSize = Node["WindowMinSize"].as<NxFr::Vector2f>();
		Style.WindowTitleAlign = Node["WindowTitleAlign"].as<NxFr::Vector2f>();
		Style.WindowMenuButtonPosition = (ImGuiDir)Node["WindowMenuButtonPosition"].as<int32>();
		Style.ChildRounding = Node["ChildRounding"].as<float>();
		Style.ChildBorderSize = Node["ChildBorderSize"].as<float>();
		Style.PopupRounding = Node["PopupRounding"].as<float>();
		Style.PopupBorderSize = Node["PopupBorderSize"].as<float>();
		Style.FramePadding = Node["FramePadding"].as<NxFr::Vector2f>();
		Style.FrameRounding = Node["FrameRounding"].as<float>();
		Style.FrameBorderSize = Node["FrameBorderSize"].as<float>();
		Style.ItemSpacing = Node["ItemSpacing"].as<NxFr::Vector2f>();
		Style.ItemInnerSpacing = Node["ItemInnerSpacing"].as<NxFr::Vector2f>();
		Style.CellPadding = Node["CellPadding"].as<NxFr::Vector2f>();
		Style.TouchExtraPadding = Node["TouchExtraPadding"].as<NxFr::Vector2f>();
		Style.IndentSpacing = Node["IndentSpacing"].as<float>();
		Style.ColumnsMinSpacing = Node["ColumnsMinSpacing"].as<float>();
		Style.ScrollbarSize = Node["ScrollbarSize"].as<float>();
		Style.ScrollbarRounding = Node["ScrollbarRounding"].as<float>();
		Style.GrabMinSize = Node["GrabMinSize"].as<float>();
		Style.GrabRounding = Node["GrabRounding"].as<float>();
		Style.LogSliderDeadzone = Node["LogSliderDeadzone"].as<float>();
		Style.ImageBorderSize = Node["ImageBorderSize"].as<float>();
		Style.TabRounding = Node["TabRounding"].as<float>();
		Style.TabBorderSize = Node["TabBorderSize"].as<float>();
		Style.TabCloseButtonMinWidthSelected = Node["TabCloseButtonMinWidthSelected"].as<float>();
		Style.TabCloseButtonMinWidthUnselected = Node["TabCloseButtonMinWidthUnselected"].as<float>();
		Style.TabBarBorderSize = Node["TabBarBorderSize"].as<float>();
		Style.TabBarOverlineSize = Node["TabBarOverlineSize"].as<float>();
		Style.TableAngledHeadersAngle = Node["TableAngledHeadersAngle"].as<float>();
		Style.TableAngledHeadersTextAlign = Node["TableAngledHeadersTextAlign"].as<NxFr::Vector2f>();
		Style.TreeLinesFlags = (ImGuiTreeNodeFlags)Node["TreeLinesFlags"].as<int32>();
		Style.TreeLinesSize = Node["TreeLinesSize"].as<float>();
		Style.TreeLinesRounding = Node["TreeLinesRounding"].as<float>();
		Style.ColorButtonPosition = (ImGuiDir)Node["ColorButtonPosition"].as<int32>();
		Style.ButtonTextAlign = Node["ButtonTextAlign"].as<NxFr::Vector2f>();
		Style.SelectableTextAlign = Node["SelectableTextAlign"].as<NxFr::Vector2f>();
		Style.SeparatorTextBorderSize = Node["SeparatorTextBorderSize"].as<float>();
		Style.SeparatorTextAlign = Node["SeparatorTextAlign"].as<NxFr::Vector2f>();
		Style.SeparatorTextPadding = Node["SeparatorTextPadding"].as<NxFr::Vector2f>();
		Style.DisplayWindowPadding = Node["DisplayWindowPadding"].as<NxFr::Vector2f>();
		Style.DisplaySafeAreaPadding = Node["DisplaySafeAreaPadding"].as<NxFr::Vector2f>();
		Style.DockingSeparatorSize = Node["DockingSeparatorSize"].as<float>();
		Style.AntiAliasedLines = Node["AntiAliasedLines"].as<bool>();
		Style.AntiAliasedLinesUseTex = Node["AntiAliasedLinesUseTex"].as<bool>();
		Style.AntiAliasedFill = Node["AntiAliasedFill"].as<bool>();
		Style.CurveTessellationTol = Node["CurveTessellationTol"].as<float>();
		Style.CircleTessellationMaxError = Node["CircleTessellationMaxError"].as<float>();
		Style.HoverStationaryDelay = Node["HoverStationaryDelay"].as<float>();
		Style.HoverDelayShort = Node["HoverDelayShort"].as<float>();
		Style.HoverDelayNormal = Node["HoverDelayNormal"].as<float>();
		Style.HoverFlagsForTooltipMouse = (ImGuiHoveredFlags)Node["HoverFlagsForTooltipMouse"].as<int32>();
		Style.HoverFlagsForTooltipNav = (ImGuiHoveredFlags)Node["HoverFlagsForTooltipNav"].as<int32>();

		YAML::Node Colors = Node["Colors"];
		{
			Style.Colors[ImGuiCol_Text] = Colors["Text"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TextDisabled] = Colors["TextDisabled"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_WindowBg] = Colors["WindowBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ChildBg] = Colors["ChildBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_PopupBg] = Colors["PopupBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_Border] = Colors["Border"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_BorderShadow] = Colors["BorderShadow"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_FrameBg] = Colors["FrameBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_FrameBgHovered] = Colors["FrameBgHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_FrameBgActive] = Colors["FrameBgActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TitleBg] = Colors["TitleBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TitleBgActive] = Colors["TitleBgActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TitleBgCollapsed] = Colors["TitleBgCollapsed"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_MenuBarBg] = Colors["MenuBarBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ScrollbarBg] = Colors["ScrollbarBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ScrollbarGrab] = Colors["ScrollbarGrab"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ScrollbarGrabHovered] = Colors["ScrollbarGrabHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ScrollbarGrabActive] = Colors["ScrollbarGrabActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_CheckMark] = Colors["CheckMark"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_SliderGrab] = Colors["SliderGrab"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_SliderGrabActive] = Colors["SliderGrabActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_Button] = Colors["Button"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ButtonHovered] = Colors["ButtonHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ButtonActive] = Colors["ButtonActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_Header] = Colors["Header"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_HeaderHovered] = Colors["HeaderHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_HeaderActive] = Colors["HeaderActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_Separator] = Colors["Separator"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_SeparatorHovered] = Colors["SeparatorHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_SeparatorActive] = Colors["SeparatorActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ResizeGrip] = Colors["ResizeGrip"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ResizeGripHovered] = Colors["ResizeGripHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ResizeGripActive] = Colors["ResizeGripActive"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_InputTextCursor] = Colors["InputTextCursor"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabHovered] = Colors["TabHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_Tab] = Colors["Tab"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabSelected] = Colors["TabSelected"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabSelectedOverline] = Colors["TabSelectedOverline"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabDimmed] = Colors["TabDimmed"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabDimmedSelected] = Colors["TabDimmedSelected"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TabDimmedSelectedOverline] = Colors["TabDimmedSelectedOverline"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_DockingPreview] = Colors["DockingPreview"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_DockingEmptyBg] = Colors["DockingEmptyBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_PlotLines] = Colors["PlotLines"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_PlotLinesHovered] = Colors["PlotLinesHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_PlotHistogram] = Colors["PlotHistogram"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_PlotHistogramHovered] = Colors["PlotHistogramHovered"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TableHeaderBg] = Colors["TableHeaderBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TableBorderStrong] = Colors["TableBorderStrong"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TableBorderLight] = Colors["TableBorderLight"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TableRowBg] = Colors["TableRowBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TableRowBgAlt] = Colors["TableRowBgAlt"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TextLink] = Colors["TextLink"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TextSelectedBg] = Colors["TextSelectedBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_TreeLines] = Colors["TreeLines"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_DragDropTarget] = Colors["DragDropTarget"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_NavCursor] = Colors["NavCursor"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_NavWindowingHighlight] = Colors["NavWindowingHighlight"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_NavWindowingDimBg] = Colors["NavWindowingDimBg"].as<NxFr::Color>();
			Style.Colors[ImGuiCol_ModalWindowDimBg] = Colors["ModalWindowDimBg"].as<NxFr::Color>();
		}
	}

	void GUISystem::LoadThemeNexus(const YAML::Node& Node)
	{
		for (YAML::const_iterator It = Node.begin(); It != Node.end(); ++It)
		{
			const YAML::Node NodeId = It->first;
			const YAML::Node NodeProperties = It->second;

			NxFr::StringId Id = NodeId.as<NxFr::StringId>();
			GUI::Style Style;

			for (uint64 Index = 0; Index < NodeProperties.size(); ++Index)
			{
				const YAML::Node NodeProperty = NodeProperties[Index];

				uint32 ImGuiId = NodeProperty["id"].as<int32>();
				GUI::Style::Type Type = (GUI::Style::Type)NodeProperty["type"].as<int32>();
				NxFr::Vector4f Data = NodeProperty["data"].as<NxFr::Vector4f>();

				switch (Type)
				{
				case NxEn::GUI::Style::Type::Color:	Style.AppendColor(ImGuiId, Data);	break;
				case NxEn::GUI::Style::Type::Var:	Style.AppendVar(ImGuiId, Data.x);	break;
				case NxEn::GUI::Style::Type::VarX:	Style.AppendVarX(ImGuiId, Data.x);	break;
				case NxEn::GUI::Style::Type::VarY:	Style.AppendVarY(ImGuiId, Data.y);	break;
				case NxEn::GUI::Style::Type::VarXY: Style.AppendColor(ImGuiId, Data);	break;
				}
			}

			Styles.Append(Id, Style);
		}
	}

	void GUISystem::SaveThemeImGui(YAML::Emitter& Emitter)
	{
		ImGuiStyle& Style = ImGui::GetStyle();

		Emitter << YAML::BeginMap;

		Emitter << YAML::Key << "FontSizeBase" << YAML::Value << Style.FontSizeBase;
		Emitter << YAML::Key << "FontScaleMain" << YAML::Value << Style.FontScaleMain;
		Emitter << YAML::Key << "FontScaleDpi" << YAML::Value << Style.FontScaleDpi;
		Emitter << YAML::Key << "Alpha" << YAML::Value << Style.Alpha;
		Emitter << YAML::Key << "DisabledAlpha" << YAML::Value << Style.DisabledAlpha;
		Emitter << YAML::Key << "WindowPadding" << YAML::Value << NxFr::Vector2f(Style.WindowPadding.x, Style.WindowPadding.y);
		Emitter << YAML::Key << "WindowRounding" << YAML::Value << Style.WindowRounding;
		Emitter << YAML::Key << "WindowBorderSize" << YAML::Value << Style.WindowBorderSize;
		Emitter << YAML::Key << "WindowBorderHoverPadding" << YAML::Value << Style.WindowBorderHoverPadding;
		Emitter << YAML::Key << "WindowMinSize" << YAML::Value << NxFr::Vector2f(Style.WindowMinSize.x, Style.WindowMinSize.y);
		Emitter << YAML::Key << "WindowTitleAlign" << YAML::Value << NxFr::Vector2f(Style.WindowTitleAlign.x, Style.WindowTitleAlign.y);
		Emitter << YAML::Key << "WindowMenuButtonPosition" << YAML::Value << (int32)Style.WindowMenuButtonPosition;
		Emitter << YAML::Key << "ChildRounding" << YAML::Value << Style.ChildRounding;
		Emitter << YAML::Key << "ChildBorderSize" << YAML::Value << Style.ChildBorderSize;
		Emitter << YAML::Key << "PopupRounding" << YAML::Value << Style.PopupRounding;
		Emitter << YAML::Key << "PopupBorderSize" << YAML::Value << Style.PopupBorderSize;
		Emitter << YAML::Key << "FramePadding" << YAML::Value << NxFr::Vector2f(Style.FramePadding.x, Style.FramePadding.y);
		Emitter << YAML::Key << "FrameRounding" << YAML::Value << Style.FrameRounding;
		Emitter << YAML::Key << "FrameBorderSize" << YAML::Value << Style.FrameBorderSize;
		Emitter << YAML::Key << "ItemSpacing" << YAML::Value << NxFr::Vector2f(Style.ItemSpacing.x, Style.ItemSpacing.y);
		Emitter << YAML::Key << "ItemInnerSpacing" << YAML::Value << NxFr::Vector2f(Style.ItemInnerSpacing.x, Style.ItemInnerSpacing.y);
		Emitter << YAML::Key << "CellPadding" << YAML::Value << NxFr::Vector2f(Style.CellPadding.x, Style.CellPadding.y);
		Emitter << YAML::Key << "TouchExtraPadding" << YAML::Value << NxFr::Vector2f(Style.TouchExtraPadding.x, Style.TouchExtraPadding.y);
		Emitter << YAML::Key << "IndentSpacing" << YAML::Value << Style.IndentSpacing;
		Emitter << YAML::Key << "ColumnsMinSpacing" << YAML::Value << Style.ColumnsMinSpacing;
		Emitter << YAML::Key << "ScrollbarSize" << YAML::Value << Style.ScrollbarSize;
		Emitter << YAML::Key << "ScrollbarRounding" << YAML::Value << Style.ScrollbarRounding;
		Emitter << YAML::Key << "GrabMinSize" << YAML::Value << Style.GrabMinSize;
		Emitter << YAML::Key << "GrabRounding" << YAML::Value << Style.GrabRounding;
		Emitter << YAML::Key << "LogSliderDeadzone" << YAML::Value << Style.LogSliderDeadzone;
		Emitter << YAML::Key << "ImageBorderSize" << YAML::Value << Style.ImageBorderSize;
		Emitter << YAML::Key << "TabRounding" << YAML::Value << Style.TabRounding;
		Emitter << YAML::Key << "TabBorderSize" << YAML::Value << Style.TabBorderSize;
		Emitter << YAML::Key << "TabCloseButtonMinWidthSelected" << YAML::Value << Style.TabCloseButtonMinWidthSelected;
		Emitter << YAML::Key << "TabCloseButtonMinWidthUnselected" << YAML::Value << Style.TabCloseButtonMinWidthUnselected;
		Emitter << YAML::Key << "TabBarBorderSize" << YAML::Value << Style.TabBarBorderSize;
		Emitter << YAML::Key << "TabBarOverlineSize" << YAML::Value << Style.TabBarOverlineSize;
		Emitter << YAML::Key << "TableAngledHeadersAngle" << YAML::Value << Style.TableAngledHeadersAngle;
		Emitter << YAML::Key << "TableAngledHeadersTextAlign" << YAML::Value << NxFr::Vector2f(Style.TableAngledHeadersTextAlign.x, Style.TableAngledHeadersTextAlign.y);
		Emitter << YAML::Key << "TreeLinesFlags" << YAML::Value << (int32)Style.TreeLinesFlags;
		Emitter << YAML::Key << "TreeLinesSize" << YAML::Value << Style.TreeLinesSize;
		Emitter << YAML::Key << "TreeLinesRounding" << YAML::Value << Style.TreeLinesRounding;
		Emitter << YAML::Key << "ColorButtonPosition" << YAML::Value << (int32)Style.ColorButtonPosition;
		Emitter << YAML::Key << "ButtonTextAlign" << YAML::Value << NxFr::Vector2f(Style.ButtonTextAlign.x, Style.ButtonTextAlign.y);
		Emitter << YAML::Key << "SelectableTextAlign" << YAML::Value << NxFr::Vector2f(Style.SelectableTextAlign.x, Style.SelectableTextAlign.y);
		Emitter << YAML::Key << "SeparatorTextBorderSize" << YAML::Value << Style.SeparatorTextBorderSize;
		Emitter << YAML::Key << "SeparatorTextAlign" << YAML::Value << NxFr::Vector2f(Style.SeparatorTextAlign.x, Style.SeparatorTextAlign.y);
		Emitter << YAML::Key << "SeparatorTextPadding" << YAML::Value << NxFr::Vector2f(Style.SeparatorTextPadding.x, Style.SeparatorTextPadding.y);
		Emitter << YAML::Key << "DisplayWindowPadding" << YAML::Value << NxFr::Vector2f(Style.DisplayWindowPadding.x, Style.DisplayWindowPadding.y);
		Emitter << YAML::Key << "DisplaySafeAreaPadding" << YAML::Value << NxFr::Vector2f(Style.DisplaySafeAreaPadding.x, Style.DisplaySafeAreaPadding.y);
		Emitter << YAML::Key << "DockingSeparatorSize" << YAML::Value << Style.DockingSeparatorSize;
		Emitter << YAML::Key << "MouseCursorScale" << YAML::Value << Style.MouseCursorScale;
		Emitter << YAML::Key << "AntiAliasedLines" << YAML::Value << Style.AntiAliasedLines;
		Emitter << YAML::Key << "AntiAliasedLinesUseTex" << YAML::Value << Style.AntiAliasedLinesUseTex;
		Emitter << YAML::Key << "AntiAliasedFill" << YAML::Value << Style.AntiAliasedFill;
		Emitter << YAML::Key << "CurveTessellationTol" << YAML::Value << Style.CurveTessellationTol;
		Emitter << YAML::Key << "CircleTessellationMaxError" << YAML::Value << Style.CircleTessellationMaxError;
		Emitter << YAML::Key << "HoverStationaryDelay" << YAML::Value << Style.HoverStationaryDelay;
		Emitter << YAML::Key << "HoverDelayShort" << YAML::Value << Style.HoverDelayShort;
		Emitter << YAML::Key << "HoverDelayNormal" << YAML::Value << Style.HoverDelayNormal;
		Emitter << YAML::Key << "HoverFlagsForTooltipMouse" << YAML::Value << (int32)Style.HoverFlagsForTooltipMouse;
		Emitter << YAML::Key << "HoverFlagsForTooltipNav" << YAML::Value << (int32)Style.HoverFlagsForTooltipNav;

		Emitter << YAML::Key << "Colors" << YAML::Value << YAML::BeginMap;
		{
			Emitter << YAML::Key << "Text" << YAML::Value << Style.Colors[0];
			Emitter << YAML::Key << "TextDisabled" << YAML::Value << Style.Colors[1];
			Emitter << YAML::Key << "WindowBg" << YAML::Value << Style.Colors[2];
			Emitter << YAML::Key << "ChildBg" << YAML::Value << Style.Colors[3];
			Emitter << YAML::Key << "PopupBg" << YAML::Value << Style.Colors[4];
			Emitter << YAML::Key << "Border" << YAML::Value << Style.Colors[5];
			Emitter << YAML::Key << "BorderShadow" << YAML::Value << Style.Colors[6];
			Emitter << YAML::Key << "FrameBg" << YAML::Value << Style.Colors[7];
			Emitter << YAML::Key << "FrameBgHovered" << YAML::Value << Style.Colors[8];
			Emitter << YAML::Key << "FrameBgActive" << YAML::Value << Style.Colors[9];
			Emitter << YAML::Key << "TitleBg" << YAML::Value << Style.Colors[10];
			Emitter << YAML::Key << "TitleBgActive" << YAML::Value << Style.Colors[11];
			Emitter << YAML::Key << "TitleBgCollapsed" << YAML::Value << Style.Colors[12];
			Emitter << YAML::Key << "MenuBarBg" << YAML::Value << Style.Colors[13];
			Emitter << YAML::Key << "ScrollbarBg" << YAML::Value << Style.Colors[14];
			Emitter << YAML::Key << "ScrollbarGrab" << YAML::Value << Style.Colors[15];
			Emitter << YAML::Key << "ScrollbarGrabHovered" << YAML::Value << Style.Colors[16];
			Emitter << YAML::Key << "ScrollbarGrabActive" << YAML::Value << Style.Colors[17];
			Emitter << YAML::Key << "CheckMark" << YAML::Value << Style.Colors[18];
			Emitter << YAML::Key << "SliderGrab" << YAML::Value << Style.Colors[19];
			Emitter << YAML::Key << "SliderGrabActive" << YAML::Value << Style.Colors[20];
			Emitter << YAML::Key << "Button" << YAML::Value << Style.Colors[21];
			Emitter << YAML::Key << "ButtonHovered" << YAML::Value << Style.Colors[22];
			Emitter << YAML::Key << "ButtonActive" << YAML::Value << Style.Colors[23];
			Emitter << YAML::Key << "Header" << YAML::Value << Style.Colors[24];
			Emitter << YAML::Key << "HeaderHovered" << YAML::Value << Style.Colors[25];
			Emitter << YAML::Key << "HeaderActive" << YAML::Value << Style.Colors[26];
			Emitter << YAML::Key << "Separator" << YAML::Value << Style.Colors[27];
			Emitter << YAML::Key << "SeparatorHovered" << YAML::Value << Style.Colors[28];
			Emitter << YAML::Key << "SeparatorActive" << YAML::Value << Style.Colors[29];
			Emitter << YAML::Key << "ResizeGrip" << YAML::Value << Style.Colors[30];
			Emitter << YAML::Key << "ResizeGripHovered" << YAML::Value << Style.Colors[31];
			Emitter << YAML::Key << "ResizeGripActive" << YAML::Value << Style.Colors[32];
			Emitter << YAML::Key << "InputTextCursor" << YAML::Value << Style.Colors[33];
			Emitter << YAML::Key << "TabHovered" << YAML::Value << Style.Colors[34];
			Emitter << YAML::Key << "Tab" << YAML::Value << Style.Colors[35];
			Emitter << YAML::Key << "TabSelected" << YAML::Value << Style.Colors[36];
			Emitter << YAML::Key << "TabSelectedOverline" << YAML::Value << Style.Colors[37];
			Emitter << YAML::Key << "TabDimmed" << YAML::Value << Style.Colors[38];
			Emitter << YAML::Key << "TabDimmedSelected" << YAML::Value << Style.Colors[39];
			Emitter << YAML::Key << "TabDimmedSelectedOverline" << YAML::Value << Style.Colors[40];
			Emitter << YAML::Key << "DockingPreview" << YAML::Value << Style.Colors[41];
			Emitter << YAML::Key << "DockingEmptyBg" << YAML::Value << Style.Colors[42];
			Emitter << YAML::Key << "PlotLines" << YAML::Value << Style.Colors[43];
			Emitter << YAML::Key << "PlotLinesHovered" << YAML::Value << Style.Colors[44];
			Emitter << YAML::Key << "PlotHistogram" << YAML::Value << Style.Colors[45];
			Emitter << YAML::Key << "PlotHistogramHovered" << YAML::Value << Style.Colors[46];
			Emitter << YAML::Key << "TableHeaderBg" << YAML::Value << Style.Colors[47];
			Emitter << YAML::Key << "TableBorderStrong" << YAML::Value << Style.Colors[48];
			Emitter << YAML::Key << "TableBorderLight" << YAML::Value << Style.Colors[49];
			Emitter << YAML::Key << "TableRowBg" << YAML::Value << Style.Colors[50];
			Emitter << YAML::Key << "TableRowBgAlt" << YAML::Value << Style.Colors[51];
			Emitter << YAML::Key << "TextLink" << YAML::Value << Style.Colors[52];
			Emitter << YAML::Key << "TextSelectedBg" << YAML::Value << Style.Colors[53];
			Emitter << YAML::Key << "TreeLines" << YAML::Value << Style.Colors[54];
			Emitter << YAML::Key << "DragDropTarget" << YAML::Value << Style.Colors[55];
			Emitter << YAML::Key << "NavCursor" << YAML::Value << Style.Colors[56];
			Emitter << YAML::Key << "NavWindowingHighlight" << YAML::Value << Style.Colors[57];
			Emitter << YAML::Key << "NavWindowingDimBg" << YAML::Value << Style.Colors[58];
			Emitter << YAML::Key << "ModalWindowDimBg" << YAML::Value << Style.Colors[59];
		}
		Emitter << YAML::EndMap;

		Emitter << YAML::EndMap;
	}

	void GUISystem::SaveThemeNexus(YAML::Emitter& Emitter)
	{
		Emitter << YAML::BeginMap;
		for (auto& [Id, Style] : Styles)
		{
			Emitter << YAML::Key << Id;
			Emitter << YAML::Value << YAML::BeginSeq;
			for (auto& [Index, Property] : Style.Properties)
			{
				Emitter << YAML::BeginMap;
				Emitter << YAML::Key << "id"	<< YAML::Value << Index;
				Emitter << YAML::Key << "type"	<< YAML::Value << (int32)Property.Flag;
				Emitter << YAML::Key << "data"	<< YAML::Value << Property.Data;
				Emitter << YAML::EndMap;
			}
			Emitter << YAML::EndSeq;
		}
		Emitter << YAML::EndMap;
	}
}
