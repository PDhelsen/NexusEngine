#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsSystem.h"

namespace NxEn
{
	const NxFr::StringView Folder = "Settings";
	const NxFr::StringView Extension = "settings";

	static NxFr::Dictionary<NxFr::StringView, Setting*>& GetSettings()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		static NxFr::Dictionary<NxFr::StringView, Setting*> Settings;
		return Settings;
	}

	static Command* CmdSettingsVar = Command::Create("Settings.Var"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Value);
	}));
	static Command* CmdSettingsSeq = Command::Create("Settings.Seq"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Index, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(NxFr::StringUtility::FromString<uint64>(Index), Value);
	}));
	static Command* CmdSettingsMap = Command::Create("Settings.Map"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Key, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Key, Value);
	}));
	static Command* CmdSettingsApply = Command::Create("Settings.Apply"_Sid, "Apply settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<SettingsSystem>()->ApplySettings();
	}));

	Setting* SettingsSystem::GetSetting(NxFr::StringView Id)
	{
		return GetSettings()[Id];
	}

	void SettingsSystem::RegisterSetting(Setting* Instance)
	{
		GetSettings().Append(Instance->GetId(), Instance);
	}

	void SettingsSystem::UnregisterSetting(Setting* Instance)
	{
		GetSettings().Remove(Instance->GetId());
	}

	void SettingsSystem::LoadSettings() const
	{
		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(Folder);
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Settings = GetAllSettings();

		for (auto& [Page, Instances] : Settings)
		{
			NxFr::String PagePath = NxFr::Path::Combine(Path, (Page + NxFr::Path::SeparatorExtension + Extension));
			if (!NxFr::Path::Exist(PagePath))
			{
				continue;
			}

			YAML::Node Root = NxFr::Yaml::DeserializeFile(PagePath);
			for (auto It = Root.begin(); It != Root.end(); ++It)
			{
				YAML::Node& Key = It->first;
				YAML::Node& Value = It->second;

				NxFr::String SettingName = Key.as<NxFr::String>();
				if (Instances.TryGet((NxFr::StringView)SettingName))
				{
					Setting* Instance = Instances[SettingName];
					Instance->OnDeserialize(Value);
				}
			}
		}

		NX_LOG(Info, System, "Settings loaded from: %s", Path.C());
	}

	void SettingsSystem::SaveSettings() const
	{
		NxFr::String Path = Application::GetInstance()->GetProject().GetSavedConfigPath(Folder);
		NxFr::Array<NxFr::Array<Setting*>> Settings = GetAllSettingsSorted();

		for (NxFr::Array<Setting*>& Page : Settings)
		{
			NxFr::String PagePath = NxFr::Path::Combine(Path, Page[0]->GetPage() + NxFr::Path::SeparatorExtension + Extension);
			YAML::Node Root = NxFr::Path::Exist(PagePath) ? NxFr::Yaml::DeserializeFile(PagePath) : YAML::Node();

			for (Setting* Instance: Page)
			{
				Instance->OnSerialize(Root);
			}

			NxFr::Yaml::SerializeFile(Root, PagePath);
		}

		NX_LOG(Info, System, "Settings saved to : %s", Path.C());
	}

	void SettingsSystem::ApplySettings()
	{
		OnChange.Invoke();
		NX_LOG(Info, System, "Settings applied");
	}

	NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> SettingsSystem::GetAllSettings() const
	{
		NxFr::Dictionary<NxFr::StringView, Setting*>& Settings = GetSettings();
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Result;

		for (auto& [Id, Instance] : Settings)
		{
			NxFr::Dictionary<NxFr::StringView, Setting*>* Page = Result.TryGet(Instance->GetPage());
			if (!Page)
			{
				Page = &Result.AppendConstruct(Instance->GetPage(), 17);
			}

			Page->Append(Instance->GetName(), Instance);
		}

		return Result;
	}

	NxFr::Array<NxFr::Array<Setting*>> SettingsSystem::GetAllSettingsSorted() const
	{
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Settings = GetAllSettings();
		NxFr::Array<NxFr::Array<Setting*>> Result = NxFr::Array<NxFr::Array<Setting*>>(Settings.GetCount());

		uint64 PageIndex = 0;
		for (auto [PageName, Page] : Settings)
		{
			Result[PageIndex] = NxFr::Array<Setting*>(Page.GetCount());

			uint64 InstanceIndex = 0;
			for (auto [InstanceName, Instance] : Page)
			{
				Result[PageIndex][InstanceIndex] = Instance;

				InstanceIndex++;
			}

			PageIndex++;
		}

		NxFr::ContainerUtility::Sort<NxFr::Array<Setting*>>(Result, [](const NxFr::Array<Setting*>& A, const NxFr::Array<Setting*>& B) { return A[0]->GetPage() <= B[0]->GetPage(); });
		for (auto& Page : Result)
		{
			NxFr::ContainerUtility::Sort<Setting*>(Page, [](Setting* A, Setting* B) { return A->GetName() <= B->GetName(); });
		}

		return Result;
	}

	void SettingsSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Directory(NxFr::Path::Combine(NxFr::Globals::Paths::Configs, Folder)).Create();
		LoadSettings();
	}

	void SettingsSystem::OnShutdown()
	{
		SaveSettings();

		System::OnShutdown();
	}
}
