#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class NX_EDITOR_API InspectorPanel : public NxEn::GUI::Panel
	{
		union NX_EDITOR_API InspectorTarget
		{
			NxEn::Object* Object;
			NxFr::Handle<NxEn::Object> Handle;
		};

		enum class InspectorMode
		{
			None, Object, Handle
		};

	public:
		NX_OBJECT(InspectorPanel)

		InspectorPanel();
		virtual ~InspectorPanel();

		void Show(NxEn::Object* Instance, bool Force = false);
		void Show(NxFr::Handle<NxEn::Object> Instance, bool Force = false);

		bool IsLocked() const { return Lock; }
		void SetLocked(bool State) { Lock = State; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		NxEn::GUI::Menu Menu;

		InspectorTarget Target;
		InspectorMode Mode;
		bool Lock;
	};
}
