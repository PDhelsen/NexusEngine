#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/SettingsSystem.h"

namespace NxEn
{
	static Command* CmdSettingsVar = Command::Create("Settings.Var"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Value)
	{
		SettingsSystem::Settings.TryGet(NxFr::StringView(Id))->Set(Value);
	}));
	static Command* CmdSettingsSeq = Command::Create("Settings.Seq"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Index, NxFr::StringView Value)
	{
		SettingsSystem::Settings.TryGet(NxFr::StringView(Id))->Set(NxFr::StringUtility::FromString<uint64>(Index), Value);
	}));
	static Command* CmdSettingsMap = Command::Create("Settings.Map"_Sid, "Set setting value", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Key, NxFr::StringView Value)
	{
		SettingsSystem::Settings.TryGet(NxFr::StringView(Id))->Set(Key, Value);
	}));
	static Command* CmdSettingsApply = Command::Create("Settings.Apply"_Sid, "Apply settings", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<SettingsSystem>()->ApplySettings();
	}));

	NxFr::Registry<Setting*> SettingsSystem::Settings;

	NxFr::String SettingsSystem::GetKey(NxFr::StringView Page, NxFr::StringView Name)
	{
		return Page + "." + Name;
	}

	void SettingsSystem::GetPageAndName(NxFr::StringView Key, NxFr::StringView& Page, NxFr::StringView& Name)
	{
		Page = NxFr::StringUtility::Split(Key, ".", 0);
		Name = NxFr::StringUtility::Split(Key, ".", 1);
	}

	SettingsSystem::SettingsSystem()
		: OnChange()
	{
	}

	SettingsSystem::~SettingsSystem()
	{
	}

	void SettingsSystem::LoadSettings() const
	{
		NxFr::String FolderPath = Application::GetInstance()->GetProject().GetSavedConfigPath(Folder);
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Settings = GetAllSettings();

		for (auto& [Page, Instances] : Settings)
		{
			NxFr::String PagePath = NxFr::Path::Combine(FolderPath, Page + NxFr::Path::SeparatorExtension + Extension);
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
				if (Instances.TryGet(SettingName))
				{
					Setting* Instance = Instances[SettingName];
					Instance->OnDeserialize(Value);
				}
			}
		}

		NX_LOG(Info, System, "Settings loaded from: %s", FolderPath.C());
	}

	void SettingsSystem::SaveSettings() const
	{
		NxFr::String FolderPath = Application::GetInstance()->GetProject().GetSavedConfigPath(Folder);
		NxFr::Array<NxFr::Array<Setting*>> Settings = GetAllSettingsSorted();

		for (NxFr::Array<Setting*>& Page : Settings)
		{
			NxFr::String PagePath = NxFr::Path::Combine(FolderPath, Page[0]->GetPage() + NxFr::Path::SeparatorExtension + Extension);

			YAML::Node Root;
			for (Setting* Instance : Page)
			{
				Instance->OnSerialize(Root);
			}
			NxFr::Yaml::SerializeFile(Root, PagePath);
		}

		NX_LOG(Info, System, "Settings saved to : %s", FolderPath.C());
	}

	void SettingsSystem::ApplySettings() const
	{
		OnChange.Invoke();
		NX_LOG(Info, System, "Settings applied");
	}

	NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> SettingsSystem::GetAllSettings() const
	{
		NxFr::Dictionary<NxFr::StringView, NxFr::Dictionary<NxFr::StringView, Setting*>> Result;

		for (auto It = Settings.Begin(); It != Settings.End(); ++It)
		{
			Setting* Instance = It->Value;

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

		uint64 Index = 0;
		for (auto [Page, Instances] : Settings)
		{
			Result[Index++] = NxFr::ContainerUtility::ToArrayValues(Instances);
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
		LoadSettings();
	}

	void SettingsSystem::OnShutdown()
	{
		SaveSettings();
	}
}
