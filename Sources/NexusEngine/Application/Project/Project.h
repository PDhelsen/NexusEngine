#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class ProjectMode
	{
		App, Editor
	};
}

NEXUS_ENUM_STRING(NxEn::ProjectMode, 2, "App", "Editor")

namespace NxEn
{
	class Project;

	namespace EntryPoint
	{
		NEXUS_ENGINE_API Project CreateProject();
	}

	class Project
	{
	public:
		NEXUS_ENGINE_API static NxFr::String GetSavedConfigPath(NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, NxFr::StringView Extension, NxFr::StringView SubFolder = "", bool Suffix = false);

		NEXUS_ENGINE_API Project();
		NEXUS_ENGINE_API Project(ProjectMode Mode);
		NEXUS_ENGINE_API Project(NxFr::StringView Path);
		NEXUS_ENGINE_API Project(ProjectMode Mode, NxFr::StringView Path);
		NEXUS_ENGINE_API ~Project();

		NEXUS_ENGINE_API ProjectMode GetTarget() const { return Mode; }
		NEXUS_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NEXUS_ENGINE_API NxFr::StringView GetName() const { return Name; }
		NEXUS_ENGINE_API NxFr::StringView GetRootPath() const { return Root; }
		NEXUS_ENGINE_API NxFr::StringView GetExecutablePath() const { return Executable; }
		NEXUS_ENGINE_API NxFr::StringView GetDllPath() const { return Dll; }

	private:
		void Initialize();
		void GenerateDefault();
		void LoadFromFile(bool UseModeFromFile);

		NxFr::String ComputeDllName();

	private:
		ProjectMode Mode;
		NxFr::String Path;
		NxFr::String Root;
		NxFr::String Executable;

		NxFr::String Name;
		NxFr::String Dll;
	};
}
