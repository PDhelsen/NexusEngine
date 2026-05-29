#pragma once

#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"

namespace NxEn
{
	class Document;
	class Table;
}

namespace NxEd
{
	class NX_EDITOR_API ViewerContextText : public ViewerContext
	{
		union InspectorTarget
		{
			NxEn::Document* Document;
			NxEn::Table* Table;
		};

		enum class ViewContextMode
		{
			Invalid, Document, Table
		};

	public:
		NX_OBJECT(ViewerContextText)

		ViewerContextText();
		virtual ~ViewerContextText();

	protected:
		void Clear() override;
		void SetupMenu(NxEn::GUI::Menu& Menu) override;
		void SetupTarget(NxEn::Object* Instance) override;

		void OnDraw() override;

	private:
		ViewContextMode Mode;
		InspectorTarget Target;
	};
}
