/*
  ==============================================================================

    SynthSound.cpp
    Created: 14 Feb 2023 6:11:50pm
    
    This class overrides from juce::SynthesiserSound to store sound wavetable

  ==============================================================================
*/

#include "SynthSound.h"

using std::vector;
using std::array;

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return midiNoteNumber < 0 ? false : midiNoteNumber < 128;
}

bool SynthSound::appliesToChannel (int midiChannel)
{
    return true;
}

void SynthSound::setWavetable (vector<float> samples)
{
    jassert(samples.size() >= wavetableSize);
    std::copy_n(samples.begin(), wavetableSize, wavetable.begin());
//    for(int i=0; i<500; ++i) {
//        auto& num = samples[i];
//        std::cout << num << ' ';
//    }
    juce::dsp::FFT forwardFFT(static_cast<int>(log2(wavetableSize)));
    forwardFFT.performRealOnlyForwardTransform(wavetable.data(), true);
    
//    std::cout << "First 100 element after forward FFT" << std::endl;
//    for(int i=0; i<100; ++i) {
//        auto& num = waveTable[i];
//        std::cout << num << ' ';
//    }
//    std::cout << std::endl;
    
//    forwardFFT.performRealOnlyInverseTransform(waveTable.data());
//    std::cout << "First 100 element after inverse FFT" << std::endl;
//    for(int i=0; i<100; ++i) {
//        auto& num = waveTable[i];
//        std::cout << num << ' ';
//    }
//    std::reverse_copy(waveTable.begin(), waveTable.end(), std::ostream_iterator<float>(std::cout, " "));
//    std::cout << "end" << std::endl;
//    for(int i=0; i<512; ++i) {
//        float propx = 1.0f - std::exp (std::log (1.0f - (float) i / (float) 512) * 0.2f);
//        auto index = juce::jlimit (0, 1024, (int) (propx * (float) 2048 * 0.5f));
//        std::cout << waveTable.data()[index] << ' ';
//    }
}

vector<float> SynthSound::getBasicWavetable(int position)
{
    vector<float> samples = vector<float>(wavetableSize);
    switch (position) {
        case 0: //sine wave
            for (auto i = 0; i < wavetableSize; ++i)
                samples[i] = std::sinf(juce::MathConstants<float>::twoPi * static_cast<float>(i) / wavetableSize);
            break;
        case 1: //saw wave
            for (auto i = 0; i < wavetableSize; ++i)
                samples[i] = 1.f - (2.f * static_cast<float>(i) / wavetableSize);
            break;
        //case 2-7
    }
    return samples;
}
