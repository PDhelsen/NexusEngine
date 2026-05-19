#pragma once

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"

namespace NxEd
{
	class DefaultImporter : public AssetImporter
	{
	protected:
		NX_EDITOR_API void OnImport(YAML::Node& Node, NxFr::StringView ContentPath, bool Reimport) override;
	};
}
