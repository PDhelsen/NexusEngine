#pragma once

#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"

namespace NxEn
{
	class Document;
	class Table;
}

namespace NxEd
{
	class ViewerContextText : public ViewerContext
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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ViewerContextText)

		NEXUS_EDITOR_API ViewerContextText();
		NEXUS_EDITOR_API virtual ~ViewerContextText();

	protected:
		NEXUS_EDITOR_API void Clear() override;
		NEXUS_EDITOR_API void SetupMenu(NxEn::GUI::Menu& Menu) override;
		NEXUS_EDITOR_API void SetupTarget(NxEn::Object* Instance) override;

		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		ViewContextMode Mode;
		InspectorTarget Target;
	};
}
