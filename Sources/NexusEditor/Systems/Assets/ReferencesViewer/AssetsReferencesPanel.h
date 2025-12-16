#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class AssetsReferencesPanel : public NxEn::GUI::Panel
	{
		struct Node
		{
			NxFr::GUID* Id;
			NxFr::String Label;
			NxFr::Vector2f Position;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsReferencesPanel)

		NEXUS_EDITOR_API AssetsReferencesPanel();
		NEXUS_EDITOR_API ~AssetsReferencesPanel();

		NEXUS_EDITOR_API void Refresh();
		NEXUS_EDITOR_API void Center();
		NEXUS_EDITOR_API void Select(NxFr::GUID Id);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawProperties();
		void DrawNodes();

		NxFr::Rectangle CreateCanvas();
		void HandleInputs(const NxFr::Rectangle& Canvas);
		void HandleInspect();
		void DrawNode(const NxFr::Rectangle& Canvas, const Node& Instance);

		void FetchDependencies();
		void AddNode(NxFr::GUID Id, NxEn::AssetsSystem* System);
		void SelectNode(NxFr::GUID Id, int32 Depth = 0, bool From = true, bool To = true);
		void AlignNodes();

		inline NxFr::Colors::Bits GetColorBackground() const { return NxFr::Colors::Bits(60, 60, 70, 255); }
		inline NxFr::Colors::Bits GetColorBorder() const { return NxFr::Colors::Bits(120, 120, 140, 255); }
		inline NxFr::Colors::Bits GetColorBright() const { return NxFr::Colors::Bits(255, 255, 255, 255); }
		inline NxFr::Vector2f GetRectangleExtents() const { return NxFr::Vector2f(60, 20) * Zoom; }
		inline NxFr::Vector2f GetTextOffset() const { return NxFr::Vector2f(8, 10) * Zoom; }
		inline float GetZoomFactor() const { return 1.25f; }
		inline void ClampZoom() { Zoom = NxFr::Math::Clamp(Zoom, 0.2f, 4.0f); }

	private:
		NxEn::InputSystem* Inputs;

		NxEn::GUI::Menu Menu;
		NxEn::GUI::Style Style;
		NxFr::String Buffer;
		NxFr::Vector2f Anchor;
		NxFr::Vector2f Mouse;
		float Zoom;
		bool Recenter;

		NxFr::Graph<NxFr::GUID> References;
		NxFr::Dictionary<NxFr::GUID, Node> Nodes;
		NxFr::Set<Node*> Selection;
		NxFr::GUID Selected;
		NxFr::GUID Inspected;
		bool Inspect;
		bool Full;
	};
}
