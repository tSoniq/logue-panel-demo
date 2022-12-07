/** Logue control/status access.
 */
#pragma once

#include "logue_dsp.h"
#include "logue_internals.h"

namespace logue
{
#if LOGUE_INTERNALS_PANEL_SUPPORT

    /** Class used to read the current state of the Logues panel controls and modulation parameters.
     */
    class Panel
    {
    public:

        Panel() { init(); }         ///< Constructor.

        DSP_INLINE uint32_t vcoWaveform(unsigned index) const { return m_main->engine[0].vco[index].waveform; }     ///< Return the VCO waveform: { 0=square, 1=triangle, 2=saw }
        DSP_INLINE uint32_t vcoShape(unsigned index) const { return m_main->engine[0].vco[index].shape; }           ///< Return the current VCO shape: [0, 0x2000]
        DSP_INLINE int32_t vcoPitch(unsigned index) const { return m_main->engine[0].vco[index].pitch; }            ///< Return the current VCO pitch, excluding modulation: 256 steps per cent;
        DSP_INLINE int32_t vcoPitchEg(unsigned index) const { return m_main->engine[0].vco[index].pitchEg; }        ///< The pitch EG modulation, after intensity scaling: [-0x3000, 0x3000]. Honors the VCO select switch & gain settings.
        DSP_INLINE uint32_t vcoLevel(unsigned index) const { return m_main->engine[0].vco[index].mixerLevel; }      ///< The VCO mixer level: [0, 0x2000]
        DSP_INLINE uint32_t multiLevel() const { return m_main->engine[0].multiMixerLevel; }                        ///< The multi-engine mixer level: [0, 0x2000]
        DSP_INLINE uint32_t crossModulationDepth() const  { return m_main->engine[0].crossModulationDepth; }        ///< The cross modulation depth: [0, 0x2000]

        DSP_INLINE uint32_t filterDriveSwitch() const { return m_main->engine[0].filterDrive; }                 ///< Read the filter drive: { off=0, low=1, high=2 }
        DSP_INLINE uint32_t filterCutoffControl() const { return m_main->engine[0].filterCutoffControl; }       ///< Filter cutoff control knob position: [0, 0x2000]. 10 octaves logarithmic at 0x334 per octave.
        DSP_INLINE int32_t filterCutoffKeyTrack() const { return m_main->engine[0].filterCutoffKeyTrack; }      ///< Filter cutoff key track offset. Additive to the filter cutoff.
        DSP_INLINE uint32_t filterResonanceControl() const { return m_main->engine[0].filterResonanceControl; } ///< Filter resonance control knob position: [0, 0x2000]
        DSP_INLINE uint32_t filterEgIntensityControl() const { return m_main->engine[0].filterEgIntensity; }    ///< Filter EG INT control knob position: [0, 0x2000]
        DSP_INLINE bool filterLowCutEnable() const { return (bool)m_main->engine[0].lowCut; }                   ///< Return the filter low-cut enable.

        DSP_INLINE bool ringEnable() const { return m_main->engine[0].vco[1].ringEnable; }          ///< Return the current ring modulation enable.
        DSP_INLINE bool syncEnable() const { return m_main->engine[0].vcoSyncEnable; }              ///< Return the current sync modulation enable.
        DSP_INLINE bool multiVcfBypassEnable() const { return m_main->engine[0].multiVcfBypass; }   ///< Return the current multi-engine VCF bypass enable.

        DSP_INLINE int32_t lfoShapeModulation() const { return m_lfo->shapeOut; }       ///< LFO shape modulation output: [-0x2000, 0x2000]
        DSP_INLINE int32_t lfoPitchModulation() const { return m_lfo->pitchOut; }       ///< LFO pitch modulation output: [-0x2000, 0x2000]
        DSP_INLINE int32_t lfoFilterModulation() const { return m_lfo->filterOut; }     ///< LFO filter Modulation Output: [-0x1000, 0x1000]
        DSP_INLINE uint32_t lfoWaveform() const { return m_lfo->waveform; }             ///< LFO waveform: { square=0, triangle=1, saw=2 }
        DSP_INLINE uint32_t lfoPhaseDelta() const { return m_lfo->phaseDelta; }         ///< Current LFO phase delta. [0, 0xffffffff]. See @e lfoClock() for update rate.
        DSP_INLINE bool lfoModulationEnableMulti() const { return 0 != m_lfo->multiEnable; }                                ///< Test if LFO pitch mod is enabled for the multi-engine.
        DSP_INLINE bool lfoModulationEnableVco(unsigned index) const { return 0 != m_vco->vco[index].lfoPitchModEnable; }   ///< Test if LFO pitch mod is enabled for a given VCO.
        DSP_INLINE int32_t egFilterIntensity() const { return m_envelope->filterEgIntensity1; }             ///< Filter EG intensity setting: [-0x20000, +0x20000]
        DSP_INLINE uint32_t egFilterModulationLevel() const { return m_envelope->modulationEgLevel; }       ///< Filter EG modulation level: [0, 0xffffffff]

        DSP_INLINE uint32_t rawEgVelocity() const { return m_main->egVelocity; }        ///< EG velocity modulation. [0, 0x32cc]. Returns 0x2000 if EG Velocity == 0 or if MIDI velocity = 0x50.
        DSP_INLINE uint32_t rawAmpVelocity() const { return m_main->ampVelocity; }      ///< AMP velocity modulation. [0, 0x1fa8]. Returns 0x1400 if AMP Velocity == 0 or if MIDI velocity = 0x50.

        // LFO Platform differences.
        //
        // On Prologue, the LFO runs continuously, with a 32 bit phase and phase delta clocked at kLfoClockHz.
        //
        // On Minilogue, the same is true, but there is an additional one-shot mode for the LFO that causes the
        // phase to run a half-cycle (not a full 360 degree cycle), after which the LFO is rendered inactive until
        // the next trigger event. When inactive, the built-in LFO outputs a zero level (ie no modulation).
        //
        // The method lfoIsOneShot() can be used to test if the LFO is currently in one-shot mode.
        //
        // The method lfoIsActive() can be used to test if the LFO has continuing to cycle. If it returns false,
        // the LFO has completely its one-shot half-cycle and is now paused. On Prologue, this method always returns
        //
        // The method lfoPhase() returns the current LFO phase. If in normal mode, the phase increments continuously
        // in the interval [0, 0xffffffff], while in one-shot mode, the phase will increment in the interval
        // [0, 0x7fffffff] and then stop at 0x7fffffff when the LFO is disabled.
        //

#if PLATFORM_KORG_PROLOGUE
        DSP_INLINE bool lfoIsActive() const     { return true; }
        DSP_INLINE bool lfoIsOneShot() const    { return false; }
        DSP_INLINE uint32_t lfoPhase() const    { return m_lfo->phaseValue; }
#elif PLATFORM_KORG_MINILOGUE_XD
        DSP_INLINE bool lfoIsActive() const     { return bool(m_lfo->active); }
        DSP_INLINE bool lfoIsOneShot() const    { return bool(m_lfo->mode & 1); }
        DSP_INLINE uint32_t lfoPhase() const
        {
            auto phase = m_lfo->phaseValue;
            if (lfoIsOneShot() && phase >= 0x80000000) phase = 0x7fffffff;
            return phase;
        }
#else
        DSP_INLINE bool lfoIsActive() const     { return false; }
        DSP_INLINE bool lfoIsOneShot() const    { return false; }
        DSP_INLINE uint32_t lfoPhase() const    { return 0; }
#endif

        // Ranges for the converted EG and AMP velocity values.
        static constexpr const float kEgVelocityMin = 0.0f;
        static constexpr const float kEgVelocityMax = 1.6f;
        static constexpr const float kEgVelocityNeutral = 1.0f;

        static constexpr const float kAmpVelocityMin = 0.0f;
        static constexpr const float kAmpVelocityMax = 1.6f;
        static constexpr const float kAmpVelocityNeutral = 1.0f;


        /** Return the EG velocity modulation as float.
         *
         *  @return     The modulation as float [0, 1.6], with a value of 1.0 if the "EG Velocity" parameter is zero.
         */
        DSP_INLINE float egVelocity() const
        {
            return float(rawEgVelocity()) * (1.0f / float(0x2000));
        }


        /** Return the EG velocity modulation as float.
         *
         *  @return     The modulation as float [0, 1.6], with a value of 1.0 if the "AMP Velocity" parameter is zero.
         */
        DSP_INLINE float ampVelocity() const
        {
            return float(rawAmpVelocity()) * (1.0f / float(0x1400));
        }


        /** Return the LFO clock speed, in Hz.
         */
        DSP_INLINE float lfoClock() const
        {
            // We should really derive this from the hardware timer programming. But in practise the rate appears
            // to be fixed. In future, this method may need to be changed.
            static_assert(kLfoClockHz > 0.0f, "no LFO clock is defined for this platform");
            return kLfoClockHz;
        }




    private:

        /** Firmware compatibility check. This will substitute dummy data if the firmware version is
         *  unknown. While there appears to be a firmware version word, it has not been possible to
         *  confirm if this is updated on new firmware releases. As a result, a fast checksum is used
         *  to cover a sufficient range of the firmware to detect potential compatibility changes.
         */
        void init()
        {
            bool isValid = (m_main != nullptr) && (m_lfo != nullptr) && (m_envelope != nullptr && m_vco != nullptr);
            if (isValid)
            {
                // The compile time parameters indicate support. Now calculate a firmware checksum to ensure
                // run-time compatibility. The checksum needs to cover at least the first 12k of flash memory
                // and run quickly to avoid triggering a timeout that will itself cause problems with the logue
                // firmware. To determine the checksum for a new firmware release, use DDEBUG to monitor the
                // output message if the checksum does not match.
                register uint32_t r4 asm("r4");
                register uint32_t r5 asm("r5");
                register uint32_t r6 asm("r6");
                register uint32_t r7 asm("r7");
                register uint32_t r8 asm("r8");
                register uint32_t r9 asm("r9");
                register uint32_t r10 asm("r10");
                register uint32_t r11 asm("r11");
                auto address = (const uint32_t*)0x08000000u;
                unsigned blocks = 384;
                uint32_t hash = 0x1200;
                do {
                    // Total cycles per loop ~= 26. Total execution time estimated at 0.11ms.
                    asm (
                        "ldmia %[r_addr]!, {r4-r11}\n"
                        : "=r" (r4), "=r" (r5), "=r" (r6), "=r" (r7), "=r" (r8), "=r" (r9), "=r" (r10), "=r" (r11), [r_addr] "+r" (address)
                        );

                    uint32_t partial = r4 + r5 + r6 + r7 + r8 + r9 + r10 + r11;         // simple block checksum
                    hash ^= partial;                                                    // combine with hash
                    hash ^= hash << 13;                                                 // LFSR based random mixing
                    hash ^= hash >> 17;                                                 //
                    hash ^= hash << 5;                                                  //
                } while (--blocks);

                isValid = (hash == kExpectedFlashHash12k);
// Uncomment to use DDEBUG to monitor unsupported hash values.
//                 if (!isValid)
//                 {
//                     DDEBUG("PANEL: unsupported firmware", hash);
//                 }
            }

            if (!isValid)
            {
                // Unrecognised firmware. Fallback to some safe defaults.
                static MainState      main;
                static LfoState       lfo;
                static EnvelopeState  envelope;
                static VcoStateArray  vco;

                memZeroBytes(&main, sizeof main);
                memZeroBytes(&lfo, sizeof lfo);
                memZeroBytes(&envelope, sizeof envelope);
                memZeroBytes(&vco, sizeof vco);

                main.engine[0].filterCutoffControl = 0x2000;
                main.egVelocity = 0x2000;
                main.ampVelocity = 0x1400;
                main.engine[0].vco[0].pitch = 0;
                main.engine[0].vco[1].pitch = 0;

                m_main      =   &main;
                m_lfo       =   &lfo;
                m_envelope  =   &envelope;
                m_vco       =   &vco;
            }
        }

    private:

        void memZeroBytes(void* address, size_t size)
        {
            auto ptr = (char*)address;
            while (size != 0) *ptr++ = 0;
        }

    private:

        static const uint32_t FirmwareHashPrologue210       =   0x0eff0f53u;
        static const uint32_t FirmwareHashMinilogueXD210    =   0x2a6b3685u;

#if PLATFORM_KORG_PROLOGUE
        static const uint32_t kExpectedFlashHash12k =   FirmwareHashPrologue210;
#elif PLATFORM_KORG_MINILOGUE_XD
        static const uint32_t kExpectedFlashHash12k =   FirmwareHashMinilogueXD210;
#else
        static const uint32_t kExpectedFlashHash12k =   0;
#endif

    private:
        const MainState*        m_main          { MainStateAddress };
        const LfoState*         m_lfo           { LfoStateAddress };
        const EnvelopeState*    m_envelope      { EnvelopeStateAddress };
        const VcoStateArray*    m_vco           { VcoStateArrayAddress };
    };





#else



    /** Dummy class for unsupported plaforms.
     */
    /** Class used to read the current state of the Logues panel controls and modulation parameters.
     */
    class Panel
    {
    public:

        DSP_INLINE uint32_t vcoWaveform(unsigned index) const           { (void)index; return 0; }
        DSP_INLINE uint32_t vcoShape(unsigned index) const              { (void)index; return 0; }
        DSP_INLINE int32_t vcoPitch(unsigned index) const               { (void)index; return 0; }
        DSP_INLINE int32_t vcoPitchEg(unsigned index) const             { (void)index; return 0; }
        DSP_INLINE uint32_t vcoLevel(unsigned index) const              { (void)index; return 0; }
        DSP_INLINE uint32_t multiLevel() const                          { return 0; }
        DSP_INLINE uint32_t crossModulationDepth() const                { return 0; }

        DSP_INLINE uint32_t filterDriveSwitch() const                   { return 0; }
        DSP_INLINE uint32_t filterCutoffControl() const                 { return 0; }
        DSP_INLINE int32_t filterCutoffKeyTrack() const                 { return 0; }
        DSP_INLINE uint32_t filterResonanceControl() const              { return 0; }
        DSP_INLINE uint32_t filterEgIntensityControl() const            { return 0; }
        DSP_INLINE bool filterLowCutEnable() const                      { return false; }

        DSP_INLINE bool ringEnable() const                              { return false; }
        DSP_INLINE bool syncEnable() const                              { return false; }
        DSP_INLINE bool multiVcfBypassEnable() const                    { return false; }

        DSP_INLINE int32_t lfoShapeModulation() const                   { return 0; }
        DSP_INLINE int32_t lfoPitchModulation() const                   { return 0; }
        DSP_INLINE int32_t lfoFilterModulation() const                  { return 0; }
        DSP_INLINE uint32_t lfoWaveform() const                         { return 0; }
        DSP_INLINE uint32_t lfoPhaseDelta() const                       { return 0; }
        DSP_INLINE bool lfoModulationEnableMulti() const                { return false; }
        DSP_INLINE bool lfoModulationEnableVco(unsigned index) const    { (void)index; return false; }
        DSP_INLINE int32_t egFilterIntensity() const                    { return 0; }
        DSP_INLINE uint32_t egFilterModulationLevel() const             { return 0; }

        DSP_INLINE uint32_t rawEgVelocity() const                       { return 0x2000; }
        DSP_INLINE uint32_t rawAmpVelocity() const                      { return 0x1400; }

        DSP_INLINE bool lfoIsActive() const                             { return false; }
        DSP_INLINE bool lfoIsOneShot() const                            { return false; }
        DSP_INLINE uint32_t lfoPhase() const                            { return 0; }

        // Ranges for the converted EG and AMP velocity values.
        static constexpr const float kEgVelocityMin = 0.0f;
        static constexpr const float kEgVelocityMax = 1.6f;
        static constexpr const float kEgVelocityNeutral = 1.0f;

        static constexpr const float kAmpVelocityMin = 0.0f;
        static constexpr const float kAmpVelocityMax = 1.6f;
        static constexpr const float kAmpVelocityNeutral = 1.0f;


        DSP_INLINE float egVelocity() const                             { return 1.0f; }
        DSP_INLINE float ampVelocity() const                            { return 1.0f; }

        DSP_INLINE float lfoClock() const                               { return 1.0f; }
    };

#endif


}   // namespace
