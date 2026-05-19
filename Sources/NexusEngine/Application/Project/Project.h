#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	enum class ProjectMode
	{
		App, Editor
	};
}

NX_ENUM_STRING(NxEn::ProjectMode, 2, "App", "Editor")

namespace NxEn
{
	class Project;

	namespace EntryPoint
	{
		NX_ENGINE_API Project CreateProject();
	}

	class Project
	{
	public:
		NX_ENGINE_API static NxFr::String GetSavedConfigPath(NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, NxFr::StringView Extension, NxFr::StringView SubFolder = "", bool Suffix = false);

		NX_ENGINE_API Project();
		NX_ENGINE_API Project(ProjectMode Mode);
		NX_ENGINE_API Project(NxFr::StringView Path);
		NX_ENGINE_API Project(ProjectMode Mode, NxFr::StringView Path);
		NX_ENGINE_API ~Project();

		NX_ENGINE_API ProjectMode GetTarget() const { return Mode; }
		NX_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NX_ENGINE_API NxFr::StringView GetName() const { return Name; }
		NX_ENGINE_API NxFr::StringView GetRootPath() const { return Root; }
		NX_ENGINE_API NxFr::StringView GetExecutablePath() const { return Executable; }
		NX_ENGINE_API NxFr::StringView GetDllPath() const { return Dll; }

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
