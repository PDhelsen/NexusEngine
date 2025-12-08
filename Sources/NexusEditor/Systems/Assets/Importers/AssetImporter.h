#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterProxy.h"

namespace NxEd
{
	class AssetImporter
	{
	public:
		NEXUS_EDITOR_API static AssetImporter* GetImporter(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetImporter(NxFr::StringId Id, AssetImporter* Instance);
		NEXUS_EDITOR_API static NxFr::StringId GetExtension(NxFr::StringView Extension);
		NEXUS_EDITOR_API static void SetExtension(NxFr::StringView Extension, NxFr::StringId Id);

		NEXUS_EDITOR_API static NxEn::Asset* Import(NxFr::StringId Id, NxFr::StringView FilePath, bool ReleaseAfterImport = false);

		template<typename I, typename T>
		static I* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {});
		template<typename T>
		static T* Import(NxFr::StringView FilePath, bool ReleaseAfterImport = false);

	protected:
		NEXUS_EDITOR_API AssetImporter() = default;
		NEXUS_EDITOR_API virtual ~AssetImporter() = default;

		NEXUS_EDITOR_API virtual void OnImport(YAML::Node& Node, NxFr::StringView FilePath, bool Reimport) = 0;

	private:
		NxEn::Asset* Import(NxFr::StringView FilePath, bool ReleaseAfterImport = false);

	private:
		AssetImporterProxy* Proxy;
	};

	template<typename I, typename T>
	inline I* AssetImporter::Create(NxFr::InitializerList<NxFr::StringView> Extensions)
	{
		I* Importer = new I();
		Importer->Proxy = new AssetImporterTypedProxy<T>();

		SetImporter(T::GetClassType(), Importer);
		for (auto& Extension : Extensions)
		{
			SetExtension(Extension, T::GetClassType());
		}

		return Importer;
	}

	template<typename T>
	inline T* AssetImporter::Import(NxFr::StringView FilePath, bool ReleaseAfterImport)
	{
		return static_cast<T*>(AssetImporter::Import(T::GetClassType(), FilePath, ReleaseAfterImport));
	}
}
