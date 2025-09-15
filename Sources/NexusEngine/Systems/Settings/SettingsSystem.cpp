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

	const static Command CmdSettings = Command::Create("Settings"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Value);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(SettingsSystem)

	NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> SettingsSystem::GetAllSettings()
	{
		NxFr::Dictionary<NxFr::StringView, Setting*>& Settings = GetSettings();
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Result;

		for (auto& [Id, Instance] : Settings)
		{
			NxFr::Dictionary<NxFr::StringView, Setting*>* Page = Result.TryGet(Instance->GetPage());
			if (!Page)
			{
				Page = &Result.AppendConstruct(Instance->GetPage(), 11);
			}

			Page->Append(Instance->GetName(), Instance);
		}

		return Result;
	}

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

		auto Pages = GetAllSettings();

		NxFr::Directory Directory(Path);
		NxFr::List<NxFr::String> Files = Directory.GetFiles();
		for (auto& File : Files)
		{
			NxFr::StringView Name = NxFr::Path::GetFileName(File);
			NxFr::StringView Page = NxFr::StringView(Name);
			auto& Settings = Pages[Page];

			YAML::Node Root = NxFr::Yaml::DeserializeFile(File);
			for (auto It = Root.begin(); It != Root.end(); ++It)
			{
				YAML::Node& Key = It->first;
				YAML::Node& Value = It->second;

				auto* Instance = Settings[Key.as<NxFr::String>()];
				Instance->Set(Value.as<NxFr::String>());
			}
		}
	}

	void SettingsSystem::SaveSettings() const
	{
		NxFr::Path Path = Project::GetSavedConfigPath(Folder, "", "", "");
		auto Pages = GetAllSettings();

		for (auto& [Page, Settings] : Pages)
		{
			YAML::Emitter Data;
			Data << YAML::BeginMap;
			for (auto& [Id, Instance] : Settings)
			{
				Data << YAML::Key;
				Data << Instance->GetName();
				Data << YAML::Value;
				Data << Instance->Get();
			}
			Data << YAML::EndMap;

			NxFr::Yaml::SerializeFile(Data, Path + (Page.ToString().C() + Extension));
		}
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
