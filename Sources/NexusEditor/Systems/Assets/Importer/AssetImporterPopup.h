#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class AssetImporterPopup : public NxEn::GUI::Popup
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetImporterPopup)

		NX_EDITOR_API static void ShowWithPath(NxFr::StringView Path);

		NX_EDITOR_API AssetImporterPopup();
		NX_EDITOR_API virtual ~AssetImporterPopup();

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

		NX_EDITOR_API void Pick();
		NX_EDITOR_API void SetPath(NxFr::StringView FilePath);
		NX_EDITOR_API void Import();

	private:
		NxEn::GUI::Style Style;

		NxFr::String Path;
		NxFr::String Type;
	};
}
