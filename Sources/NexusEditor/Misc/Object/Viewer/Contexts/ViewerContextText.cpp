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

	void ViewerContextText::Setup(NxEn::ObjectInstance<NxEn::Object> Target)
	{
		NX_ASSERT_RETURN(Target->GetObjectType() == NxEn::Document::GetClassType() || Target->GetObjectType() == NxEn::Table::GetClassType()
			, , Default, "Viewer Text expect a Document or a Table");

		Instance = Target.Get();
		if (Instance->GetObjectType() == NxEn::Document::GetClassType())
		{
			ViewMode = Mode::Document;
		}
		else if (Instance->GetObjectType() == NxEn::Table::GetClassType())
		{
			ViewMode = Mode::Table;
		}
		else
		{
			ViewMode = Mode::Invalid;
		}
	}

	void ViewerContextText::Draw()
	{
		if (ViewMode == Mode::Document)
		{
			NxEn::GUI::Transform Transform = NxEn::GUI::Transform(-NxFr::Vector2f::One, NxEn::GUI::Utils::Fill(), -1.0f);
			if (NxEn::GUI::Drawer<NxFr::String>::Field(Document->GetText(), "", "Text", Transform))
			{
				Document->SetDirty();
			}
		}
		else if (ViewMode == Mode::Table)
		{
			if (NxEn::GUI::Drawer<YAML::Node>::Field(Table->GetRoot(), "Root")) 
			{
				Table->SetDirty();
			}
		}
	}
}
