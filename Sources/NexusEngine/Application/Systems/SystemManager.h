#pragma once

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
	struct SystemDependencies
	{
	public:
		NxFr::List<NxFr::StringId> Dependencies;
		NxFr::List<NxFr::StringId> Dependents;
		uint64 WaitOn = 0;

		SystemDependencies() = default;
	};

	class NX_ENGINE_API SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		template<typename T>
		T* GetSystem() const { return (T*)GetSystem(T::GetClassType()); }
		template<typename T>
		T* CreateSystem() { T* Instance = new T(); RegisterSystem(Instance); return Instance; };
		template<typename T>
		void DestroySystem() { System* Instance = Systems[T::GetClassType()]; UnregisterSystem(Instance); delete Instance; }

		System* GetSystem(NxFr::StringId Type) const;
		void RegisterSystem(System* Instance);
		void UnregisterSystem(System* Instance);
		NxFr::Array<System*> SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const;

		uint64 GetCount() const { return Systems.GetCount(); }

	private:
		NxFr::Dictionary<NxFr::StringId, System*> Systems;
	};
}
