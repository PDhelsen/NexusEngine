#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Misc/Object/Viewer/ViewerPanel.h"

namespace NxEd
{
	class NX_EDITOR_API ViewerContextText : public ViewerContext
	{
	protected:
		void Clear() override;
		void Setup(NxEn::Object* Instance) override;
		void Draw() override;

	private:
		NxEn::Object* Target;
	};
}
