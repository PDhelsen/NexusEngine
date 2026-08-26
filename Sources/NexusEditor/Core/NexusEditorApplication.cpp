#include "NexusEditor/Core/NexusEditorApplication.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"
#include "NexusEditor/Misc/Object/Inspector/InspectorPanel.h"
#include "NexusEngine/Systems/Resources/Resources/Image.h"

namespace NxEd
{
	static NxEn::SettingMap<NxFr::String>* SettingShortcuts = NxEn::SettingMap<NxFr::String>::Create("Editor", "Shortcuts", {});

	static NxEn::Command* CmdSave = NxEn::Command::Create("Editor.Save"_Sid, "Save project", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->SaveAll();
	}));

	static const NxEn::GUI::Menu::Item* MenuItemSave = NxEn::GUI::Menu::Create("File/Save", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Editor.Save");
	}), 1);

	NX_APPLICATION_IMPLEMENTATION(::NxEd::NexusEditorApplication)

	NexusEditorApplication::NexusEditorApplication(const NxEn::Project& ProjectInfo)
		: NexusEngineApplication(ProjectInfo), OnSave(), Inputs(nullptr), Browser(nullptr), Hierarchy(nullptr)
	{
		NxEn::SystemManager& Systems = GetSystems();
		Systems.CreateSystem<EditSystem>();
	}

	NexusEditorApplication::~NexusEditorApplication()
	{
		NxEn::SystemManager& Systems = GetSystems();
		Systems.DestroySystem<EditSystem>();
	}

	void NexusEditorApplication::SaveAll()
	{
		NX_INSTUMENT_FUNCTION();

		OnSave.Invoke();
		NX_LOG(Info, Application, "Saved");
	}

	void NexusEditorApplication::OnInitialize()
	{
		NexusEngineApplication::OnInitialize();
		NxEn::Bootstrapper& Bootstrap = GetBootstrapper();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event HID - Editor", [&]()
		{
			Inputs = new NxEn::Input::Schema();
			GetSystem<NxEn::InputSystem>()->AddSchema("Editor"_Sid, Inputs);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event Shortcuts", [&]()
		{
			GetSystem<NxEn::SettingsSystem>()->GetOnChange() += { this, & NexusEditorApplication::ApplySettings };
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Connect event Save", [&]()
		{
			OnSave += []() { GetSystem<NxEn::WorldSystem>()->PackScenes(); };
			OnSave += []() { GetSystem<NxEn::AssetsSystem>()->SaveDirty(); };
			OnSave += []() { GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
			OnSave += []() { GetSystem<NxEn::GUISystem>()->SaveLayout(); };
		});

		Bootstrap.AppendSystem<EditSystem>();

		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Create Assets & Worlds Managers", [&]()
		{
			Browser = new AssetsBrowser();
			Hierarchy = new HierarchyManager();

			AssetsBrowserPanel* PanelBrowser = GetSystem<NxEn::GUISystem>()->GetPanel<AssetsBrowserPanel>();
			PanelBrowser->AppendAction(NxEn::TreeAction{ .Name = "Import / Reimport", .Action = [&]()
			{
				AssetsBrowserEditContext* Context = GetSystem<EditSystem>()->GetContext<AssetsBrowserEditContext>(AssetsBrowserEditContext::ContextId);

				NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection(AssetsBrowserFilter::NoDirectory | AssetsBrowserFilter::MultiSelection | AssetsBrowserFilter::Recursive);
				for (auto& Id : Ids)
				{
					AssetsBrowserItem* Instance = Browser->GetItem(Id);
					if (Instance->GetObjectType() == AssetsBrowserItemContent::GetClassType())
					{
						AssetImporter::Run(Instance->GetTargetPath());
					}
					else if (Instance->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
					{
						AssetImporter::Run(Instance->GetId());
					}
				}

				Browser->Refresh();
			}, .Priority = 1 });
			PanelBrowser->AppendAction(NxEn::TreeAction{ .Name = "Load / Reload", .Action = [&]()
			{
				NxEn::AssetsSystem* Assets = GetSystem<NxEn::AssetsSystem>();
				AssetsBrowserEditContext* Context = GetSystem<EditSystem>()->GetContext<AssetsBrowserEditContext>(AssetsBrowserEditContext::ContextId);

				NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection(AssetsBrowserFilter::AssetsOnly | AssetsBrowserFilter::MultiSelection | AssetsBrowserFilter::Recursive);
				for (auto& Id : Ids)
				{
					Assets->Reload(Id);
				}
			}, .Priority = 1 });
			PanelBrowser->AppendAction(NxEn::TreeAction{ .Name = "Instantiate", .Action = [&]()
			{
				NxEn::WorldSystem* Worlds = GetSystem<NxEn::WorldSystem>();
				NxEn::AssetsSystem* Assets = GetSystem<NxEn::AssetsSystem>();
				AssetsBrowserEditContext* Context = GetSystem<EditSystem>()->GetContext<AssetsBrowserEditContext>(AssetsBrowserEditContext::ContextId);

				NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection(AssetsBrowserFilter::AssetsOnly | AssetsBrowserFilter::MultiSelection | AssetsBrowserFilter::Recursive);
				for (auto& Id : Ids)
				{
					NxFr::StringId Type = Assets->GetMetadata(Id)->GetType();

					if (Type == NxEn::Prefab::GetClassType())
					{
						NxEn::Prefab* Instance = Assets->Load<NxEn::Prefab>(Id);
						Worlds->InstantiateGameObject(Instance->GetRoot());
					}
					else if (Type == NxEn::Scene::GetClassType())
					{
						NxEn::Scene* Instance = Assets->Load<NxEn::Scene>(Id);
						Worlds->InstantiateScene(Instance);
					}
					else
					{
						NX_LOG(Warning, System, "Instantiate is not supported for this asset type. Use Load instead");
					}
				}
			}, .Priority = 1 });
			PanelBrowser->AppendAction(NxEn::TreeAction{ .Name = "Inspect", .Action = [&]()
			{
				NxFr::GUID Id = GetSystem<EditSystem>()->GetSelected(AssetsBrowserEditContext::ContextId);
				NxEn::Asset* Instance = GetSystem<NxEn::AssetsSystem>()->Load(Id);

				InspectorPanel* Inspector = GetSystem<NxEn::GUISystem>()->GetPanel<InspectorPanel>();
				Inspector->Show(Instance);
			}, .Priority = 1 });

			HierarchyPanel* PanelHierarchy = GetSystem<NxEn::GUISystem>()->GetPanel<HierarchyPanel>();
			PanelHierarchy->AppendAction(NxEn::TreeAction{ .Name = "Prefab - Create", .Action = [&]()
			{
				NxEn::InputTextPopup* Popup = NxEn::GUI::Element::Acquire<NxEn::InputTextPopup>();
				Popup->RegisterCallback([=](NxFr::StringView Input)
				{
					HierarchyEditContext* Context = GetSystem<EditSystem>()->GetContext<HierarchyEditContext>(HierarchyEditContext::ContextId);
					NxEn::AssetsSystem * Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
					NxEn::WorldSystem * Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

					NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection();
					for (auto& Id : Ids)
					{
						NxFr::Handle<NxEn::GameObject> Instance = Hierarchy->GetItem(Id)->GetTarget();
						if (Assets->IsTracked(Instance->GetTemplateId()))
						{
							NX_LOG(Error, System, "%s is already a prefab", Instance->GetName().C());
							continue;
						}

						NxFr::String Path = NxFr::Path::IsDirectory(Input) ? Input + Instance->GetName() : NxFr::String(Input);
						NxEn::Prefab * Prefab = Assets->Create<NxEn::Prefab>(Path, NxEn::Prefab::Extension);
						Worlds->PackPrefab(Prefab, Instance);
					}
				});
			}, .Priority = 1 });
			PanelHierarchy->AppendAction(NxEn::TreeAction{ .Name = "Prefab - Pack", .Action = [&]()
			{
				HierarchyEditContext* Context = GetSystem<EditSystem>()->GetContext<HierarchyEditContext>(HierarchyEditContext::ContextId);
				NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
				NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

				NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection();
				for (auto& Id : Ids)
				{
					NxFr::Handle<NxEn::GameObject> Instance = Hierarchy->GetItem(Id)->GetTarget();
					if (!Assets->IsTracked(Instance->GetTemplateId()))
					{
						NX_LOG(Error, System, "%s is not a prefab", Instance->GetName().C());
						continue;
					}

					NxEn::Prefab * Prefab = Assets->Load<NxEn::Prefab>(Instance->GetTemplateId());
					Worlds->PackPrefab(Prefab, Instance);
				}
			}, .Priority = 1 });
			PanelHierarchy->AppendAction(NxEn::TreeAction{ .Name = "Prefab - Unpack", .Action = [&]()
			{
				HierarchyEditContext* Context = GetSystem<EditSystem>()->GetContext<HierarchyEditContext>(HierarchyEditContext::ContextId);
				NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
				NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

				NxFr::Array<NxFr::GUID> Ids = Context->FilterSelection();
				for (auto& Id : Ids)
				{
					NxFr::Handle<NxEn::GameObject> Instance = Hierarchy->GetItem(Id)->GetTarget();
					if (!Assets->IsTracked(Instance->GetTemplateId()))
					{
						NX_LOG(Error, System, "%s is not a prefab", Instance->GetName().C());
						continue;
					}

					Worlds->UnpackPrefab(Instance);
				}
			}, .Priority = 1 });
			PanelHierarchy->AppendAction(NxEn::TreeAction{ .Name = "Inspect", .Action = [&]()
			{
				NxFr::GUID Id = GetSystem<EditSystem>()->GetSelected(HierarchyEditContext::ContextId);
				NxFr::Handle<NxEn::Object> Instance = GetSystem<NxEn::WorldSystem>()->GetObject(Id);

				InspectorPanel* Inspector = GetSystem<NxEn::GUISystem>()->GetPanel<InspectorPanel>();
				Inspector->Show(Instance);
			}, .Priority = 1 });
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Set Icon", []()
		{
			NxEn::Image* Icon = GetSystem<NxEn::ResourcesSystem>()->Load<NxEn::Image>("Logo_Small.png");
			GetSystem<NxEn::WindowSystem>()->SetWindowIcon(Icon);
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Show Window", [&]()
		{
			GetSystem<NxEn::GUISystem>()->GetWindow()->Show();
		});
		Bootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::AfterSystem, "Load Layout", []()
		{
			GetSystem<NxEn::GUISystem>()->LoadLayout();
		});
	}

	void NexusEditorApplication::OnShutdown()
	{
		NxEn::Bootstrapper& Unbootstrap = GetBootstrapper();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Save Layout", []()
		{
			GetSystem<NxEn::GUISystem>()->SaveLayout();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Hide Window", []()
		{
			GetSystem<NxEn::GUISystem>()->GetWindow()->Hide();
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Set Icon", []()
		{
			GetSystem<NxEn::WindowSystem>()->SetWindowIcon(nullptr);
			GetSystem<NxEn::ResourcesSystem>()->Unload("Logo_Small.png");
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Destroy Assets & Worlds Managers", [&]()
		{
			delete Hierarchy;
			delete Browser;
		});

		Unbootstrap.AppendSystem<EditSystem>();

		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event HID - Editor", [&]()
		{
			GetSystem<NxEn::InputSystem>()->RemoveSchema("Editor"_Sid);
			delete Inputs;
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event Shortcuts", [&]()
		{
			GetSystem<NxEn::SettingsSystem>()->GetOnChange() -= { this, & NexusEditorApplication::ApplySettings };
		});
		Unbootstrap.AppendStep(NxEn::Bootstrapper::BootBucket::BeforeSystem, "Disconnect event Save", [&]()
		{
			OnSave -= []() { GetSystem<NxEn::GUISystem>()->SaveLayout(); };
			OnSave -= []() { GetSystem<NxEn::SettingsSystem>()->SaveSettings(); };
			OnSave -= []() { GetSystem<NxEn::WorldSystem>()->PackScenes(); };
			OnSave -= []() { GetSystem<NxEn::AssetsSystem>()->SaveDirty(); };
		});

		NexusEngineApplication::OnShutdown();
	}

	void NexusEditorApplication::OnRun()
	{
		NexusEngineApplication::OnRun();
		NxEn::Ticker& Ticks = GetTicker();
	}

	void NexusEditorApplication::ApplySettings()
	{
		auto& Shortcuts = SettingShortcuts->GetValue();

		for (auto& [Command, Shortcut] : Shortcuts)
		{
			if (Shortcut.IsEmpty())
			{
				continue;
			}

			NxFr::List<NxFr::StringView> Keys = NxFr::StringUtility::SplitAll(Shortcut, "+");

			NxEn::Input::State State = NxEn::Input::State::Released;
			NxEn::Input::Button Button = NxFr::StringUtility::FromString<NxEn::Input::Button>(Keys.Last());
			NxEn::Input::Modifier Modifiers = NxEn::Input::Modifier::None;
			if (Keys.GetCount() > 1)
			{
				for (uint64 Index = 0; Index < Keys.GetCount() - 1; ++Index)
				{
					Modifiers |= NxFr::StringUtility::FromString<NxEn::Input::Modifier>(Keys[Index]);
				}
			}

			NxEn::Input::Action Action(Button, State, Modifiers, [=]() { NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute(Command); });
			Inputs->Mapping.AppendOrAssign(NxFr::StringId(Command), Action);
		}
	}
}
