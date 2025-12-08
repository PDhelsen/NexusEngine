#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

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

	AssetImporter* AssetImporter::GetImporter(NxFr::StringId Id)
	{
		return GetImporters()[Id];
	}

	void AssetImporter::SetImporter(NxFr::StringId Id, AssetImporter* Instance)
	{
		GetImporters().AppendOrAssign(Id, Instance);
	}

	NxFr::StringId AssetImporter::GetExtension(NxFr::StringView Extension)
	{
		return GetExtensions()[Extension];
	}

	void AssetImporter::SetExtension(NxFr::StringView Extension, NxFr::StringId Id)
	{
		GetExtensions().AppendOrAssign(Extension, Id);
	}

	NxFr::StringId AssetImporter::TryGetImporterId(NxFr::StringView File)
	{
		NxFr::StringView Extension = NxFr::Path::GetExtension(File);

		if (Extension == NxEn::AssetMetadata::Extension)
		{
			NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
			NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(File);
			NxFr::GUID Id = System->PathToId(Path);
			return System->GetMetadata(Id).GetType();
		}

		return GetExtension(Extension);
	}

	NxEn::Asset* AssetImporter::Run(NxFr::StringId Id, NxFr::StringView File, bool Release)
	{
		if (Id.GetId() == 0)
		{
			Id = AssetImporter::TryGetImporterId(File);
		}

		NEXUS_ASSERT(Id.GetId(), Default, "Asset importer requires an type id in order to import the asset at path %s", File.C());

		AssetImporter* Importer = GetImporter(Id);
		return Importer->Run(0, File, Release);
	}

	NxEn::Asset* AssetImporter::Run(NxFr::StringId Id, NxFr::GUID Asset, bool Release)
	{
		NEXUS_ASSERT(Id.GetId(), Default, "Asset importer requires an type id in order to reimport the asset %d", Asset);

		AssetImporter* Importer = GetImporter(Id);
		return Importer->Run(Asset, "", Release);
	}

	NxEn::Asset* AssetImporter::Run(NxFr::GUID Asset, NxFr::StringView File, bool Release)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::Asset* Instance = nullptr;

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(File);
		NxFr::StringView Extension = NxFr::Path::GetExtension(File);
		Asset = Asset != 0 ? Asset : System->PathToId(Path);
		
		bool Exist = System->IsTracked(Asset);
		Release &= !(Exist && System->IsLoaded(Asset));

		if (!Exist)
		{
			Instance = Import(System, File, Path, Extension);
		}
		else
		{
			Instance = Reimport(System, File, Asset);
		}

		return Finalize(System, Instance, Release);
	}

	NxEn::Asset* AssetImporter::Import(NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::StringView Path, NxFr::StringView Extension)
	{
		YAML::Node Node = YAML::Node();
		OnImport(Node, File, false);

		return Proxy->Import(System, Node, Path, Extension);
	}

	NxEn::Asset* AssetImporter::Reimport(NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::GUID Asset)
	{
		YAML::Node Node = System->GetImportData(Asset);
		OnImport(Node, File, true);

		return Proxy->Reimport(System, Node, Asset);
	}

	NxEn::Asset* AssetImporter::Finalize(NxEn::AssetsSystem* System, NxEn::Asset* Instance, bool Release)
	{
		System->Save(Instance->GetId());
		if (Release)
		{
			System->Release(Instance->GetId());
			Instance = nullptr;
		}

		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Refresh();
		return Instance;
	}
}

