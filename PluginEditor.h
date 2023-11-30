/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GraniteAudioProcessorEditor  : public juce::AudioProcessorEditor , private juce::Timer
{
public:
    GraniteAudioProcessorEditor (GraniteAudioProcessor&, juce::MidiKeyboardState& keyboardState);
    ~GraniteAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GraniteAudioProcessor& audioProcessor;
    
    juce::MidiKeyboardState& keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraniteAudioProcessorEditor)
};
