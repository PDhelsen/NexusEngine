#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"

namespace NxEn
{
	class NX_ENGINE_API NodePanel : public GUI::Panel
	{
	public:
		NX_OBJECT(NodePanel)

		virtual void Clear();
		virtual void Refresh();
		virtual void Center();
		virtual void Select(NxFr::GUID Id);

	protected:
		struct NX_ENGINE_API Node
		{
			bool operator==(const Node & Other) const { return Id == Other.Id; }
			bool operator!=(const Node & Other) const { return !(*this == Other); }

			NxFr::GUID Id;
			NxFr::String Label;
			NxFr::Vector2f Position;
		};

		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDraw() override;

		void AddNode(NxFr::GUID Id, NxFr::StringView Label);
		void ConnectNode(NxFr::GUID Id, NxFr::GUID Dependency);
		void MoveNode(NxFr::GUID Id, NxFr::Vector2f Position);

		void DrawCanvas();
		void DrawNodes();
		void DrawNode(Node* Instance);
		void HandleInputs();

		WindowSystem* Window;
		InputSystem* Inputs;

		GUI::Menu Menu;
		NxFr::Rectangle Canvas;
		NxFr::Vector2f Anchor;
		NxFr::Vector2f Mouse;
		float Zoom;
		bool Recenter;
		bool Inspect;

		NxFr::Graph<Node> Nodes;
		NxFr::Dictionary<NxFr::GUID, Node*> NodesIds;
		NxFr::GUID Hovered;
		NxFr::GUID Selected;
	};
}
