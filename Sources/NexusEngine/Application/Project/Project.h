#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Project;

	namespace EntryPoint
	{
		NEXUS_ENGINE_API Project CreateProject();
	}

	enum class ProjectMode
	{
		App, Editor
	};
	NEXUS_ENUM_STRING(ProjectMode, 2, "App", "Editor")

	class Project
	{
	public:
		NEXUS_ENGINE_API static NxFr::Path GetSavedConfigPath(NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, NxFr::StringView Extension, NxFr::StringView SubFolder = "", bool Suffix = false);

		NEXUS_ENGINE_API Project();
		NEXUS_ENGINE_API Project(ProjectMode Mode);
		NEXUS_ENGINE_API Project(NxFr::StringView Path);
		NEXUS_ENGINE_API Project(ProjectMode Mode, NxFr::StringView Path);
		NEXUS_ENGINE_API ~Project();

		NEXUS_ENGINE_API ProjectMode GetTarget() const { return Mode; }
		NEXUS_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NEXUS_ENGINE_API const NxFr::String& GetName() const { return Name; }
		NEXUS_ENGINE_API const NxFr::String& GetRootPath() const { return Root; }
		NEXUS_ENGINE_API const NxFr::String& GetExecutablePath() const { return Executable; }
		NEXUS_ENGINE_API const NxFr::String& GetDllPath() const { return Dll; }

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
