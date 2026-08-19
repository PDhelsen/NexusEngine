#include "NexusEditor/Misc/Object/Viewer/Contexts/ViewerContextText.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"
#include "NexusEngine/Misc/GUI/Misc/Draw.h"

namespace NxEd
{
	NxFr::Factory<ViewerContext>::Creator* FactoryDocument = ViewerPanel::GetFactory().Register<ViewerContextText>(NxEn::Document::GetClassType());
	NxFr::Factory<ViewerContext>::Creator* FactoryTable = ViewerPanel::GetFactory().Register<ViewerContextText>(NxEn::Table::GetClassType());


	void ViewerContextText::Clear()
	{

	}

	void ViewerContextText::Setup(NxEn::Object* Instance)
	{
		NX_ASSERT_RETURN(Instance->GetObjectType() == NxEn::Document::GetClassType() || Instance->GetObjectType() == NxEn::Table::GetClassType()
			, , Default, "Viewer Text expect a Document or a Table");

		Target = Instance;
	}

	void ViewerContextText::Draw()
	{
		if (Target->GetObjectType() == NxEn::Document::GetClassType())
		{
			NxEn::Document* Document = static_cast<NxEn::Document*>(Target);
			NxEn::GUI::Transform Transform = NxEn::GUI::Transform(-NxFr::Vector2f::One, NxEn::GUI::Utils::Fill(), -1.0f);
			if (NxEn::GUI::Drawer<NxFr::String>::Field(Document->GetText(), "", "Text", Transform))
			{
				Document->SetDirty();
			}
		}
		else if (Target->GetObjectType() == NxEn::Table::GetClassType())
		{
			NxEn::Table* Table = static_cast<NxEn::Table*>(Target);
			if (NxEn::GUI::Drawer<YAML::Node>::Field(Table->GetRoot(), "Root"))
			{
				Table->SetDirty();
			}
		}
	}
}
