#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEd
{
	class NX_EDITOR_API AssetImporterPopup : public NxEn::GUI::Popup
	{
	public:
		NX_OBJECT(AssetImporterPopup)

		AssetImporterPopup();
		virtual ~AssetImporterPopup();

		void SetPath(NxFr::StringView FilePath);

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDraw() override;

	private:
		void Pick();
		void Import();

		NxFr::String Path;
		NxFr::String Type;
	};
}
