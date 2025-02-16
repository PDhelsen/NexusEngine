#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Application/Application.h"

namespace NxEd
{
	class ApplicationEditor : public NxEn::Application
	{
	public:
		NEXUS_EDITOR_API ApplicationEditor();
		NEXUS_EDITOR_API ~ApplicationEditor();
		NEXUS_EDITOR_API virtual void Run() override;
	};
}
