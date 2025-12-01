#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsRegistry.h"

namespace NxEn
{
	AssetsRegistry::AssetsRegistry(NxFr::Path Root)
		: Root(Root), Assets(), Paths()
	{
		NxFr::List<NxFr::String> Files = NxFr::Directory(Root).GetFiles(true);
		for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
		{
			NxFr::StringView File = Files[Index];
			if (!NxFr::Path::HasExtension(File, AssetMetadata::Extension))
			{
				continue;
			}

			YAML::Node Node = NxFr::Yaml::DeserializeFile(File);

			AssetMetadata Metadata;
			Metadata.Deserialize(Node);

			Assets.Append(Metadata.GetId(), Metadata);
			Paths.Append(Metadata.GetPath().Data, Metadata.GetId());
		}
	}

	AssetsRegistry::~AssetsRegistry()
	{
	}

	void AssetsRegistry::Append(NxFr::GUID Id, const AssetMetadata& Metadata)
	{
		Assets.Append(Id, Metadata);

		if (Metadata.GetPath().IsValid())
		{
			NxFr::File(FilePath(Metadata.GetPath())).Create();
		}

		Paths.Append(AssetPath(Metadata.GetPath(), Id), Id);
	}

	void AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetMetadata& Metadata = Assets[Id];

		if (Metadata.GetPath().IsValid() && Metadata.GetPath().Exist())
		{
			NxFr::File(FilePath(Metadata.GetPath())).Move(FilePath(Path));
		}

		Paths.Remove(AssetPath(Metadata.GetPath(), Id));
		Paths.Append(AssetPath(Metadata.GetPath(), Id), Id);

		Metadata.Path = Path;
	}

	void AssetsRegistry::Remove(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		if (Metadata.GetPath().IsValid() && Metadata.GetPath().Exist())
		{
			NxFr::File(FilePath(Metadata.GetPath())).Delete();
		}

		Paths.Remove(AssetPath(Metadata.GetPath(), Id));
		Assets.Remove(Id);
	}

	AssetMetadata& AssetsRegistry::Get(NxFr::GUID Id)
	{
		return Assets[Id];
	}

	void AssetsRegistry::Serialize(NxFr::GUID Id, YAML::Node& Node)
	{
		AssetMetadata& Metadata = Assets[Id];
		Metadata.Serialize(Node);
		NxFr::Yaml::SerializeFile(Node, FilePath(Metadata.GetPath()));
	}

	void AssetsRegistry::Deserialize(NxFr::GUID Id, YAML::Node& Node)
	{
		AssetMetadata& Metadata = Assets[Id];
		Node = NxFr::Yaml::DeserializeFile(FilePath(Metadata.GetPath()));
		Metadata.Deserialize(Node);
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

		for (auto [Id, Metadata] : Assets)
		{
			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				// Filter by types
				if (Types[Index].GetId() != 0)
				{
					if (Metadata.GetType() == Types[Index])
					{
						Result.Append(Id);
					}
				}
				// Filter by string matching
				else
				{
					if (NxFr::StringUtility::Contains(Metadata.GetPath().Data, Filters[Index]))
					{
						Result.Append(Id);
					}
				}
			}
		}

		return Result;
	}

	NxFr::GUID AssetsRegistry::PathToId(NxFr::StringView Path) const
	{
		const NxFr::GUID* Id = Paths.TryGet(Path);
		return Id ? *Id : 0;
	}

	NxFr::String AssetsRegistry::IdToPath(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? Metadata->GetPath().Data : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToFile(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? FilePath(Metadata->GetPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToContent(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? ContentPath(Metadata->GetContent()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::AssetPath(NxFr::StringView Path, NxFr::GUID Id) const
	{
		return !Path.IsEmpty() ? NxFr::String(Path) : NxFr::StringUtility::ToString(Id);
	}

	NxFr::String AssetsRegistry::FilePath(NxFr::StringView Path) const
	{
		return NxFr::Path(Root + (Path + "." + AssetMetadata::Extension)).Data;
	}

	NxFr::String AssetsRegistry::ContentPath(NxFr::StringView Path) const
	{
		return NxFr::Path(Root + Path).Data;
	}
}
