#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project.h"

#define NEXUS_PROJECT_DLL "NexusProject-"

namespace NxEn
{
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
			NxFr::StringView ModeArg = NxFr::Arguments::GetValue("Target").C();
			NxFr::StringView PathArg = NxFr::Arguments::GetValue("Project").C();

			// Convert arg
			ProjectMode Mode = ModeFromString(ModeArg);
			NxFr::String Path = NxFr::Path::Normalize(PathArg);

			// Lookup NexusProject
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
		NxFr::Dictionary<NxFr::String, NxFr::String> File = ReadFile();

		Mode = UseModeFromFile ? ModeFromString(File["Mode"]) : Mode;

		Name = File["Name"];
		Dll = Root + File["Dll"] + ComputeDllName();
	}

	NxFr::Dictionary<NxFr::String, NxFr::String> Project::ReadFile()
	{
		NxFr::File File(Path);
		File.Open(NxFr::File::Mode::Read);

		NxFr::String Text = File.ReadText();
		NxFr::List<NxFr::StringView> Lines = Text.SplitAll("\n");

		NxFr::Dictionary<NxFr::String, NxFr::String> Data(Lines.GetCount());
		for (auto& Line : Lines)
		{
			NxFr::List<NxFr::StringView> KeyValue = Line.SplitAll("=");
			Data.AppendConstruct(KeyValue[0].ToString(), KeyValue[1].ToString());
		}

		File.Close();
		return Data;
	}

	NxFr::String Project::ComputeDllName()
	{
		return NxFr::StringView(NEXUS_PROJECT_DLL) + NxEn::Enum::ProjectModeToString((uint64)Mode) + NxFr::StringView(".dll");
	}
}


