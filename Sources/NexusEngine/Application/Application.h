#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/EntryPoint.h"
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
	enum class CrashCode : int8
	{
		None = 0, Crash = 1
	};

	class Application
	{
		friend int EntryPoint::Main(int argc, char* argv[]);
		friend class System;

	public:
		NEXUS_ENGINE_API static Application* GetInstance();

		NEXUS_ENGINE_API Application();
		NEXUS_ENGINE_API virtual ~Application();

		NEXUS_ENGINE_API void Quit();
		NEXUS_ENGINE_API void Restart();
		NEXUS_ENGINE_API void Crash(CrashCode ErrorCode);
		NEXUS_ENGINE_API bool IsRunning();

		template<typename T>
		T* GetSystem() const { return Systems[T::GetType()]; }

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize(Bootstrapper& Bootstrap) = 0;
		NEXUS_ENGINE_API virtual void OnShutdown(Bootstrapper& Bootstrap) = 0;

	private:
		void Execute();
		void Initialize();
		void Shutdown();
		void Run();

		void RegisterSystem(NxFr::StringId Type, System* System);
		void UnregisterSystem(NxFr::StringId Type);

	private:
		NxFr::Dictionary<NxFr::StringId, System*> Systems;
		Bootstrapper Bootstrap;

		bool WantsToQuit;
	};
}

