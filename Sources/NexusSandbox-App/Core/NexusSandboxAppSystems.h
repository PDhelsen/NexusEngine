#pragma once

#include "NexusSandbox-App/Core/NexusSandboxAppCore.h"
#include "NexusEngine/Systems/System.h"

namespace NxSA
{
	class SystemA : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_SANDBOX_APP_API, SystemA)
	};

	class SystemB : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_SANDBOX_APP_API, SystemB)
	};

	class SystemC : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_SANDBOX_APP_API, SystemC)
	};

	class SystemD : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_SANDBOX_APP_API, SystemD)
	};

	class SystemE : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_SANDBOX_APP_API, SystemE)
	};
}
