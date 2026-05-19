#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/Project.h"

#define NX_PROJECT_DLL "NexusProject-"

#if NX_EDITOR
#define NX_SUFFIX "_editor"
#else
#define NX_SUFFIX "_app"
#endif

namespace NxEn
{
	namespace EntryPoint
	{
		Project NxEn::EntryPoint::CreateProject()
		{
			// Check cmd args
			NxFr::StringView ModeArg = NxFr::Globals::Args->Get("Mode");
			NxFr::StringView PathArg = NxFr::Globals::Args->Get("Project");

			// Convert arg
			ProjectMode Mode = NxFr::StringUtility::FromString<ProjectMode>(ModeArg.C());
			NxFr::String Path = NxFr::Path::Normalize(PathArg);

			// Lookup NexusProject as argument (without Project key)
			if (Path.IsEmpty())
			{
				if (NxFr::Globals::Args->Has(1))
				{
					PathArg = NxFr::Globals::Args->Get(1);
					if (NxFr::Path::GetExtension(PathArg) == "nexus")
					{
						Path = NxFr::Path::Normalize(PathArg);
					}
				}
			}

			// Lookup NexusProject in working dir
			if (Path.IsEmpty())
			{
				NxFr::String WorkingDir = NxFr::Globals::PlatformTarget->GetWorkingDirectory();
				NxFr::List<NxFr::String> Files = NxFr::Directory(WorkingDir).GetFiles();
				for (auto& File : Files)
				{
					if (NxFr::Path::GetExtension(File) == "nexus")
					{
						Path = Move(File);
						break;
					}
				}
			}

			// Check information
			bool HasPath = !Path.IsEmpty() && NxFr::Path::GetExtension(Path) == "nexus" && NxFr::Path::IsFile(Path) && NxFr::Path::Exist(Path);
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

	// TODO: Save user pref in AppData
	NxFr::String Project::GetSavedConfigPath(NxFr::StringView Config, NxFr::StringView Saved, NxFr::StringView Template, NxFr::StringView Extension, NxFr::StringView SubFolder, bool Suffix)
	{
		NxFr::String Path;
		if (!Config.IsEmpty())
		{
			Path = NxFr::Path::Combine(NxFr::Globals::Paths::Configs, SubFolder, Config + (Suffix ? NX_SUFFIX : "") + NxFr::Path::SeparatorExtension + Extension);
		}
		else
		{
			Path = NxFr::Path::Combine(NxFr::Globals::Paths::Saved, SubFolder, Saved + (Suffix ? NX_SUFFIX : "") + NxFr::Path::SeparatorExtension + Extension);

			if (!NxFr::Path::Exist(Path) && !Template.IsEmpty())
			{
				NxFr::String Target = NxFr::Path::Combine(NxFr::Globals::Paths::Configs, SubFolder, Template + (Suffix ? NX_SUFFIX : "") + NxFr::Path::SeparatorExtension + Extension);
				if (Extension.IsEmpty())
				{
					NxFr::Directory(Target).Copy(Path);
				}
				else
				{
					NxFr::File(Target).Copy(Path);
				}
			}
		}

		NxFr::Path::Normalize(Path);
		return Path;
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
		Root = !Path.IsEmpty() ? NxFr::String(NxFr::Path::GetDriveAndFolder(Path)) : NxFr::Globals::PlatformTarget->GetWorkingDirectory();
		Executable = NxFr::Path::Normalize(NxFr::Globals::Args->Get(0));
	}

	void Project::GenerateDefault()
	{
		Name = NxFr::Path::GetName(Executable);
		Dll = NxFr::Path::Combine(Root, ComputeDllName());
	}

	void Project::LoadFromFile(bool UseModeFromFile)
	{
		YAML::Node File = NxFr::Yaml::DeserializeFile(Path);

		Mode = UseModeFromFile ? NxFr::StringUtility::FromString<ProjectMode>(File["Mode"].as<NxFr::String>().C()) : Mode;

		Name = File["Name"].as<NxFr::String>();
		Dll = NxFr::Path::Combine(Root, File["Dll"].as<NxFr::String>(), ComputeDllName());
	}

	NxFr::String Project::ComputeDllName()
	{
		return NxFr::StringView(NX_PROJECT_DLL) + NxFr::StringUtility::ToString(Mode) + NxFr::StringView(".dll");
	}
}
