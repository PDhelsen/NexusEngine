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

		NX_ENGINE_API SystemDependencies() = default;
	};

	class SystemManager
	{
	public:
		NX_ENGINE_API SystemManager();
		NX_ENGINE_API ~SystemManager();

		template<typename T>
		T* GetSystem() const { return (T*)GetSystem(T::GetClassType()); }
		template<typename T>
		T* CreateSystem() { T* Instance = new T(); RegisterSystem(Instance); return Instance; };
		template<typename T>
		void DestroySystem() { System* Instance = Systems[T::GetClassType()]; UnregisterSystem(Instance); delete Instance; }

		NX_ENGINE_API System* GetSystem(NxFr::StringId Type) const;
		NX_ENGINE_API void RegisterSystem(System* Instance);
		NX_ENGINE_API void UnregisterSystem(System* Instance);
		NX_ENGINE_API NxFr::Array<System*> SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const;

		NX_ENGINE_API uint64 GetCount() const { return Systems.GetCount(); }

	private:
		NxFr::Dictionary<NxFr::StringId, System*> Systems;
	};
}
