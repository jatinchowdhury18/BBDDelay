#include "BBDDelay.h"

void BBDDelay::addParameters (Parameters& params)
{
}

void BBDDelay::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void BBDDelay::releaseResources()
{
}

void BBDDelay::processAudioBlock (AudioBuffer<float>& buffer)
{
}

// This creates new instances of the plugin
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BBDDelay();
}
