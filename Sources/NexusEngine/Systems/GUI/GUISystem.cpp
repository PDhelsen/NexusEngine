#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId GuiElementsId = "GUI - Elements"_Sid;
	}
}

namespace NxEn
{
#if NEXUS_EDITOR
	const NxFr::StringView Suffix = "_editor";
#else
	const NxFr::StringView Suffix = "_app";
#endif

	const NxFr::StringView SavedConfigName = "imgui";
	const NxFr::StringView SavedStyleName = "style";
	const NxFr::StringView SavedLayoutName = "layout";
	const NxFr::StringView ConfigExtension = ".ini";
	const NxFr::StringView StyleExtension = ".yaml";
	const NxFr::StringView LayoutExtension = ".layout";
	const NxFr::StringView Folder = "imgui";

	static GUI::Window& GetMainWindow()
	{
		static GUI::Window Window;
		return Window;
	}

	static GUI::Menu& GetMainMenu()
	{
		return GetMainWindow().GetMenu();
	}

	static NxFr::Dictionary<NxFr::StringId, GUI::Panel*>& GetPanels()
	{
		static NxFr::Dictionary<NxFr::StringId, GUI::Panel*> Panels;
		return Panels;
	}

	const static Command CmdGuiPanel = Command::Create("GUI.Panel"_Sid, "Open gui panel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		GUISystem::GetPanel(NxFr::StringId(Id))->Show();
	}));

	const static Command CmdGuiLayoutSave = Command::Create("GUI.Layout.Save"_Sid, "Save gui layout", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		Application::GetSystem<GUISystem>()->SaveLayout(Name);
	}));

	const static Command CmdGuiLayoutLoad = Command::Create("GUI.Layout.Load"_Sid, "Load gui layout", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		Application::GetSystem<GUISystem>()->LoadLayout(Name);
	}));

	static NxFr::StringId ImGuiToNexusId(NxFr::StringView Name)
	{
		NxFr::StringView Id = Name.Split("##", 1).Split("/");
		return NxFr::StringId(Id);
	}

	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	GUI::Window* GUISystem::GetWindow()
	{
		return &GetMainWindow();
	}

	GUI::Menu* GUISystem::GetMenu()
	{
		return &GetMainMenu();
	}

	void GUISystem::RegisterMenuItem(GUI::Menu::Item* Instance)
	{
		GetMainMenu().AppendItem(*Instance);
	}

	void GUISystem::UnregisterMenuItem(GUI::Menu::Item* Instance)
	{
		GetMainMenu().RemoveItem(*Instance);
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
		NEXUS_LOG(Info, System, "Register GUI element: %s", Element->GetName().C());
	}

	void GUISystem::UnregisterElement(GUI::Element* Element)
	{
		Elements.Remove(Element);
		NEXUS_LOG(Info, System, "Unegister GUI element: %s", Element->GetName().C());
	}

	void GUISystem::LoadLayout(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedConfigName, ConfigExtension);
		if (Path.Exist())
		{
			LoadLayoutImGui(Path);
		}

		Path = GetSettingsPath(Name, SavedLayoutName, LayoutExtension);
		if (Path.Exist())
		{
			LoadLayoutNexus(Path);
		}

		NEXUS_LOG(Info, System, "GUI layout %s loaded", Name.C());
	}

	void GUISystem::SaveLayout(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedConfigName, ConfigExtension);
		SaveLayoutImGui(Path);

		Path = GetSettingsPath(Name, SavedLayoutName, LayoutExtension);
		if (!Name.IsEmpty() && !Path.Exist())
		{
			AddMenuWindowLayouts(Name.ToString());
		}
		SaveLayoutNexus(Path);

		NEXUS_LOG(Info, System, "GUI layout %s saved", Name.C());
	}

	void GUISystem::LoadTheme(NxFr::StringView Name)
	{
		NxFr::Path Path = GetSettingsPath(Name, SavedStyleName, StyleExtension);
		if (!Path.Exist())
		{
			return;
		}

		YAML::Node Data = NxFr::Yaml::DeserializeFile(Path);

		LoadThemeImGui(Data["ImGui"]);
		LoadThemeNexus(Data["Nexus"]);

		NEXUS_LOG(Info, System, "GUI style %s loaded", Name.C());
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

		NEXUS_LOG(Info, System, "GUI style %s saved", Name.C());
	}

	const GUI::Style& GUISystem::GetStyle(NxFr::StringId Id)
	{
		return Styles[Id];
	}

	void GUISystem::AppendStyle(NxFr::StringId Id, const GUI::Style& Style)
	{
		Styles.AppendOrAssign(Id, Style);
		NEXUS_LOG(Info, System, "Register GUI style: %s", Id.C());
	}

	void GUISystem::RemoveStyle(NxFr::StringId Id)
	{
		Styles.Remove(Id);
		NEXUS_LOG(Info, System, "Unregister GUI style: %s", Id.C());
	}

	GUI::Panel* GUISystem::GetActivePanel() const
	{
		NxFr::StringView Name = ImGui::GetCurrentContext()->NavWindow->RootWindow->Name;
		NxFr::StringId Id = ImGuiToNexusId(Name);
		NxFr::Dictionary<NxFr::StringId, GUI::Panel*>& Panels = GetPanels();
		GUI::Panel** Active = Panels.TryGet(Id);
		return Active ? *Active : nullptr;
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::GuiElementsId, UnsignedInteger, Set);

		Imgui::Initialize();
		LoadTheme();

		GetMainWindow().Initialize(false);

		AddMenuWindowPanels();
		AddMenuWindowLayouts();
	}

	void GUISystem::OnShutdown()
	{
		GetMainWindow().Shutdown();

		SaveTheme();
		Imgui::Shutdown();
		
		System::OnShutdown();
	}

	void GUISystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::GuiElementsId, Elements.GetCount());

		Imgui::Frame();

		GetMainWindow().Tick(TimeStep);

		for (auto& Element : Elements)
		{
			if (Element->IsManual())
			{
				continue;
			}

			Element->Tick(TimeStep);
		}

		// ImGui::ShowDemoWindow();

		Imgui::Render();
	}

	void GUISystem::AddMenuWindowPanels() const
	{
		auto& Panels = GetPanels();
		for (auto& It : Panels)
		{
			AddMenuWindowPanels(It.Value);
		}
	}

	void GUISystem::AddMenuWindowPanels(NxEn::GUI::Panel* Panel) const
	{
		auto& Menu = GetMainMenu();

		Menu.AddMenuItem("Window/Panels/" + Panel->GetTitle(), [=]()
		{
			NxFr::String Cmd = NxFr::StringView("GUI.Panel,") + Panel->GetObjectType().C();
			Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		});
	}

	void GUISystem::AddMenuWindowLayouts() const
	{
		auto& Menu = GetMainMenu();

		Menu.AddMenuItem("Window/Layouts/Save", []()
		{
			NxFr::Path Path = NxFr::Path::OpenFileDialog("Save Layout", "layout", "Layout", NxFr::Paths::Configs + Folder);
			if (!Path.IsValid()) return;
			NxFr::String Cmd = NxFr::StringView("GUI.Layout.Save,") + NxFr::Path::GetFileName(Path);
			NxEn::Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		}, "", 1);

		NxFr::Path Path = GetSettingsPath("Layout", SavedLayoutName, LayoutExtension);
		NxFr::Directory Folder(Path.GetDirectoryPath());
		NxFr::List<NxFr::String> Layouts = Folder.GetFiles();

		for (auto& Layout : Layouts)
		{
			if (!NxFr::Path::HasExtension(Layout, LayoutExtension))
			{
				continue;
			}

			NxFr::String Name = NxFr::Path::GetFileName(Layout).ToString();
			AddMenuWindowLayouts(Name);
		}
	}

	void GUISystem::AddMenuWindowLayouts(const NxFr::String& Name) const
	{
		auto& Menu = GetMainMenu();

		Menu.AddMenuItem("Window/Layouts/" + Name, [=]()
		{
			NxFr::String Cmd = NxFr::StringView("GUI.Layout.Load,") + Name;
			NxEn::Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		});
	}

	NxFr::Path GUISystem::GetSettingsPath(NxFr::StringView Name, NxFr::StringView Default, NxFr::StringView Extension) const
	{
		NxFr::Path Directory = !Name.IsEmpty() ? NxFr::Paths::Configs : NxFr::Paths::Saved;
		Directory += Folder;
		NxFr::Directory(Directory).Create();

		NxFr::String File = !Name.IsEmpty() ? Name.ToString() : (Default + Suffix);
		File += Extension;

		return Directory + File;
	}

	void GUISystem::LoadLayoutImGui(const NxFr::Path& Path) const
	{
		NEXUS_PROFILE_FUNCTION();

		ImGui::LoadIniSettingsFromDisk(Path.C());
	}

	void GUISystem::LoadLayoutNexus(const NxFr::Path& Path) const
	{
		NEXUS_PROFILE_FUNCTION();

		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Read, false);

		NxFr::Set<NxFr::StringId> Ids;
		while (!Stream.IsAtTheEnd())
		{
			Ids.Append(NxFr::StringId(Stream.Read()));
		}

		auto& Panels = GetPanels();
		for (auto& It : Panels)
		{
			It.Value->SetEnabled(Ids.Contains(It.Key));
		}

		Stream.Close();
	}

	void GUISystem::SaveLayoutImGui(const NxFr::Path& Path) const
	{
		NEXUS_PROFILE_FUNCTION();

		ImGui::SaveIniSettingsToDisk(Path.C());
	}

	void GUISystem::SaveLayoutNexus(const NxFr::Path& Path) const
	{
		NEXUS_PROFILE_FUNCTION();

		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Write, true);

		auto& Panels = GetPanels();
		for (auto& It : Panels)
		{
			if (It.Value->IsEnabled())
			{
				Stream.Write(It.Key.C());
			}
		}

		Stream.Close();
	}

	void GUISystem::LoadThemeImGui(const YAML::Node& Node) const
	{
		NEXUS_PROFILE_FUNCTION();

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
		NEXUS_PROFILE_FUNCTION();

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

			AppendStyle(Id, Style);
		}
	}

	void GUISystem::SaveThemeImGui(YAML::Emitter& Emitter) const
	{
		NEXUS_PROFILE_FUNCTION();

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

	void GUISystem::SaveThemeNexus(YAML::Emitter& Emitter) const
	{
		NEXUS_PROFILE_FUNCTION();

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
