#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	class ViewerContext : public NxEn::Object
	{
		friend class ViewerPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ViewerContext)

		NEXUS_EDITOR_API ViewerContext() = default;
		NEXUS_EDITOR_API virtual ~ViewerContext() = default;

		NEXUS_EDITOR_API virtual NxEn::World* GetWorld() const { return nullptr; }

	protected:
		NEXUS_EDITOR_API virtual void Clear() = 0;
		NEXUS_EDITOR_API virtual void SetupMenu(NxEn::GUI::Menu& Menu) {};
		NEXUS_EDITOR_API virtual void SetupTarget(NxEn::Object* Instance) = 0;
	};
}
