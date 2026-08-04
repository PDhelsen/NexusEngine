#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/StatsPanel.h"

namespace NxEn
{
	static NxFr::Array<NxFr::StringView> FilterExclude = { NxFr::StatsHeader::TickId.C(), NxFr::StatsHeader::CommentId.C() };

	static StatsPanel* Panel = GUI::Panel::Create<StatsPanel>();

	StatsPanel::StatsPanel()
		: Style(), Instruments(nullptr), Stats(nullptr), Ids(), Values(), Filter()
	{
	}

	StatsPanel::~StatsPanel()
	{
	}

	void StatsPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetTitle("Statistiques");
	}

	void StatsPanel::OnEnable()
	{
		Panel::OnEnable();
		Style.Reset();

		DebugSystem* Debug = Application::GetSystem<DebugSystem>();
		Instruments = Debug->GetInstruments();
		Stats = Debug->GetStats();

		Values = Stats->GetStats();
		Ids = NxFr::List<NxFr::String>(Values.GetCount());
		for (auto& [Id, Value] : Values)
		{
			Ids.Append(Id.GetString());
		}
		NxFr::ContainerUtility::Sort<NxFr::String>(Ids);
	}

	void StatsPanel::OnDraw()
	{
		DrawButtons();
		ImGui::SameLine();
		DrawFilter();

		for (uint64 Index = 0; Index < Ids.GetCount(); ++Index)
		{
			const NxFr::String& Label = Ids[Index];
			if (!FilterStats(Label))
			{
				continue;
			}

			DrawStats(Label);
		}
	}

	void StatsPanel::DrawButtons()
	{
		{
			Style = !Instruments->IsRecording() ? GUI::Styles::ButtonNormal() : GUI::Styles::ButtonPressed();
			Style.Size.x = GUI::Styles::WidthButton();
			if (GUI::Draw::Button("Instruments", &Style))
			{
				if (!Instruments->IsRecording())
				{
					Instruments->StartRecording();
				}
				else
				{
					Instruments->StopRecording();
				}
			}
		}

		ImGui::SameLine();

		{
			Style = !Instruments->IsRecording() ? GUI::Styles::ButtonNormal() : GUI::Styles::ButtonPressed();
			Style.Size.x = GUI::Styles::WidthButton();
			if (GUI::Draw::Button("Stats", &Style))
			{
				if (!Stats->IsRecording())
				{
					Stats->StartRecording();
				}
				else
				{
					Stats->StopRecording();
				}
			}
		}
	}

	void StatsPanel::DrawFilter()
	{
		Style.Size.x = -1.0f;
		Style.Label = -1.0f;

		if (GUI::Drawer<NxFr::String>::Field(Filter.GetQuery(), "Filter:", "", &Style))
		{
			Filter.Configure();
		}

		ImGui::Separator();
	}

	void StatsPanel::DrawStats(NxFr::StringView Label)
	{
		Style.Size.x = 0.0f;
		Style.Label = GUI::Styles::WidthLabel();

		NxFr::StringId Id = NxFr::StringId(Label);
		const NxFr::Stats::Stat* Value = Values[Id];

		switch (Value->GetType())
		{
		case NxFr::Stats::Type::Label: GUI::Drawer<NxFr::String>::Property(Value->GetValueLabel(), Label, &Style); break;
		case NxFr::Stats::Type::Check: GUI::Drawer<bool>::Property(Value->GetValueCheck(), Label, &Style); break;
		case NxFr::Stats::Type::Integer: GUI::Drawer<int64>::Property(Value->GetValueInteger(), Label, &Style); break;
		case NxFr::Stats::Type::Decimal: GUI::Drawer<float>::Property(Value->GetValueDecimal(), Label, &Style); break;
		}
	}

	bool StatsPanel::FilterStats(NxFr::StringView Label)
	{
		for (uint64 Index = 0; Index < FilterExclude.GetCount(); ++Index)
		{
			if (Label == FilterExclude[Index])
			{
				return false;
			}
		}

		if (Filter.IsEmpty())
		{
			return true;
		}
		
		return Filter.FilterInstance(Label, 0, 0);
	}
}
