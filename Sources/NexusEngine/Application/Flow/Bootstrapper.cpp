#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/Bootstrapper.h"

namespace NxEn
{
	Bootstrapper::Bootstrapper()
		: Steps(), Systems()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	void Bootstrapper::RunBoot()
	{
		ExecuteSteps(BootBucket::BeforeSystem);
		ExecuteSystems([](System* Instance)
		{
			Instance->Initialize();
			Instance->SetEnabled(true);
		});
		ExecuteSteps(BootBucket::AfterSystem);

		Steps.Clear();
		Systems.Clear();
	}

	void Bootstrapper::RunUnboot()
	{
		ExecuteSteps(BootBucket::BeforeSystem);
		ExecuteSystems([](System* Instance)
		{
			Instance->SetEnabled(false);
			Instance->Shutdown();
		});
		ExecuteSteps(BootBucket::AfterSystem);

		Steps.Clear();
		Systems.Clear();
	}

	Bootstrapper& Bootstrapper::AppendStep(BootBucket Bucket, NxFr::StringView Tag, const Signature& Step)
	{
		Steps.AppendConstruct(Bucket, Tag, Step);
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

	void Bootstrapper::ExecuteSteps(BootBucket Bucket)
	{
		if (Steps.IsEmpty())
		{
			NX_LOG(Warning, Application, "Bootstrap - There is no steps to execute");
			return;
		}

		for (uint64 Index = 0; Index < Steps.GetCount(); ++Index)
		{
			StepInfo& Info = Steps[Index];
			if (Info.Bucket != Bucket)
			{
				continue;
			}

			NX_LOG(Info, Application, "Bootstrap - Steps: %s", Info.Tag.C());
			Info.Callback.Invoke();
		}
	}

	void Bootstrapper::ExecuteSystems(const NxFr::Delegate<void(System*)>& Callback)
	{
		if (Systems.IsEmpty())
		{
			NX_LOG(Warning, Application, "Bootstrap - There is no systems to execute");
			return;
		}

		SystemManager& Manager = Application::GetInstance()->GetSystems();
		NxFr::Array<System*> Instances = Manager.SortSystems(Systems);

		for (uint64 Index = 0; Index < Instances.GetCount(); ++Index)
		{
			System* Instance = Instances[Index];
			NX_LOG(Info, Application, "Bootstrap - Systems: %s", Instance->GetObjectType().C());
			Callback(Instance);
		}
	}
}
