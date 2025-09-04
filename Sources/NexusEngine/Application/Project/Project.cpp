#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/Project.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

#define NEXUS_PROJECT_DLL "NexusProject-"

#if NEXUS_EDITOR
#define NEXUS_SUFFIX "_editor"
#else
#define NEXUS_SUFFIX "_app"
#endif

namespace NxEn
{
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(ProjectMode, 2, "App", "Editor")

	static ProjectMode ModeFromString(NxFr::StringView Mode)
	{
		return Mode == NxEn::Enum::ToString(ProjectMode::Editor) ? ProjectMode::Editor : ProjectMode::App;
	}

	namespace EntryPoint
	{
		Project NxEn::EntryPoint::CreateProject()
		{
			// Check cmd args
			NxFr::StringView ModeArg = NxFr::Arguments::GetValue("Target");
			NxFr::StringView PathArg = NxFr::Arguments::GetValue("Project");

			// Convert arg
			ProjectMode Mode = ModeFromString(ModeArg);
			NxFr::String Path = NxFr::Path::Normalize(PathArg);

			// Lookup NexusProject as argument (without Project key)
			if (Path.IsEmpty())
			{
				if (NxFr::Arguments::GetCount() >= 2)
				{
					PathArg = NxFr::Arguments::GetValue(1);
					if (NxFr::Path::HasExtension(PathArg, "nexus"))
					{
						Path = NxFr::Path::Normalize(PathArg);
					}
				}
			}

			// Lookup NexusProject in working dir
			if (Path.IsEmpty())
			{
				NxFr::String WorkingDir = NxFr::Platform::GetInstance()->GetWorkingDirectory();
				NxFr::List<NxFr::String> Files = NxFr::Directory(WorkingDir).GetFiles();
				for (auto& File : Files)
				{
					if (NxFr::Path::HasExtension(File, "nexus"))
					{
						Path = Move(File);
						break;
					}
				}
			}

			// Check information
			bool HasPath = !Path.IsEmpty() && NxFr::Path::HasExtension(Path, "nexus") && NxFr::Path::IsFile(Path) && NxFr::Path::Exist(Path);
			bool HasMode = !ModeArg.IsEmpty();

			// Create Project
			if (HasPath && HasMode)
			{
				return Project(Mode, Path);
			}
			else if (HasPath && !HasMode)
			{
				return Project(Path);
			}
			else if (!HasPath && HasMode)
			{
				return Project(Mode);
			}
			else //if (!HasPath && !HasMode)
			{
				return Project();
			}
		}
	}

	NxFr::Path Project::GetSavedConfigPath(NxFr::StringView Folder, NxFr::StringView Extension, NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, bool Suffix)
	{
		if (!Config.IsEmpty())
		{
			return NxFr::Paths::Configs + Folder + (Config + (Suffix ? NEXUS_SUFFIX : "") + Extension);
		}
		else
		{
			NxFr::Path Path = NxFr::Paths::Saved + Folder + (Saved + (Suffix ? NEXUS_SUFFIX : "") + Extension);

			if (!Path.Exist() && !Template.IsEmpty())
			{
				NxFr::Path Target = NxFr::Paths::Configs + Folder + (Template + (Suffix ? NEXUS_SUFFIX : "") + Extension);
				NxFr::File(Target).Copy(Path);
			}

			return Path;
		}
	}

	Project::Project()
		: Mode(ProjectMode::App), Path()
	{
		Initialize();
		GenerateDefault();
	}

	Project::Project(ProjectMode Mode)
		: Mode(Mode), Path()
	{
		Initialize();
		GenerateDefault();
	}

	Project::Project(NxFr::StringView Path)
		: Mode(ProjectMode::App), Path(Path.C())
	{
		Initialize();
		LoadFromFile(true);
	}

	Project::Project(ProjectMode Mode, NxFr::StringView Path)
		: Mode(Mode), Path(Path.C())
	{
		Initialize();
		LoadFromFile(false);
	}

	Project::~Project()
	{
	}

	void Project::Initialize()
	{
		Root = !Path.IsEmpty() ? NxFr::Path::GetDirectoryPath(Path).ToString() : NxFr::Platform::GetInstance()->GetWorkingDirectory();
		Executable = NxFr::Path::Normalize(NxFr::Arguments::GetValue(NxFr::Arguments::KeyProgram));
	}

	void Project::GenerateDefault()
	{
		Name = NxFr::Path::GetFileName(Executable).ToString();
		Dll = Root + ComputeDllName();
	}

	void Project::LoadFromFile(bool UseModeFromFile)
	{
		YAML::Node File = NxFr::Yaml::DeserializeFile(Path);

		Mode = UseModeFromFile ? ModeFromString(File["Mode"].as<NxFr::String>()) : Mode;

		Name = File["Name"].as<NxFr::String>();
		Dll = Root + File["Dll"].as<NxFr::String>() + ComputeDllName();
	}

	NxFr::String Project::ComputeDllName()
	{
		return NxFr::StringView(NEXUS_PROJECT_DLL) + NxEn::Enum::ToString(Mode) + NxFr::StringView(".dll");
	}
}
