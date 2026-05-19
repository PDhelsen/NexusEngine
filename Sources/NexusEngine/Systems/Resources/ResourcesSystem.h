#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NX_ENGINE_API extern const NxFr::StringId ResourcesTrackedId;
		NX_ENGINE_API extern const NxFr::StringId ResourcesLoadedId;
	}
}

namespace NxEn
{
	class ResourcesSystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, ResourcesSystem)

		template<typename T> T* Load(NxFr::StringView Path);
		NX_ENGINE_API void Unload(NxFr::StringView Path);
		NX_ENGINE_API void UnloadAll();
		template<typename T> T* Create(NxFr::StringView Path);
		NX_ENGINE_API void Save(NxFr::StringView Path);
		NX_ENGINE_API void SaveAll();
		NX_ENGINE_API void Move(NxFr::StringView Path, NxFr::StringView Target);
		NX_ENGINE_API void Delete(NxFr::StringView Path);

	private:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NX_ENGINE_API NxFr::String GetResourceFilePath(NxFr::StringView Path);
		NX_ENGINE_API Resource* GetResource(NxFr::StringView Path);

	private:
		NxFr::Dictionary<NxFr::String, Resource*> Resources;
	};

	template<typename T>
	inline T* ResourcesSystem::Create(NxFr::StringView Path)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance != nullptr)
		{
			NX_LOG(Warning, Default, "Resources %s is already tracked", Path.C());
			return nullptr;
		}

		Instance = new T(Path);
		Resources.AppendConstruct(Path, Instance);

		return static_cast<T*>(Instance);
	}

	template<typename T>
	inline T* ResourcesSystem::Load(NxFr::StringView Path)
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Resource* Instance = GetResource(Path);

		if (Instance == nullptr)
		{
			Instance = Create<T>(Path);
			Instance->Loaded = false;
		}

		if (!Instance->IsLoaded())
		{
			Instance->Load(GetResourceFilePath(Path));
		}

		return static_cast<T*>(Instance);
	}
}
