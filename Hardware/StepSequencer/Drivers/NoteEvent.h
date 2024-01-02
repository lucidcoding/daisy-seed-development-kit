#pragma once
#ifndef STEP_SEQUENCER_NOTE_EVENT_H
#define STEP_SEQUENCER_NOTE_EVENT_H

#define STEP_SEQUENCER_NOTE_EVENT_TYPE_NOTE_ON 0
#define STEP_SEQUENCER_NOTE_EVENT_TYPE_NOTE_OFF 1

#include "stdint.h"

namespace developmentKit::stepSequencer
{
    class NoteEvent
    {
    public:
        uint8_t type;
        uint8_t note;
        bool octaveUp;
        bool octaveDown;
        bool gate;
        bool slide;
        bool accent;

    private:
    };
}

#endif