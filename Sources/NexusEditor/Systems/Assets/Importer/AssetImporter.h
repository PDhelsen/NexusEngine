#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class NX_EDITOR_API AssetImporter
	{
	public:
		static NxFr::Registry<AssetImporter*>& GetImporters();
		static NxFr::Registry<NxFr::StringId>& GetTypes();

		template<typename T, typename I>
		static AssetImporter* Create(NxFr::InitializerList<NxFr::StringView> Extensions = {})
		{
			NxFr::StringId Type = T::GetClassType();
			
			for (auto& Extension : Extensions)
			{
				GetTypes().Register(NxFr::StringId(Extension), Type, true);
			}

			return GetImporters().Register(Type, new I());
		}

		template<typename T>
		static T* Run(NxFr::StringView ContentPath)
		{
			return static_cast<T*>(AssetImporter::Run(ContentPath, T::GetClassType()));
		}
		template<typename T>
		static T* Run(NxFr::GUID Id)
		{
			return static_cast<T*>(AssetImporter::Run(Id));
		}

		static NxEn::Asset* Run(NxFr::StringView ContentPath, NxFr::StringId Type = 0);
		static NxEn::Asset* Run(NxFr::GUID Id);

	protected:
		AssetImporter() = default;
		virtual ~AssetImporter() = default;

		virtual void OnImport(YAML::Node& Node, NxFr::StringView ContentPath, bool Reimport) = 0;

	private:
		static NxEn::Asset* Run(NxFr::StringId Type, NxFr::StringView ContentPath, NxFr::GUID Id);
	};
}
