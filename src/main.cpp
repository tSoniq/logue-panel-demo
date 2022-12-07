/** osc5 main.
 *
 *  Copyright (c) 2021-2022 tSoniq. All Rights Reserved.
 */

#include "osc_api.h"
#include "userosc.h"


// #pragma GCC diagnostic error "-Wall"
// #pragma GCC diagnostic error "-Wextra"
// #pragma GCC diagnostic error "-Wpedantic"
// #pragma GCC diagnostic error "-Wimplicit"
// #pragma GCC diagnostic error "-Wdouble-promotion"
// #pragma GCC diagnostic error "-Wconversion"
// #pragma GCC diagnostic error "-Wfloat-conversion"
// #pragma GCC diagnostic error "-Wsign-conversion"


#include "logue_panel.h"
#include "logue_voiceid.h"
#include "demo_oscillator.h"





/** Global state.
 */
struct Globals
{
    logue::Panel        panel;
    demo::Oscillator    osc0;
    demo::Oscillator    osc1;
    int32_t             detune;
    float               shape;
    float               shiftShape;
    float               mix;
};

static Globals g_globals;



/** Helper function that generates a waveform from the current LFO phase. The LFO is updated via
 *  a high frequency interrupt. This method reads the instantaneous phase and returns an amplitude
 *  based on the current settings.
 *
 *  @param                  A gain scalar, [-1.0, 1.0]
 *  @return                 The current LFO amplitude.
 */
float lfoAmplitude(float gain)
{
    auto phase = g_globals.panel.lfoPhase();
    auto waveform = g_globals.panel.lfoWaveform();

    int32_t amplitude;
    switch (waveform)
    {
        default:
            // Unknown
            amplitude = 0;
            break;

        case 0:
            // Square
            amplitude = (phase < 0x80000000u) ? int32_t(0x7fffffff) : int32_t(-0x7fffffff);
            break;

        case 1:
            // Triangle
            amplitude = int32_t(uint32_t((int32_t(phase) ^ (int32_t(phase) >> 31)) - 0x40000000) << 1);
            break;

        case 2:
            // Saw
            amplitude = int32_t(0x7fffffffu - phase);
            break;
    }

    return logue::s32ToFloat<31>(amplitude) * gain;
}



/** Start of day initialisation.
 *
 *  @param  platform        The platform (usrprg.h), eg k_user_target_miniloguexd_osc.
 *  @param  api             The API version (usrprg.h), eg k_user_api_1_1_0
 */
void OSC_INIT(uint32_t platform, uint32_t api)
{
    (void)platform;
    (void)api;

    logue::VoiceID voiceID;
    auto x = int32_t(voiceID()) >> 28;  // Q7.8 in range [-16, +15]
    if (x >= 0) x += 1;                 // Q7.8 in range [-16, -1] or [1, 16] (because real instruments are never exactly in tune)

    g_globals.detune = x;
    g_globals.shape = 0.0f;
    g_globals.shiftShape = 0.0f;
    g_globals.mix = 0.0f;
}



/** Parameter change.
 *
 *  @param  index           The parameter number.
 *  @param  value           The parameter value.
 *
 *  The value ranges for parameters 1-6 depend on how the parameter is configured in manifest.json:
 *
 *  For percentage parameters with +ve min:                     (0,100)         =>      (0,100)
 *  For percentage parameters with -ve min:, use offset 100:    (-100,100)      =>      (0,200)
 *  For typeless:                                               (0,99)          =>      (1,100)
 *
 *  The value range for shape and shift-shape is 0-0x3ff (10 bits unsigned).
 */
void OSC_PARAM(uint16_t index, uint16_t value)
{
    if (k_user_osc_param_shape == index)
    {
        if (value > 1023) value = 1023;
        g_globals.shape = logue::u32ToFloat<10>(value);
    }
    else if (k_user_osc_param_shiftshape == index)
    {
        if (value > 1023) value = 1023;
        g_globals.shiftShape = logue::u32ToFloat<10>(value);
    }
}



/** Generate sample data.
 *
 *  @param  params      The current oscillator parameters.
 *  @param  yn          Pointer to memory to receive the samples (Q31 format, real-only).
 *  @param  frames      The number of sample frames to generate.
 */
void OSC_CYCLE(const user_osc_param_t* const params, int32_t* yn, const uint32_t frames)
{
    // Setup pitch.
    auto pitch0 = int32_t(params->pitch);               // Multi-engine pitch
    pitch0 += g_globals.detune;                         // Add voice-card dependent detune
    pitch0 += g_globals.panel.vcoPitch(0);              // Add VCO pitch
    g_globals.osc0.setPitch(pitch0);

    auto pitch1 = int32_t(params->pitch);               // Multi-engine pitch
    pitch1 -= g_globals.detune;                         // Subtract voice-card dependent detune
    pitch1 += g_globals.panel.vcoPitch(1);              // Add VCO pitch
    g_globals.osc1.setPitch(pitch1);


    // Set the waveform.
    g_globals.osc0.setWaveform(g_globals.panel.vcoWaveform(0));
    g_globals.osc1.setWaveform(g_globals.panel.vcoWaveform(1));


    // Set the shape. Note the range of values returned by vcoShape() is [0, 0x2000] (Q18.13 fixed point).
    float shape0 = logue::u32ToFloat<13>(g_globals.panel.vcoShape(0));
    g_globals.osc0.setModulation(shape0);

    float shape1 = logue::u32ToFloat<13>(g_globals.panel.vcoShape(1));
    g_globals.osc1.setModulation(shape1);


    // Calculate the mix between the two oscillators, applying modulation. The normal approach would be
    // to use the params->shape_lfo field, but here we read the LFO directly and apply a modulation
    // intensity that is set via the shift-shape control.
    auto mixTarget = g_globals.shape + lfoAmplitude(g_globals.shiftShape);
    auto mix = g_globals.mix;
    g_globals.mix = mix + (mixTarget - mix) * 0.1f;
    if (mix < 0.0f) mix = 0.0f;
    else if (mix > 1.0f) mix = 1.0f;
    const float gain1 = mix * 0.9f;
    const float gain0 = (1.0f - mix) * 0.9f;


    // Render and mix the two oscillators.
    DSP_ASSERT(frames <= 64);
    float buffer[2][64];
    g_globals.osc0.render(&buffer[0][0], frames);
    g_globals.osc1.render(&buffer[1][0], frames);
    for (unsigned i = 0; i != frames; ++i)
    {
        float sample = (gain0 * buffer[0][i]) + (gain1 * buffer[1][i]);
        yn[i] = logue::floatToS32<31>(sample);
    }
}

