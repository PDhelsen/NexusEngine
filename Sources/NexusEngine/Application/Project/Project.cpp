#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/Project.h"

namespace NxEn
{
	Project::Project(NxFr::StringView Path)
	{
		NX_ASSERT(!Path.IsEmpty() && NxFr::Path::GetExtension(Path) == "nexus" && NxFr::Path::IsFile(Path) && NxFr::Path::Exist(Path), Default, "Project needs a valid path (.nexus) to initialize");

		ProjectPath = Path;
		RootPath = NxFr::Path::GetDriveAndFolder(Path);
		ExecutablePath = NxFr::Path::Normalize(NxFr::Globals::Args->Get(0));

		YAML::Node File = NxFr::Yaml::DeserializeFile(Path);

		Mode = NxFr::StringUtility::FromString<ProjectMode>(NxFr::Globals::Args->Get("Mode", File["Mode"].as<NxFr::String>()));
		Name = File["Name"].as<NxFr::String>();
		DllPath = NxFr::Path::Combine(RootPath, File["Dll"].as<NxFr::String>(), "NexusProject-" + NxFr::StringUtility::ToString(Mode) + ".dll");
	}

	Project::~Project()
	{
	}

	// TODO: Save user pref in AppData
	NxFr::String Project::GetSavedConfigPath(NxFr::StringView Path, NxFr::StringView Template, bool Saved, bool Suffix)
	{
		NxFr::String TargetPath = Path;

		if (Suffix)
		{
			NxFr::String ModeSuffix = "_" + NxFr::StringUtility::Lower(NxFr::StringUtility::ToString(Mode));
			TargetPath = NxFr::Path::ChangeName(Path, NxFr::Path::GetName(Path) + ModeSuffix);
		}

		TargetPath = NxFr::Path::Combine(Saved ? NxFr::Globals::Paths::Saved : NxFr::Globals::Paths::Configs, TargetPath);
		NxFr::Path::Normalize(TargetPath);
		
		if (!NxFr::Path::Exist(TargetPath) && !Template.IsEmpty())
		{
			NxFr::String TemplatePath = NxFr::Path::Normalize(NxFr::Path::Combine(NxFr::Globals::Paths::Configs, Template));
			if (NxFr::Path::IsDirectory(TemplatePath))
			{
				NxFr::Directory(TemplatePath).Copy(TargetPath);
			}
			else
			{
				NxFr::File(TemplatePath).Copy(TargetPath);
			}
		}

		return TargetPath;
	}
}
