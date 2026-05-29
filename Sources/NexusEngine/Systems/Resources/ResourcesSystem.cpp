#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/ResourcesSystem.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId ResourcesTrackedId = "Resources - Tracked"_Sid;
		const NxFr::StringId ResourcesLoadedId = "Resources - Loaded"_Sid;
	}
}

namespace NxEn
{
	void ResourcesSystem::Unload(NxFr::StringView Path)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		if (Instance->IsLoaded())
		{
			Instance->Unload();
		}
	}

	void ResourcesSystem::UnloadAll()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		for (auto& [Path, Instance] : Resources)
		{
			if (Instance->IsLoaded())
			{
				Instance->Unload();
			}
		}
	}

	void ResourcesSystem::Save(NxFr::StringView Path)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance == nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s was not tracked", Path.C());
			return;
		}

		if (Instance->IsLoaded())
		{
			Instance->Save(GetResourceFilePath(Path));
		}
	}

	void ResourcesSystem::SaveAll()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		for (auto& [Path, Instance] : Resources)
		{
			if (Instance->IsLoaded())
			{
				Instance->Save(GetResourceFilePath(Path));
			}
		}
	}

	void ResourcesSystem::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance != nullptr)
		{
			Resources.Remove(Path);
			Resources.Append(Target, Instance);
			Instance->Path = Target;
		}

		NxFr::File(GetResourceFilePath(Path)).Move(GetResourceFilePath(Target), true);
	}

	void ResourcesSystem::Delete(NxFr::StringView Path)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance != nullptr && Instance->IsLoaded())
		{
			Unload(Path);
		}

		NxFr::File(GetResourceFilePath(Path)).Delete();
	}

	void ResourcesSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::ResourcesTrackedId, Integer, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::ResourcesLoadedId, Integer, Set);
	}

	void ResourcesSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		uint64 Loaded = 0;
		for (auto& [Path, Instance] : Resources)
		{
			if (Instance->IsLoaded())
			{
				Loaded++;
			}
		}

		NX_STAT_INTEGER(NxFr::StatsHeader::ResourcesTrackedId, Resources.GetCount());
		NX_STAT_INTEGER(NxFr::StatsHeader::ResourcesLoadedId, Loaded);
	}

	NxFr::String ResourcesSystem::GetResourceFilePath(NxFr::StringView Path)
	{
		return NxFr::Path::Combine(NxFr::Globals::Paths::Resources, Path);
	}

	Resource* ResourcesSystem::GetResource(NxFr::StringView Path)
	{
		Resource** Instance = Resources.TryGet(Path);
		return Instance ? *Instance : nullptr;
	}
}
