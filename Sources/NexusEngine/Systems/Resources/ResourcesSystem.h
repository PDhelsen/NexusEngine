#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NEXUS_ENGINE_API extern const NxFr::StringId ResourceTrackedId;
		NEXUS_ENGINE_API extern const NxFr::StringId ResourceLoadedId;
	}
}

namespace NxEn
{
	class ResourcesSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ResourcesSystem)

		template<typename T> T* Load(NxFr::StringView Path);
		NEXUS_ENGINE_API void Unload(NxFr::StringView Path);
		NEXUS_ENGINE_API void UnloadAll();
		template<typename T> T* Create(NxFr::StringView Path);
		NEXUS_ENGINE_API void Save(NxFr::StringView Path);
		NEXUS_ENGINE_API void SaveAll();
		NEXUS_ENGINE_API void Move(NxFr::StringView Path, NxFr::StringView Target);
		NEXUS_ENGINE_API void Delete(NxFr::StringView Path);

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API NxFr::Path GetResourceFilePath(NxFr::StringView Path);
		NEXUS_ENGINE_API Resource* GetResource(NxFr::StringView Path);

	private:
		NxFr::Dictionary<NxFr::String, Resource*> Resources;
	};

	template<typename T>
	inline T* ResourcesSystem::Create(NxFr::StringView Path)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance != nullptr)
		{
			NEXUS_LOG(Warning, Default, "Resources %s is already tracked", Path.C());
			return;
		}

		Instance = new T(Path, true);
		Resources.AppendConstruct(Path, Instance);

		return static_cast<T*>(Instance);
	}

	template<typename T>
	inline T* ResourcesSystem::Load(NxFr::StringView Path)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance == nullptr)
		{
			Instance = new T(Path, false);
			Resources.AppendConstruct(Path, Instance);
		}

		if (!Instance->IsLoaded())
		{
			Instance->Load(GetResourceFilePath(Path));
		}

		return static_cast<T*>(Instance);
	}
}
