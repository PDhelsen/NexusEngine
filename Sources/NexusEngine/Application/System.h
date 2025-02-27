#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NEXUS_SYSTEM_DECLARATION(Name) static NxFr::StringView GetSystemName() { return #Name; }

namespace NxEn
{
	class System
	{
		friend class Application;
		friend class Bootstrapper;

	public:
		NEXUS_SYSTEM_DECLARATION(System)

		NEXUS_ENGINE_API System();
		NEXUS_ENGINE_API virtual ~System();

		bool IsEnabled() const { return Enabled; };
		void SetEnabled(bool enabled) { Enabled = enabled; if (Enabled) OnEnable(); else OnDisable(); };

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize();
		NEXUS_ENGINE_API virtual void OnShutdowm();
		NEXUS_ENGINE_API virtual void OnTick();
		NEXUS_ENGINE_API virtual void OnEnable();
		NEXUS_ENGINE_API virtual void OnDisable();

	private:
		void Initialize();
		void Shutdown();
		void Tick();

		bool Enabled;
	};
}
