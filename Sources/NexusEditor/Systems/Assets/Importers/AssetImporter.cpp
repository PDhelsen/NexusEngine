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

	static NxFr::Dictionary<NxFr::StringId, NxFr::Delegate<void(NxFr::StringView, bool)>>& GetCommands()
	{
		static NxFr::Dictionary<NxFr::StringId, NxFr::Delegate<void(NxFr::StringView, bool)>> Commands;
		return Commands;
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

	const NxFr::Delegate<void(NxFr::StringView, bool)>& AssetImporter::GetCommand(NxFr::StringId Id)
	{
		return GetCommands()[Id];
	}

	void AssetImporter::SetCommand(NxFr::StringId Id, const NxFr::Delegate<void(NxFr::StringView, bool)>& Callback)
	{
		GetCommands().AppendOrAssign(Id, Callback);
	}

	void AssetImporter::Import(NxFr::StringId Id, NxFr::StringView FilePath, bool ReleaseAfterImport)
	{
		if (Id.GetId() == 0)
		{
			Id = GetExtension(NxFr::Path::GetExtension(FilePath));
		}

		NEXUS_ASSERT(Id.GetId(), Default, "Asset importer requires an type id in order to import the asset at path %s", FilePath.C());
		GetCommand(Id).Invoke(FilePath, ReleaseAfterImport);
	}
}

