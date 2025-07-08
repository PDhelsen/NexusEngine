#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/EntryPoint.h"
#include "NexusEngine/Application/Project.h"
#include "NexusEngine/Application/Bootstrapper.h"
#include "NexusEngine/Application/Ticker.h"
#include "NexusEngine/Application/TimeManager.h"

#include "NexusEngine/Systems/SystemManager.h"


#define NEXUS_APPLICATION_DECLARATION(Dll, Name)\
namespace EntryPoint\
{\
	extern "C" Dll NxEn::Application* CreateApplication(const NxEn::Project& ProjectInfo);\
	extern "C" Dll void DestroyApplication(NxEn::Application* Application);\
}
#define NEXUS_APPLICATION_IMPLEMENTATION(Name)\
namespace EntryPoint\
{\
	NxEn::Application* CreateApplication(const NxEn::Project& ProjectInfo)\
	{\
		return new Name(ProjectInfo);\
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

		NEXUS_ENGINE_API Application(const Project& ProjectInfo);
		NEXUS_ENGINE_API virtual ~Application();

		NEXUS_ENGINE_API void Quit();
		NEXUS_ENGINE_API void Restart();
		NEXUS_ENGINE_API void Crash(CrashCode ErrorCode);
		NEXUS_ENGINE_API bool IsRunning() const;

		Bootstrapper& GetBootstrapper() { return Bootstrap; }
		Ticker& GetTicker() { return Ticks; }
		SystemManager& GetSystems() { return Systems; }
		TimeManager& GetTime() { return Time; }

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize();
		NEXUS_ENGINE_API virtual void OnShutdown();
		NEXUS_ENGINE_API virtual void OnExecute();

	private:
		void Run();
		void Initialize();
		void Shutdown();
		void Execute();

	private:
		Project ProjectInfo;
		Bootstrapper Bootstrap;
		Ticker Ticks;
		SystemManager Systems;
		TimeManager Time;

		bool WantsToQuit;
	};
}

