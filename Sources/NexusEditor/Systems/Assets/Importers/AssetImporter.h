#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	class AssetImporter
	{
	public:
		NEXUS_EDITOR_API static AssetImporter* GetImporter(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetImporter(NxFr::StringId Id, AssetImporter* Instance);
		NEXUS_EDITOR_API static NxFr::StringId GetExtension(NxFr::StringView Extension);
		NEXUS_EDITOR_API static void SetExtension(NxFr::StringView Extension, NxFr::StringId Id);
		NEXUS_EDITOR_API static const NxFr::Delegate<void(NxFr::StringView, bool)>& GetCommand(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetCommand(NxFr::StringId Id, const NxFr::Delegate<void(NxFr::StringView, bool)>& Callback);

		template<typename T, typename I>
		static I* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {});
		template<typename T>
		static T* Import(NxFr::StringView FilePath, bool ReleaseAfterImport = false);

		static void Import(NxFr::StringId Id, NxFr::StringView FilePath, bool ReleaseAfterImport = false);

	protected:
		NEXUS_EDITOR_API AssetImporter() = default;
		NEXUS_EDITOR_API virtual ~AssetImporter() = default;

		NEXUS_EDITOR_API virtual void OnImport(YAML::Node& Node, NxFr::StringView FilePath, bool Reimport) = 0;
	};

	template<typename T, typename I>
	inline I* AssetImporter::Create(NxFr::InitializerList<NxFr::StringView> Extensions)
	{
		I* Instance = new I();
		SetImporter(T::GetClassType(), Instance);
		SetCommand(T::GetClassType(), [](NxFr::StringView FilePath, bool Release) { AssetImporter::Import<T>(FilePath, Release); });
		for (auto& Extension : Extensions)
		{
			SetExtension(Extension, T::GetClassType());
		}
		return Instance;
	}

	template<typename T>
	inline T* AssetImporter::Import(NxFr::StringView FilePath, bool ReleaseAfterImport)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		NxFr::StringView Path = NxFr::Path::GetPathWithoutExtension(FilePath);
		NxFr::StringView Extension = NxFr::Path::GetExtension(FilePath);
		NxFr::GUID Id = System->PathToId(Path);
		bool Reimport = Id != 0;

		YAML::Node Node = !Reimport ? YAML::Node() : System->GetImportData(Id);
		AssetImporter* Importer = GetImporter(T::GetClassType());
		Importer->OnImport(Node, FilePath, Reimport);

		T* Instance = !Reimport ?
			System->Import<T>(Node, Path, Extension) :
			System->Reimport<T>(Node, Id);

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
