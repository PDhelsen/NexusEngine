#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
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

	const static NxEn::Command CmdAssetImport = NxEn::Command::Create("Assets.Import"_Sid, "Import asset at path", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView File)
	{
		AssetImporter::Run(Id, File);
	}));

#pragma region Getter / Setter

	AssetImporter* AssetImporter::GetImporter(NxFr::StringId Type)
	{
		return GetImporters()[Type];
	}

	void AssetImporter::SetImporter(AssetImporter* Instance)
	{
		GetImporters().AppendOrAssign(Instance->Type, Instance);
	}

	NxFr::StringId AssetImporter::GetType(NxFr::StringView Extension)
	{
		return GetExtensions()[Extension];
	}

	void AssetImporter::SetType(NxFr::StringView Extension, NxFr::StringId Type)
	{
		GetExtensions().AppendOrAssign(Extension, Type);
	}

	AssetImporter* AssetImporter::TryGetImporter(NxFr::StringId Type)
	{
		AssetImporter** Importer = GetImporters().TryGet(Type);
		return Importer ? *Importer : nullptr;
	}

	AssetImporter* AssetImporter::TryGetImporter(NxFr::GUID Asset)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		if (!System->IsTracked(Asset))
		{
			return nullptr;
		}

		NxEn::AssetMetadata& Metadata = System->GetMetadata(Asset);
		return TryGetImporter(Metadata.GetType());
	}

	NxFr::StringId AssetImporter::TryGetType(NxFr::StringView File)
	{
		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(File);
		NxFr::StringView Extension = NxFr::Path::GetExtension(File);

		if (Extension == NxEn::AssetMetadata::AssetExtension)
		{
			NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
			return TryGetType(System->PathToId(Path));
		}
		else
		{
			NxFr::StringId* Type = GetExtensions().TryGet(Extension);
			return Type ? *Type : NxFr::StringId();
		}
	}

	NxFr::StringId AssetImporter::TryGetType(NxFr::GUID Asset)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		if (!System->IsTracked(Asset))
		{
			return NxFr::StringId();
		}

		NxEn::AssetMetadata& Metadata = System->GetMetadata(Asset);
		return Metadata.GetType();
	}

#pragma endregion

#pragma region API

	NxEn::Asset* AssetImporter::Run(NxFr::StringView File, NxFr::StringId Type, bool Release)
	{
		if (Type.GetId() == 0)
		{
			Type = AssetImporter::TryGetType(File);
		}

		AssetImporter* Importer = TryGetImporter(Type);
		return AssetImporter::Run(Importer, File, Type, 0, Release);
	}

	NxEn::Asset* AssetImporter::Run(NxFr::GUID Asset, bool Release)
	{
		AssetImporter* Importer = TryGetImporter(Asset);
		return AssetImporter::Run(Importer, "", 0, Asset, Release);
	}

#pragma endregion

#pragma region Pipeline

	NxEn::Asset* AssetImporter::Run(AssetImporter* Importer, NxFr::StringView File, NxFr::StringId Type, NxFr::GUID Asset, bool Release)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::Asset* Instance = nullptr;

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(File);
		NxFr::StringView Extension = NxFr::Path::GetExtension(File);
		Asset = Asset != 0 ? Asset : System->PathToId(Path);
		
		bool Exist = System->IsTracked(Asset);
		Release &= !(Exist && System->IsLoaded(Asset));

		NEXUS_ASSERT(!(!Exist && Type.GetId() == 0), Default, "AssetImporter needs a Type to import a file");
		NEXUS_ASSERT(!(!Exist && (File.IsEmpty() || !NxFr::Path::HasExtension(File, "") || NxFr::Path::HasExtension(File, NxEn::AssetMetadata::AssetExtension))), Default, "AssetImporter needs a file path with an extension different to the asset extension to import a file");
		NEXUS_ASSERT(!(Exist && Asset == 0), Default, "AssetImporter needs an Id to reimport an asset");

		if (!Exist)
		{
			Instance = Import(Importer, System, File, Type, Path, Extension);
		}
		else
		{
			Instance = Reimport(Importer, System, Asset);
		}

		return Finalize(System, Instance, Release);
	}

	NxEn::Asset* AssetImporter::Import(AssetImporter* Importer, NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension)
	{
		YAML::Node Node = YAML::Node();
		if (Importer)
		{
			Importer->OnImport(Node, File, false);
		}

		return System->Import(Type, Node, Path, Extension);
	}

	NxEn::Asset* AssetImporter::Reimport(AssetImporter* Importer, NxEn::AssetsSystem* System, NxFr::GUID Asset)
	{
		YAML::Node Node = System->GetImportData(Asset);
		if (Importer)
		{
			Importer->OnImport(Node, System->GetMetadata(Asset).GetContentPath(), true);
		}

		return System->Reimport(Asset, Node);
	}

	NxEn::Asset* AssetImporter::Finalize(NxEn::AssetsSystem* System, NxEn::Asset* Instance, bool Release)
	{
		System->Save(Instance->GetId());
		if (Release)
		{
			System->Release(Instance->GetId());
			Instance = nullptr;
		}

		return Instance;
	}

#pragma endregion
}

