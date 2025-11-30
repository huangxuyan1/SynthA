/*
  ==============================================================================

    SynthSound.h
    Created: 14 Feb 2023 6:11:50pm
    
    This class overrides from juce::SynthesiserSound to store sound wavetable

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using std::vector;
using std::array;

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
    
    /** Called to set the wavetable for the oscillator
        
        Note that only the first 2048 samples will be transformed
        Vector size less than 2048 will cause error
     */
    void setWavetable (vector<float> samples);
    
    /** Called to generate a basic waveform
        
        @param position           The position parameter indicates the waveform to be generated
                            0 - sine wave
                            1 - saw wave
     */
    vector<float> getBasicWavetable (int position); // 0:sine 1:saw
private:
    friend class SynthVoice;
    friend class SynthOscillator;
    
    static constexpr int wavetableSize { 2048 };
    array<float, 2 * wavetableSize> wavetable;
};
