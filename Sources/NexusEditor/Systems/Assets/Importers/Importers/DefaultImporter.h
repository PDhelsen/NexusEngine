#pragma once

#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	class DefaultImporter : public AssetImporter
	{
	protected:
		NEXUS_EDITOR_API void OnImport(YAML::Node& Node, NxFr::StringView Path, bool Reimport) override;
	};
}
