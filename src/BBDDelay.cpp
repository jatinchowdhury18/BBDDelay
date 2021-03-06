#include "BBDDelay.h"

BBDDelay::BBDDelay()
{
    delayMsParam = vts.getRawParameterValue ("delay_ms");
    nStagesParam = vts.getRawParameterValue ("n_stages");
    reconstructParam = vts.getRawParameterValue ("reconstruct");
    driveParam = vts.getRawParameterValue ("drive");
    bParam = vts.getRawParameterValue ("b_param");
    freqParam = vts.getRawParameterValue ("freq");
}

void BBDDelay::addParameters (Parameters& params)
{
    NormalisableRange<float> delayRange (5.0f, 500.0f);
    delayRange.setSkewForCentre (50.0f);
    params.push_back (std::make_unique<AudioParameterFloat> ("delay_ms", "Delay Time [ms]", delayRange, 10.0f));

    params.push_back (std::make_unique<AudioParameterChoice> ("n_stages", "Stages", StringArray { "512", "1024", "2048", "4096" }, 2));

    params.push_back (std::make_unique<AudioParameterBool> ("reconstruct", "Reconstruct", true));

    NormalisableRange<float> driveRange (0.1f, 10.0f);
    driveRange.setSkewForCentre (1.0f);
    params.push_back (std::make_unique<AudioParameterFloat> ("drive", "Drive", driveRange, 0.1f));

    params.push_back (std::make_unique<AudioParameterFloat> ("b_param", "B", 0.0f, 1.0f, 0.0f));

    NormalisableRange<float> freqRange (100.0f, 20000.0f);
    freqRange.setSkewForCentre (2000.0f);
    params.push_back (std::make_unique<AudioParameterFloat> ("freq", "Freq.", freqRange, 10000.0f));
}

void BBDDelay::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    fs = (float) sampleRate;

    auto prepareDelay = [=] (auto& delay) {
        delay.prepare (sampleRate);
    };

    for (int ch = 0; ch < 2; ++ch)
    {
        prepareDelay (del512[ch]);
        prepareDelay (del1024[ch]);
        prepareDelay (del2048[ch]);
        prepareDelay (del4096[ch]);
    }
}

void BBDDelay::releaseResources()
{
}

void BBDDelay::processAudioBlock (AudioBuffer<float>& buffer)
{
    const auto reconstruct = (bool) reconstructParam->load();
    const auto nStagesType = (int) nStagesParam->load();
    const auto numSamples = buffer.getNumSamples();

    auto processDelayBlock = [=] (auto* x, auto& delay) {
        delay.setDelayTime (delayMsParam->load());
        delay.setWaveshapeParams (driveParam->load(), 1.0f - bParam->load());
        delay.setFreq (freqParam->load());
        for (int n = 0; n < numSamples; ++n)
            x[n] = delay.process (x[n], reconstruct);
    };

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        if (nStagesType == 0) // 512
            processDelayBlock (buffer.getWritePointer (ch), del512[ch]);
        else if (nStagesType == 1) // 1024
            processDelayBlock (buffer.getWritePointer (ch), del1024[ch]);
        else if (nStagesType == 2) // 2048
            processDelayBlock (buffer.getWritePointer (ch), del2048[ch]);
        else if (nStagesType == 3) // 4096
            processDelayBlock (buffer.getWritePointer (ch), del4096[ch]);
    }
}

// This creates new instances of the plugin
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BBDDelay();
}
