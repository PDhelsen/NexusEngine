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
		NX_INSTUMENT_FUNCTION();

		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, System, "Resources %s was not tracked", Path.C());
			return;
		}

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			Instance->Path = Path;
		}

		Resources.Remove(Path);
		Resources.Append(Target, Instance);
		NxFr::File(GetResourceFsPath(Path)).Move(GetResourceFsPath(Target), true);
	}

	void ResourcesSystem::Delete(NxFr::StringView Path)
	{
		NX_INSTUMENT_FUNCTION();

		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, System, "Resources %s was not tracked", Path.C());
			return;
		}

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			Instance->Unload();
		}

		Resources.Remove(Path);
		NxFr::File(GetResourceFsPath(Path)).Delete();
	}

	void ResourcesSystem::Unload(NxFr::StringView Path)
	{
		NX_INSTUMENT_FUNCTION();

		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, System, "Resources %s was not tracked", Path.C());
			return;
		}

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			Instance->Unload();
		}

		Resources.Remove(Path);
	}

	void ResourcesSystem::Save(NxFr::StringView Path)
	{
		NX_INSTUMENT_FUNCTION();

		Resource* Instance = GetResource(Path);
		if (Instance == nullptr)
		{
			NX_LOG(Warning, System, "Resources %s was not tracked", Path.C());
			return;
		}

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			Instance->Save(GetResourceFsPath(Path));
		}
	}

	void ResourcesSystem::UnloadAll()
	{
		NX_INSTUMENT_FUNCTION();

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			for (auto& [Path, Instance] : Resources)
			{
				Instance->Unload();
			}
		}

		Resources.Clear();
	}

	void ResourcesSystem::SaveAll()
	{
		NX_INSTUMENT_FUNCTION();

		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::System);
			for (auto& [Path, Instance] : Resources)
			{
				Instance->Save(GetResourceFsPath(Path));
			}
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
