#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		NX_ENGINE_API extern const NxFr::StringId System;
	}
}

namespace NxEn
{
	class NX_ENGINE_API SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		template<typename T>
		T* CreateSystem()
		{
			T* Instance = new T();
			Systems.Append(T::GetClassType(), Instance);
			return Instance;
		};
		template<typename T>
		void DestroySystem()
		{
			System* Instance = Systems[T::GetClassType()];
			Systems.Remove(Instance->GetObjectType());
			delete Instance;
		}
		template<typename T>
		T* GetSystem() const
		{
			return (T*)GetSystem(T::GetClassType());
		}
		System* GetSystem(NxFr::StringId Id) const
		{
			auto Instance = Systems.TryGet(Id);
			return Instance ? *Instance : nullptr;
		}

		NxFr::Array<System*> SortSystems(const NxFr::Dictionary<NxFr::StringId, NxFr::Set<NxFr::StringId>>& SystemsAndDependencies) const;

	private:
		struct DependencyInfo
		{
		public:
			const NxFr::Set<NxFr::StringId> Dependencies;
			NxFr::List<NxFr::StringId> Dependents;
			uint64 WaitOn = 0;
		};

		NxFr::Dictionary<NxFr::StringId, System*> Systems;
	};
}
