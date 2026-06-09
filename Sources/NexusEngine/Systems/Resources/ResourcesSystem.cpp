#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/ResourcesSystem.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId ResourcesId = "Resources"_Sid;
	}
}

namespace NxEn
{
	void ResourcesSystem::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		Instance->Path = Path;
		Resources.Remove(Path);
		Resources.Append(Target, Instance);
		NxFr::File(GetResourceFsPath(Path)).Move(GetResourceFsPath(Target), true);
	}

	void ResourcesSystem::Delete(NxFr::StringView Path)
	{
		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		Instance->Unload();
		Resources.Remove(Path);
		NxFr::File(GetResourceFsPath(Path)).Delete();
	}

	void ResourcesSystem::Unload(NxFr::StringView Path)
	{
		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		Instance->Unload();
		Resources.Remove(Path);
	}

	void ResourcesSystem::Save(NxFr::StringView Path)
	{
		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		Instance->Save(GetResourceFsPath(Path));
	}

	void ResourcesSystem::UnloadAll()
	{
		for (auto& [Path, Instance] : Resources)
		{
			Instance->Unload();
		}

		Resources.Clear();
	}

	void ResourcesSystem::SaveAll()
	{
		for (auto& [Path, Instance] : Resources)
		{
			Instance->Save(GetResourceFsPath(Path));
		}
	}

	void ResourcesSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::ResourcesId, Integer, Set);
	}

	void ResourcesSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		NX_STAT_INTEGER(NxFr::StatsHeader::ResourcesId, Resources.GetCount());
	}

	Resource* ResourcesSystem::GetResource(NxFr::StringView Path)
	{
		Resource** Instance = Resources.TryGet(Path);
		return Instance ? *Instance : nullptr;
	}

	NxFr::String ResourcesSystem::GetResourceFsPath(NxFr::StringView Path)
	{
		return NxFr::Path::Combine(NxFr::Globals::Paths::Resources, Path);
	}
}
