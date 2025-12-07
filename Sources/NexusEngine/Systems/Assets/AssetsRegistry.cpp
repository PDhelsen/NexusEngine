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
			Paths.Append(Metadata.GetPath().Data, Id);
			NxFr::File(FilePath(Metadata.GetPath())).Create();
		}
	}

	void AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetMetadata& Metadata = Assets[Id];

		NxFr::File(FilePath(Metadata.GetPath())).Move(FilePath(Path));
		if (Metadata.GetContent().IsValid())
		{
			NxFr::File(ContentPath(Metadata.GetPath())).Move(ContentPath(Path));
		}

		Paths.Remove(Metadata.GetPath().Data);
		Paths.Append(Metadata.GetPath().Data, Id);

		Metadata.Path = Path;
	}

	void AssetsRegistry::Remove(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		if (Metadata.GetPath().IsValid() && Metadata.GetPath().Exist())
		{
			Paths.Remove(Metadata.GetPath().Data);
			NxFr::File(FilePath(Metadata.GetPath())).Delete();
		}

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

	YAML::Node AssetsRegistry::GetImportData(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		YAML::Node Node = NxFr::Yaml::DeserializeFile(FilePath(Metadata.GetPath()));
		return Node["Data"];
	}

	NxFr::Array<NxFr::GUID> AssetsRegistry::Find(NxFr::StringView Filter) const
	{
		NxFr::List<NxFr::GUID> Result;

		NxFr::List<NxFr::StringView> Filters = NxFr::StringUtility::SplitAll(Filter, " ");
		NxFr::Array<NxFr::StringId> Types = Filters.GetCount();
		NxFr::Array<NxFr::GUID> Ids = Filters.GetCount();
		NxFr::Array<bool> Path = Filters.GetCount();
		bool All = Filter == "*";
		bool TypeAndString = false;

		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			Types[Index] = 0;
			Ids[Index] = 0;
			Path[Index] = false;

			if (NxFr::StringUtility::Start(Filters[Index], "t:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(2, Filters[Index].GetCount() - 2);
				Types[Index] = Substring;
				TypeAndString = true;
			}
			else if (NxFr::StringUtility::Start(Filters[Index], "id:"))
			{
				NxFr::StringView Substring = Filters[Index].Substring(3, Filters[Index].GetCount() - 3);
				Ids[Index] = NxFr::StringUtility::FromString<NxFr::GUID>(Substring);
			}
			else
			{
				Path[Index] = NxFr::StringUtility::Contains(Filters[Index], NxFr::Path::SeparatorDirectory);
			}
		}

		// Check if should filter by type and string;
		TypeAndString &= Filters.GetCount() > 1;

		for (auto [Id, Metadata] : Assets)
		{
			bool MatchId = false;
			bool MatchType = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				NxFr::StringView Substring = Metadata.GetPath().Data;
				Substring = Path[Index] ? Substring : NxFr::Path::Split(Substring).Last();

				MatchId |= Ids[Index] != 0 && Metadata.GetId() == Ids[Index];
				MatchType |= Types[Index].GetId() != 0 && Metadata.GetType() == Types[Index];
				MatchString |= NxFr::StringUtility::Contains(Substring, Filters[Index]);
			}

			if (All || MatchId || (MatchType && !TypeAndString) || (MatchString && !TypeAndString) || (MatchType && MatchString && TypeAndString))
			{
				Result.Append(Id);
			}
		}

		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Result);
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

	NxFr::String AssetsRegistry::FilePath(NxFr::StringView Path) const
	{
		return NxFr::Path(Root + (Path + "." + AssetMetadata::Extension)).Data;
	}

	NxFr::String AssetsRegistry::ContentPath(NxFr::StringView Path) const
	{
		return NxFr::Path(Root + Path).Data;
	}
}
