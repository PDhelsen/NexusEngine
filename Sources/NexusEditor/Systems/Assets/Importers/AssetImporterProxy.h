#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetImporterProxy
	{
	public:
		virtual NxEn::Asset* Import(NxEn::AssetsSystem* System, YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension) = 0;
		virtual NxEn::Asset* Reimport(NxEn::AssetsSystem* System, YAML::Node& Node, NxFr::GUID Id) = 0;
	};

	template<typename T>
	class AssetImporterTypedProxy : public AssetImporterProxy
	{
		NxEn::Asset* Import(NxEn::AssetsSystem* System, YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension) override
		{
			return System->Import<T>(Node, Path, Extension);
		}

		NxEn::Asset* Reimport(NxEn::AssetsSystem* System, YAML::Node& Node, NxFr::GUID Id) override
		{
			return System->Reimport<T>(Node, Id);
		}
	};

}
