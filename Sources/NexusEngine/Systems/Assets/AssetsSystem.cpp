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

	Asset* AssetsSystem::Get(NxFr::GUID Guid, bool CreateIfDontExist)
	{
		Info* Instance = GetAsset(Guid);

		if (Instance == nullptr)
		{
			if (CreateIfDontExist)
			{
				return Create(Guid);
			}
			else
			{
				return nullptr;
			}
		}

		if (Instance->Instance == nullptr)
		{
			return Load(Guid);
		}

		Instance->Count++;
		return Instance->Instance;
	}

	void AssetsSystem::Release(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);

		if (Instance->Instance == nullptr)
		{
			return;
		}

		Instance->Count--;
		if (Instance->Count == 0)
		{
			Unload(Guid);
		}
	}

	Asset* AssetsSystem::Create(NxFr::GUID Guid)
	{
		Info* Instance = AddAsset(Guid);
		Instance->Instance = new Asset(Guid);
		Instance->Count = 1;

		if (Instance->Path.IsValid())
		{
			NxFr::File(GetFilePath(Instance->Path)).Create();
		}

		return Instance->Instance;
	}

	void AssetsSystem::Delete(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
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

		RemoveAsset(Guid);
	}

	void AssetsSystem::Move(NxFr::GUID Guid, NxFr::StringView Target)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return;
		}

		if (Instance->Path.IsValid())
		{
			NxFr::File(GetFilePath(Instance->Path)).Move(GetFilePath(Target));
		}

		MoveAsset(Guid, Target);
	}

	void AssetsSystem::Save(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return;
		}

		if (!Instance->Instance)
		{
			return;
		}

		YAML::Node Node;
		Node["Guid"] = Instance->Guid;

		Instance->Instance->OnSerialize(Node);

		NxFr::Yaml::SerializeFile(Node, GetFilePath(Instance->Path));
	}

	void AssetsSystem::SaveAll()
	{
		for (auto& [Guid, Instance] : Assets)
		{
			Save(Guid);
		}
	}

	Asset* AssetsSystem::Load(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return nullptr;
		}

		if (Instance->Instance)
		{
			return Instance->Instance;
		}

		YAML::Node Node = NxFr::Yaml::DeserializeFile(GetFilePath(Instance->Path));

		Instance->Instance = new Asset(Guid);
		Instance->Instance->OnDeserialize(Node);
		Instance->Instance->Initialize();
		Instance->Count = 1;

		return Instance->Instance;
	}

	void AssetsSystem::Unload(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return;
		}

		if (!Instance->Instance)
		{
			return;
		}

		Instance->Instance->Shutdown();
		delete Instance->Instance;
		Instance->Instance = nullptr;
		Instance->Count = 0;
	}

	void AssetsSystem::Purge()
	{
		for (auto& [Guid, Instance] : Assets)
		{
			Unload(Guid);
		}
	}

	NxFr::StringView AssetsSystem::GuidToPath(NxFr::GUID Guid)
	{
		Info* Instance = GetAsset(Guid);
		if (Instance == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return NxFr::StringUtility::Empty;
		}

		return Instance->Path;
	}

	NxFr::GUID AssetsSystem::PathToGuid(NxFr::StringView Path)
	{
		NxFr::GUID* Guid = Paths.TryGet(Path);
		if (Guid == nullptr)
		{
			NEXUS_LOG(Error, Default, "Asset %d doesn't exist", Guid);
			return 0;
		}

		return *Guid;
	}

	NxFr::List<NxFr::StringView> AssetsSystem::Find(NxFr::StringView Filter)
	{
		NxFr::List<NxFr::StringView> Result;
		for (auto& [Guid, Instance] : Assets)
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

		StatsRegister();
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

		StatsUpdate();
	}

	void AssetsSystem::StatsRegister() const
	{
		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsTrackedId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsLoadedId, UnsignedInteger, Set);
	}

	void AssetsSystem::StatsUpdate() const
	{
		uint64 Loaded = 0;
		for (auto& [Guid, Instance] : Assets)
		{
			if (Instance.Instance)
			{
				Loaded++;
			}
		}

		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsTrackedId, Assets.GetCount());
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsLoadedId, Loaded);
	}

	void AssetsSystem::LoadDatabase()
	{
		NxFr::Path CsvPath = Project::GetSavedConfigPath("", DatabaseName, "", DatabaseExtension);

		if (CsvPath.Exist())
		{
			NxFr::Csv Csv = NxFr::Csv(CsvPath);
			Csv.ReadFile();

			for (uint64 Index = 0; Index < Csv.GetLinesCount(); ++Index)
			{
				NxFr::GUID Guid = NxFr::StringUtility::FromString<NxFr::GUID>(Csv.GetData(Index, 0));
				NxFr::StringView Path = Csv.GetData(Index, 1);

				AddAsset(Guid, Path);
			}
		}
		else
		{
			// TODO: Move to AssetBrowser
			NxFr::Directory Directory(NxFr::Paths::Assets);
			NxFr::List<NxFr::String> Files = Directory.GetFiles(true);

			for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
			{
				NxFr::Path Path = NxFr::Path(Files[Index]);
				if (NxFr::Path::HasExtension(Path, AssetExtension))
				{
					Path = NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
					Path = Path.GetPathWithoutExtension();

					AddAsset(NxFr::Integer::GenerateGuid(), Path);
				}
			}
		}
	}

	void AssetsSystem::SaveDatabase()
	{
		NxFr::Path CsvPath = Project::GetSavedConfigPath("", DatabaseName, "", DatabaseExtension);

		NxFr::Csv Csv = NxFr::Csv(CsvPath);
		Csv.AppendHeader("Guid");
		Csv.AppendHeader("Path");

		for (auto& [Guid, Instance] : Assets)
		{
			Csv.AppendCell(NxFr::StringUtility::ToString(Instance.Guid));
			Csv.AppendCell(Instance.Path);
			Csv.AppendNewLine();
		}

		Csv.WriteFile();
	}

	AssetsSystem::Info* AssetsSystem::GetAsset(NxFr::GUID Guid)
	{
		Info* Instance = Assets.TryGet(Guid);
		return Instance ? Instance : nullptr;
	}

	AssetsSystem::Info* AssetsSystem::AddAsset(NxFr::GUID Guid, NxFr::StringView Path)
	{
		if (!Path.IsEmpty())
		{
			Paths.Append(Path, Guid);
		}

		Info Instance;
		Instance.Guid = Guid;
		Instance.Path = Path;
		Instance.Count = 0;
		Instance.Instance = nullptr;
		return &Assets.Append(Guid, NxFr::Move(Instance));
	}

	void AssetsSystem::RemoveAsset(NxFr::GUID Guid)
	{
		Info& Instance = Assets[Guid];
		if (Instance.Path.IsValid())
		{
			Paths.Remove(Instance.Path);
		}

		Assets.Remove(Guid);
	}

	void AssetsSystem::MoveAsset(NxFr::GUID Guid, NxFr::StringView Path)
	{
		Info& Instance = Assets[Guid];
		if (Instance.Path.IsValid())
		{
			Paths.Remove(Instance.Path);
			Paths.Append(Path, Guid);
		}

		Assets[Guid].Path = Path;
	}

	NxFr::Path AssetsSystem::GetFilePath(NxFr::StringView Path)
	{
		return NxFr::Paths::Assets + (Path + AssetExtension);
	}
}
