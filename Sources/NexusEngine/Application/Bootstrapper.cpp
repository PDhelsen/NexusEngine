#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Bootstrapper.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	Bootstrapper::Bootstrapper()
		: Logger(true, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE)
	{
		Logger.AddChannel(NxFr::LoggerChannel::Default, true);
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	Bootstrapper& Bootstrapper::AppendStep(const Signature& Step, NxFr::StringView Tag)
	{
		Steps.AppendConstruct(Step, Tag);
		return *this;
	}

	Bootstrapper& Bootstrapper::AppendSystem(System* Target)
	{
		Systems.Append(Target->GetObjectType(), SystemDependencies());
		return *this;
	}

	Bootstrapper& Bootstrapper::AppendDependency(NxFr::StringId Target, NxFr::StringId Dependency)
	{
		Systems[Target].Dependencies.Append(Dependency);
		return *this;
	}

	void Bootstrapper::RunBoot(const SystemManager& Manager)
	{
		NxFr::Globals::Logs = &Logger;

		ExecuteSteps();
		ExecuteSystemsInitialize(Manager);

		if (NxFr::Globals::Logs == &Logger) NxFr::Globals::Logs = nullptr;
	}

	void Bootstrapper::RunUnboot(const SystemManager& Manager)
	{
		NxFr::Globals::Logs = &Logger;

		ExecuteSystemsShutdown(Manager);
		ExecuteSteps();

		if (NxFr::Globals::Logs == &Logger) NxFr::Globals::Logs = nullptr;
	}

	void Bootstrapper::ExecuteSteps()
	{
		if (GetStepsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no boot steps");
			return;
		}

		for (auto It = Steps.Begin(); It != Steps.End(); ++It)
		{
			NEXUS_LOG(Info, Default, "Boot - Steps (%i / %i): %s", It.Id() + 1, Steps.GetCount(), It.Get().GetSecond().C());
			It.Get().GetFirst().Invoke();
		}

		Steps.Clear();
	}

	void Bootstrapper::ExecuteSystemsInitialize(const SystemManager& Manager)
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no systems to initialize");
			return;
		}

		NxFr::Array<System*> Instances = Manager.SortSystems(Systems);
		for (auto It = Instances.Begin(); It != Instances.End(); ++It)
		{
			NEXUS_LOG(Info, Default, "Initializing - Systems (%i / %i): %s", It.Id() + 1, Instances.GetCount(), It.Get()->GetObjectType().C());
			It.Get()->Initialize();
		}

		Systems.Clear();
	}

	void Bootstrapper::ExecuteSystemsShutdown(const SystemManager& Manager)
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no systems to shutdown");
			return;
		}

		NxFr::Array<System*> Instances = Manager.SortSystems(Systems);
		for (auto It = Instances.Begin(); It != Instances.End(); ++It)
		{
			NEXUS_LOG(Info, Default, "Shutdowning - Systems (%i / %i): %s", Instances.GetCount() - It.Id(), Instances.GetCount(), It.Get()->GetObjectType().C());
			It.Get()->Shutdown();
		}

		Systems.Clear();
	}
}
