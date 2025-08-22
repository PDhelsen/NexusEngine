#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct Style
		{
			friend class GUISystem;

			enum class Type
			{
				Color, Var, VarX, VarY, VarXY
			};

			struct Property
			{
				Type Flag;
				NxFr::Vector4f Data;
			};

		public:
			NEXUS_ENGINE_API Style();
			NEXUS_ENGINE_API ~Style();

			NEXUS_ENGINE_API void Push() const;
			NEXUS_ENGINE_API void Pop() const;

			NEXUS_ENGINE_API Style& AppendColor(int32 Id, NxFr::Color Data) { Append(Id, Type::Color, Data.ToVector4f()); return *this; }
			NEXUS_ENGINE_API Style& AppendVar(int32 Id, float Data) { Append(Id, Type::Var, NxFr::Vector4f(Data)); return *this; }
			NEXUS_ENGINE_API Style& AppendVarX(int32 Id, float Data) { Append(Id, Type::VarX, NxFr::Vector4f(Data, 0.0f)); return *this; }
			NEXUS_ENGINE_API Style& AppendVarY(int32 Id, float Data) { Append(Id, Type::VarY, NxFr::Vector4f(0.0f, Data)); return *this; }
			NEXUS_ENGINE_API Style& AppendVarXY(int32 Id, NxFr::Vector2f Data) { Append(Id, Type::VarXY, Data); return *this; }

			NEXUS_ENGINE_API Style& RemoveColor(int32 Id) { Remove(Id); return *this; }
			NEXUS_ENGINE_API Style& RemoveVar(int32 Id) { Remove(Id); return *this; }
			NEXUS_ENGINE_API Style& RemoveVarX(int32 Id) { Remove(Id); return *this; }
			NEXUS_ENGINE_API Style& RemoveVarY(int32 Id) { Remove(Id); return *this; }
			NEXUS_ENGINE_API Style& RemoveVarXY(int32 Id) { Remove(Id); return *this; }

		private:
			void Append(int32 Id, Type Flag, NxFr::Vector4f Data);
			void Remove(int32 Id);
			void Apply(int32 Id, Type Flag, NxFr::Vector4f Data) const;

		private:
			NxFr::Dictionary<int32, Property> Properties;
			NxFr::Vector2i Counts;
		};

		namespace Scope
		{
			struct Color
			{
				NEXUS_ENGINE_API Color(ImGuiCol Id, NxFr::Color Value);
				NEXUS_ENGINE_API ~Color();
			};

			struct Var
			{
				NEXUS_ENGINE_API Var(ImGuiStyleVar Id, float Value);
				NEXUS_ENGINE_API Var(ImGuiStyleVar Id, float Value, uint8 Index);
				NEXUS_ENGINE_API Var(ImGuiStyleVar Id, NxFr::Vector2f Value);
				NEXUS_ENGINE_API ~Var();
			};

			struct Style
			{
				NEXUS_ENGINE_API Style(NxFr::StringId Id);
				NEXUS_ENGINE_API Style(const ::NxEn::GUI::Style& Value);
				NEXUS_ENGINE_API ~Style();

			private:
				const ::NxEn::GUI::Style& Value;
			};

			struct Width
			{
				NEXUS_ENGINE_API Width(float Size = 0);
				NEXUS_ENGINE_API ~Width();
			};
		}

		namespace Utils
		{
			NEXUS_ENGINE_API NxFr::Vector2f Fill(NxFr::Vector2f Offset = NxFr::Vector2f::Zero, float Count = 0.0f, bool IncludePadding = true, bool Window = false);
		}
	}
}
