#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Application/Systems/SystemManager.h"
#include "NexusEngine/Application/Flow/Bootstrapper.h"
#include "NexusEngine/Application/Flow/Ticker.h"
#include "NexusEngine/Application/Flow/TimeManager.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		NX_ENGINE_API extern const NxFr::StringId Application;
	}
}

#define NX_APPLICATION_DECLARATION(Dll, Name)\
namespace EntryPoint\
{\
	extern "C" Dll NxEn::Application* CreateApplication(const NxEn::Project& ProjectInfo);\
	extern "C" Dll void DestroyApplication(NxEn::Application* Application);\
}
#define NX_APPLICATION_IMPLEMENTATION(Name)\
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
	class NX_ENGINE_API Application
	{
	public:
		enum class CrashCode
		{
			None = 0, Crash = 1
		};

		template<typename T>
		static T* GetInstance() { return (T*)GetInstance(); }
		static Application* GetInstance() { return Instance; }
		template<typename T>
		static T* GetSystem() { return (T*)GetSystem(T::GetClassType()); }
		static System* GetSystem(NxFr::StringId Id) { return Instance->GetSystems().GetSystem(Id); }

		Application(const Project& ProjectInfo);
		virtual ~Application();

		void Initialize();
		void Shutdown();
		void Run();

		void Quit();
		void Crash(CrashCode ErrorCode);
		void Restart();
		bool IsRunning() const;

		Project& GetProject() { return ProjectInfo; }
		SystemManager& GetSystems() { return Systems; }
		Bootstrapper& GetBootstrapper() { return Bootstrap; }
		Ticker& GetTicker() { return Ticks; }
		TimeManager& GetTime() { return Time; }

	protected:
		virtual void OnInitialize();
		virtual void OnShutdown();
		virtual void OnRun();

	private:
		static Application* Instance;

		Project ProjectInfo;
		SystemManager Systems;
		Bootstrapper Bootstrap;
		Ticker Ticks;
		TimeManager Time;

		bool WantsToQuit;
		CrashCode CrashReason;
	};
}

