#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/Bootstrapper.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	Bootstrapper::Bootstrapper()
		: Logger(true, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE)
	{
		Logger.AddChannel(NxFr::LoggerChannel::Default, true);
		Logger.AddChannel(NxFr::LoggerChannel::Verbose, false);
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	Bootstrapper& Bootstrapper::AppendStep(const Signature& Step, NxFr::StringView Tag)
	{
		Steps.AppendConstruct(Step, Tag);
		return *this;
	}

	Bootstrapper& Bootstrapper::AppendSystem(NxFr::StringId Type)
	{
		Systems.Append(Type, SystemDependencies());
		return *this;
	}

	Bootstrapper& Bootstrapper::AppendDependency(NxFr::StringId Type, NxFr::StringId Dependency)
	{
		Systems[Type].Dependencies.Append(Dependency);
		return *this;
	}

	void Bootstrapper::RunBoot()
	{
		NxFr::Globals::Logs = &Logger;

		ExecuteSteps(true);
		ExecuteSystems(true);

		if (NxFr::Globals::Logs == &Logger) NxFr::Globals::Logs = nullptr;
	}

	void Bootstrapper::RunUnboot()
	{
		NxFr::Globals::Logs = &Logger;

		ExecuteSystems(false);
		ExecuteSteps(false);

		if (NxFr::Globals::Logs == &Logger) NxFr::Globals::Logs = nullptr;
	}

	void Bootstrapper::ExecuteSteps(bool Boot)
	{
		if (GetStepsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "Bootstrap - There is no steps to execute");
			return;
		}

		for (auto It = Steps.Begin(); It != Steps.End(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrap - Steps (%i / %i): %s", Boot ? It.Id() + 1 : Steps.GetCount() - It.Id(), Steps.GetCount(), It.Get().GetSecond().C());
			It.Get().GetFirst().Invoke();
		}

		Steps.Clear();
	}

	void Bootstrapper::ExecuteSystems(bool Boot)
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "Bootstrap - There is no systems to execute");
			return;
		}

		SystemManager& Manager = Application::GetInstance()->GetSystems();
		NxFr::Array<System*> Instances = Manager.SortSystems(Systems);
		for (auto It = Instances.Begin(); It != Instances.End(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrap - Systems (%i / %i): %s", Boot ? It.Id() + 1 : Instances.GetCount() - It.Id(), Instances.GetCount(), It.Get()->GetObjectType().C());
			if (Boot)
			{
				It.Get()->Initialize();
			}
			else
			{
				It.Get()->Shutdown();
			}
		}

		Systems.Clear();
	}
}
