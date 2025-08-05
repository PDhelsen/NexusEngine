#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

#define NEXUS_PROJECT_DLL "NexusProject-"

namespace NxEn
{
	const static Command CmdProjectPanel = Command::Create("Project.Panel"_Sid, "Show/Hide project panel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		Application::GetInstance()->GetProject().GetPanel(Enabled == "true");
	}));

	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(ProjectMode, 2, "App", "Editor")

	static ProjectMode ModeFromString(NxFr::StringView Mode)
	{
		return Mode == NxEn::Enum::ProjectModeToString((uint64)ProjectMode::Editor) ? ProjectMode::Editor : ProjectMode::App;
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

	ProjectPanel* Project::GetPanel(bool Enable)
	{
		Panel->SetEnabled(Enable);
		return Panel;
	}

	void Project::Initialize()
	{
		Root = !Path.IsEmpty() ? NxFr::Path::GetDirectoryPath(Path).ToString() : NxFr::Platform::GetInstance()->GetWorkingDirectory();
		Executable = NxFr::Path::Normalize(NxFr::Arguments::GetValue(NxFr::Arguments::KeyProgram));

		Panel = NxEn::Object::Create<ProjectPanel>(false);
		Panel->SetProject(this);
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
		return NxFr::StringView(NEXUS_PROJECT_DLL) + NxEn::Enum::ProjectModeToString((uint64)Mode) + NxFr::StringView(".dll");
	}
}
