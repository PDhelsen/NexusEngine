#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/EntryPoint.h"
#include "NexusEngine/Application/Bootstrapper.h"
#include "NexusEngine/Application/Ticker.h"
#include "NexusEngine/Systems/SystemManager.h"

#include "NexusEngine/Misc/DebugManager.h"
#include "NexusEngine/Misc/TimeManager.h"

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

	public:
		NEXUS_ENGINE_API static Application* GetInstance();

		NEXUS_ENGINE_API Application();
		NEXUS_ENGINE_API virtual ~Application();

		NEXUS_ENGINE_API void Quit();
		NEXUS_ENGINE_API void Restart();
		NEXUS_ENGINE_API void Crash(CrashCode ErrorCode);
		NEXUS_ENGINE_API bool IsRunning() const;

		SystemManager& GetSystems() { return Systems; }
		Ticker& GetTicker() { return Ticks; }
		DebugManager& GetDebug() { return *Debug; }
		TimeManager& GetTime() { return *Time; }

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize(Bootstrapper& Bootstrap, SystemManager& Systems);
		NEXUS_ENGINE_API virtual void OnShutdown(Bootstrapper& Unbootstrap, SystemManager& Systems);
		NEXUS_ENGINE_API virtual void OnExecute(Ticker& Ticks, SystemManager& Systems);

	private:
		void Run();
		void Initialize();
		void Shutdown();
		void Execute();

	private:
		SystemManager Systems;
		Bootstrapper Bootstrap;
		Ticker Ticks;

		DebugManager* Debug;
		TimeManager* Time;

		bool WantsToQuit;
	};
}

