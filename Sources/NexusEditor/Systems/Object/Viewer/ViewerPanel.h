#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class ViewerContext;

	class ViewerPanel : public NxEn::GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ViewerPanel)

		NEXUS_EDITOR_API void Clear();
		NEXUS_EDITOR_API void Show(NxEn::Object* Instance);

		NEXUS_EDITOR_API ViewerContext* GetContext() const { return Context; }

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		NxEn::GUI::Menu Menu;

		ViewerContext* Context;
	};
}
