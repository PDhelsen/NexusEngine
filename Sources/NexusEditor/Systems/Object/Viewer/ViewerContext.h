#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	class NX_EDITOR_API ViewerContext : public NxEn::Object
	{
		friend class ViewerPanel;

	public:
		NX_OBJECT(ViewerContext)

		ViewerContext() = default;
		virtual ~ViewerContext() = default;

	protected:
		virtual void Clear() = 0;
		virtual void SetupMenu(NxEn::GUI::Menu& Menu) {};
		virtual void SetupTarget(NxEn::Object* Instance) = 0;
	};
}
