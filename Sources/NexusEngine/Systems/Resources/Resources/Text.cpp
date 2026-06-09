#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Resources/Resources/Text.h"

namespace NxEn
{
	Text::Text(NxFr::StringView Path)
		: Resource(Path), Data()
	{
	}

	Text::~Text()
	{
	}

	void Text::OnSave(NxFr::StringView FilePath) const
	{
		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Write, false);
		File.WriteText(Data);
		File.Close();
	}

	void Text::OnLoad(NxFr::StringView FilePath)
	{
		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Read, false);
		Data = File.ReadText();
		File.Close();
	}

	void Text::OnUnload()
	{
		Data = NxFr::String();
	}
}
