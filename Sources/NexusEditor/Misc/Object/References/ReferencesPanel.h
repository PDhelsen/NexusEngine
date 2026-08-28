#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/NodePanel.h"

namespace NxEd
{
	class NX_EDITOR_API ReferencesPanel : public NxEn::NodePanel
	{
	public:
		NX_OBJECT(ReferencesPanel)

		void Refresh() override;
		void Select(NxFr::GUID Id) override;

	protected:
		void OnInitialize() override;
		void OnDraw() override;

	private:
		using Connections = NxFr::Tuple<NxFr::Set<NxFr::GUID>, NxFr::Set<NxFr::GUID>>;

		enum class Type
		{
			Asset, GameObject, Component, Behaviour
		};

		struct Info
		{
			Type ObjectType;
			NxFr::String Label;
			Connections Links;
		};

		void DrawHeader();

		void FetchIds();
		void CreateNodes(NxFr::GUID Id);

		NxFr::Dictionary<NxFr::GUID, Info> ObjectsIds;
	};
}
