# Sample Code for Logue Panel Controls

This is a simple project with a trivial phase-modulation oscillator that can respond to Prologue
or Minilogue XD front panel controls.

The demo provides two oscillators that are linked to VCO 1 and VCO 2. The source code demonstrates:

- how to change a waveform via the VCO wave selector switches
- how to read and use the VCO pitch information
- how to read and use the VCO shape information
- how to render the LFO with independent intensity for the multi-engine

The demo's front panel control usage is:

    VCO1 Wave           Sets digital oscillator #1 waveform (saw, elipse, square)
    VCO1 Octave/Pitch   Sets digital oscillator #1 pitch offset, relative to the key-frequency
    VCO1 Shape          Sets digital oscillator #1 phase modulation depth

    VCO2 Wave           Sets digital oscillator #2 waveform (saw, elipse, square)
    VCO2 Octave/Pitch   Sets digital oscillator #2 pitch offset, relative to the key-frequency
    VCO2 Shape          Sets digital oscillator #2 phase modulation depth

    Multi-Shape         Sets the mix between digital oscillator 1 and 2
    Multi-Shift-Shape   Sets the LFO modulation intensity for the oscillator 1 and 2 mix
    LFO Wave            Sets the LFO waveform (saw, triangle, square)
    LFO Rate            Sets the LFO speed

The LFO modulation intensity via shift-shape is independent of the Prologue or Minilogue LFO knob,
and can be used regardless of the LFO destination.

The demo code is very simplistic and it is possible to do much more than is implemented in the
code, such as modulation based on the filter envelope or alternative LFO waveforms. See the
code in main.cpp for examples on using the panel interface, and logue_panel.h for a set of
functions that provide access to panel values.

Note that not all front panel controls are available, and those that are found may not be in a
convenient form (a good example being the MIDI velocity information, which is only available
after it has been modified by the 'EG Velocity' or 'AMP Velocity' settings).



## Source Files

    main.cpp                --      The main program and control code
    demo_oscillator.cpp     --      A trivial oscillator implementation with three waveforms and noise
    logue_panel.h           --      An application interface to the panel data
    logue_voicid.h          --      A working replacement for osc_mcu_hash()
    logue_internals.h       --      The Logue internal memory addresses and layout
    logue_dsp.h             --      A small set of support functions



## Required Pre-Processor Definitions

The following pre-processor defines are needed to configure the build:

    PLATFORM_KORG_PROLOGUE              Set non-zero if this is a prologue build
    PLATFORM_KORG_MINILOGUE_XD          Set non-zero if this is a minilogue build

If neither is defined, no panel support will function and neutral stubs are provided for all panel functions.
The NTS1 builds of the demo oscillator rely on this, and will function exactly as the Prologue or Minilogue -
only without the front panel controls.

The code also relies on CPU specific pre-processor defines that are included in the standard
Logue SDK makefiles.



## Configuring and Building

To build the demo:

1. edit src/project.mk to set the path to the directory containing the [Logue SDK](https://github.com/korginc/logue-sdk)
2. make clean; make



## Pre-built Binaries

Pre-compiled binaries for the Prologue and Minilogue XD can be found in the units folder or [downloaded directly](https://github.com/tSoniq/logue-panel-demo/blob/main/units/panel-demo.zip).
