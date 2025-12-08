#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetImporter
	{
	public:
		NEXUS_EDITOR_API static AssetImporter* GetImporter(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetImporter(AssetImporter* Instance);
		NEXUS_EDITOR_API static NxFr::StringId GetExtension(NxFr::StringView Extension);
		NEXUS_EDITOR_API static void SetExtension(NxFr::StringView Extension, NxFr::StringId Id);
		NEXUS_EDITOR_API static NxFr::StringId TryGetImporterId(NxFr::StringView File);

		NEXUS_EDITOR_API static NxEn::Asset* Run(NxFr::StringId Id, NxFr::StringView File, bool Release = false);
		NEXUS_EDITOR_API static NxEn::Asset* Run(NxFr::StringId Id, NxFr::GUID Asset, bool Release = false);

		template<typename I, typename T>
		static I* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {});
		template<typename T>
		static T* Run(NxFr::StringView File, bool Release = false);
		template<typename T>
		static T* Run(NxFr::GUID Asset, bool Release = false);

	protected:
		NEXUS_EDITOR_API AssetImporter() = default;
		NEXUS_EDITOR_API virtual ~AssetImporter() = default;

		NEXUS_EDITOR_API virtual void OnImport(YAML::Node& Node, NxFr::StringView File, bool Reimport) = 0;

	private:
		NxEn::Asset* Run(NxFr::GUID Asset, NxFr::StringView Path, bool Release);
		NxEn::Asset* Import(NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::StringView Path, NxFr::StringView Extension);
		NxEn::Asset* Reimport(NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::GUID Asset);
		NxEn::Asset* Finalize(NxEn::AssetsSystem* System, NxEn::Asset* Instance, bool Release);

	private:
		NxFr::StringId Type;
	};

	template<typename I, typename T>
	inline I* AssetImporter::Create(NxFr::InitializerList<NxFr::StringView> Extensions)
	{
		I* Importer = new I();
		Importer->Type = T::GetClassType();

		SetImporter(Importer);
		for (auto& Extension : Extensions)
		{
			SetExtension(Extension, Importer->Type);
		}

		return Importer;
	}

	template<typename T>
	inline T* AssetImporter::Run(NxFr::StringView File, bool ReleaseAfterImport)
	{
		return static_cast<T*>(AssetImporter::Run(T::GetClassType(), File, ReleaseAfterImport));
	}

	template<typename T>
	inline T* AssetImporter::Run(NxFr::GUID Asset, bool ReleaseAfterImport)
	{
		return static_cast<T*>(AssetImporter::Run(T::GetClassType(), Asset, ReleaseAfterImport));
	}
}
