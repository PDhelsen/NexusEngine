#include "NexusEditor/Systems/Assets/ReferencesViewer/AssetsReferencesPanel.h"

namespace NxEd
{
	static AssetsReferencesPanel*Panel = NxEn::GUI::Panel::Create<AssetsReferencesPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/References", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsReferencesPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsReferencesPanel)

	AssetsReferencesPanel::AssetsReferencesPanel()
		: Inputs(nullptr), Menu(), Style(), Buffer(64), Anchor(), Zoom(1.0f), Recenter(true), References(), Nodes(), Selection(), Selected(0), Inspected(0), Inspect(false), Full(false)
	{
	}

	AssetsReferencesPanel::~AssetsReferencesPanel()
	{
	}

	void AssetsReferencesPanel::Refresh()
	{
		Select(Selected);
	}

	void AssetsReferencesPanel::Center()
	{
		Recenter = true;
	}

	void AssetsReferencesPanel::Select(NxFr::GUID Id)
	{
		Selection.Clear();
		Selected = Id;

		Buffer.Clear();
		Buffer += NxFr::StringUtility::ToString(Id);

		if (Selected == 0)
		{
			return;
		}

		FetchDependencies();
		SelectNode(Id);
		AlignNodes();
		Center();
	}

	void AssetsReferencesPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Assets References");

		Menu.AddMenuItem("Refresh", { this, &AssetsReferencesPanel::Refresh }, 0);
		Menu.AddMenuItem("Center", { this, &AssetsReferencesPanel::Center }, 1);
		Menu.AddMenuToggle("Full", &Full, nullptr, 2);
	}

	void AssetsReferencesPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void AssetsReferencesPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);

		Inputs = NxEn::Application::GetSystem<NxEn::InputSystem>();

		Style.Reset();
		Style.Width = 200.0f;
		Style.WidthLabel = -1.0f;

		Full = false;
		Select(0);
	}

	void AssetsReferencesPanel::OnDisable()
	{
		Panel::OnDisable();
		Menu.SetEnabled(false);

		References.Clear();
		Nodes.Clear();
		Selection.Clear();
	}

	void AssetsReferencesPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		DrawProperties();
		DrawNodes();
	}

	void AssetsReferencesPanel::DrawProperties()
	{
		NxFr::Vector2f Size = NxFr::Vector2f(0.0f, ImGui::GetTextLineHeight() * 5.0f);

		ImGui::BeginChild("Properties", Size, ImGuiChildFlags_Border);

		NxEn::GUI::Drawer<uint64>::Property(References.GetCount(), "Tracked", &Style);
		if (NxEn::GUI::Drawer<NxFr::String>::Field(Buffer, "Selected", "IdSelected", &Style))
		{
			Select(NxFr::StringUtility::FromString<NxFr::GUID>(Buffer));
		}

		ImGui::EndChild();
	}

	void AssetsReferencesPanel::DrawNodes()
	{
		NxFr::Rectangle Rect = CreateCanvas();
		HandleInputs(Rect);

		for (const auto& N : Selection)
		{
			DrawNode(Rect, *N);
		}

		HandleInspect();
	}

	NxFr::Rectangle AssetsReferencesPanel::CreateCanvas()
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

	void AssetsReferencesPanel::HandleInputs(const NxFr::Rectangle& Canvas)
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
				Mouse = Inputs->GetMousePosition(true);
			}
		}
	}

	void AssetsReferencesPanel::HandleInspect()
	{
		if (Inspect)
		{
			Select(Inspected);
		}
	}

	void AssetsReferencesPanel::DrawNode(const NxFr::Rectangle& Canvas, const Node& Instance)
	{
		ImDrawList* Drawer = ImGui::GetWindowDrawList();
		Drawer->PushClipRect(Canvas.GetBottomLeft(), Canvas.GetTopRight());

		NxFr::Colors::Bits Background = GetColorBackground();
		NxFr::Colors::Bits Border = *Instance.Id == Selected ? GetColorBright() : GetColorBorder();
		NxFr::Colors::Bits Bright = GetColorBright();

		NxFr::Rectangle Box = NxFr::Rectangle(Anchor + Instance.Position * GetRectangleExtents() * 2.25f, GetRectangleExtents());

		if (Inspect && NxFr::ShapeUtility::Contains(Box, Mouse))
		{
			Inspected = *Instance.Id;
		}

		Drawer->AddRectFilled(Box.GetBottomLeft(), Box.GetTopRight(), IM_COL32(Background.r, Background.g, Background.b, Background.a), 1);
		Drawer->AddRect(Box.GetBottomLeft(), Box.GetTopRight(), IM_COL32(Border.r, Border.g, Border.b, Border.a), 1);

		Drawer->PushClipRect(Box.GetBottomLeft(), Box.GetTopRight(), true);
		Drawer->AddText(Box.GetBottomLeft() + GetTextOffset(), IM_COL32(Bright.r, Bright.g, Bright.b, Bright.a), Instance.Label.C());
		Drawer->PopClipRect();

		uint64 Count = References.GetConnectionCount(Instance.Id);
		for (uint64 Index = 0; Index < Count; Index++)
		{
			NxFr::GUID* Link = References.TryGetConnection(Instance.Id, NxFr::Node::NodeGraphConnectionType::To, Index);
			if (Link)
			{
				Node& Dependency = Nodes[*Link];
				if (Selection.Contains(&Dependency))
				{
					NxFr::Rectangle Target = NxFr::Rectangle(Anchor + Dependency.Position * GetRectangleExtents() * 2.25f, GetRectangleExtents());

					NxFr::Vector2f Line = NxFr::VectorUtility::Normalize(Target.Center - Box.Center);
					NxFr::Vector2f Start = Box.Center + Line * NxFr::ShapeUtility::Distance(Box, Target.Center);
					NxFr::Vector2f End = Target.Center - Line * NxFr::ShapeUtility::Distance(Target, Box.Center);
					
					Drawer->AddLine(Start, End, IM_COL32(Bright.r, Bright.g, Bright.b, Bright.a), 2.0f);
				}
			}
		}

		Drawer->PopClipRect();
	}

	void AssetsReferencesPanel::FetchDependencies()
	{
		if (!Full && Selected == 0)
		{
			return;
		}

		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::Array<NxFr::GUID> Assets = Full ? System->Find("*") : NxFr::Array<NxFr::GUID>({ Selected });

		Nodes.Clear();
		References.Clear();
		for (auto& Asset : Assets)
		{
			NxFr::Array<NxFr::GUID> Dependencies = System->GetDependencies(Asset, false);

			AddNode(Asset, System);

			for (auto& Dependency : Dependencies)
			{
				AddNode(Dependency, System);

				References.Connect(Nodes[Asset].Id, Nodes[Dependency].Id);
			}
		}
	}

	void AssetsReferencesPanel::AddNode(NxFr::GUID Id, NxEn::AssetsSystem* System)
	{
		if (Nodes.ContainsKey(Id))
		{
			return;
		}

		NxFr::GUID* Node = &References.Append(Id);
		NxFr::String Label = System->IdToPath(Id) + NxFr::StringUtility::NewLine + NxFr::StringUtility::ToString(Id);
		Nodes.AppendConstruct(NxFr::Move(Id), NxFr::Move(Node), NxFr::Move(Label));
	}

	void AssetsReferencesPanel::SelectNode(NxFr::GUID Id, int32 Depth, bool From, bool To)
	{
		if (Selection.Contains(&Nodes[Id]))
		{
			return;
		}

		Node* Instance = Selection.Append(&Nodes[Id]);
		Instance->Position = NxFr::Vector2i(Depth, 0);

		uint64 Count = References.GetConnectionCount(Instance->Id);
		for (uint64 Index = 0; Index < Count && To; Index++)
		{
			NxFr::GUID* Link = References.TryGetConnection(Instance->Id, NxFr::Node::NodeGraphConnectionType::To, Index);
			if (Link)
			{
				SelectNode(*Link, Depth + 1, false, true);
			}
		}
		for (uint64 Index = 0; Index < Count && From; Index++)
		{
			NxFr::GUID* Link = References.TryGetConnection(Instance->Id, NxFr::Node::NodeGraphConnectionType::From, Index);
			if (Link)
			{
				SelectNode(*Link, Depth - 1, true, false);
			}
		}
	}

	void AssetsReferencesPanel::AlignNodes()
	{
		NxFr::Dictionary<float, NxFr::List<Node*>> Instances;
		for (auto& S : Selection)
		{
			NxFr::List<Node*>* List = Instances.TryGet(S->Position.x);
			if (!List)
			{
				List = &Instances.Append(S->Position.x, NxFr::List<Node*>());
			}

			List->Append(S);
		}

		for (auto [Depth, List] : Instances)
		{
			for (uint64 Index = 0; Index < List.GetCount(); ++Index)
			{
				List[Index]->Position.y = (float)Index - (List.GetCount() * 0.5f);
			}
		}
	}
}
