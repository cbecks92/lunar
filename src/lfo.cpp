#include "lfo.h"
#include <random>
#include <algorithm>

namespace CarouselReverb {

LFOEngine::LFOEngine() : mPhase(0.0f), mRandomValue(0.0f) {}

void LFOEngine::initialize(int32_t sampleRate) {
    mSampleRate = sampleRate;
    reset();
}

float LFOEngine::process(float rate, float depth, Waveform waveform, bool tempoSync) {
    if (mSampleRate == 0) return 0.5f;
    
    float lfoValue = 0.0f;
    
    switch (waveform) {
        case Waveform::Sine:
            lfoValue = processSine(rate);
            break;
        case Waveform::Triangle:
            lfoValue = processTriangle(rate);
            break;
        case Waveform::Sawtooth:
            lfoValue = processSawtooth(rate);
            break;
        case Waveform::Square:
            lfoValue = processSquare(rate);
            break;
        case Waveform::Random:
            lfoValue = processRandom(rate);
            break;
        default:
            lfoValue = processSine(rate);
    }
    
    // Convert from 0-1 range to bipolar (-1 to 1) and apply depth
    float modulation = (lfoValue - 0.5f) * 2.0f;  // -1 to 1
    modulation *= depth;
    
    return 0.5f + (modulation * 0.5f);  // Back to 0-1 range with modulation applied
}

float LFOEngine::processSine(float rate) {
    // Phase increment: rate Hz -> phase change per sample
    float phaseIncrement = (rate * 6.28318f) / mSampleRate;  // 2*pi*rate/samplerate
    
    mPhase += phaseIncrement;
    while (mPhase >= 6.28318f) mPhase -= 6.28318f;
    
    // Sine from 0 to 1
    return (std::sin(mPhase) + 1.0f) * 0.5f;
}

float LFOEngine::processTriangle(float rate) {
    float phaseIncrement = rate / mSampleRate;
    
    mPhase += phaseIncrement;
    while (mPhase >= 1.0f) mPhase -= 1.0f;
    
    // Triangle wave: 0->1 first half, 1->0 second half
    if (mPhase < 0.5f) {
        return mPhase * 2.0f;
    } else {
        return 2.0f - (mPhase * 2.0f);
    }
}

float LFOEngine::processSawtooth(float rate) {
    float phaseIncrement = rate / mSampleRate;
    
    mPhase += phaseIncrement;
    while (mPhase >= 1.0f) mPhase -= 1.0f;
    
    // Sawtooth: 0->1 linearly
    return mPhase;
}

float LFOEngine::processSquare(float rate) {
    float phaseIncrement = rate / mSampleRate;
    
    mPhase += phaseIncrement;
    while (mPhase >= 1.0f) mPhase -= 1.0f;
    
    // Square: 0 first half, 1 second half
    return mPhase < 0.5f ? 0.0f : 1.0f;
}

float LFOEngine::processRandom(float rate) {
    float phaseIncrement = rate / mSampleRate;
    
    mPhase += phaseIncrement;
    mSamplesSinceRandomUpdate++;
    
    // Update random value periodically
    if (mSamplesSinceRandomUpdate >= RANDOM_UPDATE_INTERVAL) {
        mLastRandomValue = mRandomValue;
        mRandomValue = generateRandomValue();
        mSamplesSinceRandomUpdate = 0;
    }
    
    // Smooth interpolation between random values
    float t = static_cast<float>(mSamplesSinceRandomUpdate) / RANDOM_UPDATE_INTERVAL;
    return mLastRandomValue + (mRandomValue - mLastRandomValue) * t;
}

float LFOEngine::generateRandomValue() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

float LFOEngine::getValueAtPhase(float phase, Waveform waveform) const {
    // Normalize phase to 0-1 range
    while (phase >= 1.0f) phase -= 1.0f;
    while (phase < 0.0f) phase += 1.0f;
    
    switch (waveform) {
        case Waveform::Sine:
            return (std::sin(phase * 6.28318f) + 1.0f) * 0.5f;
        case Waveform::Triangle:
            return phase < 0.5f ? phase * 2.0f : 2.0f - (phase * 2.0f);
        case Waveform::Sawtooth:
            return phase;
        case Waveform::Square:
            return phase < 0.5f ? 0.0f : 1.0f;
        case Waveform::Random:
            return 0.5f;  // Can't show random for static phase
        default:
            return 0.5f;
    }
}

void LFOEngine::fillWaveformBuffer(Waveform waveform, std::vector<float>& buffer, size_t numSamples) {
    buffer.resize(numSamples);
    
    for (size_t i = 0; i < numSamples; ++i) {
        float phase = static_cast<float>(i) / numSamples;
        buffer[i] = getValueAtPhase(phase, waveform);
    }
}

void LFOEngine::reset() {
    mPhase = 0.0f;
    mRandomValue = 0.5f;
    mLastRandomValue = 0.5f;
    mSamplesSinceRandomUpdate = 0;
}

const char* LFOEngine::getWaveformName(Waveform wf) {
    switch (wf) {
        case Waveform::Sine: return "Sine";
        case Waveform::Triangle: return "Triangle";
        case Waveform::Sawtooth: return "Sawtooth";
        case Waveform::Square: return "Square";
        case Waveform::Random: return "Random";
        default: return "Unknown";
    }
}

} // namespace CarouselReverb
