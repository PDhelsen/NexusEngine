#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	namespace ScenesManager
	{
		NEXUS_EDITOR_API void Toggle(NxFr::GUID Id);
		NEXUS_EDITOR_API void Load(NxFr::GUID Id);
		NEXUS_EDITOR_API void Unload(NxFr::GUID Id);

		NEXUS_EDITOR_API void Refresh();
		NEXUS_EDITOR_API void Create();

		NEXUS_EDITOR_API bool GetShouldUnloadBeforeLoading();
		NEXUS_EDITOR_API void SetShouldUnloadBeforeLoading(bool Value);
	}
}
