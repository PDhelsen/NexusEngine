#include "NexusEditor/Systems/Assets/Importer/Importers/DefaultImporter.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Artifact.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEd
{
	const static DefaultImporter* DocumentImporter = AssetImporter::Create<DefaultImporter, NxEn::Document>({ "txt" });
	const static DefaultImporter* ArtifactImporter = AssetImporter::Create<DefaultImporter, NxEn::Artifact>({ "bin" });
	const static DefaultImporter* TableImporter = AssetImporter::Create<DefaultImporter, NxEn::Table>({ "yaml" });

	void DefaultImporter::OnImport(YAML::Node& Node, NxFr::StringView ContentPath, bool Reimport)
	{
	}
}
