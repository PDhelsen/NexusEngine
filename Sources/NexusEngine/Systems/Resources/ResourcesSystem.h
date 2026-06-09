#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API ResourcesSystem : public System
	{
	public:
		NX_OBJECT(ResourcesSystem)

		template<typename T> T* Create(NxFr::StringView Path);
		void Move(NxFr::StringView Path, NxFr::StringView Target);
		void Delete(NxFr::StringView Path);

		template<typename T> T* Load(NxFr::StringView Path);
		void Unload(NxFr::StringView Path);
		void Save(NxFr::StringView Path);

		void UnloadAll();
		void SaveAll();

	protected:
		void OnInitialize() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		Resource* GetResource(NxFr::StringView Path);
		NxFr::String GetResourceFsPath(NxFr::StringView Path);

		NxFr::Dictionary<NxFr::String, Resource*> Resources;
	};

	template<typename T>
	inline T* ResourcesSystem::Create(NxFr::StringView Path)
	{
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
		Resource* Instance = GetResource(Path);
		if (Instance != nullptr)
		{
			return static_cast<T*>(Instance);
		}

		Instance = new T(Path);
		Instance->Load(GetResourceFsPath(Path));
		Resources.AppendConstruct(Path, Instance);

		return static_cast<T*>(Instance);
	}
}
