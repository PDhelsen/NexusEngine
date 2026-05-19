#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class System : public Object
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, System)

		NX_ENGINE_API System();
		NX_ENGINE_API virtual ~System();

	protected:
		NX_ENGINE_API virtual void OnInitialize() override;
		NX_ENGINE_API virtual void OnShutdown() override;
		NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
	};
}
