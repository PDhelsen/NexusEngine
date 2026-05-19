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
		NX_OBJECT_DECLARATION(NX_EDITOR_API, ScenesPanel)

		NX_EDITOR_API void Refresh();

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

		NX_EDITOR_API void OnScenesChanged(NxFr::StringId, NxFr::GUID, NxFr::GUID);
		NX_EDITOR_API void OnToggle(uint64 Index);

		NX_EDITOR_API void Create();
		NX_EDITOR_API void Load(NxFr::GUID SceneId);
		NX_EDITOR_API void Unload(NxFr::GUID SceneId);

		NX_EDITOR_API NxFr::GUID GetWorld() const { return WorldsIds[WorldsIndex]; }

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
