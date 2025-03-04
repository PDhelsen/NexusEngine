#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class System : public Object
	{
		friend class Application;
		friend class Bootstrapper;

	public:
		NEXUS_OBJECT_DECLARATION(System)

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize() override;
		NEXUS_ENGINE_API virtual void OnShutdown() override;
	};
}
