#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"

namespace NxEd
{
	static NxEn::Command* CmdAssetImport = NxEn::Command::Create("Assets.Import"_Sid, "Import asset at path", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView ContentPath)
	{
		AssetImporter::Run(ContentPath);
	}));

	static NxFr::Dictionary<NxFr::StringId, AssetImporter*>& GetImporters()
	{
		static NxFr::Dictionary<NxFr::StringId, AssetImporter*> Importers;
		return Importers;
	}

	static NxFr::Dictionary<NxFr::String, NxFr::StringId>& GetExtensions()
	{
		static NxFr::Dictionary<NxFr::String, NxFr::StringId> Extensions;
		return Extensions;
	}

	AssetImporter* AssetImporter::GetImporter(NxFr::StringId Type)
	{
		AssetImporter** Importer = GetImporters().TryGet(Type);
		return Type && Importer ? *Importer : nullptr;
	}

	void AssetImporter::SetImporter(NxFr::StringId Type, AssetImporter* Instance)
	{
		GetImporters().AppendOrAssign(Type, Instance);
	}

	NxFr::StringId AssetImporter::GetType(NxFr::StringView Extension)
	{
		NxFr::StringId* Type = GetExtensions().TryGet(Extension);
		return !Extension.IsEmpty() && Type ? *Type : NxFr::StringId();
	}

	void AssetImporter::SetType(NxFr::StringView Extension, NxFr::StringId Type)
	{
		GetExtensions().AppendOrAssign(Extension, Type);
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

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(ContentPath);
		NxFr::StringView Extension = NxFr::Path::GetExtension(ContentPath);
		Id = Id != 0 ? Id : Assets->PathToId(Path);
		bool Exist = Assets->IsTracked(Id);
		Metadata = Exist ? &Assets->GetMetadata(Id) : nullptr;
		Type = Exist ? Metadata->GetType() : Type.GetId() != 0 ? Type : GetType(Extension);

		NX_ASSERT(!(!Exist && Type.GetId() == 0), Default, "AssetImporter needs a Type to import a file");
		NX_ASSERT(!(!Exist && (ContentPath.IsEmpty() || !NxFr::Path::HasExtension(ContentPath) || NxFr::Path::GetExtension(ContentPath) == NxEn::AssetMetadata::AssetExtension)), Default, "AssetImporter needs a file path with an extension different to the asset extension to import a file");
		NX_ASSERT(!(Exist && Id == 0), Default, "AssetImporter needs an Id to reimport an asset");

		AssetImporter* Importer = GetImporter(Type);
		YAML::Node Node = Exist ? Assets->GetImportData(Id) : YAML::Node();
		if (Importer)
		{
			Importer->OnImport(Node, ContentPath, Exist);
		}
		else
		{
			NX_LOG(Warning, System, "There is no importer linked to this type %s", Type.C());
		}

		Instance = Exist ? Assets->Reimport(Id, Node) : Assets->Import(Type, Node, Path, Extension);
		Assets->Save(Instance->GetId());

		return Instance;
	}
}
