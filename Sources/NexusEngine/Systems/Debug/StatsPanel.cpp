#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/StatsPanel.h"

namespace NxEn
{
	static NxFr::Array<NxFr::StringView> FilterExclude = { NxFr::StatsHeader::TickId.C(), NxFr::StatsHeader::CommentId.C() };

	static StatsPanel* Panel = GUI::Panel::Create<StatsPanel>();

	NEXUS_OBJECT_IMPLEMENTATION(StatsPanel)

	StatsPanel::StatsPanel()
		: Style(), Instruments(nullptr), Stats(nullptr), Ids(), Values(), Filters(), Filter()
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
		Style.Flag = ImGuiInputTextFlags_EnterReturnsTrue;

		DebugSystem* Debug = Application::GetSystem<DebugSystem>();
		Instruments = Debug->GetInstrumentor();
		Stats = Debug->GetStats();

		Values = Stats->GetAllCurrentStats();
		Ids = NxFr::List<NxFr::String>(Values.GetCount());
		for (auto& [Id, Value] : Values)
		{
			Ids.Append(Id.GetString());
		}
		Ids.Sort();
	}

	void StatsPanel::OnGui(float TimeStep)
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
			GUI::Scope Style(!Instruments->IsRecording() ? GUI::Style::IdButton_Normal : GUI::Style::IdButton_Pressed);
			if (ImGui::Button("Instruments", { GUI::Style::GetVar(GUI::Style::IdWidthButton) , 0 }))
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
			GUI::Scope Style(!Stats->IsRecording() ? GUI::Style::IdButton_Normal : GUI::Style::IdButton_Pressed);
			if (ImGui::Button("Stats", { GUI::Style::GetVar(GUI::Style::IdWidthButton) , 0 }))
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
		Style.Width = -1.0f;
		Style.WidthLabel = 0.0f;

		if (GUI::Drawer<NxFr::String>::Field(Filter, "Filter:", "", &Style))
		{
			Filters = NxFr::StringUtility::SplitAll(Filter, ",");
		}

		ImGui::Separator();
	}

	void StatsPanel::DrawStats(const NxFr::String& Label)
	{
		Style.Width = 0.0f;
		Style.WidthLabel = -1.0f;

		NxFr::StringId Id = NxFr::StringId(Label);
		const NxFr::Stats::Stat* Value = Values[Id];

		switch (Value->GetType())
		{
		case NxFr::Stats::StatType::Label: GUI::Drawer<NxFr::String>::Property(Value->GetValue<const NxFr::String&>(), Label, &Style); break;
		case NxFr::Stats::StatType::Check: GUI::Drawer<bool>::Property(Value->GetValue<bool>(), Label, &Style); break;
		case NxFr::Stats::StatType::Integer: GUI::Drawer<int64>::Property(Value->GetValue<int64>(), Label, &Style); break;
		case NxFr::Stats::StatType::UnsignedInteger: GUI::Drawer<uint64>::Property(Value->GetValue<uint64>(), Label, &Style); break;
		case NxFr::Stats::StatType::Decimal: GUI::Drawer<float>::Property(Value->GetValue<float>(), Label, &Style); break;
		case NxFr::Stats::StatType::DecimalPrecision: GUI::Drawer<double>::Property(Value->GetValue<double>(), Label, &Style); break;
		}
	}

	bool StatsPanel::FilterStats(const NxFr::String& Label)
	{
		for (uint64 Index = 0; Index < FilterExclude.GetCount(); ++Index)
		{
			if (Label == FilterExclude[Index])
			{
				return false;
			}
		}

		if (Filters.IsEmpty())
		{
			return true;
		}
		
		for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
		{
			if (NxFr::StringUtility::Contains(Label, Filters[Index]))
			{
				return true;
			}
		}

		return false;
	}
}
