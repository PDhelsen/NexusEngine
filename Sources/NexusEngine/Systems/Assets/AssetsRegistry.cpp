#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsRegistry.h"

#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	static const NxFr::String KeyMetadata = "Metadata";
	static const NxFr::String KeyAssetdata = "Assetdata";

	AssetsRegistry::AssetsRegistry(NxFr::String Root)
		: Root(Root), Assets(), Paths()
	{
		NxFr::List<NxFr::String> Files = NxFr::Directory(Root).GetFiles(true);
		for (uint64 Index = 0; Index < Files.GetCount(); ++Index)
		{
			NxFr::StringView File = Files[Index];
			if (NxFr::Path::GetExtension(File) != AssetMetadata::AssetExtension)
			{
				continue;
			}

			AssetMetadata Metadata;
			Metadata.Deserialize(Read(File, KeyMetadata));

			Assets.Append(Metadata.GetId(), NxFr::Move(Metadata));
			Paths.Append(Metadata.GetPath(), Metadata.GetId());
		}
	}

	AssetsRegistry::~AssetsRegistry()
	{
	}

	AssetMetadata& AssetsRegistry::Append(NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension)
	{
		NxFr::GUID Id = NxFr::Integer::GenerateGuid();
		AssetMetadata& Metadata = Assets.AppendConstruct(Id);
		Metadata.Id = Id;
		Metadata.Type = Type;
		Metadata.Name = NxFr::Path::GetName(Path);
		Metadata.Path = Path;
		Metadata.Extension = Extension;

		if (!Metadata.GetPath().IsEmpty())
		{
			Paths.Append(Metadata.GetPath(), Id);
			NxFr::File(PathToFsPath(Metadata.GetAssetPath())).Create();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFsPath(Metadata.GetContentPath())).Create();
			}
		}

		return Metadata;
	}

	AssetMetadata& AssetsRegistry::Move(NxFr::GUID Id, NxFr::StringView Path)
	{
		AssetMetadata& Metadata = Assets[Id];

		NxFr::String Before = PathToFsPath(Metadata.GetAssetPath());
		NxFr::String After = PathToFsPath(AssetMetadata::ComputeAssetPath(Path));
		NxFr::File(Before).Move(After);

		if (!Metadata.GetExtension().IsEmpty())
		{
			Before = PathToFsPath(Metadata.GetContentPath());
			After = PathToFsPath(AssetMetadata::ComputeContentPath(Path, Metadata.GetExtension()));
			NxFr::File(Before).Move(After);
		}

		Paths.Remove(Metadata.GetPath());
		Metadata.Path = Path;
		Paths.Append(Metadata.GetPath(), Id);

		return Metadata;
	}

	AssetMetadata& AssetsRegistry::Copy(NxFr::GUID Id, NxFr::StringView Path)
	{
		NxFr::GUID CopyId = NxFr::Integer::GenerateGuid();
		AssetMetadata& Reference = Assets[Id];
		AssetMetadata& Metadata = Assets.Append(CopyId, Reference);
		Metadata.Id = CopyId;
		Metadata.Path = Path;
		Metadata.Data.Clear();

		NxFr::String Before = PathToFsPath(Reference.GetAssetPath());
		NxFr::String After = PathToFsPath(Metadata.GetAssetPath());
		NxFr::File(Before).Copy(After);

		if (!Reference.GetExtension().IsEmpty())
		{
			Before = PathToFsPath(Reference.GetContentPath());
			After = PathToFsPath(Metadata.GetContentPath());
			NxFr::File(Before).Copy(After);
		}

		Paths.Append(Metadata.GetPath(), CopyId);
		return Metadata;
	}

	void AssetsRegistry::Remove(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];

		if (!Metadata.GetPath().IsEmpty())
		{
			Paths.Remove(Metadata.GetPath());
			NxFr::File(PathToFsPath(Metadata.GetAssetPath())).Delete();
			if (!Metadata.GetExtension().IsEmpty())
			{
				NxFr::File(PathToFsPath(Metadata.GetContentPath())).Delete();
			}
		}

		Assets.Remove(Id);
	}

	AssetMetadata& AssetsRegistry::Get(NxFr::GUID Id)
	{
		return Assets[Id];
	}

	void AssetsRegistry::SerializeAndSave(NxFr::GUID Id, const YAML::Node& Assetdata)
	{
		AssetMetadata& Metadata = Assets[Id];
		Write(PathToFsPath(Metadata.GetAssetPath()), Metadata.Serialize(), Assetdata);
	}

	YAML::Node AssetsRegistry::DeserializeAndLoad(NxFr::GUID Id)
	{
		AssetMetadata& Metadata = Assets[Id];
		return Read(PathToFsPath(Metadata.GetAssetPath()), KeyAssetdata);
	}

	NxFr::Array<NxFr::GUID> AssetsRegistry::Find(NxFr::StringView Query) const
	{
		NxFr::List<NxFr::GUID> Result;

		Utils::Filter Filter(Query);
		for (auto& [Id, Metadata] : Assets)
		{
			if (Filter.FilterInstance(Metadata.GetPath(), Metadata.GetId(), Metadata.GetType()))
			{
				Result.Append(Id);
			}
		}

		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Result);
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

	NxFr::String AssetsRegistry::IdToAssetFsPath(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFsPath(Metadata->GetAssetPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::IdToContentFsPath(NxFr::GUID Id) const
	{
		const AssetMetadata* Metadata = Assets.TryGet(Id);
		return Metadata ? PathToFsPath(Metadata->GetContentPath()) : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsRegistry::PathToFsPath(NxFr::StringView Path) const
	{
		return NxFr::Path::Combine(Root, Path);
	}

	void AssetsRegistry::Write(NxFr::StringView Path, const YAML::Node& Metadata, const YAML::Node& Assetdata)
	{
		YAML::Node Root;
		Root[KeyMetadata] = Metadata;
		Root[KeyAssetdata] = !Assetdata.IsNull() ? Assetdata : Read(Path, KeyAssetdata);
		NxFr::Yaml::SerializeFile(Root, Path);
	}

	YAML::Node AssetsRegistry::Read(NxFr::StringView Path, NxFr::StringView Key)
	{
		YAML::Node Root = NxFr::Yaml::DeserializeFile(Path);
		return Root[Key];
	}
}
