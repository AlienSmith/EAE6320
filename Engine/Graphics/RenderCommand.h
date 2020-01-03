#pragma once
#include<cstdint>
#include<algorithm>
namespace eae6320
{
	namespace Graphics {
		typedef uint64_t RENDER_COMMAND;

		//eventhough this stored in 8 bits, it can have size of 4 bits int render command
		enum class eRenderCommandType : uint8_t {
			BindAndDraw = 0
		};

		enum class eRenderCommandBitMasks : uint64_t {
			MeshHandleID = 0xFF, //(0-7)
			MeshZDepth = 0xFF00, //(8-15)
			EffectHandleID = 0xFF0000, //(16-23)
			/*
			Other stuff
			*/
			RenderCommandType = 0xF000000000000000, //60-63
		};

		enum class eRenderCommandBitShifts : uint8_t {
			MeshHandleID = 0,
			MeshZDepth = 8,
			EffectHandleID = 24,
			/*Other Elements*/
			RnederCommandType = 60,
		};

		static void sortRenderCommand(RENDER_COMMAND* O_Render_Commands, uint8_t size) {
			std::sort(O_Render_Commands, O_Render_Commands + size);
		}
	}
}