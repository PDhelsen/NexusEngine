#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"

namespace NxEd
{
	class NX_EDITOR_API ReferencesPanel : public NxEn::GUI::Panel
	{
		struct NX_EDITOR_API Node
		{
			bool operator==(const Node& Other) const { return Id == Other.Id; }
			bool operator!=(const Node& Other) const { return !(*this == Other); }

			NxFr::GUID Id;
			NxFr::String Label;
			NxFr::Vector2f Position;
		};

	public:
		NX_OBJECT(ReferencesPanel)

		void Clear();
		void Select(NxFr::GUID Id, bool Keep = false, bool Force = false);
		void Refresh();
		void Center();

		bool IsLocked() const { return Lock; }
		void SetLocked(bool State) { Lock = State; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

	private:
		void DrawHeader();
		void DrawCanvas();

		NxFr::Rectangle CreateCanvas();
		void DrawNode(const NxFr::Rectangle& Canvas, Node* Instance);
		void HandleInputs(const NxFr::Rectangle& Canvas);
		void HandleInspect();

		void FetchNodes(NxFr::GUID Id);
		void AddNode(NxFr::GUID Id, NxFr::StringView Label);
		void ConnectNode(NxFr::GUID Id, NxFr::GUID Dependency);
		void SortNodes(NxFr::GUID Id, int32 Depth = 0, bool From = true, bool To = true);
		void AlignNodes();

		void OnSelectionChanged(NxFr::GUID Id, bool State);

		inline NxFr::Colors::Bits GetColorBackground() const { return NxFr::Colors::Bits(60, 60, 70, 255); }
		inline NxFr::Colors::Bits GetColorBorder() const { return NxFr::Colors::Bits(120, 120, 140, 255); }
		inline NxFr::Colors::Bits GetColorBright() const { return NxFr::Colors::Bits(255, 255, 255, 255); }
		inline NxFr::Vector2f GetRectangleExtents() const { return NxFr::Vector2f(60, 20) * Zoom; }
		inline NxFr::Vector2f GetTextOffset() const { return NxFr::Vector2f(8, 10) * Zoom; }
		inline float GetZoomFactor() const { return 1.25f; }
		inline void ClampZoom() { Zoom = NxFr::Math::Clamp(Zoom, 0.2f, 4.0f); }

	private:
		NxEn::GUI::Style Style;
		NxEn::GUI::Menu Menu;
		NxFr::Vector2f Anchor;
		NxFr::Vector2f Mouse;
		float Zoom;
		bool Recenter;
		bool Inspect;

		NxEn::InputSystem* Inputs;
		NxEn::AssetsSystem* Assets;
		EditSystem* Edit;

		NxFr::Graph<Node> Nodes;
		NxFr::Dictionary<NxFr::GUID, Node*> Ids;
		NxFr::Set<NxFr::GUID> Selection;
		NxFr::GUID Selected;
		NxFr::GUID Inspected;
		bool Full;
		bool Lock;
	};
}
