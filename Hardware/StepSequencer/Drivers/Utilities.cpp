#include "Constants.h"
#include "Utilities.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint8_t Utilities::lookup[8] = {0, 2, 4, 5, 7, 9, 11, 12};
    
    uint8_t Utilities::GetNoteFromKeyPressed(uint32_t keyState)
    {
        for (uint8_t currentKeyIndex = 0; currentKeyIndex <= 12; currentKeyIndex++)
        {
            if ((keyState & (1 << currentKeyIndex)) > 0)
            {
                return currentKeyIndex;
            }
        }

        return STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY;
    }

    uint8_t Utilities::GetNoteFromPatternIndex(uint8_t patternIndex)
    {
        return Utilities::lookup[patternIndex];
    }

    uint8_t Utilities::GetPatternIndexFromNote(uint8_t noteNumber)
    {
        for (uint8_t index = 0; index < 8; index++)
        {
            if (Utilities::lookup[index] == noteNumber)
            {
                return index;
            }
        }

        return STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY;
    }
}