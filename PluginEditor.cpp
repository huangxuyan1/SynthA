/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraniteAudioProcessorEditor::GraniteAudioProcessorEditor (GraniteAudioProcessor& p, juce::MidiKeyboardState& keyboardState)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardState (keyboardState), keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible (keyboardComponent);

    setSize (600, 200);
    startTimer (400);
}

GraniteAudioProcessorEditor::~GraniteAudioProcessorEditor()
{
}

//==============================================================================
void GraniteAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GraniteAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //5 * 4 * 29
    keyboardComponent.setBounds (10, 50, getWidth() - 20, getHeight() - 60);
}

void GraniteAudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}
