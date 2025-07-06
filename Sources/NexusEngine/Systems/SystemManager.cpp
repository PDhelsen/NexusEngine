#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/SystemManager.h"

namespace NxEn
{
	SystemManager::SystemManager()
	{
	}

	SystemManager::~SystemManager()
	{
	}

	System* SystemManager::GetSystem(NxFr::StringId Type) const
	{
		return Systems[Type];
	}

	System* SystemManager::RegisterSystem(System* Target)
	{
		Systems.Append(Target->GetObjectType(), { Target });
		return Target;
	}

	System* SystemManager::UnregisterSystem(System* Target)
	{
		Systems.Remove(Target->GetObjectType());
		return Target;
	}

	System* SystemManager::PatchSystem(System* Target)
	{
		System* Instance = Systems[Target->GetObjectType()];
		Systems[Target->GetObjectType()] = Target;
		return Instance;
	}

	void SystemManager::ClearSystems()
	{
		for (auto& Info : Systems)
		{
			delete Info.Value;
		}

		Systems.Clear();
	}

	NxFr::Array<System*> SystemManager::SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const
	{
		NxFr::Array<System*> Result = NxFr::Array<System*>(SystemsDependencies.GetCount());
		NxFr::Queue<NxFr::StringId> Queue;
		uint64 Index = 0;

		for (auto& [Type, Dependencies] : SystemsDependencies)
		{
			for (auto& Dependency : Dependencies.Dependencies)
			{
				SystemsDependencies[Dependency].Dependents.Append(Type);
			}
			
			Dependencies.WaitOn = Dependencies.Dependencies.GetCount();
			if (Dependencies.WaitOn == 0)
			{
				Queue.Append(Type);
			}
		}

		NEXUS_ASSERT(!Queue.IsEmpty(), Default, "Impossible to start sorting the systems because each one depened on another one, probably circular dependency.");

		while (!Queue.IsEmpty())
		{
			NxFr::StringId Type = Queue.Get();
			Queue.Remove();
			
			SystemDependencies& Dependencies = SystemsDependencies[Type];
			Result[Index++] = const_cast<System*>(Systems[Type]);
			
			for (auto& Dependent : Dependencies.Dependents)
			{
				if (--SystemsDependencies[Dependent].WaitOn == 0)
				{
					Queue.Append(Dependent);
				}
			}
			
			Dependencies.Dependencies.Clear();
			Dependencies.Dependents.Clear();
		}
		
		NEXUS_ASSERT(Index == SystemsDependencies.GetCount(), Default, "Some Systems were not sorted, probaly unable to resolve all the dependencies");

		return Result;
	}
}
