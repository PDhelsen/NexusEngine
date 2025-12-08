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

	const static NxEn::Command CmdAssetImport = NxEn::Command::Create("Assets.Import"_Sid, "Import asset at path", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Id, NxFr::StringView Path)
	{
		AssetImporter::Import(Id, Path);
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

	NxEn::Asset* AssetImporter::Import(NxFr::StringId Id, NxFr::StringView FilePath, bool ReleaseAfterImport)
	{
		if (Id.GetId() == 0)
		{
			Id = GetExtension(NxFr::Path::GetExtension(FilePath));
		}

		NEXUS_ASSERT(Id.GetId(), Default, "Asset importer requires an type id in order to import the asset at path %s", FilePath.C());

		AssetImporter* Importer = GetImporter(Id);
		return Importer->Import(FilePath, ReleaseAfterImport);
	}

	NxEn::Asset* AssetImporter::Import(NxFr::StringView FilePath, bool ReleaseAfterImport)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(FilePath);
		NxFr::StringView Extension = NxFr::Path::GetExtension(FilePath);
		NxFr::GUID Id = System->PathToId(Path);
		bool Reimport = Id != 0;
		ReleaseAfterImport &= !(Reimport && System->IsLoaded(Id));

		YAML::Node Node = !Reimport ? YAML::Node() : System->GetImportData(Id);
		OnImport(Node, FilePath, Reimport);

		NxEn::Asset* Instance = !Reimport ?
			Proxy->Import(System, Node, Path, Extension) :
			Proxy->Reimport(System, Node, Id);

		System->Save(Instance->GetId());
		if (ReleaseAfterImport)
		{
			System->Release(Instance->GetId());
			Instance = nullptr;
		}

		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Refresh();
		return Instance;
	}
}

