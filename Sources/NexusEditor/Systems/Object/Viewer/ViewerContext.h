#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	class ViewerContext : public NxEn::Object
	{
		friend class ViewerPanel;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, ViewerContext)

		NX_EDITOR_API ViewerContext() = default;
		NX_EDITOR_API virtual ~ViewerContext() = default;

		NX_EDITOR_API virtual NxEn::World* GetWorld() const { return nullptr; }

	protected:
		NX_EDITOR_API virtual void Clear() = 0;
		NX_EDITOR_API virtual void SetupMenu(NxEn::GUI::Menu& Menu) {};
		NX_EDITOR_API virtual void SetupTarget(NxEn::Object* Instance) = 0;
	};
}
