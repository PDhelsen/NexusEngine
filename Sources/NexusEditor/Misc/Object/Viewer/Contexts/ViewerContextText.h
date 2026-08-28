#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Misc/Object/Viewer/ViewerPanel.h"

namespace NxEn
{
	class Document;
	class Table;
}

namespace NxEd
{
	class NX_EDITOR_API ViewerContextText : public ViewerContext
	{
	protected:
		void Clear() override;
		void Setup(NxEn::ObjectInstance<NxEn::Object> Target) override;
		void Draw() override;

	private:
		enum class Mode
		{
			Invalid, Document, Table
		};

		Mode ViewMode;
		union
		{
			NxEn::Object* Instance;
			NxEn::Document* Document;
			NxEn::Table* Table;
		};
	};
}
