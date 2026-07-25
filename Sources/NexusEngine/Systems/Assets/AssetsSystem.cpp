#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"

#include "NexusEngine/Systems/Assets/AssetsRegistry.h"
#include "NexusEngine/Systems/Assets/AssetsManager.h"

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
	static Command* CmdAssetCreate = Command::Create("Assets.Create"_Sid, "Create asset", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView Path, NxFr::StringView Extension)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Create(NxFr::StringId(Type), Path, Extension);
	}));
	static Command* CmdAssetMove = Command::Create("Assets.Move"_Sid, "Move asset", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Move(Assets->PathToId(Path), Target);
	}));
	static Command* CmdAssetCopy = Command::Create("Assets.Copy"_Sid, "Copy asset", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Copy(Assets->PathToId(Path), Target);
	}));
	static Command* CmdAssetDelete = Command::Create("Assets.Delete"_Sid, "Delete asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Delete(Assets->PathToId(Path));
	}));
	static Command* CmdAssetSave = Command::Create("Assets.Save"_Sid, "Save asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Save(Assets->PathToId(Path));
	}));
	static Command* CmdAssetSaveDirty = Command::Create("Assets.SaveDirty"_Sid, "Save all dirty assets", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->SaveDirty();
	}));
	static Command* CmdAssetLoad = Command::Create("Assets.Load"_Sid, "Load asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Load(Assets->PathToId(Path));
	}));
	static Command* CmdAssetReload = Command::Create("Assets.Reload"_Sid, "Reload asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Reload(Assets->PathToId(Path));
	}));
	static Command* CmdAssetUnload = Command::Create("Assets.Unload"_Sid, "Unload asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Unload(Assets->PathToId(Path));
	}));
	static Command* CmdAssetPurge = Command::Create("Assets.Purge"_Sid, "Unload all unreferenced assets", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Save)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Purge(NxFr::StringUtility::FromString<bool>(Save));
	}));
	static Command* CmdAssetImport = Command::Create("Assets.Import"_Sid, "Import asset", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView, NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Type, NxFr::StringView Node, NxFr::StringView Path, NxFr::StringView Extension)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Import(NxFr::StringId(Type), NxFr::Yaml::Deserialize(Node), Path, Extension);
	}));
	static Command* CmdAssetReimport = Command::Create("Assets.Reimport"_Sid, "Reimport asset", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Node)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		Assets->Reimport(Assets->PathToId(Path), NxFr::Yaml::Deserialize(Node));
	}));
	static Command* CmdAssetIdToPath = Command::Create("Assets.IdToPath"_Sid, "Print the path of the asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::GUID AssetId = NxFr::StringUtility::FromString<NxFr::GUID>(Id);
		NxFr::StringView Path = Assets->IdToPath(AssetId);
		NX_LOG(Info, System, "Asset (%llu) has path %s", Id, Path.C());
	}));
	static Command* CmdAssetPathToId = Command::Create("Assets.PathToId"_Sid, "Print the id of the asset", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::GUID Id = Assets->PathToId(Path);
		NX_LOG(Info, System, "Asset (%s) has Id %llu", Path.C(), Id);
	}));

	NxFr::Factory<Asset>& AssetsSystem::GetFactory()
	{
		static NxFr::Factory<Asset> Factory;
		return Factory;
	}

	Asset* AssetsSystem::Create(NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NxEn::Asset* Instance = GetFactory().Create(Type);

		Track(Instance, Path, Extension);
		Instance->Initialize();

		return Instance;
	}

	void AssetsSystem::Rename(NxFr::GUID Id, NxFr::StringView Name)
	{
		NxFr::StringView AssetPath = IdToPath(Id);
		Move(Id, NxFr::Path::ChangeName(AssetPath, Name));
	}

	void AssetsSystem::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (Path.IsEmpty())
		{
			NX_LOG(Warning, System, "Can't move to empty path");
			return;
		}

		if (!Registry->HasFile(Id))
		{
			NX_LOG(Warning, System, "Asset %llu has no associated path", Id);
			return;
		}

		Registry->Move(Id, Path);
		Registry->SerializeAndSave(Id, YAML::Node());

		OnEvent.Invoke(EventMovedId, Id);
	}

	void AssetsSystem::Copy(NxFr::GUID Id, NxFr::StringView Path)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (Path.IsEmpty())
		{
			NX_LOG(Warning, System, "Can't copy to empty path");
			return;
		}

		if (!Registry->HasFile(Id))
		{
			NX_LOG(Warning, System, "Asset %llu has no associated path", Id);
			return;
		}

		AssetMetadata& Metadata = Registry->Copy(Id, Path);
		Registry->SerializeAndSave(Metadata.GetId(), YAML::Node());

		OnEvent.Invoke(EventCopiedId, Id);
	}

	void AssetsSystem::Delete(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		Unload(Id);
		Registry->Remove(Id);

		OnEvent.Invoke(EventDeletedId, Id);
	}

	void AssetsSystem::Save(NxFr::GUID Id, bool Force)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (!Registry->HasFile(Id))
		{
			NX_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		if (!IsLoaded(Id))
		{
			NX_LOG(Warning, System, "Asset %llu is not loaded", Id);
			return;
		}

		AssetMetadata& Metadata = Registry->Get(Id);
		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();
		if (!Instance->IsDirty() && !Force)
		{
			return;
		}

		OnEvent.Invoke(EventSaveId, Id);

		Metadata.Name = Instance->GetName();
		Metadata.Dependencies = Instance->GetDependencies();

		YAML::Node Assetdata = Manager->SerializeAndSave(Id, Registry->IdToContentFsPath(Id));
		Registry->SerializeAndSave(Id, Assetdata);

		Instance->Dirty = false;

		OnEvent.Invoke(EventSavedId, Id);
	}

	void AssetsSystem::SaveMetadata(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (!Registry->HasFile(Id))
		{
			NX_LOG(Warning, System, "Asset %d has no associated path");
			return;
		}

		OnEvent.Invoke(EventSaveId, Id);

		Registry->SerializeAndSave(Id, YAML::Node());

		OnEvent.Invoke(EventSavedId, Id);
	}

	void AssetsSystem::SaveDirty()
	{
		OnEvent.Invoke(EventSaveId, 0);

		NxFr::List<NxFr::GUID> Ids = Manager->GetDirty();
		for (auto& Id : Ids)
		{
			if (Registry->Get(Id).HasFile())
			{
				Save(Id);
			}
		}

		OnEvent.Invoke(EventSavedId, 0);
	}

	void AssetsSystem::Track(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NX_ASSERT_RETURN(Instance->GetId() == Object::NullId, , System, "Already tracked asset %llu", Instance->GetId());
		
		AssetMetadata& Metadata = Registry->Append(Instance->GetObjectType(), Path, Extension);
		AssetHandle& Handle = Manager->Append(Metadata.GetId(), Instance);

		Instance->Id = Metadata.GetId();
		Instance->Name = Metadata.GetName();
		Instance->Dirty = true;

		OnEvent.Invoke(EventCreatedId, Metadata.GetId());
	}

	Asset* AssetsSystem::Clone(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);
		NX_ASSERT_RETURN(IsLoaded(Id), nullptr, System, "Asset %llu is not loaded", Id);

		AssetMetadata& Metadata = Registry->Get(Id);
		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = GetFactory().Create(Metadata.GetType());

		Instance->Id = Metadata.GetId();
		Instance->Name = Metadata.GetName();

		Instance->Object::Clone((const Object*)Handle.GetInstance());
		Instance->Initialize();

		return Instance;
	}

	Asset* AssetsSystem::Acquire(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);

		Asset* Instance = Load(Id);
		Manager->Acquire(Id);
		return Instance;
	}

	void AssetsSystem::Release(NxFr::GUID Id, bool Keep)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		Manager->Release(Id);
		if (!Manager->IsUsed(Id) && !Keep)
		{
			Unload(Id);
		}
	}

	Asset* AssetsSystem::Load(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);

		if (IsLoaded(Id))
		{
			return Manager->Get(Id).GetInstance();
		}

		NX_ASSERT_RETURN(Registry->HasFile(Id), nullptr, System, "Asset has no associated path(%llu)", Id);

		AssetMetadata& Metadata = Registry->Get(Id);
		Asset* Instance = GetFactory().Create(Metadata.GetType());
		AssetHandle& Handle = Manager->Append(Id, Instance);

		Instance->Id = Id;
		Instance->Name = Metadata.GetName();

		YAML::Node Assetdata = Registry->LoadAndDeserialize(Id);
		Manager->LoadAndDeserialize(Id, Assetdata, Registry->IdToContentFsPath(Id));
		Instance->Initialize();

		OnEvent.Invoke(EventLoadedId, Id);
		return Instance;
	}

	void AssetsSystem::Reload(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (!IsLoaded(Id))
		{
			Load(Id);
			return;
		}

		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		Instance->Shutdown();
		Manager->Unload(Id);

		Instance->Dirty = false;

		YAML::Node Assetdata = Registry->LoadAndDeserialize(Id);
		Manager->LoadAndDeserialize(Id, Assetdata, Registry->IdToContentFsPath(Id));
		Instance->Initialize();

		OnEvent.Invoke(EventLoadedId, Id);
	}

	void AssetsSystem::Unload(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		if (!IsLoaded(Id))
		{
			return;
		}

		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		Instance->Shutdown();
		Manager->Unload(Id);
		Manager->Remove(Id);

		delete Instance;

		OnEvent.Invoke(EventUnloadedId, Id);
	}

	void AssetsSystem::Purge(bool SaveIfDirty)
	{
		NxFr::List<NxFr::GUID> Ids = Manager->GetUnused();
		for (auto& Id : Ids)
		{
			if (SaveIfDirty)
			{
				Save(Id);
			}

			Unload(Id);
		}

		OnEvent.Invoke(EventUnloadedId, 0);
	}

	Asset* AssetsSystem::Import(NxFr::StringId Type, const YAML::Node& Assetdata, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NxEn::Asset* Instance = GetFactory().Create(Type);

		Track(Instance, Path, Extension);
		Manager->LoadAndDeserialize(Instance->GetId(), Assetdata, Registry->IdToContentFsPath(Instance->GetId()));
		Instance->Initialize();

		OnEvent.Invoke(EventImportedId, Instance->GetId());
		return Instance;
	}

	Asset* AssetsSystem::Reimport(NxFr::GUID Id, const YAML::Node& Assetdata)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);
		NX_ASSERT_RETURN(Registry->HasFile(Id), nullptr, System, "Asset has no associated path(%llu)", Id);

		if (!IsLoaded(Id))
		{
			return Load(Id);
		}

		AssetHandle& Handle = Manager->Get(Id);
		Asset* Instance = Handle.GetInstance();

		Instance->Shutdown();
		Manager->Unload(Id);

		Instance->Dirty = true;

		Manager->LoadAndDeserialize(Id, Assetdata, Registry->IdToContentFsPath(Id));
		Instance->Initialize();

		OnEvent.Invoke(EventImportedId, Instance->GetId());
		return Instance;
	}

	Asset* AssetsSystem::GetAsset(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);

		if (!IsLoaded(Id))
		{
			return nullptr;
		}

		return Manager->Get(Id).GetInstance();
	}

	AssetHandle* AssetsSystem::GetHandle(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsLoaded(Id), nullptr, System, "Unloaded asset %llu", Id);

		return &Manager->Get(Id);
	}

	AssetMetadata* AssetsSystem::GetMetadata(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), nullptr, System, "Unknown asset %llu", Id);

		return &Registry->Get(Id);
	}

	YAML::Node AssetsSystem::GetAssetdata(NxFr::GUID Id)
	{
		NX_ASSERT_RETURN(IsTracked(Id), YAML::Node(), System, "Unknown asset %llu", Id);

		return Registry->LoadAndDeserialize(Id);
	}

	NxFr::Array<NxFr::GUID> AssetsSystem::GetDependencies(NxFr::GUID Id, bool Recursive)
	{
		NxFr::Set<NxFr::GUID> Dependencies;
		GetDependencies(Id, Recursive, Dependencies);
		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Dependencies);
	}

	void AssetsSystem::GetDependencies(NxFr::GUID Id, bool Recursive, NxFr::Set<NxFr::GUID>& Result)
	{
		NX_ASSERT_RETURN(IsTracked(Id), , System, "Unknown asset %llu", Id);

		NxEn::Asset* Instance = GetAsset(Id);

		NxFr::List<NxFr::GUID> Dependencies;
		if (Instance && Instance->IsDirty())
		{
			Dependencies.AppendRange(Manager->Get(Id).GetInstance()->GetDependencies());
		}
		else
		{
			Dependencies.AppendRange(Registry->Get(Id).GetDependencies());
		}

		for (auto& Dependency : Dependencies)
		{
			if (Dependency == 0 || Result.TryGet(Dependency))
			{
				continue;
			}

			Result.Append(Dependency);
			if (Recursive)
			{
				GetDependencies(Dependency, Recursive, Result);
			}
		}
	}

	NxFr::Array<NxFr::GUID> AssetsSystem::Find(NxFr::StringView Filter) const
	{
		return Registry->Find(Filter);
	}

	NxFr::GUID AssetsSystem::PathToId(NxFr::StringView Path) const
	{
		return Registry->PathToId(Path);
	}

	NxFr::StringView AssetsSystem::IdToPath(NxFr::GUID Id) const
	{
		return Registry->IdToPath(Id);
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

		Registry = new AssetsRegistry(NxFr::Globals::Paths::Assets);
		Manager = new AssetsManager();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsTrackedId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::AssetsLoadedId, Integer, Set);
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

		NX_STAT_INTEGER(NxFr::StatsHeader::AssetsTrackedId, Registry->GetCount());
		NX_STAT_INTEGER(NxFr::StatsHeader::AssetsLoadedId, Manager->GetCount());
	}
}
