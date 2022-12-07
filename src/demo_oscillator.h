/** Naive demo oscillator.
 */
#pragma once

#include "biquad.hpp"
#include "logue_dsp.h"

namespace demo
{
    /** Class used to implement a trivial oscillator.
     *
     *  This is not intended to be used as a real oscillator. Its purpose is to provide a means to
     *  demonstrate the Logue panel API.
     */
    class Oscillator
    {
    public:

        static const unsigned Square = 0;
        static const unsigned Parabola = 1;
        static const unsigned Sawtooth = 2;


        void setWaveform(unsigned index)
        {
            // index: [0, 2]
            m_waveformIndex = index;
        }


        void setModulation(float modulation)
        {
            // modulation: [0.0, 1.0]
            if (modulation < 0.0f) modulation = 0.0f;
            else if (modulation > 1.0f) modulation = 1.0f;
            m_modulation = modulation;
        }


        void setPitch(int32_t pitch)
        {
            // pitch: MIDI note number in fixed point Q7.8
            if (pitch < 0) pitch = 0;
            else if (pitch >= 0x7fff) pitch = 0x7fff;
            m_note = logue::s32ToFloat<8>(pitch);
            m_phaseDelta = osc_w0f_for_note(uint8_t(unsigned(pitch >> 8) & 0xffu), uint8_t(unsigned(pitch) & 0xffu));
        }


        void render(float* yn, unsigned frames)
        {
            WaveFunction waveFunction;
            float bandLimit;

            switch (m_waveformIndex)
            {
                case Square:        waveFunction = &osc_bl2_sqrf;   bandLimit = osc_bl_sqr_idx(m_note);     break;
                case Parabola:      waveFunction = &osc_bl2_parf;   bandLimit = osc_bl_par_idx(m_note);     break;
                case Sawtooth:      waveFunction = &osc_bl2_sawf;   bandLimit = osc_bl_saw_idx(m_note);     break;
                default:            waveFunction = &osc_bl2_sawf;   bandLimit = osc_bl_saw_idx(m_note);     break;
            }

            const auto mod = m_modulation * 0.75f;
            const auto phaseDelta = m_phaseDelta;
            auto phase = m_phase;

            for (unsigned i = 0; i != frames; ++i)
            {
                float modulatedPhase = phase + (mod * osc_sinf(phase));
                if (modulatedPhase < 0.0f) modulatedPhase += 1.0f;
                else if (modulatedPhase >= 1.0f) modulatedPhase -= 1.0f;

                yn[i] = (waveFunction)(modulatedPhase, bandLimit);

                phase += phaseDelta;
                if (phase >= 1.0f) phase -= 1.0f;
            }

            m_phase = phase;
        }


    private:

        typedef float (*WaveFunction)(float phase, float bandwidthLimiter);

        float noise()
        {
            m_noiseState ^= m_noiseState << 13;
            m_noiseState ^= m_noiseState >> 17;
            m_noiseState ^= m_noiseState << 5;
            return float(int32_t(m_noiseState)) * (1.0f / float(1u << 31));
        }

    private:

        unsigned    m_waveformIndex { 0 };                              // [0, 2]
        float       m_note          { 0 };                              // [0.0, 152.0]
        float       m_phase         { 0.0f };                           // [0, 1.0)
        float       m_phaseDelta    { 0.0f };                           // [0, 0.5]
        float       m_modulation    { 0.0f };                           // [0, 1.0]
        uint32_t    m_noiseState    { 1 };                              // non-zero
    };

}   // namespace
