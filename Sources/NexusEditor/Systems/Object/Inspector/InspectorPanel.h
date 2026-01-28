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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, InspectorPanel)

		NEXUS_EDITOR_API InspectorPanel();
		NEXUS_EDITOR_API virtual ~InspectorPanel();

		NEXUS_EDITOR_API void Show(NxEn::Object* Instance);
		NEXUS_EDITOR_API void Show(NxFr::Handle<NxEn::Object> Instance);

		NEXUS_EDITOR_API bool IsLocked() const { return Lock; }
		NEXUS_EDITOR_API void SetLocked(bool State) { Lock = State; }

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		NxEn::GUI::Menu Menu;

		InspectorTarget Target;
		InspectorMode Mode;
		bool Lock;
	};
}
