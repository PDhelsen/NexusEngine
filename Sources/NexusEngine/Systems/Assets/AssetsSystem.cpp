#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId AssetsTrackedId = "Assets - Tracked"_Sid;
		const NxFr::StringId AssetsLoadedId = "Assets - Loaded"_Sid;
	}
}

namespace NxEn
{
	static NxFr::StringView AssetExtension = ".asset";
	static NxFr::StringView DatabaseExtension = ".csv";
	static NxFr::StringView DatabaseName = "Assets_Database";

	NEXUS_OBJECT_IMPLEMENTATION(AssetsSystem)

	AssetsSystem::Info::Info()
		: Instance(nullptr), Path(""), Count(0)
	{
	}

	Asset* AssetsSystem::Get(NxFr::StringView Path, bool CreateIfDontExist)
	{
		Info* Instance = GetAsset(Path);

		if (Instance == nullptr)
		{
			if (CreateIfDontExist)
			{
				return Create(Path);
			}
			else
			{
				return nullptr;
			}
		}

		if (Instance->Instance == nullptr)
		{
			return Load(Path);
		}

		Instance->Count++;
		return Instance->Instance;
	}

	void AssetsSystem::Release(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);

		if (Instance->Instance == nullptr)
		{
			return;
		}

		Instance->Count--;
		if (Instance->Count == 0)
		{
			Unload(Path);
		}
	}

	Asset* AssetsSystem::Create(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);
		if (Instance != nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s already exist", Path.C());
			return nullptr;
		}

		Instance = &Assets.AppendConstruct(Path);
		Instance->Instance = new Asset();
		Instance->Path = Path;
		Instance->Count = 1;

		if (Instance->Path.IsValid())
		{
			NxFr::File(GetFilePath(Instance->Path)).Create();
		}

		return Instance->Instance;
	}

	void AssetsSystem::Delete(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s doesn't exist", Path.C());
			return;
		}

		if (Instance->Instance)
		{
			Instance->Instance->Shutdown();
			delete Instance->Instance;
			Instance->Instance = nullptr;
		}

		if (Instance->Path.IsValid())
		{
			NxFr::File(GetFilePath(Instance->Path)).Delete();
		}

		Assets.Remove(Path);
	}

	void AssetsSystem::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		Info* Instance = GetAsset(Path);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s doesn't exist", Path.C());
			return;
		}

		if (Instance->Path.IsValid())
		{
			NxFr::File(GetFilePath(Instance->Path)).Move(GetFilePath(Target));
			Instance->Path = Target;
		}
	}

	void AssetsSystem::Save(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s doesn't exist", Path.C());
			return;
		}

		if (!Instance->Instance)
		{
			NEXUS_LOG(Warning, Default, "Asset %s not loaded.", Path.C());
			return;
		}

		YAML::Node Node;
		Node["Id"] = Path;

		Instance->Instance->OnSerialize(Node);

		NxFr::Yaml::SerializeFile(Node, GetFilePath(Instance->Path));
	}

	void AssetsSystem::SaveAll()
	{
		for (auto& [Path, Instance] : Assets)
		{
			if (Instance.Instance)
			{
				Save(Path);
			}
		}
	}

	Asset* AssetsSystem::Load(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s doesn't exist", Path.C());
			return nullptr;
		}

		if (Instance->Instance)
		{
			NEXUS_LOG(Warning, Default, "Asset %s already loaded. Use Get instead to properly track the instance", Path.C());
			return Instance->Instance;
		}

		YAML::Node Node = NxFr::Yaml::DeserializeFile(GetFilePath(Instance->Path));

		Instance->Instance = new Asset();
		Instance->Instance->OnDeserialize(Node);
		Instance->Instance->Initialize();
		Instance->Count = 1;

		return Instance->Instance;
	}

	void AssetsSystem::Unload(NxFr::StringView Path)
	{
		Info* Instance = GetAsset(Path);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %s doesn't exist", Path.C());
			return;
		}

		if (!Instance->Instance)
		{
			NEXUS_LOG(Warning, Default, "Asset %s not loaded.", Path.C());
			return;
		}

		Instance->Instance->Shutdown();
		delete Instance->Instance;
		Instance->Instance = nullptr;
		Instance->Count = 0;
	}

	void AssetsSystem::Purge()
	{
		for (auto& [Path, Instance] : Assets)
		{
			if (Instance.Instance)
			{
				Unload(Path);
			}
		}
	}

	NxFr::List<NxFr::StringView> AssetsSystem::Find(NxFr::String Filter)
	{
		NxFr::List<NxFr::StringView> Result;
		for (auto& [Id, Instance] : Assets)
		{
			if (NxFr::StringUtility::Contains(Instance.Path, Filter))
			{
				Result.Append(Instance.Path);
			}
		}

		return Result;
	}

	void AssetsSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsTrackedId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsLoadedId, UnsignedInteger, Set);

		LoadDatabase();
	}

	void AssetsSystem::OnShutdown()
	{
		System::OnShutdown();

		SaveDatabase();
	}

	void AssetsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		uint64 Loaded = 0;
		for (auto& [Path, Instance] : Assets)
		{
			Loaded++;
		}

		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsTrackedId, Assets.GetCount());
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsLoadedId, Loaded);
	}

	void AssetsSystem::LoadDatabase()
	{
		NxFr::Path Path = Project::GetSavedConfigPath("", DatabaseName, "", DatabaseExtension);

		if (Path.Exist())
		{
			NxFr::Csv Csv = NxFr::Csv(Path);
			Csv.ReadFile();

			for (uint64 Index = 0; Index < Csv.GetLinesCount(); ++Index)
			{
				NxFr::StringView File = Csv.GetData(Index, 0);

				Info* Instance = &Assets.AppendConstruct(File);
				Instance->Path = File;
			}
		}
		else
		{
			// TODO: Move to AssetBrowser
			NxFr::Directory Directory(NxFr::Paths::Assets);
			NxFr::List<NxFr::String> Files = Directory.GetFiles(true);

			for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
			{
				NxFr::Path File = NxFr::Path(Files[Index]);
				if (NxFr::Path::HasExtension(File, AssetExtension))
				{
					File.ConvertAbsoluteToRelative(NxFr::Paths::Assets);
					File = File.GetPathWithoutExtension();

					Info* Instance = &Assets.AppendConstruct((NxFr::StringView)File);
					Instance->Path = File;
				}
			}
		}
	}

	void AssetsSystem::SaveDatabase()
	{
		NxFr::Path Path = Project::GetSavedConfigPath("", DatabaseName, "", DatabaseExtension);

		NxFr::Csv Csv = NxFr::Csv(Path);
		Csv.AppendHeader("Path");

		for (auto& [File, Instance] : Assets)
		{
			Csv.AppendCell(Instance.Path);
			Csv.AppendNewLine();
		}

		Csv.WriteFile();
	}

	AssetsSystem::Info* AssetsSystem::GetAsset(NxFr::StringView Path)
	{
		Info* Instance = Assets.TryGet(Path);
		return Instance ? Instance : nullptr;
	}

	NxFr::Path AssetsSystem::GetFilePath(NxFr::StringView Path)
	{
		return NxFr::Paths::Assets + (Path + AssetExtension);
	}
}
