#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class NX_EDITOR_API ScenesPanel : public NxEn::GUI::Panel
	{
		struct SceneInfo;

	public:
		NX_OBJECT(ScenesPanel)

		void Refresh();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		void OnScenesChanged(NxFr::StringId Action, NxFr::GUID SceneId, bool IsScene);

		void Create();
		void Save(const SceneInfo& Info);
		void Toggle(const SceneInfo& Info);
		void Load(const SceneInfo& Info);
		void Unload(const SceneInfo& Info);

		NxFr::GUID GetWorldId() const { return WorldsInstances[WorldIndex]; }
		bool IsSceneLoaded(NxFr::GUID SceneId, NxFr::GUID WorldId) const { return Worlds->IsSceneInstantiated(SceneId, WorldId); }

	private:
		enum class Action
		{
			None, Toggle, Save
		};

		struct SceneInfo
		{
			NxFr::GUID Id;
			NxFr::String Path;
		};

		NxEn::GUI::Style Style;
		NxEn::GUI::Menu Menu;

		NxEn::AssetsSystem* Assets;
		NxEn::WorldSystem* Worlds;

		NxFr::Array<SceneInfo> ScenesInstances;
		NxFr::Array<NxFr::GUID> WorldsInstances;
		uint64 WorldIndex;
	};
}
