#pragma once

#include <JuceHeader.h>

namespace waveshape_poly
{
constexpr float f(float x, float BCube)
{
    return x - (x * x) / 180.0f - (x * x * x) / BCube;
}

constexpr float f_prime(float x, float BCube)
{
    return 1.0f - 2.0f * x / 180.0f - 3.0f * (x * x) / BCube;
}
}

class BBDWaveshape
{
public:
    BBDWaveshape() = default;

    void setParams (float driveGain, float bParam01)
    {
        drive = driveGain;
        invDrive = 1.0f / drive;

        auto Bval = jmap (bParam01, 1.45f, 3.6f);
        BCube = Bval * Bval * Bval;
    }

    inline float process (float x) const noexcept
    {
        using namespace waveshape_poly;

        x *= drive;
        if (x < -1) {
            x = f_prime (-1.0f, BCube) * (x + 1.0f) + f(-1.0f, BCube);
        }
        else if (x > 1) {
            x = f_prime (1.0f, BCube) * (x - 1.0f) + f(1.0f, BCube);
        }
        else
            x = f(x, BCube);
        
        return x * invDrive;
    }

private:
    float drive = 1.0f;
    float invDrive = 1.0f / drive;
    float BCube = 1.45f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BBDWaveshape)
};
