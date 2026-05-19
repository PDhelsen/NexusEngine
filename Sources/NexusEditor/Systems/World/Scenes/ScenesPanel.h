#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class NX_EDITOR_API ScenesPanel : public NxEn::GUI::Panel
	{
		struct Info
		{
			NxFr::GUID Id;
			NxFr::String Path;
			NxFr::StringId World;

			bool IsLoaded() const { return World.GetId() != 0; }
		};

	public:
		NX_OBJECT_DECLARATION(ScenesPanel)

		void Refresh();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

		void OnScenesChanged(NxFr::StringId, NxFr::GUID, NxFr::GUID);
		void OnToggle(uint64 Index);

		void Create();
		void Load(NxFr::GUID SceneId);
		void Unload(NxFr::GUID SceneId);

		NxFr::GUID GetWorld() const { return WorldsIds[WorldsIndex]; }

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
