#pragma once
#ifndef EXTERNAL_CODEC_H
#define EXTERNAL_CODEC_H

#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::hardware::externalCodec::drivers
{
    using namespace daisysp;
    using namespace daisy;

    class ExternalCodec
    {
    public:
        void Init(DaisySeed *seed);
    private:
    };
}

#endif