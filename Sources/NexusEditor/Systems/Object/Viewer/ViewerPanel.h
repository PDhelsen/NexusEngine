#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class ViewerContext;

	class NX_EDITOR_API ViewerPanel : public NxEn::GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(ViewerPanel)

		void Clear();
		void Show(NxEn::Object* Instance);

		ViewerContext* GetContext() const { return Context; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

	private:
		NxEn::GUI::Menu Menu;

		ViewerContext* Context;
	};
}
