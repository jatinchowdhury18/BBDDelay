#pragma once

#include "BBDFilters.h"
#include "BBDNonlin.h"

template<size_t STAGES>
class BBDDelayLine
{
public:
    BBDDelayLine() = default;

    void prepare (double sampleRate);
    void setDelayTime (float delayMs);
    void setWaveshapeParams (float drive) { waveshape2.setDrive (drive); }
    void setFreq (float freq);

    inline float process (float u, bool reconstruct)
    {
        std::array<std::complex<float>, FilterSpec::N_filt> xOutAccum;
        std::fill (xOutAccum.begin(), xOutAccum.end(), std::complex<float>());

        float ogOutput = 0.0f;
        while (tn < Ts)
        {
            if (evenOn)
            {
                std::array<std::complex<float>, FilterSpec::N_filt> gIn;
                for (size_t i = 0; i < FilterSpec::N_filt; ++i)
                    gIn[i] = inputFilters[i].calcG (tn);
                buffer[bufferPtr++] = std::real (std::inner_product (xIn.begin(), xIn.end(), gIn.begin(), std::complex<float>()));
                bufferPtr = (bufferPtr < STAGES) ? bufferPtr : 0;
            }
            else
            {
                auto yBBD = buffer[bufferPtr];
                auto delta = yBBD - yBBD_old;
                yBBD_old = yBBD;
                ogOutput = yBBD;
                for (size_t i = 0; i < FilterSpec::N_filt; ++i)
                    xOutAccum[i] += outputFilters[i].calcG (tn) * delta;
            }

            evenOn = ! evenOn;
            tn += Ts_bbd;
        }
        tn -= Ts;

        for (auto& iFilt : inputFilters)
            iFilt.process (u);

        for (size_t i = 0; i < FilterSpec::N_filt; ++i)
            outputFilters[i].process (xOutAccum[i]);

        float output = ogOutput;
        if (reconstruct)
            output = H0 * yBBD_old + std::real (std::accumulate (xOut.begin(), xOut.end(), std::complex<float>()));

        return waveshape2.processSample (output);
    }

private:
    float FS = 48000.0f;
    float Ts = 1.0f / FS;
    float Fs_bbd = 48000.0f;
    float Ts_bbd = 1.0f / Fs_bbd;

    std::vector<InputFilter> inputFilters;
    std::vector<OutputFilter> outputFilters;
    float H0 = 1.0f;

    std::array<std::complex<float>, FilterSpec::N_filt> xIn;
    std::array<std::complex<float>, FilterSpec::N_filt> xOut;

    std::array<float, STAGES> buffer;
    size_t bufferPtr = 0;

    float yBBD_old = 0.0f;
    float tn = 0.0f;
    bool evenOn = true;

    BBDNonlin waveshape2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BBDDelayLine)
};
