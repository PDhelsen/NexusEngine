#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsRegistry.h"
#include "NexusEngine/Systems/Assets/AssetSerializer.h"

#include "NexusEngine/Misc/Utils/Filter.h"

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
			NxFr::File(PathToFilePath(Metadata.GetAssetPath())).Create();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFilePath(Metadata.GetContentPath())).Create();
			}
		}
	}

	void AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetMetadata& Metadata = Assets[Id];

		NxFr::String Before = PathToFilePath(Metadata.GetAssetPath());
		NxFr::String After = PathToFilePath(AssetMetadata::ComputeAssetPath(Path));
		NxFr::File(After).EnsureParent();
		NxFr::File(Before).Move(After);

		if (!Metadata.GetExtension().IsEmpty())
		{
			Before = PathToFilePath(Metadata.GetContentPath());
			After = PathToFilePath(AssetMetadata::ComputeContentPath(Path, Metadata.GetExtension()));
			NxFr::File(Before).Move(After);
		}

		Paths.Remove(Metadata.GetPath());
		Metadata.Path = Path;
		Paths.Append(Metadata.GetPath(), Id);
	}

	void AssetsRegistry::Copy(NxFr::GUID Id, const AssetMetadata& Metadata)
	{
		AssetMetadata& Reference = Assets[Id];

		NxFr::String Before = PathToFilePath(Reference.GetAssetPath());
		NxFr::String After = PathToFilePath(Metadata.GetAssetPath());
		NxFr::File(Before).Copy(After);
		if (!Reference.GetExtension().IsEmpty())
		{
			Before = PathToFilePath(Reference.GetContentPath());
			After = PathToFilePath(Metadata.GetContentPath());
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
			NxFr::File(PathToFilePath(Metadata.GetAssetPath())).Delete();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFilePath(Metadata.GetContentPath())).Delete();
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
		AssetSerializer::Serialize(PathToFilePath(Metadata.GetAssetPath()), Meta, Node);
	}

	void AssetsRegistry::SerializeMetadata(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		YAML::Node Meta = Metadata.Serialize();
		AssetSerializer::SerializeMetadata(PathToFilePath(Metadata.GetAssetPath()), Meta);
	}

	YAML::Node AssetsRegistry::Deserialize(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		YAML::Node Meta, Data;
		AssetSerializer::Deserialize(PathToFilePath(Metadata.GetAssetPath()), Meta, Data);

		Metadata.Deserialize(Meta);
		return Data;
	}

	YAML::Node AssetsRegistry::DeserializeData(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		return AssetSerializer::DeserializeData(PathToFilePath(Metadata.GetAssetPath()));
	}

	NxFr::Array<NxFr::GUID> AssetsRegistry::Find(NxFr::StringView Query) const
	{
		NxFr::List<NxFr::GUID> Result;

		Utils::Filter Filter(Query);
		for (auto& [Id, Metadata] : Assets)
		{
			if (Filter.FilterObject(Metadata.GetPath(), Metadata.GetId(), Metadata.GetType()))
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

	NxFr::String AssetsRegistry::IdToAssetFilePath(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFilePath(Metadata->GetAssetPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToContentFilePath(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFilePath(Metadata->GetContentPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::PathToFilePath(NxFr::StringView Path) const
	{
		return NxFr::Path::Combine(Root, Path);
	}
}
