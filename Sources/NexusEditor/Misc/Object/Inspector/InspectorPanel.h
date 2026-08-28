#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"
#include "NexusEngine/Misc/Object/ObjectInstance.h"

namespace NxEd
{
	class NX_EDITOR_API InspectorPanel : public NxEn::GUI::Panel
	{
	public:
		NX_OBJECT(InspectorPanel)

		InspectorPanel();
		virtual ~InspectorPanel();

		void Show(NxEn::ObjectInstance<NxEn::Object> Target, bool Force = false);

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

		NxEn::ObjectInstance<NxEn::Object> Instance;
		bool Lock;
	};
}
