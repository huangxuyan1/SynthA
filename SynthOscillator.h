/*
  ==============================================================================

    SynthOscillator.h
    Created: 17 Feb 2023 7:37:22pm
    
    This class implemented a basic oscillator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthOscillator
{
public:
    SynthOscillator ();
    SynthOscillator (double sampleRate);
    SynthOscillator (double sampleRate, bool isActive);
//    ~SynthOscillator();
    
    void setSampleRate (double newSampleRate);
    void setActiveness (bool isActive);
    void setFrequency (float newFrequency);
    void setWavetable (std::vector<float> newWavetable);
    void setWavetableFromHarmonics (std::array<float, 2 * SynthSound::wavetableSize> harmonics);
    void stop();
    
    float getNextSample ();
    
    /** A simple implementation of hermite interpolation
        implementation code from: https://www.musicdsp.org/en/latest/Other/93-hermite-interpollation.html
        @param value0 x[n-1] on the wavetable
        @param value3 x[n+2] on the wavetable
     */
    inline float hermiteInterpolation (float value0, float value1, float value2, float value3, float sample_offset);
    inline float linearInterpolation (float start, float end, float sample_offset);
    
    /** Render samples to a channel
        @param channelBuffer The channel to be rendered, get it by AudioBuffer::getWritePointer()
     */
    void renderNextChannel (float* channelBuffer, int startSample, int numSamples, int unison);
private:
    friend class SynthVoice;
    
    double sampleRate_ { 44100 };
    bool isActive_ { false }; //isActive shows whether this oscillator will produce sound
    
    float phase { 0.f };
    float delta_phase { 0.f };
    std::array<float, 2 * SynthSound::wavetableSize> harmonics;
    std::vector<float> wavetable;
};
