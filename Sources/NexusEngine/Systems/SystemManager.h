#pragma once

#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	struct SystemDependencies
	{
	public:
		NxFr::List<NxFr::StringId> Dependencies;
		NxFr::List<NxFr::StringId> Dependents;
		uint64 WaitOn = 0;

		NEXUS_ENGINE_API SystemDependencies() = default;
	};

	class SystemManager
	{
	public:
		NEXUS_ENGINE_API SystemManager();
		NEXUS_ENGINE_API ~SystemManager();

		template<typename T>
		T* GetSystem() const { return (T*)GetSystem(T::GetClassType()); }
		template<typename T>
		T* CreateSystem(bool Register = true) { T* System = new T(); if (Register) RegisterSystem(System); return System; };
		template<typename T>
		void DeleteSystem(T* Instance, bool Unregister = true) { if (Unregister) UnregisterSystem(Instance); delete Instance; }
		template<typename T>
		void OverrideSystem(bool Destroy = true) { T* System = CreateSystem<T>(false); System = PatchSystem(System); if (Destroy) DeleteSystem(false); return !Destroy ? System : nullptr; }

		NEXUS_ENGINE_API System* GetSystem(NxFr::StringId Type) const;
		NEXUS_ENGINE_API System* RegisterSystem(System* Instance);
		NEXUS_ENGINE_API System* UnregisterSystem(System* Instance);
		NEXUS_ENGINE_API System* PatchSystem(System* Instance);
		NEXUS_ENGINE_API void ClearSystems();
		NEXUS_ENGINE_API NxFr::Array<System*> SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const;

		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId>& GetOnSystemChanged() { return OnSystemChanged; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Systems.GetCount(); }

	private:
		NxFr::Event<NxFr::StringId> OnSystemChanged;
		NxFr::Dictionary<NxFr::StringId, System*> Systems;
	};
}
