#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetImporter
	{
	public:
		NEXUS_EDITOR_API static AssetImporter* GetImporter(NxFr::StringId Type);
		NEXUS_EDITOR_API static void SetImporter(AssetImporter* Instance);
		NEXUS_EDITOR_API static NxFr::StringId GetType(NxFr::StringView Extension);
		NEXUS_EDITOR_API static void SetType(NxFr::StringView Extension, NxFr::StringId Type);

		NEXUS_EDITOR_API static AssetImporter* TryGetImporter(NxFr::StringId Type);
		NEXUS_EDITOR_API static AssetImporter* TryGetImporter(NxFr::GUID Asset);
		NEXUS_EDITOR_API static NxFr::StringId TryGetType(NxFr::StringView File);
		NEXUS_EDITOR_API static NxFr::StringId TryGetType(NxFr::GUID Asset);

		template<typename I, typename T>
		static I* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {});
		template<typename T>
		static T* Run(NxFr::StringView File, bool Release = false);
		template<typename T>
		static T* Run(NxFr::GUID Asset, bool Release = false);

		NEXUS_EDITOR_API static NxEn::Asset* Run(NxFr::StringView File, NxFr::StringId Type, bool Release = false);
		NEXUS_EDITOR_API static NxEn::Asset* Run(NxFr::GUID Asset, bool Release = false);

	private:
		NEXUS_EDITOR_API static NxEn::Asset* Run(AssetImporter* Importer, NxFr::StringView File, NxFr::StringId Type, NxFr::GUID Asset, bool Release);
		NEXUS_EDITOR_API static NxEn::Asset* Import(AssetImporter* Importer, NxEn::AssetsSystem* System, NxFr::StringView File, NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension);
		NEXUS_EDITOR_API static NxEn::Asset* Reimport(AssetImporter* Importer, NxEn::AssetsSystem* System, NxFr::GUID Asset);
		NEXUS_EDITOR_API static NxEn::Asset* Finalize(NxEn::AssetsSystem* System, NxEn::Asset* Instance, bool Release);

	protected:
		NEXUS_EDITOR_API AssetImporter() = default;
		NEXUS_EDITOR_API virtual ~AssetImporter() = default;

		NEXUS_EDITOR_API virtual void OnImport(YAML::Node& Node, NxFr::StringView File, bool Reimport) = 0;

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
			SetType(Extension, Importer->Type);
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
		return static_cast<T*>(AssetImporter::Run(Asset, ReleaseAfterImport));
	}
}
