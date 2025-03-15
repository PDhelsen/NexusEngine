#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
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
		T* RegisterSystem(T* Target) { return (T*)RegisterSystem(Target); }
		template<typename T>
		void UnregisterSystem(T* Target) { return (T*)UnregisterSystem(Target); }
		template<typename T>
		void DeleteSystem(T* Target, bool Unregister = true) { if (Unregister) UnregisterSystem(Target); delete Target; }
		template<typename T>
		T* PatchSystem(T* Target, bool Destroy = true) { return (T*)PatchSystem(Target, Destroy); }

		NEXUS_ENGINE_API System* GetSystem(NxFr::StringId Type) const;
		NEXUS_ENGINE_API System* RegisterSystem(System* Target);
		NEXUS_ENGINE_API System* UnregisterSystem(System* Target);
		NEXUS_ENGINE_API System* PatchSystem(System* Target, bool Destroy = true);
		NEXUS_ENGINE_API void ClearSystems();

		NEXUS_ENGINE_API NxFr::Array<System*> SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const;

		uint64 GetCount() const { return Systems.GetCount(); }

	private:
		NxFr::Dictionary<NxFr::StringId, System*> Systems;
	};
}
