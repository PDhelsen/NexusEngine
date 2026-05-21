#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const NxFr::StringId System = "System"_Sid;
	}
}

namespace NxEn
{
	SystemManager::SystemManager()
		: Systems()
	{
		NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::System, true);
	}

	SystemManager::~SystemManager()
	{
	}

	NxFr::Array<System*> SystemManager::SortSystems(const NxFr::Dictionary<NxFr::StringId, NxFr::Set<NxFr::StringId>>& SystemsAndDependencies) const
	{
		NxFr::Array<System*> Result = NxFr::Array<System*>(SystemsAndDependencies.GetCount());
		NxFr::Dictionary<NxFr::StringId, DependencyInfo> Infos = SystemsAndDependencies.GetCount();
		NxFr::Queue<NxFr::StringId> Queue;
		uint64 Index = 0;

		for (auto& [Type, Dependencies] : SystemsAndDependencies)
		{
			Infos.AppendConstruct(Type, Dependencies, NxFr::List<NxFr::StringId>(), Dependencies.GetCount());
		}

		for (auto& [Type, Info] : Infos)
		{
			for (auto& Dependency : Info.Dependencies)
			{
				Infos[Dependency].Dependents.Append(Type);
			}

			if (Info.WaitOn == 0)
			{
				Queue.Append(Type);
			}
		}

		NX_ASSERT(!Queue.IsEmpty(), Application, "Impossible to start sorting the systems because each one depened on another one, probably circular dependency.");

		while (!Queue.IsEmpty())
		{
			NxFr::StringId Type = Queue.Get();
			Queue.Remove();

			DependencyInfo& Info = Infos[Type];
			Result[Index++] = const_cast<System*>(Systems[Type]);

			for (auto& Dependent : Info.Dependents)
			{
				if (--Infos[Dependent].WaitOn == 0)
				{
					Queue.Append(Dependent);
				}
			}
		}

		NX_ASSERT(Index == Infos.GetCount(), Application, "Some Systems were not sorted, probaly unable to resolve all the dependencies");

		return Result;
	}
}
