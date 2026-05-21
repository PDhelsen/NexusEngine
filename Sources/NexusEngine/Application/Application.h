#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Application/Flow/EntryPoint.h"
#include "NexusEngine/Application/Flow/Bootstrapper.h"
#include "NexusEngine/Application/Flow/Ticker.h"
#include "NexusEngine/Application/Flow/TimeManager.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

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
		NxFr::Allocator::Scope Allocator(NxEn::MemorySystem::GetAllocator(NxEn::AllocatorType::General));\
		return new Name(ProjectInfo);\
	}\
	\
	void DestroyApplication(NxEn::Application* Application)\
	{\
		NxFr::Allocator::Scope Allocator(NxEn::MemorySystem::GetAllocator(NxEn::AllocatorType::General));\
		delete Application;\
	}\
}

namespace NxEn
{
	enum class CrashCode : int8
	{
		None = 0, Crash = 1
	};

	class NX_ENGINE_API Application
	{
		friend int EntryPoint::Main(int argc, char* argv[]);

	public:
		template<typename T>
		static T* GetInstance() { return (T*)GetInstance(); }
		template<typename T>
		static T* GetSystem() { return (T*)GetSystem(T::GetClassType()); }

		static Application* GetInstance();
		static System* GetSystem(NxFr::StringId Id);

		Application(const Project& ProjectInfo);
		virtual ~Application();

		void Quit();
		void Restart();
		void Crash(CrashCode ErrorCode);
		bool IsRunning() const;

		Project& GetProject() { return ProjectInfo; }
		Bootstrapper& GetBootstrapper() { return Bootstrap; }
		Ticker& GetTicker() { return Ticks; }
		SystemManager& GetSystems() { return Systems; }
		TimeManager& GetTime() { return Time; }

	protected:
		virtual void OnInitialize();
		virtual void OnShutdown();
		virtual void OnExecute();

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

