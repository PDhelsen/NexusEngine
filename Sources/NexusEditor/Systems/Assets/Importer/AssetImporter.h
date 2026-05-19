#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class NX_EDITOR_API AssetImporter
	{
	public:
		static AssetImporter* GetImporter(NxFr::StringId Type);
		static void SetImporter(NxFr::StringId Type, AssetImporter* Instance);
		static NxFr::StringId GetType(NxFr::StringView Extension);
		static void SetType(NxFr::StringView Extension, NxFr::StringId Type);

		template<typename I, typename T>
		static I* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {});
		template<typename T>
		static T* Run(NxFr::StringView ContentPath);
		template<typename T>
		static T* Run(NxFr::GUID Id);

		static NxEn::Asset* Run(NxFr::StringView ContentPath, NxFr::StringId Type = 0);
		static NxEn::Asset* Run(NxFr::GUID Id);

	protected:
		AssetImporter() = default;
		virtual ~AssetImporter() = default;

		virtual void OnImport(YAML::Node& Node, NxFr::StringView ContentPath, bool Reimport) = 0;

	private:
		static NxEn::Asset* Run(NxFr::StringId Type, NxFr::StringView ContentPath, NxFr::GUID Id);
	};

	template<typename I, typename T>
	inline I* AssetImporter::Create(NxFr::InitializerList<NxFr::StringView> Extensions)
	{
		I* Importer = new I();
		NxFr::StringId Type = T::GetClassType();

		SetImporter(Type, Importer);
		for (auto& Extension : Extensions)
		{
			SetType(Extension, Type);
		}

		return Importer;
	}

	template<typename T>
	inline T* AssetImporter::Run(NxFr::StringView ContentPath)
	{
		return static_cast<T*>(AssetImporter::Run(T::GetClassType(), ContentPath));
	}

	template<typename T>
	inline T* AssetImporter::Run(NxFr::GUID Id)
	{
		return static_cast<T*>(AssetImporter::Run(Id));
	}
}
