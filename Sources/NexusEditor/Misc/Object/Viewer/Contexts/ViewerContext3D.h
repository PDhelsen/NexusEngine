#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Misc/Object/Viewer/ViewerPanel.h"

namespace NxEn
{
	class World;
}

namespace NxEd
{
	class NX_EDITOR_API ViewerContext3D : public ViewerContext
	{
	protected:
		void Clear() override;
		void Setup(NxEn::ObjectInstance<NxEn::Object> Target) override;
		void Draw() override;

	private:
		NxEn::World* Instance;
	};
}
