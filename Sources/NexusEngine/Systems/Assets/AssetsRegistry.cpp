#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsRegistry.h"

namespace NxEn
{
	const NxFr::String Extension = "asset";

	AssetsRegistry::Info::Info(AssetMetadata Metadata)
		: Type(Metadata.Type), Path(Metadata.Path)
	{
	}

	AssetsRegistry::Info::Info(Asset* Instance, NxFr::StringView Path)
		: Type(Instance->GetObjectType()), Path(Path)
	{
	}

	AssetsRegistry::AssetsRegistry(NxFr::Path Root)
		: Root(Root), Assets(), Paths()
	{
		NxFr::List<NxFr::String> Files = NxFr::Directory(Root).GetFiles(true);
		for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
		{
			NxFr::StringView File = Files[Index];
			if (!NxFr::Path::HasExtension(File, Extension))
			{
				continue;
			}

			YAML::Node Node = NxFr::Yaml::DeserializeFile(File);
			AssetMetadata Metadata = Node["Metadata"].as<AssetMetadata>();

			Assets.Append(Metadata.Id, Metadata);
			Paths.Append(Metadata.Path, Metadata.Id);
		}
	}

	AssetsRegistry::~AssetsRegistry()
	{
	}

	void AssetsRegistry::Append(NxFr::GUID Id, const Info& Instance)
	{
		Assets.Append(Id, Instance);

		if (Instance.Path.IsValid())
		{
			NxFr::File File = NxFr::File(FilePath(Instance.Path));
			File.Create();
		}

		Paths.Append(AssetPath(Instance.Path, Id), Id);
	}

	void AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		Info& Instance = Assets[Id];

		if (Instance.Path.IsValid() && Instance.Path.Exist())
		{
			NxFr::File File = NxFr::File(FilePath(Instance.Path));
			File.Move(FilePath(Path));
		}

		Paths.Remove(AssetPath(Instance.Path, Id));
		Paths.Append(AssetPath(Path, Id), Id);

		Instance.Path = Path;
	}

	void AssetsRegistry::Remove(NxFr::GUID Id)
	{
		Info& Instance = Assets[Id];

		Paths.Remove(AssetPath(Instance.Path, Id));
		Assets.Remove(Id);
	}

	void AssetsRegistry::Serialize(NxFr::GUID Id, YAML::Node& Node) const
	{
		const Info& Instance = Assets[Id];
		NxFr::Yaml::SerializeFile(Node, FilePath(Instance.Path));
	}

	void AssetsRegistry::Deserialize(NxFr::GUID Id, YAML::Node& Node) const
	{
		const Info& Instance = Assets[Id];
		Node = NxFr::Yaml::DeserializeFile(FilePath(Instance.Path));
	}

	NxFr::List<NxFr::GUID> AssetsRegistry::Find(NxFr::StringView Filter) const
	{
		NxFr::List<NxFr::GUID> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		NxFr::Array<NxFr::StringId> Types = Filters.GetCount();

		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			if (!NxFr::StringUtility::Start(Filters[Index], "t:"))
			{
				Types[Index] = 0;
				continue;
			}

			Types[Index] = Filters[Index].Substring(2, Filters[Index].GetCount() - 2);
		}

		for (auto [Id, Instance] : Assets)
		{
			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				// Filter by types
				if (Types[Index].GetId() != 0)
				{
					if (Instance.Type == Types[Index])
					{
						Result.Append(Id);
					}
				}
				// Filter by string matching
				else
				{
					if (NxFr::StringUtility::Contains(Instance.Path.Data, Filters[Index]))
					{
						Result.Append(Id);
					}
				}
			}
		}

		return Result;
	}

	NxFr::String AssetsRegistry::IdToPath(NxFr::GUID Id) const
	{
		const Info* Instance = Assets.TryGet(Id);
		return Instance ? Instance->Path.C() : NxFr::StringUtility::Empty.C();
	}

	NxFr::GUID AssetsRegistry::PathToId(NxFr::StringView Path) const
	{
		const NxFr::GUID* Id = Paths.TryGet(Path);
		return Id ? *Id : 0;
	}

	NxFr::Path AssetsRegistry::FilePath(NxFr::StringView Path) const
	{
		return Root + (Path + "." + Extension);
	}

	NxFr::String AssetsRegistry::AssetPath(NxFr::StringView Path, NxFr::GUID Id) const
	{
		return !Path.IsEmpty() ? Path.C() : NxFr::StringUtility::ToString(Id);
	}
}
