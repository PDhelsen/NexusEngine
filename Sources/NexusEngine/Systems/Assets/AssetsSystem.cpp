#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"

#include "NexusEngine/Systems/Assets/AssetsRegistry.h"
#include "NexusEngine/Systems/Assets/AssetsManager.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

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
	NEXUS_OBJECT_IMPLEMENTATION(AssetsSystem)

	void AssetsSystem::Create_Append(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NEXUS_ASSERT(Instance->GetId() == 0, System, "Already tracked asset %d", Instance->GetId());

		NxFr::GUID Id = NxFr::Integer::GenerateGuid();

		Registry->Append(Id, AssetMetadata(Instance, Path, Extension));
		Manager->Append(Id, AssetHandle(Instance));
		Manager->Acquire(Id);

		Instance->Id = Id;

		OnEvent.Invoke(EventCreateId, Instance->GetId());
	}

	void AssetsSystem::Rename(NxFr::GUID Id, NxFr::StringView Name)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (Name.IsEmpty())
		{
			NEXUS_LOG(Warning, System, "Can't rename with empty name");
			return;
		}

		if (!Registry->HasFile(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		NxFr::StringView AssetPath = IdToPath(Id);
		Registry->Move(Id, NxFr::Path::ChangeFileName(AssetPath, Name));

		OnEvent.Invoke(EventRenameId, Id);
	}

	void AssetsSystem::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (Path.IsEmpty())
		{
			NEXUS_LOG(Warning, System, "Can't move to empty path");
			return;
		}

		if (!Registry->HasFile(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		Registry->Move(Id, Path);

		OnEvent.Invoke(EventMoveId, Id);
	}

	void AssetsSystem::Delete(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (Manager->IsValid(Id))
		{
			AssetHandle& Handle = Manager->Get(Id);
			Asset* Instance = Handle.GetInstance();

			Manager->Unload(Id);
			Manager->Remove(Id);

			Instance->Shutdown();
			delete Instance;
		}

		Registry->Remove(Id);

		OnEvent.Invoke(EventDeleteId, Id);
	}

	void AssetsSystem::Save(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (!Registry->HasFile(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		if (!Manager->IsValid(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d is not loaded");
			return;
		}

		AssetMetadata& Metadata = Registry->Get(Id);
		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		if (!Instance->IsDirty())
		{
			return;
		}

		OnSave.Invoke(Instance);
		Metadata.Dependencies = Instance->GetDependencies();

		YAML::Node Node = Manager->Save(Id, Registry->IdToContent(Id));
		Registry->Serialize(Id, Node);

		OnEvent.Invoke(EventSaveId, Id);
	}

	void AssetsSystem::Save()
	{
		NxFr::List<NxFr::GUID> Ids = Manager->GetDirty();
		for (auto& Id : Ids)
		{
			Save(Id);
		}

		OnEvent.Invoke(EventSaveId, 0);
	}

	void AssetsSystem::Track(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NEXUS_ASSERT(Instance->GetId() == 0, System, "Already tracked asset %d", Instance->GetId());

		NxFr::GUID Id = NxFr::Integer::GenerateGuid();

		Registry->Append(Id, AssetMetadata(Instance, Path, Extension));
		Manager->Append(Id, AssetHandle(Instance));
		Manager->Acquire(Id);

		Instance->Id = Id;

		OnEvent.Invoke(EventTrackId, Id);
	}

	Asset* AssetsSystem::Acquire_Check(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (!Manager->IsValid(Id))
		{
			return nullptr;
		}

		Manager->Acquire(Id);

		OnEvent.Invoke(EventAcquireId, Id);

		return Manager->Get(Id).GetInstance();
	}

	void AssetsSystem::Acquire_Load(NxFr::GUID Id, Asset* Instance)
	{
		NEXUS_ASSERT(Registry->HasFile(Id), System, "Asset has no associated path(%d)", Id);
		NEXUS_ASSERT(Instance->GetObjectType() == Registry->Get(Id).GetType(), System, "Asset file type doesn't match runtime type (%d)", Id);

		YAML::Node Node = Registry->Deserialize(Id);
		Manager->Append(Id, AssetHandle(Instance));
		Manager->Acquire(Id);
		Manager->Load(Id, Node, Registry->IdToContent(Id));

		Instance->Id = Id;
		Instance->Initialize();

		OnEvent.Invoke(EventAcquireId, Id);
	}

	void AssetsSystem::Release(NxFr::GUID Id, bool Keep)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (!Manager->IsValid(Id) || !Manager->IsUsed(Id))
		{
			return;
		}

		Manager->Release(Id);
		if (!Manager->IsUsed(Id) && !Keep)
		{
			AssetHandle& Handle = Manager->Get(Id);
			Asset* Instance = Handle.GetInstance();

			Manager->Unload(Id);
			Manager->Remove(Id);

			Instance->Shutdown();
			delete Instance;
		}

		OnEvent.Invoke(EventReleaseId, Id);
	}

	void AssetsSystem::Purge()
	{
		NxFr::List<NxFr::GUID> Ids = Manager->GetUnused();
		for (auto& Id : Ids)
		{
			Release(Id, false);
		}

		OnEvent.Invoke(EventReleaseId, 0);
	}

#if NEXUS_EDITOR
	void AssetsSystem::Import_Append(Asset* Instance, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NEXUS_ASSERT(Instance->GetId() == 0, System, "Already imported asset %d", Instance->GetId());

		NxFr::GUID Id = NxFr::Integer::GenerateGuid();

		Registry->Append(Id, AssetMetadata(Instance, Path, Extension));
		Manager->Append(Id, AssetHandle(Instance));
		Manager->Acquire(Id);
		Manager->Load(Id, Node, Registry->IdToContent(Id));

		Instance->Id = Id;
		Instance->SetDirty();
		Instance->Initialize();

		OnEvent.Invoke(EventImportId, Id);
	}
#endif

	void AssetsSystem::Reimport_Load(Asset* Instance, const YAML::Node& Node, NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->HasFile(Id), System, "Asset has no associated path(%d)", Id);
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (Manager->IsValid(Id))
		{
			Manager->Unload(Id);
			Instance->Shutdown();
		}
		else
		{
			Manager->Append(Id, AssetHandle(Instance));
			Manager->Acquire(Id);

			Instance->Id = Id;
		}

		Manager->Load(Id, Node, Registry->IdToContent(Id));

		Instance->SetDirty();
		Instance->Initialize();

		OnEvent.Invoke(EventImportId, Id);
	}

	NxFr::Array<NxFr::GUID> AssetsSystem::Find(NxFr::StringView Filter) const
	{
		return Registry->Find(Filter);
	}

	NxFr::GUID AssetsSystem::PathToId(NxFr::StringView Path) const
	{
		return Registry->PathToId(Path);
	}

	NxFr::String AssetsSystem::IdToPath(NxFr::GUID Id) const
	{
		return Registry->IdToPath(Id);
	}

	Asset* AssetsSystem::GetAsset(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		if (!Manager->IsValid(Id))
		{
			return nullptr;
		}

		return Manager->Get(Id).GetInstance();
	}

	AssetHandle& AssetsSystem::GetHandle(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Manager->IsValid(Id), System, "Unloaded asset %d", Id);

		return Manager->Get(Id);
	}

	AssetMetadata& AssetsSystem::GetMetadata(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		return Registry->Get(Id);
	}

	YAML::Node AssetsSystem::GetImportData(NxFr::GUID Id)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		YAML::Node Node = YAML::Node();
		return Registry->GetImportData(Id);
	}

	NxFr::Array<NxFr::GUID> AssetsSystem::GetDependencies(NxFr::GUID Id, bool Recusive)
	{
		NEXUS_ASSERT(Registry->IsValid(Id), System, "Unknown asset %d", Id);

		NxFr::Set<NxFr::GUID> Dependencies;
		FetchDependencies(Id, Recusive, Dependencies);
		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Dependencies);
	}

	void AssetsSystem::OnInitialize()
	{
		System::OnInitialize();

		RecordStats();

		Registry = new AssetsRegistry(NxFr::Paths::Assets);
		Manager = new AssetsManager();
	}

	void AssetsSystem::OnShutdown()
	{
		System::OnShutdown();

		delete Registry;
		delete Manager;
	}

	void AssetsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		UpdateStats();
	}

	void AssetsSystem::RecordStats() const
	{
		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsTrackedId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsLoadedId, UnsignedInteger, Set);
	}

	void AssetsSystem::UpdateStats() const
	{
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsTrackedId, Registry->GetCount());
		NEXUS_STAT_UNSIGNEDINTEGER(NxFr::StatsHeader::AssetsLoadedId, Manager->GetCount());
	}

	void AssetsSystem::FetchDependencies(NxFr::GUID Id, bool Recusive, NxFr::Set<NxFr::GUID>& Result)
	{
		NxEn::Asset* Instance = GetAsset(Id);

		NxFr::List<NxFr::GUID> Dependencies;
		if (Instance && Instance->IsDirty())
		{
			Dependencies.AppendRange(GetHandle(Id).GetInstance()->GetDependencies());
		}
		else
		{
			Dependencies.AppendRange(GetMetadata(Id).GetDependencies());
		}

		for (auto& Dependency : Dependencies)
		{
			if (Result.Contains(Dependency) || Dependency == 0)
			{
				continue;
			}

			Result.Append(Dependency);
			if (Recusive)
			{
				FetchDependencies(Dependency, true, Result);
			}
		}
	}
}
