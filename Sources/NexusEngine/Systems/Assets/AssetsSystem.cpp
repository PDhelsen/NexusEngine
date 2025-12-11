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
	const static Command CmdAssetPurge = Command::Create("Assets.Purge"_Sid, "Unload all unreferenced assets", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<AssetsSystem>()->Purge();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsSystem)

	Asset* AssetsSystem::Create(NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NxEn::Asset* Instance = AssetsFactory::Create(Type);
		Track(Instance, Path, Extension);
		return Instance;
	}

	void AssetsSystem::Rename(NxFr::GUID Id, NxFr::StringView Name)
	{
		NxFr::StringView AssetPath = IdToPath(Id);
		Move(Id, NxFr::Path::ChangeFileName(AssetPath, Name));
	}

	void AssetsSystem::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

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

		Load(Id);
		Registry->Move(Id, Path);
		Save(Id, true);

		OnEvent.Invoke(EventMovedId, Id);
	}

	void AssetsSystem::Delete(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		Unload(Id);
		Registry->Remove(Id);

		OnEvent.Invoke(EventDeletedId, Id);
	}

	void AssetsSystem::Save(NxFr::GUID Id, bool Force)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		if (!Registry->HasFile(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		if (!IsLoaded(Id))
		{
			NEXUS_LOG(Warning, System, "Asset %d is not loaded");
			return;
		}

		AssetMetadata& Metadata = Registry->Get(Id);
		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		if (!Instance->IsDirty() && !Force)
		{
			return;
		}

		OnSave.Invoke(Instance);
		Metadata.Dependencies = Instance->GetDependencies();

		YAML::Node Node = Manager->Save(Id, Registry->IdToContent(Id));
		Registry->Serialize(Id, Node);

		OnEvent.Invoke(EventSavedId, Id);
	}

	void AssetsSystem::Save()
	{
		NxFr::List<NxFr::GUID> Ids = Manager->GetDirty();
		for (auto& Id : Ids)
		{
			Save(Id);
		}

		OnEvent.Invoke(EventSavedId, 0);
	}

	void AssetsSystem::Track(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NEXUS_ASSERT(Instance->GetId() == 0, System, "Already tracked asset %d", Instance->GetId());

		NxFr::GUID Id = NxFr::Integer::GenerateGuid();
		Instance->Id = Id;

		Registry->Append(Id, AssetMetadata(Instance, Path, Extension));
		Manager->Append(Id, AssetHandle(Instance));

		Instance->SetDirty();

		OnEvent.Invoke(EventCreatedId, Id);
	}

	Asset* AssetsSystem::Acquire(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		Asset* Instance = Load(Id);
		Manager->Acquire(Id);
		return Instance;
	}

	void AssetsSystem::Release(NxFr::GUID Id, bool Keep)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		Manager->Release(Id);
		if (!Manager->IsUsed(Id) && !Keep)
		{
			Unload(Id);
		}
	}

	Asset* AssetsSystem::Load(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		Asset* Instance = nullptr;

		if (IsLoaded(Id))
		{
			Instance = Manager->Get(Id).GetInstance();
		}
		else
		{
			NEXUS_ASSERT(Registry->HasFile(Id), System, "Asset has no associated path(%d)", Id);

			Instance = AssetsFactory::Create(Registry->Get(Id).GetType());
			Instance->Id = Id;

			YAML::Node Node = Registry->Deserialize(Id);
			Manager->Append(Id, AssetHandle(Instance));
			Manager->Load(Id, Node, Registry->IdToContent(Id));

			Instance->Initialize();
		}

		OnEvent.Invoke(EventLoadedId, Id);
		return Instance;
	}

	void AssetsSystem::Reload(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		Asset* Instance = Reset(Id);

		YAML::Node Node = Registry->Deserialize(Id);
		Manager->Load(Id, Node, Registry->IdToContent(Id));

		Instance->Initialize();

		OnEvent.Invoke(EventLoadedId, Id);
	}

	void AssetsSystem::Unload(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		if (!IsLoaded(Id))
		{
			return;
		}

		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		Manager->Unload(Id);
		Manager->Remove(Id);

		Instance->Shutdown();
		delete Instance;

		OnEvent.Invoke(EventUnloadedId, Id);
	}

	void AssetsSystem::Purge()
	{
		NxFr::List<NxFr::GUID> Ids = Manager->GetUnused();
		for (auto& Id : Ids)
		{
			Unload(Id);
		}

		OnEvent.Invoke(EventUnloadedId, 0);
	}

	Asset* AssetsSystem::Import(NxFr::StringId Type, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension)
	{
#if !NEXUS_EDITOR
		return nullptr;
#endif
		NxEn::Asset* Instance = Create(Type, Path, Extension);
		Manager->Load(Instance->GetId(), Node, Registry->IdToContent(Instance->GetId()));

		Instance->SetDirty();
		Instance->Initialize();

		OnEvent.Invoke(EventImportedId, Instance->GetId());
		return Instance;
	}

	Asset* AssetsSystem::Reimport(NxFr::GUID Id, const YAML::Node& Node)
	{
#if !NEXUS_EDITOR
		return nullptr;
#endif

		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);
		NEXUS_ASSERT(Registry->HasFile(Id), System, "Asset has no associated path(%d)", Id);

		Asset* Instance = Reset(Id);
		Manager->Load(Id, Node, Registry->IdToContent(Id));

		Instance->SetDirty();
		Instance->Initialize();

		OnEvent.Invoke(EventImportedId, Instance->GetId());
		return Instance;
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
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		if (!IsLoaded(Id))
		{
			return nullptr;
		}

		return Manager->Get(Id).GetInstance();
	}

	AssetHandle& AssetsSystem::GetHandle(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsLoaded(Id), System, "Unloaded asset %d", Id);

		return Manager->Get(Id);
	}

	AssetMetadata& AssetsSystem::GetMetadata(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		return Registry->Get(Id);
	}

	YAML::Node AssetsSystem::GetImportData(NxFr::GUID Id)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		YAML::Node Node = YAML::Node();
		return Registry->GetImportData(Id);
	}

	NxFr::Array<NxFr::GUID> AssetsSystem::GetDependencies(NxFr::GUID Id, bool Recusive)
	{
		NEXUS_ASSERT(IsTracked(Id), System, "Unknown asset %d", Id);

		NxFr::Set<NxFr::GUID> Dependencies;
		FetchDependencies(Id, Recusive, Dependencies);
		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Dependencies);
	}

	bool AssetsSystem::IsTracked(NxFr::GUID Id) const
	{
		return Registry->IsValid(Id);
	}

	bool AssetsSystem::IsLoaded(NxFr::GUID Id) const
	{
		return Manager->IsValid(Id);
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

	Asset* AssetsSystem::Reset(NxFr::GUID Id)
	{
		Asset* Instance = nullptr;

		if (IsLoaded(Id))
		{
			Instance = Manager->Get(Id).GetInstance();

			Manager->Unload(Id);
			Instance->Shutdown();
		}
		else
		{
			Instance = AssetsFactory::Create(Registry->Get(Id).GetType());
			Instance->Id = Id;

			Manager->Append(Id, AssetHandle(Instance));
		}

		return Instance;
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
