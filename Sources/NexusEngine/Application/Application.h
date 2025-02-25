#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Bootstrapper.h"
#include "NexusEngine/Application/System.h"

#define NEXUS_APPLICATION_DECLARATION(Dll, Name)\
namespace EntryPoint\
{\
	extern "C" Dll NxEn::Application* CreateApplication();\
	extern "C" Dll void DestroyApplication(NxEn::Application* Application);\
}
#define NEXUS_APPLICATION_IMPLEMENTATION(Name)\
namespace EntryPoint\
{\
	NxEn::Application* CreateApplication()\
	{\
		return new Name();\
	}\
	\
	void DestroyApplication(NxEn::Application* Application)\
	{\
		delete Application;\
	}\
}

namespace NxEn
{
	class Application
	{
	public:
		NEXUS_ENGINE_API static Application* GetInstance();

		NEXUS_ENGINE_API Application();
		NEXUS_ENGINE_API virtual ~Application();
		NEXUS_ENGINE_API void Execute();

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize(Bootstrapper& Bootstrap) = 0;
		NEXUS_ENGINE_API virtual void OnShutdown(Bootstrapper& Bootstrap) = 0;

	private:
		void Initialize();
		void Shutdown();
		void Run();

	private:
		Bootstrapper Bootstrap;
		NxFr::Array<System*> Systems;
	};
}

