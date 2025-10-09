#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	const NxFr::StringView Folder = "Settings";
	const NxFr::StringView Extension = ".settings";

	static NxFr::Dictionary<NxFr::StringView, Setting*>& GetSettings()
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		static NxFr::Dictionary<NxFr::StringView, Setting*> Settings;
		return Settings;
	}

	const static Command CmdSettingsVar = Command::Create("Settings.Var"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Value);
	}));
	const static Command CmdSettingsSeq = Command::Create("Settings.Seq"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Index, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(NxFr::StringUtility::FromString<uint64>(Index), Value);
	}));
	const static Command CmdSettingsMap = Command::Create("Settings.Map"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Key, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Key, Value);
	}));
	const static Command CmdSettingsApply = Command::Create("Settings.Apply"_Sid, "Apply settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<SettingsSystem>()->ApplySettings();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(SettingsSystem)

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
		NxFr::Path Path = Project::GetSavedConfigPath(Folder, "", "", "");
		if (!Path.Exist())
		{
			return;
		}

		NxFr::Directory Directory(Path);
		NxFr::List<NxFr::String> Files = Directory.GetFiles();

		auto Pages = GetAllSettings();
		for (auto& File : Files)
		{
			NxFr::StringView Page = NxFr::Path::GetFileName(File);
			if (!Pages.ContainsKey(Page))
			{
				continue;
			}

			auto& Settings = Pages[Page];

			YAML::Node Root = NxFr::Yaml::DeserializeFile(File);
			for (auto It = Root.begin(); It != Root.end(); ++It)
			{
				YAML::Node& Key = It->first;
				YAML::Node& Value = It->second;

				auto Setting = Key.as<NxFr::String>();
				if (Settings.ContainsKey(Setting))
				{
					auto* Instance = Settings[Setting];
					Instance->Deserialize(Value);
				}
			}
		}

		NEXUS_LOG(Info, System, "Settings loaded from: %s", Path.C());
	}

	void SettingsSystem::SaveSettings() const
	{
		NxFr::Path Path = Project::GetSavedConfigPath(Folder, "", "", "");
		auto Settings = GetAllSettingsSorted();

		for (auto& Page : Settings)
		{
			YAML::Emitter Data;

			Data << YAML::BeginMap;
			for (auto& Setting : Page)
			{
				Data << YAML::Key;
				Data << Setting->GetName();
				Data << YAML::Value;
				Setting->Serialize(Data);
			}
			Data << YAML::EndMap;

			NxFr::Yaml::SerializeFile(Data, Path + (Page[0]->GetPage().C(true) + Extension));
		}

		NEXUS_LOG(Info, System, "Settings saved to : %s", Path.C());
	}

	void SettingsSystem::ApplySettings()
	{
		OnChange.Invoke();
		NEXUS_LOG(Info, System, "Settings applied");
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
		auto Settings = GetAllSettings();

		uint64 IndexPage = 0;
		NxFr::Array<NxFr::Array<Setting*>> Sorted = NxFr::Array<NxFr::Array<Setting*>>(Settings.GetCount());
		for (auto [Page, Instances] : Settings)
		{
			uint64 IndexName = 0;
			auto& Array = Sorted[IndexPage++] = NxFr::Array<Setting*>(Instances.GetCount());
			for (auto [Name, Instance] : Instances)
			{
				Array[IndexName++] = Instance;
			}

			Array.Sort([](Setting* A, Setting* B) { return A->GetName() <= B->GetName(); });
		}

		Sorted.Sort([](const NxFr::Array<Setting*>& A, const NxFr::Array<Setting*>& B) { return A[0]->GetPage() <= B[0]->GetPage(); });

		return Sorted;
	}

	void SettingsSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Directory(NxFr::Paths::Configs + Folder).Create();
		LoadSettings();
	}

	void SettingsSystem::OnShutdown()
	{
		SaveSettings();

		System::OnShutdown();
	}
}
