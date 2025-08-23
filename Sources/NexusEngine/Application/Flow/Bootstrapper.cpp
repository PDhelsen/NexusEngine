#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/Bootstrapper.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	Bootstrapper::Bootstrapper()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	Bootstrapper& Bootstrapper::AppendStep(StepBucket Bucket, NxFr::StringView Tag, const Signature& Step)
	{
		Steps.AppendConstruct(Bucket, Step, Tag);
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
		ExecuteSteps(true, StepBucket::BeforeSystem);
		ExecuteSystems(true);
		ExecuteSteps(true, StepBucket::AfterSystem);

		Steps.Clear();
		Systems.Clear();
	}

	void Bootstrapper::RunUnboot()
	{
		ExecuteSteps(false, StepBucket::BeforeSystem);
		ExecuteSystems(false);
		ExecuteSteps(false, StepBucket::AfterSystem);

		Steps.Clear();
		Systems.Clear();
	}

	void Bootstrapper::ExecuteSteps(bool Boot, StepBucket Bucket)
	{
		if (GetStepsCount() == 0)
		{
			NEXUS_LOG(Warning, Application, "Bootstrap - There is no steps to execute");
			return;
		}

		for (uint64 Index = 0; Index < Steps.GetCount(); ++Index)
		{
			StepInfo& Info = Steps[Index];
			if (Info.Bucket != Bucket)
			{
				continue;
			}

			NEXUS_LOG(Info, Application, "Bootstrap - Steps: %s", Info.Tag.C());
			Info.Callback.Invoke();
		}
	}

	void Bootstrapper::ExecuteSystems(bool Boot)
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Application, "Bootstrap - There is no systems to execute");
			return;
		}

		SystemManager& Manager = Application::GetInstance()->GetSystems();
		NxFr::Array<System*> Instances = Manager.SortSystems(Systems);
		for (uint64 Index = 0; Index < Instances.GetCount(); ++Index)
		{
			System* Instance = Instances[Index];
			NEXUS_LOG(Info, Application, "Bootstrap - Systems: %s", Instance->GetObjectType().C());
			if (Boot)
			{
				Instance->Initialize();
			}
			else
			{
				Instance->Shutdown();
			}
		}
	}
}
