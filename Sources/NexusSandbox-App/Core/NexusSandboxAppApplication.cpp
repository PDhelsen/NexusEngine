#include "NexusSandbox-App/Core/NexusSandboxAppApplication.h"
#include "NexusSandbox-App/Core/NexusSandboxAppSystems.h"

namespace NxSA
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxSA::NexusSandboxAppApplication)

	void NexusSandboxAppApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		NexusAppApplication::OnInitialize(Bootstrap);

		Bootstrap.AddSystem<SystemA>().AddDependency<SystemA, SystemB>().AddDependency<SystemA, SystemD>();
		Bootstrap.AddSystem<SystemB>().AddDependency<SystemB, SystemC>();
		Bootstrap.AddSystem<SystemC>();
		Bootstrap.AddSystem<SystemD>().AddDependency<SystemD, SystemE>();
		Bootstrap.AddSystem<SystemE>().AddDependency<SystemE, SystemB>().AddDependency<SystemE, SystemC>();
	}

	void NexusSandboxAppApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap)
	{
		NexusAppApplication::OnShutdown(Unbootstrap);

		Unbootstrap.AddSystem<SystemA>().AddDependency<SystemA, SystemB>().AddDependency<SystemA, SystemD>();
		Unbootstrap.AddSystem<SystemB>().AddDependency<SystemB, SystemC>();
		Unbootstrap.AddSystem<SystemC>();
		Unbootstrap.AddSystem<SystemD>().AddDependency<SystemD, SystemE>();
		Unbootstrap.AddSystem<SystemE>().AddDependency<SystemE, SystemB>().AddDependency<SystemE, SystemC>();
	}

	void NexusSandboxAppApplication::OnExecute(NxEn::Ticker& Ticks)
	{
		NexusAppApplication::OnExecute(Ticks);

		Ticks.AddSystem<SystemA>(NxEn::Ticker::Bucket::Project);
		Ticks.AddSystem<SystemB>(NxEn::Ticker::Bucket::Project);
		Ticks.AddSystem<SystemC>(NxEn::Ticker::Bucket::Engine).AddDependency<SystemC, SystemE>();
		Ticks.AddSystem<SystemD>(NxEn::Ticker::Bucket::Engine);
		Ticks.AddSystem<SystemE>(NxEn::Ticker::Bucket::Engine);
	}
}
