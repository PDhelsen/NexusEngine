#include "NexusEditor/Systems/Object/References/ReferencesPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	static ReferencesPanel* Panel = NxEn::GUI::Panel::Create<ReferencesPanel>();

	static const NxEn::GUI::Menu::Item& MenuItemReferences = NxEn::GUI::Menu::Item::Create("Object/Objects/References", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ReferencesPanel");
	}));

	void ReferencesPanel::Clear()
	{
		Nodes.Clear();
		Ids.Clear();
		Selection.Clear();
		Selection = 0;
		Inspected = 0;
	}

	void ReferencesPanel::Select(NxFr::GUID Id, bool Keep, bool Force)
	{
		if (Lock && !Force)
		{
			return;
		}

		if (Keep)
		{
			Selection.Clear();
		}
		else
		{
			Clear();
		}

		Selected = Id;
		if (Selected == 0)
		{
			return;
		}

		FetchNodes(Selected);
		SortNodes(Selected);
		AlignNodes();
		Center();
	}

	void ReferencesPanel::Refresh()
	{
		Select(Selected);
	}

	void ReferencesPanel::Center()
	{
		Recenter = true;
	}

	void ReferencesPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("References");
		Lock = false;

		Menu.AddMenuItem("Refresh", { this, &ReferencesPanel::Refresh }, 0);
		Menu.AddMenuItem("Center", { this, &ReferencesPanel::Center }, 1);
		Menu.AddMenuToggle("Full", &Full, nullptr, 2);
		Menu.AddMenuToggle("Lock", &Lock, nullptr, 3);
	}

	void ReferencesPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ReferencesPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);
		Style.Reset();

		Window = NxEn::Application::GetSystem<NxEn::WindowSystem>();
		Inputs = NxEn::Application::GetSystem<NxEn::InputSystem>();
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();
		if (Browser)
		{
			Edit::Context* Ctx = Edit->GetContext(Browser->GetImGuiId());
			if (Ctx)
			{
				Ctx->GetOnSelectionChanged() += { this, &ReferencesPanel::OnSelectionChanged };
			}
		}

		Full = false;
		Select(0);
	}

	void ReferencesPanel::OnDisable()
	{
		Clear();

		AssetsBrowserPanel* Browser = NxEn::GUISystem::GetPanel<AssetsBrowserPanel>();
		if (Browser)
		{
			Edit::Context* Ctx = Edit->GetContext(Browser->GetImGuiId());
			if (Ctx)
			{
				Ctx->GetOnSelectionChanged() -= { this, &ReferencesPanel::OnSelectionChanged };
			}
		}

		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void ReferencesPanel::OnDraw()
	{
		Menu.Draw();

		DrawHeader();
		DrawCanvas();
	}

	void ReferencesPanel::DrawHeader()
	{
		NxFr::Vector2f Size = NxFr::Vector2f(0.0f, ImGui::GetTextLineHeight() * 5.0f);
		ImGui::BeginChild("References", Size, ImGuiChildFlags_Border);

		NxEn::GUI::Drawer<uint64>::Property(Nodes.GetCount(), "Tracked", &Style);
		if (NxEn::GUI::Drawer<NxFr::GUID>::Field(Selected, "Selected", "IdSelected", &Style))
		{
			Select(Selected);
		}

		ImGui::EndChild();
	}

	void ReferencesPanel::DrawCanvas()
	{
		NxFr::Rectangle Canvas = CreateCanvas();
		HandleInputs(Canvas);

		for (const auto& Id : Selection)
		{
			DrawNode(Canvas, Ids[Id]);
		}

		HandleInspect();
	}

	NxFr::Rectangle ReferencesPanel::CreateCanvas()
	{
		NxFr::Vector2f Size = NxFr::Vector2f(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

		ImGui::InvisibleButton("Canvas", Size);
		NxFr::Vector2f Origin = NxFr::Vector2f(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);

		NxFr::Vector2f Extents = Size * 0.5f;
		NxFr::Vector2f Center = NxFr::Vector2f(Origin.x + Extents.x, Origin.y + Extents.y);

		if (Recenter)
		{
			Anchor = Center;
			Zoom = 1.0f;

			Recenter = false;
		}

		return NxFr::Rectangle(Center, Extents);
	}

	void ReferencesPanel::DrawNode(const NxFr::Rectangle& Canvas, Node* Instance)
	{
		ImDrawList* Drawer = ImGui::GetWindowDrawList();
		Drawer->PushClipRect(Canvas.GetBottomLeft(), Canvas.GetTopRight());

		NxFr::Colors::Bits Background = GetColorBackground();
		NxFr::Colors::Bits Border = Instance->Id == Selected ? GetColorBright() : GetColorBorder();
		NxFr::Colors::Bits Bright = GetColorBright();

		NxFr::Rectangle Box = NxFr::Rectangle(Anchor + Instance->Position * GetRectangleExtents() * 2.25f, GetRectangleExtents());

		if (Inspect && NxFr::ShapeUtility::Contains(Box, Mouse))
		{
			Inspected = Instance->Id;
		}

		Drawer->AddRectFilled(Box.GetBottomLeft(), Box.GetTopRight(), IM_COL32(Background.r, Background.g, Background.b, Background.a), 1);
		Drawer->AddRect(Box.GetBottomLeft(), Box.GetTopRight(), IM_COL32(Border.r, Border.g, Border.b, Border.a), 1);

		Drawer->PushClipRect(Box.GetBottomLeft(), Box.GetTopRight(), true);
		Drawer->AddText(Box.GetBottomLeft() + GetTextOffset(), IM_COL32(Bright.r, Bright.g, Bright.b, Bright.a), Instance->Label.C());
		Drawer->PopClipRect();

		uint64 Count = Nodes.GetConnectionCount(Instance);
		for (uint64 Index = 0; Index < Count; Index++)
		{
			Node* Dependency = Nodes.TryGetConnection(Instance, NxFr::Node::NodeGraphConnectionType::To, Index);
			if (Dependency)
			{
				if (Selection.TryGet(Dependency->Id))
				{
					NxFr::Rectangle Target = NxFr::Rectangle(Anchor + Dependency->Position * GetRectangleExtents() * 2.25f, GetRectangleExtents());

					NxFr::Vector2f Line = NxFr::VectorUtility::Normalize(Target.Center - Box.Center);
					NxFr::Vector2f Start = Box.Center + Line * NxFr::ShapeUtility::Distance(Box, Target.Center);
					NxFr::Vector2f End = Target.Center - Line * NxFr::ShapeUtility::Distance(Target, Box.Center);

					Drawer->AddLine(Start, End, IM_COL32(Bright.r, Bright.g, Bright.b, Bright.a), 2.0f);
				}
			}
		}

		Drawer->PopClipRect();
	}

	void ReferencesPanel::HandleInputs(const NxFr::Rectangle& Canvas)
	{
		Inspected = 0;
		Inspect = false;

		if (ImGui::IsItemHovered())
		{
			if (Inputs->CheckAxis(NxEn::Input::Axis::ScrollY))
			{
				if (Inputs->GetAxis(NxEn::Input::Axis::ScrollY) < 0.0f)
					Zoom /= GetZoomFactor();
				else
					Zoom *= GetZoomFactor();

				ClampZoom();
			}
			if (Inputs->CheckButton(NxEn::Input::Button::MouseMiddle, NxEn::Input::State::Down))
			{
				NxFr::Vector2f Delta = Inputs->GetMouseDelta();
				Anchor += NxFr::Vector2f(Delta.x, Delta.y) / Zoom;
			}
			if (Inputs->CheckButton(NxEn::Input::Button::MouseRight))
			{
				Inspect = true;
				Mouse = Window->GetWindow().Position + Inputs->GetMousePosition();
			}
		}
	}

	void ReferencesPanel::HandleInspect()
	{
		if (Inspect && Inspected)
		{
			Select(Inspected, true);
		}
	}

	void ReferencesPanel::FetchNodes(NxFr::GUID Id)
	{
		auto GetLabel = [&](NxFr::GUID TargetId)
		{
			NxFr::StringView Label;
			if (Label.IsEmpty())
			{
				Label = Assets->IdToPath(TargetId);
			}
			if (Label.IsEmpty())
			{
				Label = Worlds->GetObject(TargetId)->GetName();
			}
			return Label;
		};
		auto GetDependencies = [&](NxFr::GUID TargetId)
		{
			NxFr::Array<NxFr::GUID> Dependencies;
			if (Assets->IsTracked(TargetId))
			{
				Dependencies = Assets->GetDependencies(TargetId, false);
			}
			if (Worlds->GetObject(TargetId))
			{
				Dependencies = Worlds->GetDependencies(TargetId, false);
			}
			return Dependencies;
		};

		NxFr::Set<NxFr::GUID> Ids = { Id };
		if (Full)
		{
			Ids.TryAppendRange(Assets->Find("*"));
			Ids.TryAppendRange(Worlds->Find("*"));
		}

		for (auto Id : Ids)
		{
			AddNode(Id, GetLabel(Id));

			NxFr::Array<NxFr::GUID> Dependencies = GetDependencies(Id);
			for (auto& Dependency : Dependencies)
			{
				AddNode(Dependency, GetLabel(Dependency));
				ConnectNode(Id, Dependency);
			}
		}
	}

	void ReferencesPanel::AddNode(NxFr::GUID Id, NxFr::StringView Label)
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

	void ReferencesPanel::ConnectNode(NxFr::GUID Id, NxFr::GUID Dependency)
	{
		Nodes.Connect(Ids[Id], Ids[Dependency]);
	}

	void ReferencesPanel::SortNodes(NxFr::GUID Id, int32 Depth, bool From, bool To)
	{
		if (!Ids.TryGet(Id) || Selection.TryGet(Id))
		{
			return;
		}

		Node* Instance = Ids[Id];
		Instance->Position = NxFr::Vector2f((float)Depth, 0.0f);
		Selection.Append(Id);

		uint64 Count = Nodes.GetConnectionCount(Instance);
		for (uint64 Index = 0; Index < Count && To; Index++)
		{
			Node* Link = Nodes.TryGetConnection(Instance, NxFr::Node::NodeGraphConnectionType::To, Index);
			if (Link)
			{
				SortNodes(Link->Id, Depth + 1, false, true);
			}
		}
		for (uint64 Index = 0; Index < Count && From; Index++)
		{
			Node* Link = Nodes.TryGetConnection(Instance, NxFr::Node::NodeGraphConnectionType::From, Index);
			if (Link)
			{
				SortNodes(Link->Id, Depth - 1, true, false);
			}
		}
	}

	void ReferencesPanel::AlignNodes()
	{
		NxFr::Dictionary<float, NxFr::List<Node*>> Instances;
		for (auto& Id : Selection)
		{
			Node* Instance = Ids[Id];
			NxFr::List<Node*>* List = Instances.TryGet(Instance->Position.x);
			if (!List)
			{
				List = &Instances.Append(Instance->Position.x, NxFr::List<Node*>());
			}
			List->Append(Instance);
		}

		for (auto [Depth, List] : Instances)
		{
			for (uint64 Index = 0; Index < List.GetCount(); ++Index)
			{
				List[Index]->Position.y = (float)Index - (List.GetCount() * 0.5f);
			}
		}
	}

	void ReferencesPanel::OnSelectionChanged(NxFr::GUID Id, bool State)
	{
		if (!State)
		{
			return;
		}

		Select(Id);
	}
}
