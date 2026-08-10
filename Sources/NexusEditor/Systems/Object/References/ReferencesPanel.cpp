#include "NexusEditor/Systems/Object/References/ReferencesPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	static ReferencesPanel* Panel = NxEn::GUI::Panel::Create<ReferencesPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemReferences = NxEn::GUI::Menu::Create("Object/Objects/References", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ReferencesPanel");
	}));

	void ReferencesPanel::Select(NxFr::GUID Id)
	{
		if (Lock)
		{
			return;
		}

		Clear();

		Selected = 0;
		if (Id == 0)
		{
			return;
		}

		FetchNodes(Id);
		SortNodes(Id);
		AlignNodes();

		NodePanel::Select(Id);
	}

	void ReferencesPanel::OnInitialize()
	{
		NodePanel::OnInitialize();
		Lock = false;

		Menu.AddMenuToggle("Lock", &Lock, nullptr, 2);
	}

	void ReferencesPanel::OnEnable()
	{
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		AssetsBrowserPanel* Browser = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<AssetsBrowserPanel>();
		if (Browser)
		{
			Edit::Context* Ctx = Edit->GetContext(Browser->GetNamedId());
			if (Ctx)
			{
				Ctx->GetOnSelectionChanged() += { this, &ReferencesPanel::OnSelectionChanged };
			}
		}

		NodePanel::OnEnable();
	}

	void ReferencesPanel::OnDisable()
	{
		AssetsBrowserPanel* Browser = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<AssetsBrowserPanel>();
		if (Browser)
		{
			Edit::Context* Ctx = Edit->GetContext(Browser->GetNamedId());
			if (Ctx)
			{
				Ctx->GetOnSelectionChanged() -= { this, &ReferencesPanel::OnSelectionChanged };
			}
		}

		NodePanel::OnDisable();
	}

	void ReferencesPanel::OnDraw()
	{
		Menu.Draw();
		DrawHeader();

		DrawCanvas();
		DrawNodes();

		HandleInputs();
	}

	void ReferencesPanel::DrawHeader()
	{
		NxFr::Vector2f Size = NxFr::Vector2f(0.0f, ImGui::GetTextLineHeight() * 5.0f);
		ImGui::BeginChild("References", Size, ImGuiChildFlags_Border);

		NxEn::GUI::Drawer<uint64>::Property(Nodes.GetCount(), "Tracked");
		if (NxEn::GUI::Drawer<NxFr::GUID>::Field(Selected, "Selected", "IdSelected"))
		{
			Select(Selected);
		}

		ImGui::EndChild();
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

	void ReferencesPanel::SortNodes(NxFr::GUID Id, int32 Depth, bool From, bool To)
	{
		Node* Instance = Ids[Id];
		Instance->Position = NxFr::Vector2f((float)Depth, 0.0f);

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
		for (auto [Id, Instance] : Ids)
		{
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
