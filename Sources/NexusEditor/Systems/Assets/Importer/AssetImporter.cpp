#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"

namespace NxEd
{
	static NxEn::Command* CmdAssetImportPath = NxEn::Command::Create("Assets.Importer.Path"_Sid, "Import asset at path", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView ContentPath)
	{
		AssetImporter::Run(ContentPath);
	}));
	static NxEn::Command* CmdAssetImportId = NxEn::Command::Create("Assets.Importer.Id"_Sid, "Reimport asset with id", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		AssetImporter::Run(NxFr::StringUtility::FromString<NxFr::GUID>(Id));
	}));

	NxFr::Registry<AssetImporter*>& AssetImporter::GetImporters()
	{
		static NxFr::Registry<AssetImporter*> Importers;
		return Importers;
	}

	NxFr::Registry<NxFr::StringId>& AssetImporter::GetTypes()
	{
		static NxFr::Registry<NxFr::StringId> Types;
		return Types;
	}

	NxEn::Asset* AssetImporter::Run(NxFr::StringView ContentPath, NxFr::StringId Type)
	{
		return Run(Type, ContentPath, 0);
	}

	NxEn::Asset* AssetImporter::Run(NxFr::GUID Id)
	{
		return Run(0, "", Id);
	}

	NxEn::Asset* NxEd::AssetImporter::Run(NxFr::StringId Type, NxFr::StringView ContentPath, NxFr::GUID Id)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::AssetMetadata* Metadata = nullptr;
		NxEn::Asset* Instance = nullptr;
		AssetImporter* Importer = nullptr;

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(ContentPath);
		NxFr::StringView Extension = NxFr::Path::GetExtension(ContentPath);
		Id = Id != 0 ? Id : Assets->PathToId(Path);
		bool Exist = Assets->IsTracked(Id);
		Metadata = Exist ? Assets->GetMetadata(Id) : nullptr;
		Type = Exist ? Metadata->GetType() : Type.GetId() != 0 ? Type : GetTypes().TryGet(Extension) != nullptr ? *GetTypes().TryGet(Extension) : NxFr::StringUtility::Id;

		NX_ASSERT_RETURN(Type != NxFr::StringUtility::Id, nullptr, Default, "AssetImporter needs a Type to import a file");
		NX_ASSERT_RETURN(Exist || (!Path.IsEmpty() && !Extension.IsEmpty() && Extension != NxEn::AssetMetadata::AssetExtension), nullptr, Default, "AssetImporter needs a file path with an extension different from the asset extension to import a file");
		NX_ASSERT_RETURN(!Exist || Id != 0, nullptr, Default, "AssetImporter needs an Id to reimport an asset");

		NxFr::Yaml::Node AssetData = Exist ? Assets->GetAssetdata(Id) : NxFr::Yaml::Node();
		Importer = GetImporters().TryGet(Type);
		if (Importer)
		{
			Importer->OnImport(AssetData, ContentPath, Exist);
		}

		Instance = Exist ? Assets->Reimport(Id, AssetData) : Assets->Import(Type, AssetData, Path, Extension);
		return Instance;
	}
}
