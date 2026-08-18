#include "NexusEditor/Misc/Object/References/ReferencesPanel.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

namespace NxEd
{
	static ReferencesPanel* Panel = NxEn::GUI::Panel::Create<ReferencesPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemReferences = NxEn::GUI::Menu::Create("Object/Objects/References", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ReferencesPanel");
	}));

	void ReferencesPanel::Refresh()
	{
		FetchIds();
		NodePanel::Refresh();
	}

	void ReferencesPanel::Select(NxFr::GUID Id)
	{
		Clear();

		Selected = 0;
		if (!ObjectsIds.TryGet(Id))
		{
			return;
		}

		CreateNodes(Id);

		NodePanel::Select(Id);
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

		NxEn::GUI::Drawer<uint64>::Property(ObjectsIds.GetCount(), "Tracked");
		if (NxEn::GUI::Drawer<NxFr::GUID>::Field(Selected, "Selected", "IdSelected"))
		{
			Select(Selected);
		}

		ImGui::EndChild();
	}

	void ReferencesPanel::FetchIds()
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();

		auto GetLabel = [&](NxFr::GUID TargetId)
		{
			NxFr::String Label;
			if (Label.IsEmpty() && Assets->IsTracked(TargetId))
			{
				NxEn::AssetMetadata* MetaData = Assets->GetMetadata(TargetId);
				Label = NxFr::StringUtility::FormatTo("%s (%s)", MetaData->GetPath().C(), MetaData->GetType().C());
			}
			if (Label.IsEmpty() && Worlds->GetObject(TargetId))
			{
				NxFr::Handle<NxEn::Object> Object = Worlds->GetObject(TargetId);
				Label = NxFr::StringUtility::FormatTo("%s (%s)", Object->GetName().C(), Object->GetObjectType().C());
			}

			Label += NxFr::StringUtility::NewLine + NxFr::StringUtility::ToString(TargetId);
			return Label;
		};

		auto CreateObjectId = [&](NxFr::GUID TargetId)
		{
			if (!ObjectsIds.TryGet(TargetId))
			{
				NxFr::Handle<NxEn::Object> WorldInstance = Worlds->GetObject(TargetId);
				NxEn::WorldObjectType WorldType = WorldInstance ? Worlds->GetType(WorldInstance) : NxEn::WorldObjectType::None;

				Type ObjectType = WorldType == NxEn::WorldObjectType::GameObject ? Type::GameObject :
					WorldType == NxEn::WorldObjectType::Behaviour ? Type::Behaviour :
					WorldType == NxEn::WorldObjectType::Component ? Type::Component : Type::Asset;

				Info Info =
				{
					.ObjectType = ObjectType,
					.Label = GetLabel(TargetId),
					.Links = Connections(NxFr::Set<NxFr::GUID>(), NxFr::Set<NxFr::GUID>())
				};

				ObjectsIds.Append(TargetId, Info);
			}
		};

		auto RecordDependency = [&](NxFr::GUID TargetId, NxFr::GUID DependencyId, bool Both)
		{
			ObjectsIds[TargetId].Links.GetSecond().TryAppend(DependencyId);
			if (Both)
			{
				ObjectsIds[DependencyId].Links.GetFirst().TryAppend(TargetId);
			}
		};

		ObjectsIds.Clear();
		NxFr::Set<NxFr::GUID> Dependencies;

		NxFr::Array<NxFr::GUID> AssetsIds = Assets->Find("*");
		for (auto AssetId : AssetsIds)
		{
			CreateObjectId(AssetId);

			Dependencies.Clear();
			Assets->GetDependencies(AssetId, false, Dependencies);

			for (auto Dependency : Dependencies)
			{
				CreateObjectId(Dependency);
				RecordDependency(AssetId, Dependency, true);
			}
		}

		NxFr::Array<NxFr::GUID> WorldsIds = Worlds->Find("*", NxEn::WorldObjectType::All, NxEn::WorldSystem::MainWorldId);
		for (auto WorldId : WorldsIds)
		{
			NxFr::Handle<NxEn::Object> Instance = Worlds->GetObject(WorldId);
			bool IsGameObject = Worlds->GetType(Instance) == NxEn::WorldObjectType::GameObject;
			NxFr::GUID TemplateId = IsGameObject ? Worlds->Cast<NxEn::GameObject>(Instance)->GetTemplateId() : 0;

			CreateObjectId(WorldId);

			Dependencies.Clear();
			Worlds->GetDependencies(WorldId, false, Dependencies);

			for (auto Dependency : Dependencies)
			{
				CreateObjectId(Dependency);
				RecordDependency(WorldId, Dependency, !IsGameObject || TemplateId == Dependency);
			}
		}
	}

	void ReferencesPanel::CreateNodes(NxFr::GUID Id)
	{
		Info* Target = ObjectsIds.TryGet(Id);
		if (!Target)
		{
			return;
		}

		AddNode(Id, Target->Label);

		uint64 Index = 0;
		float Offset = (Target->Links.GetFirst().GetCount() - 1) * 0.5f;
		for (auto FromId : Target->Links.GetFirst())
		{
			Info* Depencency = ObjectsIds.TryGet(FromId);
			if (!Depencency)
			{
				continue;
			}

			AddNode(FromId, Depencency->Label);
			ConnectNode(FromId, Id);
			MoveNode(FromId, NxFr::Vector2f(-1.0f, Index++ - Offset));
		}

		Index = 0;
		Offset = (Target->Links.GetSecond().GetCount() - 1) * 0.5f;
		for (auto ToId : Target->Links.GetSecond())
		{
			Info* Depencency = ObjectsIds.TryGet(ToId);
			if (!Depencency)
			{
				continue;
			}

			AddNode(ToId, Depencency->Label);
			ConnectNode(Id, ToId);
			MoveNode(ToId, NxFr::Vector2f(1.0f, Index++ - Offset));
		}
	}
}
