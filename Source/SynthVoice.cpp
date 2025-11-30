/*
  ==============================================================================

    SynthVoice.cpp
    Created: 14 Feb 2023 6:12:59pm
    
    This class overrides from juce::SynthesiserVoice to handle custom voicing

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    jassert(currentUnison > 0);
    if (currentUnison == 1)
    {
        oscA[0].setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
//        std::cout << juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) << std::endl;
    }
    else {
        float pitch = static_cast<float>(midiNoteNumber) - detuneValue / 2;
        for (int oscIndex = 0; oscIndex < currentUnison; ++oscIndex)
        {
            oscA[oscIndex].setFrequency(440.f * std::powf(2, (pitch - 69.f) / 12.f));
            pitch += detuneValue / (currentUnison - 1);
        }
    }
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
//    jassert(isPrepared);
    if (!isPrepared)
        return;
    clearCurrentNote();
    for (int oscIndex = 0; oscIndex < currentUnison; ++oscIndex)
    {
//        std::cout << ((oscA.begin()+oscIndex) == nullptr) << std::endl;
//        oscA[oscIndex].phase = 0.f;
//        oscA[oscIndex].delta_phase = 0.f;
        oscA[oscIndex].stop();
    }
//    if (!allowTailOff)
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    return;
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    return;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    if (!isVoiceActive() || !isActive)
        return;
    
//    int actCount = 0;
//    for(int i=0; i<oscA.size(); ++i) {
//        if (oscA[i].isActive_)
//            actCount++;
//    }
//    std::cout << actCount << std::endl;
//    std::cout << "processing" << std::endl;
    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    voiceBuffer.clear();
    
    jassert(voiceBuffer.getNumChannels() == 2);
    for (int ch = 0; ch < voiceBuffer.getNumChannels(); ++ch)
    {
        float* buffer = voiceBuffer.getWritePointer (ch, 0);
        
        for (int oscIndex = 0; oscIndex < currentUnison; ++oscIndex)
        {
            if (!oscA[oscIndex].isActive_)
                continue;
            int midOsc = currentUnison >> 1;
            if ( (currentUnison & 1) && (oscIndex == midOsc) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( (currentUnison & 1) && (oscIndex <= midOsc) && ((currentUnison >> 1) & 1) && (ch == 1) == (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( (currentUnison & 1) && (oscIndex <= midOsc) && !((currentUnison >> 1) & 1) && (ch == 1) != (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( (currentUnison & 1) && (oscIndex > midOsc) && ((currentUnison >> 1) & 1) && (ch == 1) != (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( (currentUnison & 1) && (oscIndex > midOsc) && !((currentUnison >> 1) & 1) && (ch == 1) == (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( ((currentUnison >> 1) & 1) && (ch == 1) == (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
            else if ( !((currentUnison >> 1) & 1) && (ch == 1) != (oscIndex & 1) )
            {
                oscA[oscIndex].renderNextChannel(buffer, startSample, numSamples, currentUnison);
            }
        }
    }
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, voiceBuffer, channel, 0, numSamples);
    }
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    jassert(currentUnison <= maxUnison);
    int activeOscCount = 0;
    for(int i = 0; i < maxUnison; ++i)
    {
        if (activeOscCount < currentUnison)
        {
            oscA.emplace_back(sampleRate, true);
            activeOscCount++;
        }
        else
        {
            oscA.emplace_back(sampleRate, false);
        }
    }
    isPrepared = true;
}

void SynthVoice::setActiveness (bool isActive)
{
    this->isActive = isActive;
}

void SynthVoice::setUnison (int newUnison)
{
    currentUnison = juce::jlimit(1, maxUnison, newUnison);
}

void SynthVoice::setSound (juce::SynthesiserSound* sound)
{
    jassert(this->canPlaySound(sound));
    for (int oscIndex = 0; oscIndex < currentUnison; ++oscIndex)
    {
//        oscA[oscIndex].setWavetableFromHarmonics(dynamic_cast<SynthSound*>(sound)->wavetable);
        oscA[oscIndex].setWavetable(dynamic_cast<SynthSound*>(sound)->getBasicWavetable(1));
    }
}
