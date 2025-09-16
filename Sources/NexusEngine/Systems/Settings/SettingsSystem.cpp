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

	const static Command CmdSettingsVar = Command::Create("SettingsVar"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Value);
	}));
	const static Command CmdSettingsSeq = Command::Create("SettingsSeq"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Index, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(NxFr::StringUtility::ToInteger(Index), Value);
	}));
	const static Command CmdSettingsMap = Command::Create("SettingsMap"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Key, NxFr::StringView Value)
	{
		SettingsSystem::GetSetting(NxFr::StringView(Id))->Set(Key.ToString(), Value);
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
			NxFr::StringView Page = NxFr::Path::GetFileName(File);
			auto& Settings = Pages[Page];

			YAML::Node Root = NxFr::Yaml::DeserializeFile(File);
			for (auto It = Root.begin(); It != Root.end(); ++It)
			{
				YAML::Node& Key = It->first;
				YAML::Node& Value = It->second;

				auto* Instance = Settings[Key.as<NxFr::String>()];
				Instance->Deserialize(Value);
			}
		}
	}

	//TODO: Sort settings
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
				Instance->Serialize(Data);
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
