/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraniteAudioProcessor::GraniteAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    SynthSound* saw = new SynthSound();
    std::vector<float> sawSamples = saw->getBasicWavetable(1);
    SynthSound* sine = new SynthSound();
    std::vector<float> sineSamples = sine->getBasicWavetable(0);
    saw->setWavetable(sawSamples);
    sine->setWavetable(sineSamples);
    synth.addSound(saw);
//    synth.addSound(sine);
    
    for (int i=0; i<maxVoices; i++)
    {
        synth.addVoice(new SynthVoice());
    }
}

GraniteAudioProcessor::~GraniteAudioProcessor()
{
}

//==============================================================================
const juce::String GraniteAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GraniteAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GraniteAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GraniteAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GraniteAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GraniteAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GraniteAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GraniteAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GraniteAudioProcessor::getProgramName (int index)
{
    return {};
}

void GraniteAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GraniteAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    int activeVoicesCount = 0;
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
//            synth.getSound(0);
            voice->setSound(dynamic_cast<SynthSound*>(synth.getSound(0).get()));
            if (activeVoicesCount < currentVoices) {
                voice->setActiveness(true);
                activeVoicesCount++;
            }
            else
                voice->setActiveness(false);
        }
    }
}

void GraniteAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GraniteAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GraniteAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int activeVoicesCount = 0;
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            if (activeVoicesCount < currentVoices) {
                voice->setActiveness(true);
                activeVoicesCount++;
            }
            else
                voice->setActiveness(false);
        }
    }
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
//    for (const auto midiMetadata : midiMessages)
//    {
//        const auto message = midiMetadata.getMessage();
//        if (message.isNoteOn())
//            std::cout << message.getNoteNumber() << std::endl;
//    }
    
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool GraniteAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GraniteAudioProcessor::createEditor()
{
    return new GraniteAudioProcessorEditor (*this, keyboardState);
}

//==============================================================================
void GraniteAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GraniteAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraniteAudioProcessor();
}
