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
	class NX_ENGINE_API Project
	{
	public:
		Project(NxFr::StringView Path);
		~Project();

		NxFr::String GetSavedConfigPath(NxFr::StringView Path, NxFr::StringView Template = "", bool Saved = false, bool Suffix = false);

		ProjectMode GetMode() const { return Mode; }
		NxFr::StringView GetProjectPath() const { return ProjectPath; }
		NxFr::StringView GetRootPath() const { return RootPath; }
		NxFr::StringView GetExecutablePath() const { return ExecutablePath; }
		NxFr::StringView GetName() const { return Name; }
		NxFr::StringView GetDllPath() const { return DllPath; }

	private:
		ProjectMode Mode;
		NxFr::String ProjectPath;
		NxFr::String RootPath;
		NxFr::String ExecutablePath;
		NxFr::String Name;
		NxFr::String DllPath;
	};
}
