#pragma once

#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class AssetImporterPopup : public NxEn::GUI::Popup
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetImporterPopup)

		NEXUS_EDITOR_API AssetImporterPopup();
		NEXUS_EDITOR_API virtual ~AssetImporterPopup();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

		NEXUS_EDITOR_API void Pick();
		NEXUS_EDITOR_API void Import();

	private:
		NxEn::GUI::Style Style;

		NxFr::String Type;
		NxFr::String Path;
		bool Release;
	};
}
