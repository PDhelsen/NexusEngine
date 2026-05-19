#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class InspectorPanel : public NxEn::GUI::Panel
	{
		union InspectorTarget
		{
			NxEn::Object* Object;
			NxFr::Handle<NxEn::Object> Handle;
		};

		enum class InspectorMode
		{
			None, Object, Handle
		};

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, InspectorPanel)

		NX_EDITOR_API InspectorPanel();
		NX_EDITOR_API virtual ~InspectorPanel();

		NX_EDITOR_API void Show(NxEn::Object* Instance, bool Force = false);
		NX_EDITOR_API void Show(NxFr::Handle<NxEn::Object> Instance, bool Force = false);

		NX_EDITOR_API bool IsLocked() const { return Lock; }
		NX_EDITOR_API void SetLocked(bool State) { Lock = State; }

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

	private:
		NxEn::GUI::Menu Menu;

		InspectorTarget Target;
		InspectorMode Mode;
		bool Lock;
	};
}
