#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/NodePanel.h"

namespace NxEn
{
	void NodePanel::Clear()
	{
		Nodes.Clear();
		Ids.Clear();
		Selected = 0;
		Hovered = 0;
	}

	void NodePanel::Refresh()
	{
		Select(Selected);
	}

	void NodePanel::Center()
	{
		Recenter = true;
	}

	void NodePanel::Select(NxFr::GUID Id)
	{
		if (!Ids.TryGet(Id))
		{
			return;
		}

		Selected = Id;
		Center();
	}

	void NodePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

		Menu.AddMenuItem("Refresh", { this, &NodePanel::Refresh }, 0);
		Menu.AddMenuItem("Center", { this, &NodePanel::Center }, 1);
	}

	void NodePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void NodePanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.Show();

		Window = Application::GetSystem<WindowSystem>();
		Inputs = Application::GetSystem<InputSystem>();

		Selected = 0;
		Hovered = 0;
		Refresh();
	}

	void NodePanel::OnDisable()
	{
		Clear();

		Menu.Hide();
		Panel::OnDisable();
	}

	void NodePanel::OnDraw()
	{
		Menu.Draw();

		DrawCanvas();
		DrawNodes();

		HandleInputs();
	}

	void NodePanel::AddNode(NxFr::GUID Id, NxFr::StringView Label)
	{
		if (Ids.TryGet(Id))
		{
			return;
		}

		Node& Instance = Nodes.AppendConstruct(
			Id,
			Label + NxFr::StringUtility::NewLine + NxFr::StringUtility::ToString(Id),
			NxFr::Vector2f::Zero
		);
		Ids.Append(Id, &Instance);
	}

	void NodePanel::ConnectNode(NxFr::GUID Id, NxFr::GUID Dependency)
	{
		Nodes.Connect(Ids[Id], Ids[Dependency]);
	}

	void NodePanel::MoveNode(NxFr::GUID Id, NxFr::Vector2f Position)
	{
		Ids[Id]->Position = Position;
	}

	void NodePanel::DrawCanvas()
	{
		NxFr::Vector2f Size = ImGui::GetContentRegionAvail();
		if (Size.x <= 0.0f || Size.y <= 0.0f)
		{
			Canvas = NxFr::Rectangle();
			return;
		}

		ImGui::InvisibleButton("Canvas", Size);
		NxFr::Vector2f Origin = ImGui::GetItemRectMin();

		NxFr::Vector2f Extents = Size * 0.5f;
		NxFr::Vector2f Center = NxFr::Vector2f(Origin.x + Extents.x, Origin.y + Extents.y);
		Canvas = NxFr::Rectangle(Center, Extents);

		if (Recenter)
		{
			Anchor = Center;
			Zoom = 1.0f;
			Recenter = false;
		}
	}

	void NodePanel::DrawNodes()
	{
		Hovered = 0;

		ImDrawList* Drawer = ImGui::GetWindowDrawList();
		Drawer->PushClipRect(Canvas.GetBottomLeft(), Canvas.GetTopRight());
		for (auto& It : Nodes)
		{
			DrawNode(&It);
		}
		Drawer->PopClipRect();
	}

	void NodePanel::DrawNode(Node* Instance)
	{
		NxFr::Vector2f TextOffset = NxFr::Vector2f(8, 10) * Zoom;
		NxFr::Vector2f RectangleExtents = NxFr::Vector2f(60, 20) * Zoom;
		float RectangleCenterMultiplier = 2.25f;
		float Rounding = 1.0f;
		float Thickness = 2.0f;

		uint32 Background = GUI::Utils::NexusToImGuiColor(NxFr::Colors::Bits(GUI::Styles::GrayDark()));
		uint32 Border = GUI::Utils::NexusToImGuiColor(NxFr::Colors::Bits(GUI::Styles::Gray()));
		uint32 Bright = GUI::Utils::NexusToImGuiColor(NxFr::Colors::Bits(GUI::Styles::White()));

		NxFr::Rectangle Box = NxFr::Rectangle(Anchor + Instance->Position * RectangleExtents * RectangleCenterMultiplier, RectangleExtents);
		if (NxFr::ShapeUtility::Contains(Box, Mouse))
		{
			Hovered = Instance->Id;
		}

		ImDrawList* Drawer = ImGui::GetWindowDrawList();
		Drawer->AddRectFilled(Box.GetBottomLeft(), Box.GetTopRight(), Background, Rounding);
		Drawer->AddRect(Box.GetBottomLeft(), Box.GetTopRight(), Border, Rounding);
		Drawer->PushClipRect(Box.GetBottomLeft(), Box.GetTopRight(), true);
		Drawer->AddText(Box.GetBottomLeft() + TextOffset, Bright, Instance->Label.C());
		Drawer->PopClipRect();

		uint64 Count = Nodes.GetConnectionCount(Instance);
		for (uint64 Index = 0; Index < Count; Index++)
		{
			Node* Dependency = Nodes.TryGetConnection(Instance, NxFr::Node::NodeGraphConnectionType::To, Index);
			if (Dependency)
			{
				NxFr::Rectangle Target = NxFr::Rectangle(Anchor + Dependency->Position * RectangleExtents * RectangleCenterMultiplier, RectangleExtents);

				NxFr::Vector2f Line = NxFr::VectorUtility::Normalize(Target.Center - Box.Center);
				NxFr::Vector2f Start = Box.Center + Line * NxFr::ShapeUtility::Distance(Box, Target.Center);
				NxFr::Vector2f End = Target.Center - Line * NxFr::ShapeUtility::Distance(Target, Box.Center);

				Drawer->AddLine(Start, End, Bright, Thickness);
			}
		}
	}

	void NodePanel::HandleInputs()
	{
		static NxFr::Vector2f ZoomMinMax = NxFr::Vector2f(0.2f, 4.0f);
		static float ZoomScale = 1.25f;

		Inspect = false;

		if (ImGui::IsItemHovered())
		{
			Mouse = Window->GetWindow().Position + Inputs->GetMousePosition();

			if (Inputs->CheckAxis(Input::Axis::ScrollY))
			{
				if (Inputs->GetAxis(Input::Axis::ScrollY) < 0.0f)
					Zoom /= 1.25f;
				else
					Zoom *= 1.25f;

				Zoom = NxFr::Math::Clamp(Zoom, ZoomMinMax.x, ZoomMinMax.y);
			}
			if (Inputs->CheckButton(Input::Button::MouseMiddle, Input::State::Down))
			{
				NxFr::Vector2f Delta = Inputs->GetMouseDelta();
				Anchor += NxFr::Vector2f(Delta.x, Delta.y) / Zoom;
			}
			if (Inputs->CheckButton(Input::Button::MouseRight))
			{
				Inspect = true;
			}
		}

		if (Inspect && Hovered)
		{
			Select(Hovered);
		}
	}
}
