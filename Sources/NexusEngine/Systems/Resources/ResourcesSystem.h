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

		template<typename T>
		T* Load(NxFr::StringView Path);
		void Unload(NxFr::StringView Path);

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API NxFr::Path GetResourceFilePath(NxFr::StringView Path);
		NEXUS_ENGINE_API Resource* GetResource(NxFr::StringView Path);

	private:
		NxFr::Dictionary<NxFr::String, Resource*> Resources;
	};

	template<typename T>
	inline T* ResourcesSystem::Load(NxFr::StringView Path)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance == nullptr)
		{
			Instance = new T(Path);
			Resources.AppendConstruct(Path, Instance);
		}

		if (!Instance->IsLoaded())
		{
			Instance->Load(GetResourceFilePath(Path));
		}

		return static_cast<T*>(Instance);
	}
}
