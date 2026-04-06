#pragma once

#include <vector>
#include <cmath>
#include <array>

namespace CarouselReverb {

/**
 * Advanced LFO Engine
 * Multiple waveforms, assignable modulation, visual waveform rendering
 */
class LFOEngine {
public:
    enum class Waveform {
        Sine = 0,
        Triangle = 1,
        Sawtooth = 2,
        Square = 3,
        Random = 4,
        COUNT = 5
    };

    LFOEngine();
    
    void initialize(int32_t sampleRate);
    
    // Process one sample and return modulation value (0.0 - 1.0)
    float process(float rate, float depth, Waveform waveform, bool tempoSync);
    
    // Get modulation at specific phase for UI visualization
    float getValueAtPhase(float phase, Waveform waveform) const;
    
    // Fill buffer with waveform data for UI drawing
    void fillWaveformBuffer(Waveform waveform, std::vector<float>& buffer, size_t numSamples);
    
    void reset();
    void setPhase(float phase) { mPhase = phase; }
    float getPhase() const { return mPhase; }

private:
    float mPhase = 0.0f;
    float mRandomValue = 0.0f;
    float mLastRandomValue = 0.0f;
    int32_t mSampleRate = 44100;
    int32_t mSamplesSinceRandomUpdate = 0;
    
    static constexpr int32_t RANDOM_UPDATE_INTERVAL = 2048; // Update random every ~2048 samples
    
    float processSine(float rate);
    float processTriangle(float rate);
    float processSawtooth(float rate);
    float processSquare(float rate);
    float processRandom(float rate);
    
    float generateRandomValue();
    
    static const char* getWaveformName(Waveform wf);
};

/**
 * LFO Assignment - maps LFO to a target parameter
 */
struct LFOAssignment {
    bool enabled = false;
    int32_t targetParamId = -1;  // Parameter to modulate
    LFOEngine::Waveform waveform = LFOEngine::Waveform::Sine;
    float rate = 1.0f;            // Hz
    float depth = 0.5f;           // Modulation amount (0-1)
    bool tempoSync = false;       // Sync to DAW tempo
    
    void reset() {
        enabled = false;
        targetParamId = -1;
        waveform = LFOEngine::Waveform::Sine;
        rate = 1.0f;
        depth = 0.5f;
        tempoSync = false;
    }
};

} // namespace CarouselReverb
