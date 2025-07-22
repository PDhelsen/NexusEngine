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

		LoadThemeNexus(Data["Nexus"]);
		LoadThemeImGui(Data["ImGui"]);
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

	void GUISystem::LoadThemeNexus(const YAML::Node& Node)
	{
		for (YAML::const_iterator It = Node.begin(); It != Node.end(); ++It)
		{
			const YAML::Node NodeId = It->first;
			const YAML::Node NodeProperties = It->second;

			NxFr::StringId Id(NodeId.as<NxFr::String>());
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
			Style.Colors[ImGuiCol_Text] = Colors["Text"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TextDisabled] = Colors["TextDisabled"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_WindowBg] = Colors["WindowBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ChildBg] = Colors["ChildBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_PopupBg] = Colors["PopupBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_Border] = Colors["Border"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_BorderShadow] = Colors["BorderShadow"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_FrameBg] = Colors["FrameBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_FrameBgHovered] = Colors["FrameBgHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_FrameBgActive] = Colors["FrameBgActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TitleBg] = Colors["TitleBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TitleBgActive] = Colors["TitleBgActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TitleBgCollapsed] = Colors["TitleBgCollapsed"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_MenuBarBg] = Colors["MenuBarBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ScrollbarBg] = Colors["ScrollbarBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ScrollbarGrab] = Colors["ScrollbarGrab"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ScrollbarGrabHovered] = Colors["ScrollbarGrabHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ScrollbarGrabActive] = Colors["ScrollbarGrabActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_CheckMark] = Colors["CheckMark"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_SliderGrab] = Colors["SliderGrab"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_SliderGrabActive] = Colors["SliderGrabActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_Button] = Colors["Button"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ButtonHovered] = Colors["ButtonHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ButtonActive] = Colors["ButtonActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_Header] = Colors["Header"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_HeaderHovered] = Colors["HeaderHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_HeaderActive] = Colors["HeaderActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_Separator] = Colors["Separator"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_SeparatorHovered] = Colors["SeparatorHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_SeparatorActive] = Colors["SeparatorActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ResizeGrip] = Colors["ResizeGrip"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ResizeGripHovered] = Colors["ResizeGripHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ResizeGripActive] = Colors["ResizeGripActive"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_InputTextCursor] = Colors["InputTextCursor"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabHovered] = Colors["TabHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_Tab] = Colors["Tab"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabSelected] = Colors["TabSelected"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabSelectedOverline] = Colors["TabSelectedOverline"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabDimmed] = Colors["TabDimmed"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabDimmedSelected] = Colors["TabDimmedSelected"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TabDimmedSelectedOverline] = Colors["TabDimmedSelectedOverline"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_DockingPreview] = Colors["DockingPreview"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_DockingEmptyBg] = Colors["DockingEmptyBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_PlotLines] = Colors["PlotLines"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_PlotLinesHovered] = Colors["PlotLinesHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_PlotHistogram] = Colors["PlotHistogram"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_PlotHistogramHovered] = Colors["PlotHistogramHovered"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TableHeaderBg] = Colors["TableHeaderBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TableBorderStrong] = Colors["TableBorderStrong"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TableBorderLight] = Colors["TableBorderLight"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TableRowBg] = Colors["TableRowBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TableRowBgAlt] = Colors["TableRowBgAlt"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TextLink] = Colors["TextLink"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TextSelectedBg] = Colors["TextSelectedBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_TreeLines] = Colors["TreeLines"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_DragDropTarget] = Colors["DragDropTarget"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_NavCursor] = Colors["NavCursor"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_NavWindowingHighlight] = Colors["NavWindowingHighlight"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_NavWindowingDimBg] = Colors["NavWindowingDimBg"].as<NxFr::Vector4f>();
			Style.Colors[ImGuiCol_ModalWindowDimBg] = Colors["ModalWindowDimBg"].as<NxFr::Vector4f>();
		}
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
			Emitter << YAML::Key << "Text" << YAML::Value << NxFr::Vector4f(Style.Colors[ 0].x, Style.Colors[ 0].y, Style.Colors[ 0].z, Style.Colors[ 0].w);
			Emitter << YAML::Key << "TextDisabled" << YAML::Value << NxFr::Vector4f(Style.Colors[ 1].x, Style.Colors[ 1].y, Style.Colors[ 1].z, Style.Colors[ 1].w);
			Emitter << YAML::Key << "WindowBg" << YAML::Value << NxFr::Vector4f(Style.Colors[ 2].x, Style.Colors[ 2].y, Style.Colors[ 2].z, Style.Colors[ 2].w);
			Emitter << YAML::Key << "ChildBg" << YAML::Value << NxFr::Vector4f(Style.Colors[ 3].x, Style.Colors[ 3].y, Style.Colors[ 3].z, Style.Colors[ 3].w);
			Emitter << YAML::Key << "PopupBg" << YAML::Value << NxFr::Vector4f(Style.Colors[ 4].x, Style.Colors[ 4].y, Style.Colors[ 4].z, Style.Colors[ 4].w);
			Emitter << YAML::Key << "Border" << YAML::Value << NxFr::Vector4f(Style.Colors[ 5].x, Style.Colors[ 5].y, Style.Colors[ 5].z, Style.Colors[ 5].w);
			Emitter << YAML::Key << "BorderShadow" << YAML::Value << NxFr::Vector4f(Style.Colors[ 6].x, Style.Colors[ 6].y, Style.Colors[ 6].z, Style.Colors[ 6].w);
			Emitter << YAML::Key << "FrameBg" << YAML::Value << NxFr::Vector4f(Style.Colors[ 7].x, Style.Colors[ 7].y, Style.Colors[ 7].z, Style.Colors[ 7].w);
			Emitter << YAML::Key << "FrameBgHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[ 8].x, Style.Colors[ 8].y, Style.Colors[ 8].z, Style.Colors[ 8].w);
			Emitter << YAML::Key << "FrameBgActive" << YAML::Value << NxFr::Vector4f(Style.Colors[ 9].x, Style.Colors[ 9].y, Style.Colors[ 9].z, Style.Colors[ 9].w);
			Emitter << YAML::Key << "TitleBg" << YAML::Value << NxFr::Vector4f(Style.Colors[10].x, Style.Colors[10].y, Style.Colors[10].z, Style.Colors[10].w);
			Emitter << YAML::Key << "TitleBgActive" << YAML::Value << NxFr::Vector4f(Style.Colors[11].x, Style.Colors[11].y, Style.Colors[11].z, Style.Colors[11].w);
			Emitter << YAML::Key << "TitleBgCollapsed" << YAML::Value << NxFr::Vector4f(Style.Colors[12].x, Style.Colors[12].y, Style.Colors[12].z, Style.Colors[12].w);
			Emitter << YAML::Key << "MenuBarBg" << YAML::Value << NxFr::Vector4f(Style.Colors[13].x, Style.Colors[13].y, Style.Colors[13].z, Style.Colors[13].w);
			Emitter << YAML::Key << "ScrollbarBg" << YAML::Value << NxFr::Vector4f(Style.Colors[14].x, Style.Colors[14].y, Style.Colors[14].z, Style.Colors[14].w);
			Emitter << YAML::Key << "ScrollbarGrab" << YAML::Value << NxFr::Vector4f(Style.Colors[15].x, Style.Colors[15].y, Style.Colors[15].z, Style.Colors[15].w);
			Emitter << YAML::Key << "ScrollbarGrabHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[16].x, Style.Colors[16].y, Style.Colors[16].z, Style.Colors[16].w);
			Emitter << YAML::Key << "ScrollbarGrabActive" << YAML::Value << NxFr::Vector4f(Style.Colors[17].x, Style.Colors[17].y, Style.Colors[17].z, Style.Colors[17].w);
			Emitter << YAML::Key << "CheckMark" << YAML::Value << NxFr::Vector4f(Style.Colors[18].x, Style.Colors[18].y, Style.Colors[18].z, Style.Colors[18].w);
			Emitter << YAML::Key << "SliderGrab" << YAML::Value << NxFr::Vector4f(Style.Colors[19].x, Style.Colors[19].y, Style.Colors[19].z, Style.Colors[19].w);
			Emitter << YAML::Key << "SliderGrabActive" << YAML::Value << NxFr::Vector4f(Style.Colors[20].x, Style.Colors[20].y, Style.Colors[20].z, Style.Colors[20].w);
			Emitter << YAML::Key << "Button" << YAML::Value << NxFr::Vector4f(Style.Colors[21].x, Style.Colors[21].y, Style.Colors[21].z, Style.Colors[21].w);
			Emitter << YAML::Key << "ButtonHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[22].x, Style.Colors[22].y, Style.Colors[22].z, Style.Colors[22].w);
			Emitter << YAML::Key << "ButtonActive" << YAML::Value << NxFr::Vector4f(Style.Colors[23].x, Style.Colors[23].y, Style.Colors[23].z, Style.Colors[23].w);
			Emitter << YAML::Key << "Header" << YAML::Value << NxFr::Vector4f(Style.Colors[24].x, Style.Colors[24].y, Style.Colors[24].z, Style.Colors[24].w);
			Emitter << YAML::Key << "HeaderHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[25].x, Style.Colors[25].y, Style.Colors[25].z, Style.Colors[25].w);
			Emitter << YAML::Key << "HeaderActive" << YAML::Value << NxFr::Vector4f(Style.Colors[26].x, Style.Colors[26].y, Style.Colors[26].z, Style.Colors[26].w);
			Emitter << YAML::Key << "Separator" << YAML::Value << NxFr::Vector4f(Style.Colors[27].x, Style.Colors[27].y, Style.Colors[27].z, Style.Colors[27].w);
			Emitter << YAML::Key << "SeparatorHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[28].x, Style.Colors[28].y, Style.Colors[28].z, Style.Colors[28].w);
			Emitter << YAML::Key << "SeparatorActive" << YAML::Value << NxFr::Vector4f(Style.Colors[29].x, Style.Colors[29].y, Style.Colors[29].z, Style.Colors[29].w);
			Emitter << YAML::Key << "ResizeGrip" << YAML::Value << NxFr::Vector4f(Style.Colors[30].x, Style.Colors[30].y, Style.Colors[30].z, Style.Colors[30].w);
			Emitter << YAML::Key << "ResizeGripHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[31].x, Style.Colors[31].y, Style.Colors[31].z, Style.Colors[31].w);
			Emitter << YAML::Key << "ResizeGripActive" << YAML::Value << NxFr::Vector4f(Style.Colors[32].x, Style.Colors[32].y, Style.Colors[32].z, Style.Colors[32].w);
			Emitter << YAML::Key << "InputTextCursor" << YAML::Value << NxFr::Vector4f(Style.Colors[33].x, Style.Colors[33].y, Style.Colors[33].z, Style.Colors[33].w);
			Emitter << YAML::Key << "TabHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[34].x, Style.Colors[34].y, Style.Colors[34].z, Style.Colors[34].w);
			Emitter << YAML::Key << "Tab" << YAML::Value << NxFr::Vector4f(Style.Colors[35].x, Style.Colors[35].y, Style.Colors[35].z, Style.Colors[35].w);
			Emitter << YAML::Key << "TabSelected" << YAML::Value << NxFr::Vector4f(Style.Colors[36].x, Style.Colors[36].y, Style.Colors[36].z, Style.Colors[36].w);
			Emitter << YAML::Key << "TabSelectedOverline" << YAML::Value << NxFr::Vector4f(Style.Colors[37].x, Style.Colors[37].y, Style.Colors[37].z, Style.Colors[37].w);
			Emitter << YAML::Key << "TabDimmed" << YAML::Value << NxFr::Vector4f(Style.Colors[38].x, Style.Colors[38].y, Style.Colors[38].z, Style.Colors[38].w);
			Emitter << YAML::Key << "TabDimmedSelected" << YAML::Value << NxFr::Vector4f(Style.Colors[39].x, Style.Colors[39].y, Style.Colors[39].z, Style.Colors[39].w);
			Emitter << YAML::Key << "TabDimmedSelectedOverline" << YAML::Value << NxFr::Vector4f(Style.Colors[40].x, Style.Colors[40].y, Style.Colors[40].z, Style.Colors[40].w);
			Emitter << YAML::Key << "DockingPreview" << YAML::Value << NxFr::Vector4f(Style.Colors[41].x, Style.Colors[41].y, Style.Colors[41].z, Style.Colors[41].w);
			Emitter << YAML::Key << "DockingEmptyBg" << YAML::Value << NxFr::Vector4f(Style.Colors[42].x, Style.Colors[42].y, Style.Colors[42].z, Style.Colors[42].w);
			Emitter << YAML::Key << "PlotLines" << YAML::Value << NxFr::Vector4f(Style.Colors[43].x, Style.Colors[43].y, Style.Colors[43].z, Style.Colors[43].w);
			Emitter << YAML::Key << "PlotLinesHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[44].x, Style.Colors[44].y, Style.Colors[44].z, Style.Colors[44].w);
			Emitter << YAML::Key << "PlotHistogram" << YAML::Value << NxFr::Vector4f(Style.Colors[45].x, Style.Colors[45].y, Style.Colors[45].z, Style.Colors[45].w);
			Emitter << YAML::Key << "PlotHistogramHovered" << YAML::Value << NxFr::Vector4f(Style.Colors[46].x, Style.Colors[46].y, Style.Colors[46].z, Style.Colors[46].w);
			Emitter << YAML::Key << "TableHeaderBg" << YAML::Value << NxFr::Vector4f(Style.Colors[47].x, Style.Colors[47].y, Style.Colors[47].z, Style.Colors[47].w);
			Emitter << YAML::Key << "TableBorderStrong" << YAML::Value << NxFr::Vector4f(Style.Colors[48].x, Style.Colors[48].y, Style.Colors[48].z, Style.Colors[48].w);
			Emitter << YAML::Key << "TableBorderLight" << YAML::Value << NxFr::Vector4f(Style.Colors[49].x, Style.Colors[49].y, Style.Colors[49].z, Style.Colors[49].w);
			Emitter << YAML::Key << "TableRowBg" << YAML::Value << NxFr::Vector4f(Style.Colors[50].x, Style.Colors[50].y, Style.Colors[50].z, Style.Colors[50].w);
			Emitter << YAML::Key << "TableRowBgAlt" << YAML::Value << NxFr::Vector4f(Style.Colors[51].x, Style.Colors[51].y, Style.Colors[51].z, Style.Colors[51].w);
			Emitter << YAML::Key << "TextLink" << YAML::Value << NxFr::Vector4f(Style.Colors[52].x, Style.Colors[52].y, Style.Colors[52].z, Style.Colors[52].w);
			Emitter << YAML::Key << "TextSelectedBg" << YAML::Value << NxFr::Vector4f(Style.Colors[53].x, Style.Colors[53].y, Style.Colors[53].z, Style.Colors[53].w);
			Emitter << YAML::Key << "TreeLines" << YAML::Value << NxFr::Vector4f(Style.Colors[54].x, Style.Colors[54].y, Style.Colors[54].z, Style.Colors[54].w);
			Emitter << YAML::Key << "DragDropTarget" << YAML::Value << NxFr::Vector4f(Style.Colors[55].x, Style.Colors[55].y, Style.Colors[55].z, Style.Colors[55].w);
			Emitter << YAML::Key << "NavCursor" << YAML::Value << NxFr::Vector4f(Style.Colors[56].x, Style.Colors[56].y, Style.Colors[56].z, Style.Colors[56].w);
			Emitter << YAML::Key << "NavWindowingHighlight" << YAML::Value << NxFr::Vector4f(Style.Colors[57].x, Style.Colors[57].y, Style.Colors[57].z, Style.Colors[57].w);
			Emitter << YAML::Key << "NavWindowingDimBg" << YAML::Value << NxFr::Vector4f(Style.Colors[58].x, Style.Colors[58].y, Style.Colors[58].z, Style.Colors[58].w);
			Emitter << YAML::Key << "ModalWindowDimBg" << YAML::Value << NxFr::Vector4f(Style.Colors[59].x, Style.Colors[59].y, Style.Colors[59].z, Style.Colors[59].w);
		}
		Emitter << YAML::EndMap;

		Emitter << YAML::EndMap;
	}
}
