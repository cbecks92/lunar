#pragma once

#include <vector>
#include <memory>
#include <cmath>

namespace CarouselReverb {

// Represents a single reverb layer in the carousel
struct ReverbLayer {
    std::vector<float> buffer;
    float fadeAlpha = 0.0f;      // 0.0 = silent, 1.0 = full volume
    float decayRate = 0.99f;     // feedback coefficient
    size_t writePos = 0;         // current write position
    size_t layerSize = 0;        // size of this layer's buffer
    bool isActive = false;

    ReverbLayer(size_t bufferSize) 
        : buffer(bufferSize, 0.0f), layerSize(bufferSize) {}

    void reset() {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        writePos = 0;
        fadeAlpha = 0.0f;
        isActive = false;
    }

    float process(float input, float feedback) {
        if (layerSize == 0) return 0.0f;

        // Read with feedback
        float output = buffer[writePos] * fadeAlpha;

        // Write new sample with feedback
        buffer[writePos] = input + (output * feedback * decayRate);

        // Advance write position
        writePos = (writePos + 1) % layerSize;

        return output;
    }
};

// Wankel Rotary Engine Effect + Leslie Speaker modulation
// Simulates both the high-pitched buzzing of a Wankel 13B engine 
// and classic Leslie speaker modulation
class RotaryEffect {
public:
    RotaryEffect() : mPhase(0.0f), mEnvelopePhase(0.0f) {}

    void initialize(int32_t sampleRate) {
        mSampleRate = sampleRate;
    }

    float process(float input, float speed, float depth) {
        // Wankel Rotary Engine + Leslie Speaker Effect
        // Speed: 0.1-2.0 Hz for Leslie, 0.5-3.0 for engine RPM simulation
        // Depth: 0-1.0 for modulation amount
        
        if (mSampleRate == 0) return input;
        
        // Phase increment for envelope/rotor speed
        float phaseIncrement = (speed * 2.0f * 3.14159f) / mSampleRate;
        mPhase += phaseIncrement;
        if (mPhase > 6.28318f) mPhase -= 6.28318f;
        
        // Wankel engine has 2 rotors per cycle, creating unique harmonics
        // Each rotor fires twice per rotor revolution
        // Fundamental frequency + harmonics characteristic of dual-rotor system
        
        // Primary rotor modulation (fundamental engine sound)
        float rotor1 = std::sin(mPhase);
        
        // Secondary rotor (offset by 90 degrees for dual-rotor effect)
        float rotor2 = std::sin(mPhase * 0.5f + 1.57f);
        
        // Wankel harmonic content: 2x and 3x harmonics are strong
        float harmonic2x = std::sin(mPhase * 2.0f) * 0.4f;
        float harmonic3x = std::sin(mPhase * 3.0f) * 0.2f;
        
        // Engine-like response with envelope
        float envelope = std::abs(std::sin(mEnvelopePhase)) * 0.5f + 0.5f;
        mEnvelopePhase += phaseIncrement * 0.5f;
        if (mEnvelopePhase > 6.28318f) mEnvelopePhase -= 6.28318f;
        
        // Combine rotors with harmonics for that characteristic buzz
        float engineCharacter = (rotor1 * 0.6f + rotor2 * 0.3f + harmonic2x + harmonic3x) * envelope;
        
        // Leslie speaker spinning effect (subtle cross-modulation)
        float leslie = rotor1 * 0.3f;
        
        // Blend engine buzz with leslie effect
        float modulation = 1.0f + (engineCharacter * depth * 0.5f) + (leslie * depth * 0.1f);
        
        // Add slight saturation for that aggressive turbo character
        if (depth > 0.5f) {
            modulation = std::tanh(modulation * 1.5f) / 1.5f;
        }
        
        return input * modulation;
    }

    void reset() {
        mPhase = 0.0f;
        mEnvelopePhase = 0.0f;
    }

private:
    float mPhase = 0.0f;
    float mEnvelopePhase = 0.0f;
    int32_t mSampleRate = 44100;
};

// Main carousel reverb processor
class CarouselReverb {
public:
    CarouselReverb() : mLayers(8) {
        for (size_t i = 0; i < mLayers.size(); ++i) {
            mLayers[i] = std::make_unique<ReverbLayer>(44100);
        }
    }

    ~CarouselReverb() = default;

    void initialize(int32_t sampleRate) {
        mSampleRate = sampleRate;
        mRotary.initialize(sampleRate);
        updateLayerSizes();
        reset();
    }

    float process(float input, float tempo, float decayTime, 
                  float feedback, unsigned int activeLayerCount) {
        if (activeLayerCount == 0) return input;

        float output = 0.0f;

        // Update fade levels based on layer age
        updateLayerFades(tempo, decayTime);

        // Process all active layers
        for (unsigned int i = 0; i < activeLayerCount && i < mLayers.size(); ++i) {
            output += mLayers[i]->process(input, feedback);
        }

        return output / static_cast<float>(activeLayerCount);
    }

    float processWithRotary(float input, float tempo, float decayTime, 
                           float feedback, unsigned int activeLayerCount,
                           float rotarySpeed, float rotaryDepth) {
        // Get base reverb output
        float revOut = process(input, tempo, decayTime, feedback, activeLayerCount);
        
        // Apply rotary effect
        return mRotary.process(revOut, rotarySpeed, rotaryDepth);
    }

    void setNewLayer(unsigned int layerIndex) {
        if (layerIndex < mLayers.size()) {
            auto& layer = mLayers[layerIndex];
            layer->fadeAlpha = 1.0f;  // New layer starts at full volume
            layer->isActive = true;
            layer->writePos = 0;
            std::fill(layer->buffer.begin(), layer->buffer.end(), 0.0f);
        }
    }

    void reset() {
        for (auto& layer : mLayers) {
            if (layer) layer->reset();
        }
        mBarCount = 0;
    }

    void incrementBar() {
        mBarCount++;
    }

    int32_t getBarCount() const { return mBarCount; }

private:
    std::vector<std::unique_ptr<ReverbLayer>> mLayers;
    RotaryEffect mRotary;
    int32_t mSampleRate = 44100;
    int32_t mBarCount = 0;

    void updateLayerSizes() {
        // Size layers proportionally (each layer slightly larger)
        for (size_t i = 0; i < mLayers.size(); ++i) {
            size_t newSize = mSampleRate + (i * mSampleRate / 16);
            mLayers[i]->layerSize = newSize;
            mLayers[i]->buffer.resize(newSize, 0.0f);
        }
    }

    void updateLayerFades(float tempo, float decayTime) {
        // Fade out older layers exponentially
        float fadeDecay = std::exp(-1.0f / (decayTime * tempo));
        
        for (size_t i = 0; i < mLayers.size(); ++i) {
            mLayers[i]->fadeAlpha *= fadeDecay;

            // Reset if too quiet
            if (mLayers[i]->fadeAlpha < 0.001f) {
                mLayers[i]->isActive = false;
            }
        }
    }
};

} // namespace CarouselReverb
