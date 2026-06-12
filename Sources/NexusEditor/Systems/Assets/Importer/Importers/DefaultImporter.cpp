#include "NexusEditor/Systems/Assets/Importer/Importers/DefaultImporter.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Artifact.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEd
{
	static AssetImporter* DocumentImporter = AssetImporter::Create<NxEn::Document, DefaultImporter>({ "txt" });
	static AssetImporter* ArtifactImporter = AssetImporter::Create<NxEn::Artifact, DefaultImporter>({ "bin" });
	static AssetImporter* TableImporter = AssetImporter::Create<NxEn::Table, DefaultImporter>({ "yaml" });

	void DefaultImporter::OnImport(YAML::Node& Node, NxFr::StringView ContentPath, bool Reimport)
	{
	}
}
