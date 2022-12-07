/** Internal structures and data for Logue devices.
 */
#pragma once

#include <cstdint>

namespace logue {

    /** VCO Parameter data. Firmware 2.10. 0x20009e40 & 0x20009e7c
     */
     typedef struct VcoControl_210
     {
        uint32_t    shape;                  // 0x00/32, 0x1c/32     VCO Shape                       [0, 0x2000]
        int32_t     pitch;                  // 0x04/32, 0x20/32     VCO Pitch                       Linear pitch offset. 3072 per octave (256 per cent).
        uint32_t    reserved0;              // 0x08/32, 0x24/32     -
        uint16_t    mixerLevel;             // 0x0c/16, 0x28/16     VCO Output Gain                 [0, 0x2000]
        int16_t     pitchEg;                // 0x0e/16, 0x2a/16     VCO Active Pitch EG Intensity   [0xd00c, 0x2ff4]. The current value of the pitch EG after intensity has been applied. Does not include LFO.
        uint32_t    reserved1;              // 0x10/32, 0x2c/32
        uint32_t    reserved2;              // 0x14/32, 0x30/32     { 0x0800dac8, 0x0800dae8, 0x0800db08 }          Flash addresses related to oscillator wave-shape (Square, Triangle, Saw)
        uint8_t     waveform;               // 0x18/8,  0x34/8      VCO Waveform                    [0, 1, 2]       Square, Triangle, Saw
        uint8_t     ringEnable;             // 0x19/8,  0x35/8      Ring Modulation Enable          [0, 1]          VCO2 only. zero => off.
        uint8_t     reserved3;              // 0x1a/8,  0x36/8      -
        uint8_t     vcoPitchEgEnable;       // 0x1b/8,  0x37/8      VCO Pitch EG Enable             [0, 1]          VCO1 only? zero => off.
     } VcoControl_210;


     /** Engine structure. In firmware 2.10, found at 0x20009e40 on both Prologue and Minilogue XD.
      *
      * Notes on filterCutoffKeyTrack:
      *     - if key-tracking is off (down), value is zero
      *     - if key-tracking is mid (centre), value increases by 0x19a per octave. Zero corresponds to middle C.
      *     - if key-tracking is max (top), value increases by 0x334 per octave. Zero corresponds to middle C.
      * The key tracking value is added directly to the filter control knob value, with the result saturated to [0, 0x1fff].
      * This implies that the VCF control values from 0 to 0x2000 span almost exactly 10 octaves.
      */
    typedef struct Engine_210
    {
        VcoControl_210 vco[2];              // 0x00-0x37            VCO1 and VCO2 data.

        uint32_t    pitchEgModulation;      // 0x38/32  Appears to mimic 0x72 (multi-engine pitch modulation)
        uint32_t    u3c;                    // 0x3c/32
        uint16_t    u40;                    // 0x40/32  Used when calculating the filter cutoff
        uint16_t    midiPitch;              // 0x42/32  MIDI Note Number * 256 (copied from 0x7c/16); does not appear to include velocity information.
        int16_t     filterCutoffKeyTrack;   // 0x44/16  VCF Cutoff Pitch Key-tracking. Added to the VCF cutoff. See notes for main structure.
        uint16_t    ampVelocity;            // 0x46/16  Copied from 0x80
        uint16_t    multiMixerLevel;        // 0x48/16  Multi-engine output gain.       [0, 0xffff]
        uint16_t    filterResonanceControl; // 0x4a/16  Filter resonance knob           [0, 0x2000]     Just the knob position - does not include key tracking
        uint16_t    filterCutoffControl;    // 0x4c/16  Filter cutoff knob              [0, 0x2000]     Logarithmic: frequency = f0 * 2^(10 * (filterCutoffControl / 0x2000))
        uint16_t    crossModulationDepth;   // 0x4e/16  VCO Cross-Modulation Depth      [0, 0x2000]
        uint32_t    u50;                    // 0x50/32  (zero)
        uint16_t    u54;                    // 0x54/32  (zero)                                          Set zero in sub_80025cc
        uint16_t    u56;                    // 0x56/32  (zero)                                          Set zero in sub_80025cc
        uint16_t    filterEgIntensity;      // 0x58/16  Filter envelope intensity       [0, 0x2000]     nominal zero position reads 0x1000.
        uint16_t    egVelocity;             // 0x5a/16  EG Velocity (see 0x78)
        uint32_t    u5c;                    // 0x5c/32
        uint8_t     u60;                    // 0x60/8   (0x01)
        uint8_t     vcoSyncEnable;          // 0x61/8   VCO2 sync enable                [0, 1]          0 is off, 1 is on       Value here ^ 1 is written to 0x42410290
        uint8_t     u62;                    // 0x62/8   (0x00)
        uint8_t     lfoWaveform;            // 0x63/8   LFO waveform                    [0, 1, 2]       square, triangle, saw
        uint8_t     u64;                    // 0x64/8   (0x00)                                          possibly true/false for LFO one-shot enabled/disabled
        uint8_t     multiVcfBypass;         // 0x65/8   set zero if the multi-output is routed via the VCF, non-zero for post-VCF.
        uint8_t     filterDrive;            // 0x66/8   Filter drive switch             [0, 2]          0 is off, 2 is maximum
        uint8_t     lowCut;                 // 0x67/8   Low cut switch                  [0, 1]          0 is off, 1 is on
        uint8_t     u68;                    // 0x68/8   (0x00)
        uint8_t     u69;                    // 0x69/8   (0x01)      Set to 0x01 in sub_80025cc
        uint8_t     u6a;                    // 0x6a/8   (0x01)      Set to 0x01 in sub_80025cc
        uint8_t     u6b;                    // 0x6b/8   (0x00)
    } Engine_210;


     /** Top-level control structure. In firmware 2.10, found at 0x20009e40 on both Prologue and Minilogue XD.
      */
    typedef struct MainState_210
    {
        Engine_210  engine[1];              // 0x00/6c  Engine structure.
        uint32_t    multiPitch;             // 0x6c/32  Multi-OSC Pitch                 { 0xfffff400, 0, 0xc00, 0x1800 }        Appears to not include LFO modulation etc
        uint8_t     multiPitchEgEnable;     // 0x70/8   Multi-engine pitch env enable   [0, 1]          0 for VCO1+2 or VCO2, 1 for All (multi-engine VCO enable)
        uint8_t     u71;                    // 0x71/8
        int16_t     multiPitchEgModulation; // 0x72/16  Multi-engine pitch modulation   [8000, 7fff]    modulation for multi-engine - works the same as the vco.pitchEgModulation values.
        uint8_t     u74;                    // 0x74/8   Index in to table of structures of length 0x14, starting at 0x2000d500. Entry offset 0x10 is pitch.
        uint8_t     u75;                    // 0x75/8
        uint16_t    u76;                    // 0x76/16  (read in sub_80025cc and written to u44)
        uint16_t    egVelocity;             // 0x78/16  Velocity after application of EG Velocity parameter and curve. [0, 0x32cc]. Returns 0x2000 if EG Velocity == 0.
        uint16_t    u7a;                    // 0x7a/16
        uint16_t    u7c;                    // 0x7c/16  MIDI note number << 8; unfortunately no velocity information.
        uint16_t    u7e;                    // 0x7e/16
        uint16_t    ampVelocity;            // 0x80/16  Velocity after application of AMP Velocity parameter and curve. [0, 0x1fa8]. Returns 0x1400 if AMP Velocity == 0. Copied to 0x46/16
        uint16_t    u82;                    // 0x82/16
        uint32_t    vco1PitchEgIntensity;   // 0x84/32  VCO Pitch EG Intensity          [0xfffffd000, 0x3000]       0 at off position (calculated from 0x46 and 0x88)
        uint32_t    vco2PitchEgIntensity;   // 0x88/32  (assumed usage - the value is identical to VCO1, even if the pitch env is not applied to VCO1; this does *not* agree with code at loc_800262a)
        uint32_t    u8c;                    // 0x8c/32  (0x00030000)
        uint32_t    timestamp;              // 0x90/32  (looks like some kind of fast timestamp updated at key-on; appears to increment at the 85MHz ARM clock speed)
        uint32_t    u94;                    // 0x94/32
        uint32_t    u98;                    // 0x98/32
        uint32_t    magic1234567;           // 0x9c/32 Initialised to 0x1234567 in reset_data_at_9e40_d1d8, reads as zero on running system.
        // There may be more fields that follow here, possibly up to 0x100.
    } MainState_210;

    static_assert(sizeof (VcoControl_210) == 0x1c, "Structure sanity check");
    static_assert(sizeof (Engine_210) == 0x6c, "Structure sanity check");
    static_assert(offsetof (Engine_210, multiMixerLevel) == 0x48, "Structure sanity check");
    static_assert(offsetof (Engine_210, filterEgIntensity) == 0x58, "Structure sanity check");
    static_assert(offsetof (Engine_210, lowCut) == 0x67, "Structure sanity check");
    static_assert(offsetof (MainState_210, multiPitchEgModulation) == 0x72, "Structure sanity check");
    static_assert(offsetof (MainState_210, magic1234567) == 0x9c, "Structure sanity check");


    /** LFO Generation. 0x20008970. Prologue.
     *
     *  The user oscillator parameter fields are:
     *
     *      0x00/32     int32_t     shape_lfo;
     *      0x04/16     uint16_t    Current pitch. high byte: note number, low byte: fine (0-255)
     *      0x06/16     uint16_t    Current cutoff value (0x0000-0x1fff)
     *      0x08/16     uint16_t    Current resonance value (0x0000-0x1fff)
     *      0x0c/16                 reserved
     *      0x10/16                 reserved
     *      0x12/16                 reserved
     */
    typedef struct LfoState_Prologue_210
    {
        uint8_t     waveform;       // 0x00/8   LFO shape                               [0, 2]      { square, triangle, saw }
        uint8_t     invert;         // 0x01/8   Invert                                  [0, non-0]  if non-zero, invert the polarity of the LFO output
        uint8_t     multiEnable;    // 0x02/8   LFO Target                              Set if the multi-engine is a target, clear if not
        uint8_t     u03;            // 0x03/8   LFO Target                              Set if the multi-engine is a target, clear if not
        int32_t     intensity;      // 0x04/32  LFO Intensity knob value                [-0x1ff0, 0x1ff0]   (negative values with shift-int).
        uint32_t    u08;            // 0x08/32
        uint32_t    u0c;            // 0x0c/32  Shape modulation gain
        uint32_t    phaseDelta;     // 0x10/32  LFO phase delta                         Changes with both range control and knob
        uint32_t    phaseValue;     // 0x14/32  LFO phase value                         Appears to be the current phase
        uint32_t    u18;            // 0x18/32
        uint32_t    u1c;            // 0x1c/32
        int32_t     pitchOut;       // 0x20/32  LFO Pitch modulation (if panel set to 'pitch')          [-0x4d29, +0x4d29]
        int32_t     filterOut;      // 0x24/32  LFO Filter modulation (if panel set to 'filter')        (-0x1000, +0x1000)
        int32_t     shapeOut;       // 0x28/32  LFO Shape modulation (if panel set to 'shape')          (-0x2000, +0x2000)
        void*       u2c;            // 0x2c/32  Pointer to struct user_osc_param. Value at offset 0x04 scales the pitch modulation value. eg 0x2000d528.
    } LfoState_Prologue_210;



    /** LFO Generation. 0x20008970. Minilogue.
     *
     *  The user oscillator parameter fields are:
     *
     *      0x00/32     int32_t     shape_lfo;
     *      0x04/16     uint16_t    Current pitch. high byte: note number, low byte: fine (0-255)
     *      0x06/16     uint16_t    Current cutoff value (0x0000-0x1fff)
     *      0x08/16     uint16_t    Current resonance value (0x0000-0x1fff)
     *      0x0c/16                 reserved
     *      0x10/16                 reserved
     *      0x12/16                 reserved
     */
    typedef struct LfoState_Minilogue_210
    {
        uint8_t     waveform;       // 0x00/8   LFO shape                               [0, 2]      { square, triangle, saw }
        uint8_t     invert;         // 0x01/8   Invert                                  [0, non-0]  if non-zero, invert the polarity of the LFO output
        uint8_t     multiEnable;    // 0x02/8   LFO Target                              Set if the multi-engine is a target, clear if not
        uint8_t     u03;            // 0x03/8   LFO Target                              Set if the multi-engine is a target, clear if not
        uint8_t     mode;           // 0x04/8   Bit zero is set if running in one-shot mode. In one shot mode the phase runs from 0 to 0x7fffffff after which the LFO amplitude is forced to zero.
        uint8_t     active;         // 0x05/8   Non-zero while the LFO is active. This is used for one-shot mode and is not available on Prologue.
        uint8_t     u06;            // 0x06/8
        uint8_t     u07;            // 0x07/8
        int32_t     intensity;      // 0x08/32  LFO Intensity knob value                [-0x1ff0, 0x1ff0]   (negative values with shift-int).
        uint32_t    u08;            // 0x0c/32
        uint32_t    u0c;            // 0x10/32  Shape modulation gain
        uint32_t    phaseDelta;     // 0x14/32  LFO phase delta                         Changes with both range control and knob
        uint32_t    phaseValue;     // 0x18/32  LFO phase value                         Appears to be the current phase
        uint32_t    u1c;            // 0x1c/32
        int32_t     pitchOut;       // 0x20/32  LFO Pitch modulation (if panel set to 'pitch')          [-0x4d29, +0x4d29]
        int32_t     filterOut;      // 0x24/32  LFO Filter modulation (if panel set to 'filter')        (-0x1000, +0x1000)
        int32_t     shapeOut;       // 0x28/32  LFO Shape modulation (if panel set to 'shape')          (-0x2000, +0x2000)
        void*       u2c;            // 0x2c/32  Pointer to struct user_osc_param. Value at offset 0x04 scales the pitch modulation value. eg 0x2000d528.
    } LfoState_Minilogue_210;


    // Since the LFO structures are almost the same, we can just use a typedef to select the correct structure for the build.
#if PLATFORM_KORG_PROLOGUE
    typedef LfoState_Prologue_210 LfoState_210;
#elif PLATFORM_KORG_MINILOGUE_XD
    typedef LfoState_Minilogue_210 LfoState_210;
#endif



    /** Envelope processing. Firmware 2.10.
     */
    typedef struct EnvelopeState_210
    {
        uint32_t    u00;
        uint32_t    u04;
        uint32_t    u08;
        uint32_t    u0c;
        uint32_t    u10;
        uint32_t    u14;
        uint32_t    u18;
        uint32_t    u1c;
        uint32_t    u20;
        uint32_t    u24;
        uint32_t    u28;
        uint32_t    u2c;
        uint32_t    u30;
        uint32_t    u34;
        uint32_t    u38;
        uint32_t    u3c;
        uint32_t    u40;
        uint32_t    u44;
        int32_t     filterEgIntensity1;     // 0x48/32      Filter EG Intensity knob.   [-0x20000, 0x20000]
        uint32_t    u4c;
        uint32_t    u50;
        uint32_t    u54;
        uint32_t    u58;
        int32_t     filterEgIntensity2;     // 0x5c/32      Filter EG Intensity knob.   [-0x20000, 0x20000]     Replicates 0x48.
        uint32_t    u60;
        uint32_t    u64;
        uint32_t    modulationEgLevel;      // 0x68/32      Modulation envelope output level    [0, 0xffffffff]
        uint32_t    u6c;
        uint32_t    u70;
        uint32_t    u74;
        uint32_t    u78;
        uint32_t    u7c;
    } LogueEnvelopeState_210;


    /** VCO State information. This is the low level VCO control information. This is useful
     *  for access to the parsed LFO target bit mask (which unfortunately seems not to be stored
     *  after it is received from the central  control processor).
     */
    typedef struct VcoState_210
    {
        uint8_t     u00[16];                //              Many things.
        uint8_t     lfoPitchModEnable;      // 0x10/8       Logical true to enable pitch modulation on this VCO.
        uint8_t     u11;                    // 0x11/8       Appears to replicate 0x10/8
        uint8_t     u12;                    // 0x12/8
        uint8_t     u13;                    // 0x12/8
        uint8_t     u14[0x4c];              //              Many things.

    } VcoState_210;

    static_assert(sizeof (VcoState_210) == 0x60, "Sanity check on structure size");


    /** State information for all VCOs. Located at 0x20008850 on firmware 2.10.
     */
    typedef struct VcoStateArray_210
    {
        VcoState_210    vco[2];             // Array of state structures.
    } VcoStateArray_210;




    // Set the canonical definitions based on the platform.
#if PLATFORM_KORG_PROLOGUE
#define LOGUE_INTERNALS_PANEL_SUPPORT   (1)     ///< Set logical false if the target does not (can never possibly) support panel control.
    typedef VcoControl_210 VcoControl;
    typedef Engine_210 Engine;
    typedef MainState_210 MainState;
    typedef LfoState_210 LfoState;
    typedef EnvelopeState_210 EnvelopeState;
    typedef VcoState_210 VcoState;
    typedef VcoStateArray_210 VcoStateArray;
    static constexpr const MainState* MainStateAddress              =   (const MainState*)(0x20009e40);
    static constexpr const LfoState* LfoStateAddress                =   (const LfoState*)(0x20008970);
    static constexpr const EnvelopeState* EnvelopeStateAddress      =   (const EnvelopeState*)(0x200089a0);
    static constexpr const VcoStateArray* VcoStateArrayAddress      =   (const VcoStateArray*)(0x20008850);
    static constexpr const float kLfoClockHz                        =   40500.0f;
#elif PLATFORM_KORG_MINILOGUE_XD
#define LOGUE_INTERNALS_PANEL_SUPPORT   (1)     ///< Set logical false if the target does not (can never possibly) support panel control.
    typedef VcoControl_210 VcoControl;
    typedef Engine_210 Engine;
    typedef MainState_210 MainState;
    typedef LfoState_210 LfoState;
    typedef EnvelopeState_210 EnvelopeState;
    typedef VcoState_210 VcoState;
    typedef VcoStateArray_210 VcoStateArray;
    static constexpr const MainState* MainStateAddress              =   (const MainState*)(0x20009e40);
    static constexpr const LfoState* LfoStateAddress                =   (const LfoState*)(0x20008970);
    static constexpr const EnvelopeState* EnvelopeStateAddress      =   (const EnvelopeState*)(0x200089a0);
    static constexpr const VcoStateArray* VcoStateArrayAddress      =   (const VcoStateArray*)(0x20008850);
    static constexpr const float kLfoClockHz                        =   40500.0f;
#else
#define LOGUE_INTERNALS_PANEL_SUPPORT   (0)     ///< Set logical false if the target does not (can never possibly) support panel control.
#endif

}   // namespace
