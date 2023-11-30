/*
  ==============================================================================

    SynthOscillator.cpp
    Created: 17 Feb 2023 7:37:22pm
    
    This class implemented a basic oscillator

  ==============================================================================
*/

#include "SynthOscillator.h"

SynthOscillator::SynthOscillator () : sampleRate_(44100), isActive_(true)
{
}

SynthOscillator::SynthOscillator (double sampleRate) : sampleRate_(sampleRate), isActive_(true)
{
}

SynthOscillator::SynthOscillator (double sampleRate, bool isActive) : sampleRate_(sampleRate), isActive_(isActive)
{
}

void SynthOscillator::setSampleRate (double newSampleRate)
{
    this->sampleRate_ = newSampleRate;
}

void SynthOscillator::setActiveness (bool isActive)
{
    this->isActive_ = isActive;
}

void SynthOscillator::setFrequency (float newFrequency)
{
//    std::array<float, 2 * SynthSound::wavetableSize> temp;
//    std::copy(harmonics.begin(), harmonics.end(), temp.begin());
//
//    juce::dsp::FFT inverseFFT((static_cast<int>(log2(SynthSound::wavetableSize))));
//    inverseFFT.performRealOnlyInverseTransform(temp.data());
//
//    std::copy(std::begin(temp), std::begin(temp) + SynthSound::wavetableSize, std::back_inserter(wavetable));
    
//    std::cout << newFrequency << std::endl;
//    for (int i=0; i<wavetable.size(); ++i) {
//        std::cout << wavetable[i] << ' ';
//    }
//    std::cout << newFrequency << std::endl;
    
    phase = 0.f;
    delta_phase = newFrequency / static_cast<float>(sampleRate_);
}

void SynthOscillator::setWavetable(std::vector<float> newWavetable)
{
//    wavetable = newWavetable;
    wavetable.clear();
    wavetable.assign(newWavetable.begin(), newWavetable.end());
}

void SynthOscillator::setWavetableFromHarmonics (std::array<float, 2 * SynthSound::wavetableSize> harmonics)
{
//    std::copy(harmonics.begin(), harmonics.end(), this->harmonics.begin());
//
//    std::array<float, 2 * SynthSound::wavetableSize> temp;
//    std::copy(harmonics.begin(), harmonics.end(), temp.begin());
//
//    juce::dsp::FFT inverseFFT((static_cast<int>(log2(SynthSound::wavetableSize))));
//    inverseFFT.performRealOnlyInverseTransform(temp.data());
//
//    std::copy(std::begin(temp), std::begin(temp) + SynthSound::wavetableSize, std::back_inserter(wavetable));
}

void SynthOscillator::stop()
{
    phase = 0.f;
    delta_phase = 0.f;
}

float SynthOscillator::getNextSample ()
{
    phase += delta_phase;
    phase = phase - std::floor(phase);
    float scaled_phase = phase * SynthSound::wavetableSize;
    int sample_index = scaled_phase;
    float sample_offset = scaled_phase - sample_index;
    
    int x0 = (sample_index - 1) >= 0 ? sample_index - 1 : sample_index - 1 + SynthSound::wavetableSize;
    int x2 = (sample_index + 1) < SynthSound::wavetableSize ? sample_index + 1 : sample_index + 1 - SynthSound::wavetableSize;
    int x3 = (sample_index + 2) < SynthSound::wavetableSize ? sample_index + 2 : sample_index + 2 - SynthSound::wavetableSize;
    
//    return hermiteInterpolation(wavetable[x0], wavetable[sample_index], wavetable[x2], wavetable[x3], sample_offset);
    return linearInterpolation(wavetable[sample_index], wavetable[x2], sample_offset);
}

inline float SynthOscillator::hermiteInterpolation (float value0, float value1, float value2, float value3, float sample_offset)
{
    const float c = (value2 - value0) * 0.5f;
    const float v = value1 - value2;
    const float w = c + v;
    const float a = w + v + (value3 - value1) * 0.5f;
    const float b_neg = w + a;
    return ((((a * sample_offset) - b_neg) * sample_offset + c) * sample_offset + value1);
}

inline float SynthOscillator::linearInterpolation (float start, float end, float sample_offset)
{
    return std::lerp(start, end, sample_offset);
}

void SynthOscillator::renderNextChannel (float* channelBuffer, int startSample, int numSamples, int unison)
{
    if (!isActive_)
        return;
//    phase = 0.f;
    for (int s = 0; s < numSamples; ++s)
    {
        channelBuffer[s + startSample] += this->getNextSample() * 0.85f / unison;
    }
}
