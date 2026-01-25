#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class ScenesPanel : public NxEn::GUI::Panel
	{
		struct Info
		{
			NxFr::GUID Id;
			NxFr::String Path;
			NxFr::StringId World;

			bool IsLoaded() const { return World.GetId() != 0; }
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ScenesPanel)

		NEXUS_EDITOR_API void Refresh();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

		NEXUS_EDITOR_API void OnScenesChanged(NxFr::StringId, NxFr::GUID, NxFr::GUID);
		NEXUS_EDITOR_API void OnToggle(uint64 Index);

		NEXUS_EDITOR_API void Create();
		NEXUS_EDITOR_API void Load(NxFr::GUID SceneId);
		NEXUS_EDITOR_API void Unload(NxFr::GUID SceneId);

		NEXUS_EDITOR_API NxFr::GUID GetWorld() const { return WorldsIds[WorldsIndex]; }

	private:
		NxEn::GUI::Style Style;
		NxEn::GUI::Menu Menu;

		NxEn::AssetsSystem* Assets;
		NxEn::WorldSystem* Worlds;

		NxFr::Array<NxFr::GUID> WorldsIds;
		NxFr::Array<Info> ScenesInfos;
		uint64 WorldsIndex;
		bool LoadSingle;
	};
}
