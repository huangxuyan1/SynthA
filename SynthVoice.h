/*
  ==============================================================================

    SynthVoice.h
    Created: 14 Feb 2023 6:12:59pm
    
    This class overrides from juce::SynthesiserVoice to handle custom voicing

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthOscillator.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setActiveness (bool isActive);
    void setUnison (int newUnisonCount);
    void setSound (juce::SynthesiserSound* sound);
private:
    friend class SynthOscillator;
    bool isActive { true }; //Note that if isActive is false, the voice will not play any sound and note
                            //isVoiceActive() returns whether this voice is not playing a note (and thus able/not able to play a new note)
    static constexpr int maxUnison { 16 };
    int currentUnison { 1 };
    float detuneValue = 2.f;
    
    std::vector<SynthOscillator> oscA;
    
    juce::AudioBuffer<float> voiceBuffer; //AudioBuffer that stores the audio in a voice
    bool isPrepared { false };
};
