#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsRegistry.h"
#include "NexusEngine/Systems/Assets/AssetSerializer.h"

namespace NxEn
{
	AssetsRegistry::AssetsRegistry(NxFr::Path Root)
		: Root(Root), Assets(), Paths()
	{
		NxFr::List<NxFr::String> Files = NxFr::Directory(Root).GetFiles(true);
		for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
		{
			NxFr::StringView File = Files[Index];
			if (!NxFr::Path::HasExtension(File, AssetMetadata::AssetExtension))
			{
				continue;
			}

			AssetMetadata Metadata;
			Metadata.Deserialize(AssetSerializer::DeserializeMetadata(File));

			Assets.Append(Metadata.GetId(), Metadata);
			Paths.Append(Metadata.GetPath(), Metadata.GetId());
		}
	}

	AssetsRegistry::~AssetsRegistry()
	{
	}

	void AssetsRegistry::Append(NxFr::GUID Id, const AssetMetadata& Metadata)
	{
		Assets.Append(Id, Metadata);

		if (!Metadata.GetPath().IsEmpty())
		{
			Paths.Append(Metadata.GetPath(), Id);
			NxFr::File(PathToFile(Metadata.GetAssetPath())).Create();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFile(Metadata.GetContentPath())).Create();
			}
		}
	}

	void AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetMetadata& Metadata = Assets[Id];

		NxFr::String Before = PathToFile(Metadata.GetAssetPath());
		NxFr::String After = PathToFile(Path + "." + AssetMetadata::AssetExtension);
		NxFr::File(After).EnsureParent();
		NxFr::File(Before).Move(After);

		if (!Metadata.GetExtension().IsEmpty())
		{
			Before = PathToFile(Metadata.GetContentPath());
			After = PathToFile(Path + "." + Metadata.GetExtension());
			NxFr::File(Before).Move(After);
		}

		Paths.Remove(Metadata.GetPath());
		Metadata.Path = Path;
		Paths.Append(Metadata.GetPath(), Id);
	}

	void AssetsRegistry::Copy(NxFr::GUID Id, const AssetMetadata& Metadata)
	{
		AssetMetadata& Reference = Assets[Id];

		NxFr::String Before = PathToFile(Reference.GetAssetPath());
		NxFr::String After = PathToFile(Metadata.GetAssetPath());
		NxFr::File(Before).Copy(After);
		if (!Reference.GetExtension().IsEmpty())
		{
			Before = PathToFile(Reference.GetContentPath());
			After = PathToFile(Metadata.GetContentPath());
			NxFr::File(Before).Copy(After);
		}

		Assets.Append(Metadata.Id, Metadata);
		Paths.Append(Metadata.GetPath(), Metadata.Id);
	}

	void AssetsRegistry::Remove(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		if (!Metadata.GetPath().IsEmpty())
		{
			Paths.Remove(Metadata.GetPath());
			NxFr::File(PathToFile(Metadata.GetAssetPath())).Delete();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFile(Metadata.GetContentPath())).Delete();
			}
		}

		Assets.Remove(Id);
	}

	AssetMetadata& AssetsRegistry::Get(NxFr::GUID Id)
	{
		return Assets[Id];
	}

	void AssetsRegistry::Serialize(NxFr::GUID Id, const YAML::Node& Node)
	{
		AssetMetadata& Metadata = Assets[Id];
		YAML::Node Meta = Metadata.Serialize();
		AssetSerializer::Serialize(PathToFile(Metadata.GetAssetPath()), Meta, Node);
	}

	void AssetsRegistry::SerializeMetadata(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		YAML::Node Meta = Metadata.Serialize();
		AssetSerializer::SerializeMetadata(PathToFile(Metadata.GetAssetPath()), Meta);
	}

	YAML::Node AssetsRegistry::Deserialize(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		YAML::Node Meta, Data;
		AssetSerializer::Deserialize(PathToFile(Metadata.GetAssetPath()), Meta, Data);

		Metadata.Deserialize(Meta);
		return Data;
	}

	YAML::Node AssetsRegistry::DeserializeData(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		return AssetSerializer::DeserializeData(PathToFile(Metadata.GetAssetPath()));
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
				NxFr::StringView Substring = Metadata.GetPath();
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
		return Metadata ? NxFr::String(Metadata->GetPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToFileAsset(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFile(Metadata->GetAssetPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToFileContent(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFile(Metadata->GetContentPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::PathToFile(NxFr::StringView Path) const
	{
		return NxFr::Path::Combine(Root, Path);
	}
}
