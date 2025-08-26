#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/StatsPanel.h"

namespace NxEn
{
	static NxFr::StringId ButtonNormal = "Button_Normal"_Sid;
	static NxFr::StringId ButtonPressed = "Button_Pressed"_Sid;
	static NxFr::String Filter = 256;
	static NxFr::StringView FilterExclude[] = { NxFr::StatsHeader::TickId.C(), NxFr::StatsHeader::CommentId.C() };

	static StatsPanel* Panel = GUI::Panel::Create<StatsPanel>();

	NEXUS_OBJECT_IMPLEMENTATION(StatsPanel)

	StatsPanel::StatsPanel()
		: Instruments(nullptr), Stats(nullptr), Filters(), Ids(), Values()
	{
	}

	StatsPanel::~StatsPanel()
	{
	}

	void StatsPanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Statistiques";
	}

	void StatsPanel::OnEnable()
	{
		Panel::OnEnable();

		DebugSystem* Debug = Application::GetSystem<DebugSystem>();
		Instruments = Debug->GetInstrumentor();
		Stats = Debug->GetStats();

		Values = Stats->GetAllCurrentStats();
		Ids = NxFr::List<const NxFr::String*>(Values.GetCount());
		for (auto& [Id, Value] : Values)
		{
			Ids.Append(&Id.ToString());
		}
		Ids.Sort([](const NxFr::String* A, const NxFr::String* B) { return *A <= *B; });
	}

	void StatsPanel::OnGui(float TimeStep)
	{
		DrawButtons();
		ImGui::SameLine();
		DrawFilter();

		for (uint64 Index = 0; Index < Ids.GetCount(); ++Index)
		{
			const NxFr::String* Label = Ids[Index];
			if (!FilterStats(*Label))
			{
				continue;
			}

			DrawStats(*Label);
		}
	}

	void StatsPanel::DrawButtons()
	{
		static float ButtonWidth = 100.0f;

		{
			GUI::Scope::Style Style(!Instruments->IsRecording() ? ButtonNormal : ButtonPressed);
			if (ImGui::Button("Instruments", { ButtonWidth , 0 }))
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
			GUI::Scope::Style Style(!Stats->IsRecording() ? ButtonNormal : ButtonPressed);
			if (ImGui::Button("Stats", { ButtonWidth , 0 }))
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
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Filter:");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##Filter", Filter.C_Buffer(), Filter.GetCapacity());
		ImGui::PopItemWidth();
		ImGui::Separator();

		Filter.Validate();
		Filters = NxFr::StringUtility::SplitAll(Filter, ",");
	}

	void StatsPanel::DrawStats(const NxFr::String& Label) const
	{
		NxFr::StringId Id = NxFr::StringId(Label);
		const NxFr::Stats::Stat* Value = Values[Id];

		switch (Value->GetType())
		{
		case NxFr::Stats::StatType::Label: ImGui::Text("%s: %s", Label.C(), Value->GetValue<const NxFr::String&>().C()); break;
		case NxFr::Stats::StatType::Check: ImGui::Text("%s: %s", Label.C(), Value->GetValue<bool>() ? "true" : "false"); break;
		case NxFr::Stats::StatType::Integer: ImGui::Text("%s: %d", Label.C(), Value->GetValue<uint64>()); break;
		case NxFr::Stats::StatType::UnsignedInteger: ImGui::Text("%s: %d", Label.C(), Value->GetValue<uint64>()); break;
		case NxFr::Stats::StatType::Decimal: ImGui::Text("%s: %f", Label.C(), Value->GetValue<float>()); break;
		case NxFr::Stats::StatType::DecimalPrecision: ImGui::Text("%s: %f", Label.C(), Value->GetValue <double>()); break;
		}
	}

	bool StatsPanel::FilterStats(const NxFr::String& Label)
	{
		for (uint64 Index = 0; Index < FilterExclude->GetCount(); ++Index)
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
			if (Label.Contains(Filters[Index]))
			{
				return true;
			}
		}

		return false;
	}
}
