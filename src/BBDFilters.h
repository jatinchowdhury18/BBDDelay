#pragma once

#include <JuceHeader.h>

/**
 * Bucket-Bridage Device filters, as derived by
 * Martin Holters and Julian Parker:
 * http://dafx2018.web.ua.pt/papers/DAFx2018_paper_12.pdf
 * 
 * @TODO: implement real-valued coefficients from section 3.3
 */ 

/**
 * Anti-aliasing/reconstruction filters used by JUNO-60 chorus.
 * Root/Pole analysis borrowed from the above paper.
 */ 
namespace FilterSpec
{
constexpr size_t N_filt = 5;

constexpr std::complex<float> iFiltRoot[] = {{ 251589.0f,  0.0f},
                                             {-130428.0f,  -4165.0f},
                                             {-130428.0f,  +4165.0f},
                                             {   4634.0f, -22873.0f},
                                             {   4634.0f, +22873.0f}};
constexpr std::complex<float> iFiltPole[] = {{-46580.0f,  0.0f},
                                             {-55482.0f, -25082.0f},
                                             {-55482.0f, +25082.0f},
                                             {-26292.0f, -59437.0f},
                                             {-26292.0f, +59437.0f}};

constexpr std::complex<float> oFiltRoot[] = {{  5092.0f,  0.0f},
                                             {-11256.0f, -99566.0f},
                                             {-11256.0f, +99566.0f},
                                             {-13802.0f, -24606.0f},
                                             {-13802.0f, +24606.0f}};
constexpr std::complex<float> oFiltPole[] = {{-176261.0f, 0.0f},
                                             {-51468.0f, -21437.0f},
                                             {-51468.0f, +21437.0f},
                                             {-26276.0f, -59699.0f},
                                             {-26276.0f, +59699.0f}};
}

struct InputFilter
{
public:
    InputFilter (std::complex<float> sPlaneRoot, std::complex<float> sPlanePole,
                float sampleTime, std::complex<float>* xPtr) :
        root (sPlaneRoot),
        pole (sPlanePole),
        x (xPtr),
        Ts (sampleTime)
    {
        root_corr = root;
        pole_corr = std::exp (pole * Ts);
        *x = 0.0f;
    }

    inline std::complex<float> calcG (float d_n) const noexcept
    {
        return gCoef * std::pow (pole_corr, d_n);
    }

    inline void process (float u)
    {
        *x = pole_corr * (*x) + u;
    }

    inline void set_freq (float freq)
    {
        constexpr float originalCutoff = 9400.0f;
        root_corr = root * (freq / originalCutoff);
        pole_corr = std::exp (pole * (freq / originalCutoff) * Ts);
        
        gCoef = Ts * root_corr;
    }

private:
    const std::complex<float> root;
    const std::complex<float> pole;

    std::complex<float> root_corr;
    std::complex<float> pole_corr;

    std::complex<float> *const x;
    const float Ts;
    std::complex<float> gCoef;
};

struct OutputFilter
{
public:
    OutputFilter (std::complex<float> sPlaneRoot, std::complex<float> sPlanePole,
                  float sampleTime, std::complex<float>* xPtr) :
        gCoef (sPlaneRoot / sPlanePole),
        pole (sPlanePole),
        x (xPtr),
        Ts (sampleTime)
    {
        pole_corr = std::exp (pole * Ts);
        *x = 0.0f;
    }

    inline std::complex<float> calcG (float d_n) const noexcept
    {
        return gCoef * std::pow (pole_corr, 1.0f - d_n);
    }

    inline void process (std::complex<float> u)
    {
        *x = pole_corr * (*x) + u;
    }

    inline std::complex<float> getGCoef() const noexcept
    {
        return gCoef;
    }

    inline void set_freq (float freq)
    {
        constexpr float originalCutoff = 11000.0f;
        pole_corr = std::exp (pole * (freq / originalCutoff) * Ts);
    }

private:
    const std::complex<float> gCoef;
    const std::complex<float> pole;
    
    std::complex<float> pole_corr;

    std::complex<float> *const x;
    const float Ts;
};
