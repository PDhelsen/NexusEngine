#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetImporter
	{
	public:
		NEXUS_EDITOR_API static AssetImporter* GetImporter(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetImporter(NxFr::StringId Id, AssetImporter* Instance);
		NEXUS_EDITOR_API static const NxFr::Delegate<void(NxFr::StringView, bool)>& GetCommand(NxFr::StringId Id);
		NEXUS_EDITOR_API static void SetCommand(NxFr::StringId Id, const NxFr::Delegate<void(NxFr::StringView, bool)>& Callback);

		template<typename T, typename I>
		static I* Create();
		template<typename T>
		static T* Import(NxFr::StringView Path, bool ReleaseAfterImport = false);
		static void Import(NxFr::StringId Id, NxFr::StringView Path, bool ReleaseAfterImport = false);

	protected:
		NEXUS_EDITOR_API AssetImporter() = default;
		NEXUS_EDITOR_API virtual ~AssetImporter() = default;

		NEXUS_EDITOR_API virtual void OnImport(YAML::Node& Node, NxFr::StringView Path) = 0;
	};

	template<typename T, typename I>
	inline I* AssetImporter::Create()
	{
		I* Instance = new I();
		SetImporter(T::GetClassType(), Instance);
		SetCommand(T::GetClassType(), [](NxFr::StringView Path, bool Release) { AssetImporter::Import<T>(Path, Release); });
		return Instance;
	}

	template<typename T>
	inline T* AssetImporter::Import(NxFr::StringView Path, bool ReleaseAfterImport)
	{
		YAML::Node Node;

		AssetImporter* Importer = GetImporter(T::GetClassType());
		Importer->OnImport(Node, Path);

		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		T* Instance = System->Import<T>(Node, NxFr::Path::GetPathWithoutExtension(Path), NxFr::Path::GetExtension(Path));
		System->Save(Instance->GetId());

		if (ReleaseAfterImport)
		{
			System->Release(Instance->GetId());
			Instance = nullptr;
		}

		return Instance;
	}
}
