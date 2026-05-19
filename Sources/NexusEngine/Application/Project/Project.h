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
		Project CreateProject();
	}

	class NX_ENGINE_API Project
	{
	public:
		static NxFr::String GetSavedConfigPath(NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, NxFr::StringView Extension, NxFr::StringView SubFolder = "", bool Suffix = false);

		Project();
		Project(ProjectMode Mode);
		Project(NxFr::StringView Path);
		Project(ProjectMode Mode, NxFr::StringView Path);
		~Project();

		ProjectMode GetTarget() const { return Mode; }
		NxFr::StringView GetPath() const { return Path; }
		NxFr::StringView GetName() const { return Name; }
		NxFr::StringView GetRootPath() const { return Root; }
		NxFr::StringView GetExecutablePath() const { return Executable; }
		NxFr::StringView GetDllPath() const { return Dll; }

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
