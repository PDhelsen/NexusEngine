#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class ReferencesPanel : public NxEn::GUI::Panel
	{
		struct Node
		{
			NxFr::GUID Id;
			NxFr::String Label;
			NxFr::Vector2f Position;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ReferencesPanel)

		NEXUS_EDITOR_API void Clear();
		NEXUS_EDITOR_API void Select(NxFr::GUID Id, bool Keep = false);
		NEXUS_EDITOR_API void Refresh();
		NEXUS_EDITOR_API void Center();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawHeader();
		void DrawCanvas();

		NxFr::Rectangle CreateCanvas();
		void DrawNode(const NxFr::Rectangle& Canvas, Node* Instance);
		void HandleInputs(const NxFr::Rectangle& Canvas);
		void HandleInspect();

		void FetchNodes(NxFr::GUID Id);
		void AddNode(NxFr::GUID Id);
		void ConnectNode(NxFr::GUID Id, NxFr::GUID Dependency);
		void SortNodes(NxFr::GUID Id, int32 Depth = 0, bool From = true, bool To = true);
		void AlignNodes();

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

		NxEn::AssetsSystem* Assets;
		NxEn::InputSystem* Inputs;

		NxFr::Graph<Node> Nodes;
		NxFr::Dictionary<NxFr::GUID, Node*> Ids;
		NxFr::Set<NxFr::GUID> Selection;
		NxFr::GUID Selected;
		NxFr::GUID Inspected;
		bool Full;
	};
}
