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

	void Text::Load(NxFr::StringView FilePath)
	{
		Resource::Load(FilePath);

		NxFr::File File(FilePath);
		File.Open(NxFr::File::Mode::Read, false);
		Data = File.ReadText();
		File.Close();
	}

	void Text::Unload()
	{
		Data = NxFr::String();

		Resource::Unload();
	}
}
