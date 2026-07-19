#pragma once

#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"

namespace NxEd
{
	class NX_EDITOR_API ViewerContext3D : public ViewerContext
	{
	public:
		NX_OBJECT(ViewerContext3D)

		ViewerContext3D();
		virtual ~ViewerContext3D();

	protected:
		void Clear() override;
		void SetupMenu(NxEn::GUI::Menu& Menu) override;
		void SetupTarget(NxEn::Object* Instance) override;

		void OnDraw() override;

	private:
		NxEn::Object* Target;
	};
}
