#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	class NX_ENGINE_API System : public Object
	{
	public:
		NX_OBJECT(System)

		NX_NOCOPY_NOMOVE(System);
		System();
		virtual ~System();

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnTick(float TimeStep = 0.0f) override;
	};
}
