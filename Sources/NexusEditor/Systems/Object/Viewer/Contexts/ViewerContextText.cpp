#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContextText.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"

namespace NxEd
{
	static ViewerFactory::Factory* FactoryDocument = ViewerFactory::Register<NxEn::Document, ViewerContextText>();
	static ViewerFactory::Factory* FactoryTable = ViewerFactory::Register<NxEn::Table, ViewerContextText>();

	static void DrawYamlNode(YAML::Node Node, NxFr::StringView Label)
	{
		if (!Node)
		{
			ImGui::Text("%s: null", Label.C());
			return;
		}

		switch (Node.Type())
		{
		case YAML::NodeType::Scalar:
		{
			ImGui::PushID(Label.C());
			ImGui::Text("%s: %s", Label.C(), Node.as<NxFr::String>().C());
			ImGui::PopID();
			break;
		}

		case YAML::NodeType::Sequence:
		{
			ImGui::PushID(Label.C());
			if (ImGui::TreeNode(Label.C()))
			{
				for (uint64 Index = 0; Index < Node.size(); ++Index)
				{
					ImGui::PushID(static_cast<int>(Index));
					DrawYamlNode(Node[Index], "[" + NxFr::StringUtility::ToString(Index) + "]");
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			break;
		}

		case YAML::NodeType::Map:
		{
			ImGui::PushID(Label.C());
			if (ImGui::TreeNode(Label.C()))
			{
				for (auto It = Node.begin(); It != Node.end(); ++It)
				{
					NxFr::String Key = It->first.as<NxFr::String>();
					ImGui::PushID(Key.C());
					DrawYamlNode(It->second, Key);
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			break;
		}

		default:
			ImGui::Text("%s: <unknown>", Label.C());
			break;
		}
	}

	ViewerContextText::ViewerContextText()
		: Mode(), Target()
	{

	}

	ViewerContextText::~ViewerContextText()
	{
		Clear();
	}

	void ViewerContextText::Clear()
	{

	}

	void ViewerContextText::SetupMenu(NxEn::GUI::Menu& Menu)
	{
	}

	void ViewerContextText::SetupTarget(NxEn::Object* Instance)
	{
		if (Instance->GetObjectType() == NxEn::Document::GetClassType())
		{
			Mode = ViewContextMode::Document;

			Target.Document = static_cast<NxEn::Document*>(Instance);
		}
		else if (Instance->GetObjectType() == NxEn::Table::GetClassType())
		{
			Mode = ViewContextMode::Table;

			Target.Table = static_cast<NxEn::Table*>(Instance);
		}
		else
		{
			Mode = ViewContextMode::Invalid;

			NEXUS_LOG(Error, System, "Unsupported type for ViewerContextText")
		}
	}

	void ViewerContextText::OnGui(float TimeStep)
	{
		if (Mode == ViewContextMode::Document)
		{
			NxEn::GUI::Drawer<NxFr::StringView>::Property(Target.Document->GetText());
		}
		else if (Mode == ViewContextMode::Table)
		{
			DrawYamlNode(Target.Table->GetRoot(), "Root");
		}
	}
}
