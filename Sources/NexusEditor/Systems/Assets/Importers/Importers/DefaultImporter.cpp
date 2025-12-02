#include "NexusEditor/Systems/Assets/Importers/Importers/DefaultImporter.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Artifact.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEd
{
	const static DefaultImporter* DocumentImporter = AssetImporter::Create<NxEn::Document, DefaultImporter>();
	const static DefaultImporter* ArtifactImporter = AssetImporter::Create<NxEn::Artifact, DefaultImporter>();
	const static DefaultImporter* TableImporter = AssetImporter::Create<NxEn::Table, DefaultImporter>();

	void DefaultImporter::OnImport(YAML::Node& Node, NxFr::StringView Path)
	{
	}
}
