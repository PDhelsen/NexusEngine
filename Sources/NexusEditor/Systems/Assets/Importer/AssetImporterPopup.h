#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class NX_EDITOR_API AssetImporterPopup : public NxEn::GUI::Popup
	{
	public:
		NX_OBJECT(AssetImporterPopup)

		static void ShowWithPath(NxFr::StringView Path);

		AssetImporterPopup();
		virtual ~AssetImporterPopup();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDraw() override;

	private:
		void Pick();
		void SetPath(NxFr::StringView FilePath);
		void Import();

		NxEn::GUI::Style Style;

		NxFr::String Path;
		NxFr::String Type;
	};
}
