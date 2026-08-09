#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContextText.h"

#include "NexusEngine/Systems/Assets/Assets/Document.h"
#include "NexusEngine/Systems/Assets/Assets/Table.h"
#include "NexusEngine/Misc/GUI/Misc/Draw.h"

namespace NxEd
{
	static ViewerFactory::Factory* FactoryDocument = ViewerFactory::Register<NxEn::Document, ViewerContextText>();
	static ViewerFactory::Factory* FactoryTable = ViewerFactory::Register<NxEn::Table, ViewerContextText>();

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

			NX_LOG(Error, System, "Unsupported type for ViewerContextText")
		}
	}

	void ViewerContextText::OnDraw()
	{
		if (Mode == ViewContextMode::Document)
		{
			NxEn::GUI::Drawer<NxFr::String>::Field(Target.Document->GetText());
		}
		else if (Mode == ViewContextMode::Table)
		{
			if (NxEn::GUI::Drawer<YAML::Node>::Field(Target.Table->GetRoot(), "Root"))
			{
				Target.Table->SetDirty();
			}
		}
	}
}
