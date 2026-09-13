#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui/ImGui.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId GuiElementsId = "GUI - Elements"_Sid;
	}
}

namespace NxEn
{
	const NxFr::StringView Folder = "ImGui";
	const NxFr::StringView NameDefault = "Default";
	const NxFr::StringView NameImGui = "imgui";
	const NxFr::StringView NameStyle = "style";
	const NxFr::StringView NameLayout = "layout";
	const NxFr::StringView ExtensionImGui = "ini";
	const NxFr::StringView ExtensionStyle = "style";
	const NxFr::StringView ExtensionLayout = "layout";

	static NxFr::String GeneratePath(NxFr::StringView Name, NxFr::StringView Extension)
	{
		return NxFr::Path::Combine(Folder, Name + NxFr::Path::SeparatorExtension + Extension);
	}

	static Command* CmdGuiPanel = Command::Create("GUI.Panel"_Sid, "Open gui panel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Type)
	{
		Application::GetSystem<GUISystem>()->GetPanel(Type)->Show();
	}));

	static Command* CmdGuiElement = Command::Create("GUI.Element"_Sid, "Open gui element", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		Application::GetSystem<GUISystem>()->GetElement(NxFr::StringUtility::FromString<NxFr::GUID>(Id))->Show();
	}));

	static Command* CmdGuiLayoutSave = Command::Create("GUI.Layout.Save"_Sid, "Save gui layout", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		Application::GetSystem<GUISystem>()->SaveLayout(Name);
	}));

	static Command* CmdGuiLayoutLoad = Command::Create("GUI.Layout.Load"_Sid, "Load gui layout", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Name)
	{
		Application::GetSystem<GUISystem>()->LoadLayout(Name);
	}));

	NxFr::Registry<GUI::Panel*>& GUISystem::GetPanels()
	{
		NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::Constant);

		static NxFr::Registry<GUI::Panel*> Panels;
		return Panels;
	}

	NxFr::Registry<GUI::Menu::Item*>& GUISystem::GetMenuItems()
	{
		NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::Constant);

		static NxFr::Registry<GUI::Menu::Item*> MenuItems;
		return MenuItems;
	}

	GUISystem::GUISystem()
		: Window(), Drawing(), Destroyed(), Availables()
	{

	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::LoadLayout(NxFr::StringView Name)
	{
		NX_INSTUMENT_FUNCTION();

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameImGui), ExtensionImGui),
			GeneratePath(NameDefault, ExtensionImGui),
			Name.IsEmpty());
		if (NxFr::Path::Exist(Path))
		{
			LoadLayoutImGui(Path);
		}

		Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameLayout), ExtensionLayout),
			GeneratePath(NameDefault, ExtensionLayout),
			Name.IsEmpty());
		if (NxFr::Path::Exist(Path))
		{
			LoadLayoutNexus(Path);
		}

		NX_LOG(Info, System, "GUI layout %s loaded", Name.C());
	}

	void GUISystem::SaveLayout(NxFr::StringView Name)
	{
		NX_INSTUMENT_FUNCTION();

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameImGui), ExtensionImGui),
			"", Name.IsEmpty());
		SaveLayoutImGui(Path);

		Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameLayout), ExtensionLayout),
			"", Name.IsEmpty());
		if (!Name.IsEmpty() && !NxFr::Path::Exist(Path))
		{
			AddMenuWindowLayouts(Name);
		}
		SaveLayoutNexus(Path);

		NX_LOG(Info, System, "GUI layout %s saved", Name.C());
	}

	void GUISystem::LoadTheme(NxFr::StringView Name)
	{
		NX_INSTUMENT_FUNCTION();

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameStyle), ExtensionStyle),
			GeneratePath(NameDefault, ExtensionStyle),
			Name.IsEmpty());
		if (NxFr::Path::Exist(Path))
		{
			NxFr::Yaml::Node Data = NxFr::Yaml::LoadAndDeserialize(Path);

			LoadThemeImGui(Data["ImGui"]);
			LoadThemeNexus(Data["Nexus"]);
		}

		NX_LOG(Info, System, "GUI style %s loaded", Name.C());
	}

	void GUISystem::SaveTheme(NxFr::StringView Name)
	{
		NX_INSTUMENT_FUNCTION();

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(
			GeneratePath((!Name.IsEmpty() ? Name : NameStyle), ExtensionStyle),
			"", Name.IsEmpty());

		NxFr::Yaml::Node Data;
		Data["ImGui"] = SaveThemeImGui();
		Data["Nexus"] = SaveThemeNexus();

		NxFr::Yaml::SerializeAndSave(Data, Path);

		NX_LOG(Info, System, "GUI style %s saved", Name.C());
	}

	GUI::Window* GUISystem::GetWindow()
	{
		return &Window;
	}

	GUI::Element* GUISystem::GetElement(NxFr::GUID Id)
	{
		auto It = NxFr::ContainerUtility::Where<GUI::Element*>(Drawing, [&](GUI::Element* Element)
		{
			return Element->GetId() == Id;
		});

		return It.Get();
	}

	GUI::Panel* GUISystem::GetPanel(NxFr::StringId Type)
	{
		return GetPanels().TryGet(Type);
	}

	GUI::Panel* GUISystem::GetActivePanel()
	{
		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return nullptr;
		}

		NxFr::StringView Name = ImGui::GetCurrentContext()->NavWindow->RootWindow->Name;
		NxFr::StringId Id = GUI::Utils::ImGuiToNexusId(Name);
		return GetPanel(Id);
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Directory(NxFr::Path::Combine(NxFr::Globals::Paths::Configs, Folder)).Create();
		NxFr::Directory(NxFr::Path::Combine(NxFr::Globals::Paths::Saved, Folder)).Create();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::GuiElementsId, Integer, Set);

		if (!Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			Imgui::Initialize();
		}

		LoadTheme();

		Window.Initialize();

		AddMenuWindowItems();
		AddMenuWindowPanels();
		AddMenuWindowLayouts();
	}

	void GUISystem::OnShutdown()
	{
		Destroy(Destroyed);
		Destroy(Availables);

		Window.Shutdown();

		SaveTheme();

		if (!Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			Imgui::Shutdown();
		}
		
		System::OnShutdown();
	}

	void GUISystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Destroy(Destroyed);

		NX_STAT_INTEGER(NxFr::StatsHeader::GuiElementsId, Drawing.GetCount());

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		Imgui::Tick();

		Window.Draw();
		Draw(Drawing);

		Imgui::Render();
	}

	void GUISystem::Draw(NxFr::Set<GUI::Element*>& Elements)
	{
		for (auto& Element : Drawing)
		{
			Element->Draw();
		}
	}

	void GUISystem::Destroy(NxFr::Set<GUI::Element*>& Elements)
	{
		for (auto& Element : Elements)
		{
			Element->SetEnabled(false);
			Element->Shutdown();
			delete Element;
		}
		Elements.Clear();
	}

	void GUISystem::DrawElement(GUI::Element* Element, bool State)
	{
		if (State)
		{
			Drawing.TryAppend(Element);
		}
		else
		{
			Drawing.TryRemove(Element);
		}
	}

	void GUISystem::DestroyElement(GUI::Element* Element)
	{
		Destroyed.TryAppend(Element);
	}

	void GUISystem::RecycleElement(GUI::Element* Element)
	{
		Element->SetEnabled(false);
		Element->Shutdown();
		Availables.Append(Element);
	}

	GUI::Element* GUISystem::AcquireElement(NxFr::StringId Type)
	{
		GUI::Element* Instance = nullptr;

		for (auto& Element : Availables)
		{
			if (Element->GetObjectType() == Type)
			{
				Instance = Element;
				Availables.Remove(Instance);
				break;
			}
		}

		return Instance;
	}

	void GUISystem::AddMenuWindowItems()
	{
		auto& MenuItems = GetMenuItems();
		for (auto It = MenuItems.Begin(); It != MenuItems.End(); ++It)
		{
			AddMenuWindowItems(*It->Value);
		}
	}

	void GUISystem::AddMenuWindowItems(const GUI::Menu::Item& Item)
	{
		auto& Menu = Window.GetMenu();
		Menu.AddMenuItem(Item.Path, Item.Callback, GUI::Menu::MainMenuPriority(Item.Path, Item.Priority), Item.Validate);
	}

	void GUISystem::AddMenuWindowPanels()
	{
		auto& Panels = GetPanels();
		for (auto It = Panels.Begin(); It != Panels.End(); ++It)
		{
			AddMenuWindowPanels(It->Value);
		}
	}

	void GUISystem::AddMenuWindowPanels(NxEn::GUI::Panel* Panel)
	{
		NxFr::String Path = "Window/Panels/" + (!Panel->GetName().IsEmpty() ? Panel->GetName() : Panel->GetObjectType().GetString());
		auto& Menu = Window.GetMenu();
		Menu.AddMenuItem(Path, [=]()
		{
			NxFr::String Cmd = "GUI.Panel " + Panel->GetObjectType().GetString();
			Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		}, GUI::Menu::MainMenuPriority(Path, 0));
	}

	void GUISystem::AddMenuWindowLayouts()
	{
		NxFr::String Path = "Window/Layouts/Save";
		auto& Menu = Window.GetMenu();
		Menu.AddMenuItem(Path, []()
		{
			NxFr::String LayoutPath = NxFr::Path::OpenFileDialog("Save Layout", ExtensionLayout, "Layout", NxFr::Path::Combine(NxFr::Globals::Paths::Configs, Folder));
			if (LayoutPath.IsEmpty()) return;

			NxFr::String Cmd = "GUI.Layout.Save " + NxFr::Path::GetName(LayoutPath);
			NxEn::Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		}, GUI::Menu::MainMenuPriority(Path, 1));

		NxFr::String FolderPath = NxFr::Path::Combine(NxFr::Globals::Paths::Configs, Folder);
		NxFr::Directory Folder(FolderPath);
		NxFr::List<NxFr::String> Layouts = Folder.GetFiles();

		for (auto& Layout : Layouts)
		{
			if (NxFr::Path::GetExtension(Layout) != ExtensionLayout)
			{
				continue;
			}

			AddMenuWindowLayouts(NxFr::Path::GetName(Layout));
		}
	}

	void GUISystem::AddMenuWindowLayouts(NxFr::StringView Name)
	{
		//Force copy name to allow the lambda to capture it.
		NxFr::String Copy = Name;
		NxFr::String Path = "Window/Layouts/" + Name;

		auto& Menu = Window.GetMenu();
		Menu.AddMenuItem(Path, [=]()
		{
			NxFr::String Cmd = "GUI.Layout.Load " + Copy;
			NxEn::Application::GetSystem<CommandsSystem>()->Execute(Cmd);
		}, GUI::Menu::MainMenuPriority(Path, 0));
	}

	void GUISystem::LoadLayoutImGui(NxFr::StringView Path) const
	{
		ImGui::LoadIniSettingsFromDisk(Path.C());
	}

	void GUISystem::LoadLayoutNexus(NxFr::StringView Path) const
	{
		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Read, false);

		NxFr::Set<NxFr::StringId> Ids;
		while (!Stream.IsAtTheEnd())
		{
			Ids.Append(Stream.ReadLine());
		}

		auto& Panels = GetPanels();
		for (auto It = Panels.Begin(); It != Panels.End(); ++It)
		{
			It->Value->SetEnabled(Ids.TryGet(It->Key));
		}

		Stream.Close();
	}

	void GUISystem::SaveLayoutImGui(NxFr::StringView Path) const
	{
		ImGui::SaveIniSettingsToDisk(Path.C());
	}

	void GUISystem::SaveLayoutNexus(NxFr::StringView Path) const
	{
		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Write, true);

		auto& Panels = GetPanels();
		for (auto It = Panels.Begin(); It != Panels.End(); ++It)
		{
			if (It->Value->IsEnabled())
			{
				Stream.WriteLine(It->Key.C());
			}
		}

		Stream.Close();
	}

	void GUISystem::LoadThemeImGui(const NxFr::Yaml::Node& Node) const
	{
		ImGuiStyle& Style = ImGui::GetStyle();

		Style.FontSizeBase = Node["FontSizeBase"].As<float>();
		Style.FontScaleMain = Node["FontScaleMain"].As<float>();
		Style.FontScaleDpi = Node["FontScaleDpi"].As<float>();
		Style.Alpha = Node["Alpha"].As<float>();
		Style.DisabledAlpha = Node["DisabledAlpha"].As<float>();
		Style.WindowPadding = Node["WindowPadding"].As<NxFr::Vector2f>();
		Style.WindowRounding = Node["WindowRounding"].As<float>();
		Style.WindowBorderSize = Node["WindowBorderSize"].As<float>();
		Style.WindowBorderHoverPadding = Node["WindowBorderHoverPadding"].As<float>();
		Style.WindowMinSize = Node["WindowMinSize"].As<NxFr::Vector2f>();
		Style.WindowTitleAlign = Node["WindowTitleAlign"].As<NxFr::Vector2f>();
		Style.WindowMenuButtonPosition = (ImGuiDir)Node["WindowMenuButtonPosition"].As<int32>();
		Style.ChildRounding = Node["ChildRounding"].As<float>();
		Style.ChildBorderSize = Node["ChildBorderSize"].As<float>();
		Style.PopupRounding = Node["PopupRounding"].As<float>();
		Style.PopupBorderSize = Node["PopupBorderSize"].As<float>();
		Style.FramePadding = Node["FramePadding"].As<NxFr::Vector2f>();
		Style.FrameRounding = Node["FrameRounding"].As<float>();
		Style.FrameBorderSize = Node["FrameBorderSize"].As<float>();
		Style.ItemSpacing = Node["ItemSpacing"].As<NxFr::Vector2f>();
		Style.ItemInnerSpacing = Node["ItemInnerSpacing"].As<NxFr::Vector2f>();
		Style.CellPadding = Node["CellPadding"].As<NxFr::Vector2f>();
		Style.TouchExtraPadding = Node["TouchExtraPadding"].As<NxFr::Vector2f>();
		Style.IndentSpacing = Node["IndentSpacing"].As<float>();
		Style.ColumnsMinSpacing = Node["ColumnsMinSpacing"].As<float>();
		Style.ScrollbarSize = Node["ScrollbarSize"].As<float>();
		Style.ScrollbarRounding = Node["ScrollbarRounding"].As<float>();
		Style.GrabMinSize = Node["GrabMinSize"].As<float>();
		Style.GrabRounding = Node["GrabRounding"].As<float>();
		Style.LogSliderDeadzone = Node["LogSliderDeadzone"].As<float>();
		Style.ImageBorderSize = Node["ImageBorderSize"].As<float>();
		Style.TabRounding = Node["TabRounding"].As<float>();
		Style.TabBorderSize = Node["TabBorderSize"].As<float>();
		Style.TabCloseButtonMinWidthSelected = Node["TabCloseButtonMinWidthSelected"].As<float>();
		Style.TabCloseButtonMinWidthUnselected = Node["TabCloseButtonMinWidthUnselected"].As<float>();
		Style.TabBarBorderSize = Node["TabBarBorderSize"].As<float>();
		Style.TabBarOverlineSize = Node["TabBarOverlineSize"].As<float>();
		Style.TableAngledHeadersAngle = Node["TableAngledHeadersAngle"].As<float>();
		Style.TableAngledHeadersTextAlign = Node["TableAngledHeadersTextAlign"].As<NxFr::Vector2f>();
		Style.TreeLinesFlags = (ImGuiTreeNodeFlags)Node["TreeLinesFlags"].As<int32>();
		Style.TreeLinesSize = Node["TreeLinesSize"].As<float>();
		Style.TreeLinesRounding = Node["TreeLinesRounding"].As<float>();
		Style.ColorButtonPosition = (ImGuiDir)Node["ColorButtonPosition"].As<int32>();
		Style.ButtonTextAlign = Node["ButtonTextAlign"].As<NxFr::Vector2f>();
		Style.SelectableTextAlign = Node["SelectableTextAlign"].As<NxFr::Vector2f>();
		Style.SeparatorTextBorderSize = Node["SeparatorTextBorderSize"].As<float>();
		Style.SeparatorTextAlign = Node["SeparatorTextAlign"].As<NxFr::Vector2f>();
		Style.SeparatorTextPadding = Node["SeparatorTextPadding"].As<NxFr::Vector2f>();
		Style.DisplayWindowPadding = Node["DisplayWindowPadding"].As<NxFr::Vector2f>();
		Style.DisplaySafeAreaPadding = Node["DisplaySafeAreaPadding"].As<NxFr::Vector2f>();
		Style.DockingSeparatorSize = Node["DockingSeparatorSize"].As<float>();
		Style.AntiAliasedLines = Node["AntiAliasedLines"].As<bool>();
		Style.AntiAliasedLinesUseTex = Node["AntiAliasedLinesUseTex"].As<bool>();
		Style.AntiAliasedFill = Node["AntiAliasedFill"].As<bool>();
		Style.CurveTessellationTol = Node["CurveTessellationTol"].As<float>();
		Style.CircleTessellationMaxError = Node["CircleTessellationMaxError"].As<float>();
		Style.HoverStationaryDelay = Node["HoverStationaryDelay"].As<float>();
		Style.HoverDelayShort = Node["HoverDelayShort"].As<float>();
		Style.HoverDelayNormal = Node["HoverDelayNormal"].As<float>();
		Style.HoverFlagsForTooltipMouse = (ImGuiHoveredFlags)Node["HoverFlagsForTooltipMouse"].As<int32>();
		Style.HoverFlagsForTooltipNav = (ImGuiHoveredFlags)Node["HoverFlagsForTooltipNav"].As<int32>();

		NxFr::Yaml::Node Colors = Node["Colors"];
		Style.Colors[ImGuiCol_Text] = Colors["Text"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TextDisabled] = Colors["TextDisabled"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_WindowBg] = Colors["WindowBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ChildBg] = Colors["ChildBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_PopupBg] = Colors["PopupBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_Border] = Colors["Border"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_BorderShadow] = Colors["BorderShadow"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_FrameBg] = Colors["FrameBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_FrameBgHovered] = Colors["FrameBgHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_FrameBgActive] = Colors["FrameBgActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TitleBg] = Colors["TitleBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TitleBgActive] = Colors["TitleBgActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TitleBgCollapsed] = Colors["TitleBgCollapsed"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_MenuBarBg] = Colors["MenuBarBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ScrollbarBg] = Colors["ScrollbarBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ScrollbarGrab] = Colors["ScrollbarGrab"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ScrollbarGrabHovered] = Colors["ScrollbarGrabHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ScrollbarGrabActive] = Colors["ScrollbarGrabActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_CheckMark] = Colors["CheckMark"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_SliderGrab] = Colors["SliderGrab"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_SliderGrabActive] = Colors["SliderGrabActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_Button] = Colors["Button"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ButtonHovered] = Colors["ButtonHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ButtonActive] = Colors["ButtonActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_Header] = Colors["Header"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_HeaderHovered] = Colors["HeaderHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_HeaderActive] = Colors["HeaderActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_Separator] = Colors["Separator"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_SeparatorHovered] = Colors["SeparatorHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_SeparatorActive] = Colors["SeparatorActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ResizeGrip] = Colors["ResizeGrip"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ResizeGripHovered] = Colors["ResizeGripHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ResizeGripActive] = Colors["ResizeGripActive"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_InputTextCursor] = Colors["InputTextCursor"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabHovered] = Colors["TabHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_Tab] = Colors["Tab"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabSelected] = Colors["TabSelected"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabSelectedOverline] = Colors["TabSelectedOverline"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabDimmed] = Colors["TabDimmed"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabDimmedSelected] = Colors["TabDimmedSelected"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TabDimmedSelectedOverline] = Colors["TabDimmedSelectedOverline"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_DockingPreview] = Colors["DockingPreview"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_DockingEmptyBg] = Colors["DockingEmptyBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_PlotLines] = Colors["PlotLines"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_PlotLinesHovered] = Colors["PlotLinesHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_PlotHistogram] = Colors["PlotHistogram"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_PlotHistogramHovered] = Colors["PlotHistogramHovered"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TableHeaderBg] = Colors["TableHeaderBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TableBorderStrong] = Colors["TableBorderStrong"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TableBorderLight] = Colors["TableBorderLight"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TableRowBg] = Colors["TableRowBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TableRowBgAlt] = Colors["TableRowBgAlt"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TextLink] = Colors["TextLink"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TextSelectedBg] = Colors["TextSelectedBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_TreeLines] = Colors["TreeLines"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_DragDropTarget] = Colors["DragDropTarget"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_NavCursor] = Colors["NavCursor"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_NavWindowingHighlight] = Colors["NavWindowingHighlight"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_NavWindowingDimBg] = Colors["NavWindowingDimBg"].As<NxFr::Color>();
		Style.Colors[ImGuiCol_ModalWindowDimBg] = Colors["ModalWindowDimBg"].As<NxFr::Color>();
	}

	void GUISystem::LoadThemeNexus(const NxFr::Yaml::Node& Node)
	{
		NxFr::Yaml::Node Vars = Node["Vars"];
		for (auto It = Vars.Begin(); It != Vars.End(); ++It)
		{
			NxFr::Yaml::Node NodeId = It.Key();
			NxFr::Yaml::Node NodeProperties = It.Value();

			NxFr::StringId Id = NodeId.As<NxFr::StringId>();
			float Var = NodeProperties.As<float>();

			GUI::Style::GetVars().Register(Id, Var, true);
		}

		NxFr::Yaml::Node Colors = Node["Colors"];
		for (auto It = Colors.Begin(); It != Colors.End(); ++It)
		{
			NxFr::Yaml::Node NodeId = It.Key();
			NxFr::Yaml::Node NodeProperties = It.Value();

			NxFr::StringId Id = NodeId.As<NxFr::StringId>();
			NxFr::Color Color = NodeProperties.As<NxFr::Color>();

			GUI::Style::GetColors().Register(Id, Color, true);
		}

		NxFr::Yaml::Node Styles = Node["Styles"];
		for (auto It = Styles.Begin(); It != Styles.End(); ++It)
		{
			NxFr::Yaml::Node NodeId = It.Key();
			NxFr::Yaml::Node NodeProperties = It.Value();

			NxFr::StringId Id = NodeId.As<NxFr::StringId>();
			GUI::Style Style = NodeProperties.As<GUI::Style>();

			GUI::Style::GetStyles().Register(Id, Style, true);
		}
	}

	NxFr::Yaml::Node GUISystem::SaveThemeImGui() const
	{
		ImGuiStyle& Style = ImGui::GetStyle();

		NxFr::Yaml::Node Node;
		Node["FontSizeBase"] = Style.FontSizeBase;
		Node["FontScaleMain"] = Style.FontScaleMain;
		Node["FontScaleDpi"] = Style.FontScaleDpi;
		Node["Alpha"] = Style.Alpha;
		Node["DisabledAlpha"] = Style.DisabledAlpha;
		Node["WindowPadding"] = NxFr::Vector2f(Style.WindowPadding.x, Style.WindowPadding.y);
		Node["WindowRounding"] = Style.WindowRounding;
		Node["WindowBorderSize"] = Style.WindowBorderSize;
		Node["WindowBorderHoverPadding"] = Style.WindowBorderHoverPadding;
		Node["WindowMinSize"] = NxFr::Vector2f(Style.WindowMinSize.x, Style.WindowMinSize.y);
		Node["WindowTitleAlign"] = NxFr::Vector2f(Style.WindowTitleAlign.x, Style.WindowTitleAlign.y);
		Node["WindowMenuButtonPosition"] = (int32)Style.WindowMenuButtonPosition;
		Node["ChildRounding"] = Style.ChildRounding;
		Node["ChildBorderSize"] = Style.ChildBorderSize;
		Node["PopupRounding"] = Style.PopupRounding;
		Node["PopupBorderSize"] = Style.PopupBorderSize;
		Node["FramePadding"] = NxFr::Vector2f(Style.FramePadding.x, Style.FramePadding.y);
		Node["FrameRounding"] = Style.FrameRounding;
		Node["FrameBorderSize"] = Style.FrameBorderSize;
		Node["ItemSpacing"] = NxFr::Vector2f(Style.ItemSpacing.x, Style.ItemSpacing.y);
		Node["ItemInnerSpacing"] = NxFr::Vector2f(Style.ItemInnerSpacing.x, Style.ItemInnerSpacing.y);
		Node["CellPadding"] = NxFr::Vector2f(Style.CellPadding.x, Style.CellPadding.y);
		Node["TouchExtraPadding"] = NxFr::Vector2f(Style.TouchExtraPadding.x, Style.TouchExtraPadding.y);
		Node["IndentSpacing"] = Style.IndentSpacing;
		Node["ColumnsMinSpacing"] = Style.ColumnsMinSpacing;
		Node["ScrollbarSize"] = Style.ScrollbarSize;
		Node["ScrollbarRounding"] = Style.ScrollbarRounding;
		Node["GrabMinSize"] = Style.GrabMinSize;
		Node["GrabRounding"] = Style.GrabRounding;
		Node["LogSliderDeadzone"] = Style.LogSliderDeadzone;
		Node["ImageBorderSize"] = Style.ImageBorderSize;
		Node["TabRounding"] = Style.TabRounding;
		Node["TabBorderSize"] = Style.TabBorderSize;
		Node["TabCloseButtonMinWidthSelected"] = Style.TabCloseButtonMinWidthSelected;
		Node["TabCloseButtonMinWidthUnselected"] = Style.TabCloseButtonMinWidthUnselected;
		Node["TabBarBorderSize"] = Style.TabBarBorderSize;
		Node["TabBarOverlineSize"] = Style.TabBarOverlineSize;
		Node["TableAngledHeadersAngle"] = Style.TableAngledHeadersAngle;
		Node["TableAngledHeadersTextAlign"] = NxFr::Vector2f(Style.TableAngledHeadersTextAlign.x, Style.TableAngledHeadersTextAlign.y);
		Node["TreeLinesFlags"] = (int32)Style.TreeLinesFlags;
		Node["TreeLinesSize"] = Style.TreeLinesSize;
		Node["TreeLinesRounding"] = Style.TreeLinesRounding;
		Node["ColorButtonPosition"] = (int32)Style.ColorButtonPosition;
		Node["ButtonTextAlign"] = NxFr::Vector2f(Style.ButtonTextAlign.x, Style.ButtonTextAlign.y);
		Node["SelectableTextAlign"] = NxFr::Vector2f(Style.SelectableTextAlign.x, Style.SelectableTextAlign.y);
		Node["SeparatorTextBorderSize"] = Style.SeparatorTextBorderSize;
		Node["SeparatorTextAlign"] = NxFr::Vector2f(Style.SeparatorTextAlign.x, Style.SeparatorTextAlign.y);
		Node["SeparatorTextPadding"] = NxFr::Vector2f(Style.SeparatorTextPadding.x, Style.SeparatorTextPadding.y);
		Node["DisplayWindowPadding"] = NxFr::Vector2f(Style.DisplayWindowPadding.x, Style.DisplayWindowPadding.y);
		Node["DisplaySafeAreaPadding"] = NxFr::Vector2f(Style.DisplaySafeAreaPadding.x, Style.DisplaySafeAreaPadding.y);
		Node["DockingSeparatorSize"] = Style.DockingSeparatorSize;
		Node["MouseCursorScale"] = Style.MouseCursorScale;
		Node["AntiAliasedLines"] = Style.AntiAliasedLines;
		Node["AntiAliasedLinesUseTex"] = Style.AntiAliasedLinesUseTex;
		Node["AntiAliasedFill"] = Style.AntiAliasedFill;
		Node["CurveTessellationTol"] = Style.CurveTessellationTol;
		Node["CircleTessellationMaxError"] = Style.CircleTessellationMaxError;
		Node["HoverStationaryDelay"] = Style.HoverStationaryDelay;
		Node["HoverDelayShort"] = Style.HoverDelayShort;
		Node["HoverDelayNormal"] = Style.HoverDelayNormal;
		Node["HoverFlagsForTooltipMouse"] = (int32)Style.HoverFlagsForTooltipMouse;
		Node["HoverFlagsForTooltipNav"] = (int32)Style.HoverFlagsForTooltipNav;

		NxFr::Yaml::Node NodeColor;
		NodeColor["Text"] = (NxFr::Color)Style.Colors[0];
		NodeColor["TextDisabled"] = (NxFr::Color)Style.Colors[1];
		NodeColor["WindowBg"] = (NxFr::Color)Style.Colors[2];
		NodeColor["ChildBg"] = (NxFr::Color)Style.Colors[3];
		NodeColor["PopupBg"] = (NxFr::Color)Style.Colors[4];
		NodeColor["Border"] = (NxFr::Color)Style.Colors[5];
		NodeColor["BorderShadow"] = (NxFr::Color)Style.Colors[6];
		NodeColor["FrameBg"] = (NxFr::Color)Style.Colors[7];
		NodeColor["FrameBgHovered"] = (NxFr::Color)Style.Colors[8];
		NodeColor["FrameBgActive"] = (NxFr::Color)Style.Colors[9];
		NodeColor["TitleBg"] = (NxFr::Color)Style.Colors[10];
		NodeColor["TitleBgActive"] = (NxFr::Color)Style.Colors[11];
		NodeColor["TitleBgCollapsed"] = (NxFr::Color)Style.Colors[12];
		NodeColor["MenuBarBg"] = (NxFr::Color)Style.Colors[13];
		NodeColor["ScrollbarBg"] = (NxFr::Color)Style.Colors[14];
		NodeColor["ScrollbarGrab"] = (NxFr::Color)Style.Colors[15];
		NodeColor["ScrollbarGrabHovered"] = (NxFr::Color)Style.Colors[16];
		NodeColor["ScrollbarGrabActive"] = (NxFr::Color)Style.Colors[17];
		NodeColor["CheckMark"] = (NxFr::Color)Style.Colors[18];
		NodeColor["SliderGrab"] = (NxFr::Color)Style.Colors[19];
		NodeColor["SliderGrabActive"] = (NxFr::Color)Style.Colors[20];
		NodeColor["Button"] = (NxFr::Color)Style.Colors[21];
		NodeColor["ButtonHovered"] = (NxFr::Color)Style.Colors[22];
		NodeColor["ButtonActive"] = (NxFr::Color)Style.Colors[23];
		NodeColor["Header"] = (NxFr::Color)Style.Colors[24];
		NodeColor["HeaderHovered"] = (NxFr::Color)Style.Colors[25];
		NodeColor["HeaderActive"] = (NxFr::Color)Style.Colors[26];
		NodeColor["Separator"] = (NxFr::Color)Style.Colors[27];
		NodeColor["SeparatorHovered"] = (NxFr::Color)Style.Colors[28];
		NodeColor["SeparatorActive"] = (NxFr::Color)Style.Colors[29];
		NodeColor["ResizeGrip"] = (NxFr::Color)Style.Colors[30];
		NodeColor["ResizeGripHovered"] = (NxFr::Color)Style.Colors[31];
		NodeColor["ResizeGripActive"] = (NxFr::Color)Style.Colors[32];
		NodeColor["InputTextCursor"] = (NxFr::Color)Style.Colors[33];
		NodeColor["TabHovered"] = (NxFr::Color)Style.Colors[34];
		NodeColor["Tab"] = (NxFr::Color)Style.Colors[35];
		NodeColor["TabSelected"] = (NxFr::Color)Style.Colors[36];
		NodeColor["TabSelectedOverline"] = (NxFr::Color)Style.Colors[37];
		NodeColor["TabDimmed"] = (NxFr::Color)Style.Colors[38];
		NodeColor["TabDimmedSelected"] = (NxFr::Color)Style.Colors[39];
		NodeColor["TabDimmedSelectedOverline"] = (NxFr::Color)Style.Colors[40];
		NodeColor["DockingPreview"] = (NxFr::Color)Style.Colors[41];
		NodeColor["DockingEmptyBg"] = (NxFr::Color)Style.Colors[42];
		NodeColor["PlotLines"] = (NxFr::Color)Style.Colors[43];
		NodeColor["PlotLinesHovered"] = (NxFr::Color)Style.Colors[44];
		NodeColor["PlotHistogram"] = (NxFr::Color)Style.Colors[45];
		NodeColor["PlotHistogramHovered"] = (NxFr::Color)Style.Colors[46];
		NodeColor["TableHeaderBg"] = (NxFr::Color)Style.Colors[47];
		NodeColor["TableBorderStrong"] = (NxFr::Color)Style.Colors[48];
		NodeColor["TableBorderLight"] = (NxFr::Color)Style.Colors[49];
		NodeColor["TableRowBg"] = (NxFr::Color)Style.Colors[50];
		NodeColor["TableRowBgAlt"] = (NxFr::Color)Style.Colors[51];
		NodeColor["TextLink"] = (NxFr::Color)Style.Colors[52];
		NodeColor["TextSelectedBg"] = (NxFr::Color)Style.Colors[53];
		NodeColor["TreeLines"] = (NxFr::Color)Style.Colors[54];
		NodeColor["DragDropTarget"] = (NxFr::Color)Style.Colors[55];
		NodeColor["NavCursor"] = (NxFr::Color)Style.Colors[56];
		NodeColor["NavWindowingHighlight"] = (NxFr::Color)Style.Colors[57];
		NodeColor["NavWindowingDimBg"] = (NxFr::Color)Style.Colors[58];
		NodeColor["ModalWindowDimBg"] = (NxFr::Color)Style.Colors[59];
		Node["Colors"] = NodeColor;

		return Node;
	}

	NxFr::Yaml::Node GUISystem::SaveThemeNexus() const
	{
		NxFr::Yaml::Node Node;

		NxFr::Yaml::Node NodeVars;
		for (auto It = GUI::Style::GetVars().Begin(); It != GUI::Style::GetVars().End(); ++It)
		{
			NodeVars[It->Key] = It->Value;
		}
		Node["Vars"] = NodeVars;

		NxFr::Yaml::Node NodeColors;
		for (auto It = GUI::Style::GetColors().Begin(); It != GUI::Style::GetColors().End(); ++It)
		{
			NodeColors[It->Key] = It->Value;
		}
		Node["Colors"] = NodeColors;

		NxFr::Yaml::Node NodeStyles;
		for (auto It = GUI::Style::GetStyles().Begin(); It != GUI::Style::GetStyles().End(); ++It)
		{
			NodeStyles[It->Key] = It->Value;
		}
		Node["Styles"] = NodeStyles;

		return Node;
	}
}
