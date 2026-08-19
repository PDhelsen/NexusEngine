#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	struct NX_EDITOR_API ViewerContext
	{
		friend class ViewerPanel;

	public:
		ViewerContext() = default;
		virtual ~ViewerContext() = default;

	protected:
		virtual void Clear() = 0;
		virtual void Setup(NxEn::Object* Instance) = 0;
		virtual void Draw() = 0;
	};

	class NX_EDITOR_API ViewerPanel : public NxEn::GUI::Panel
	{
	public:
		static NxFr::Factory<ViewerContext>& GetFactory();

		NX_OBJECT(ViewerPanel)

		void Clear();
		void Show(NxEn::Object* Instance);

	protected:
		void OnInitialize() override;
		void OnDraw() override;

	private:
		ViewerContext* Context;
	};
}
