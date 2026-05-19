#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class ViewerContext;

	class ViewerPanel : public NxEn::GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, ViewerPanel)

		NX_EDITOR_API void Clear();
		NX_EDITOR_API void Show(NxEn::Object* Instance);

		NX_EDITOR_API ViewerContext* GetContext() const { return Context; }

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

	private:
		NxEn::GUI::Menu Menu;

		ViewerContext* Context;
	};
}
