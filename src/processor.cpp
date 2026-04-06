#include "processor.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/vstpresetkeys.h"
#include <cmath>
#include <algorithm>

namespace CarouselReverb {

using namespace Steinberg;

IMPLEMENT_REFCOUNT(Processor)

Processor::Processor() {
    setControllerClass(Vst::ControllerClassIID(0x87654321));
    mReverb = std::make_unique<CarouselReverb>();
    mLFO = std::make_unique<LFOEngine>();
}

Processor::~Processor() {}

tresult PLUGIN_API Processor::initialize(FUnknown* context) {
    tresult result = AudioEffect::initialize(context);
    if (result != kResultTrue)
        return result;

    // Setup audio i/o
    addAudioInput(STR16("Audio In"), Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Audio Out"), Vst::SpeakerArr::kStereo);

    // Initialize reverb and LFO
    if (mReverb) {
        mReverb->initialize(44100);
    }
    if (mLFO) {
        mLFO->initialize(44100);
    }

    return kResultTrue;
}

tresult PLUGIN_API Processor::terminate() {
    mReverb = nullptr;
    return AudioEffect::terminate();
}

tresult PLUGIN_API Processor::setActive(TBool state) {
    if (state && mReverb) {
        mReverb->reset();
    }
    return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::setIoMode(Vst::IoMode mode) {
    return kResultTrue;
}

tresult PLUGIN_API Processor::process(Vst::ProcessData& data) {
    // Process parameter changes
    if (data.inputParameterChanges) {
        processParameterChanges(data.inputParameterChanges);
    }

    // Get timing info from DAW
    if (data.processContext && data.processContext->state & Vst::ProcessContext::kTempoValid) {
        updateTiming(data.processContext->tempo, processSetup.sampleRate);
    }

    // Process audio
    if (data.numInputs > 0 && data.numOutputs > 0) {
        float** inputs = data.inputs[0].channelBuffers32;
        float** outputs = data.outputs[0].channelBuffers32;
        processAudio(inputs, outputs, data.numSamples, data.inputs[0].numChannels);
    }

    return kResultTrue;
}

void Processor::processParameterChanges(Vst::IParameterChanges* changes) {
    int32 numParamChanges = changes->getParameterCount();
    for (int32 i = 0; i < numParamChanges; ++i) {
        Vst::IParamValueQueue* queue = changes->getParameterData(i);
        if (!queue) continue;

        int32 sampleOffset = 0;
        Vst::ParamValue value = 0.0;

        int32 numPoints = queue->getPointCount();
        if (queue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
            uint32 paramId = queue->getParameterId();
            
            switch (paramId) {
                case PARAM_DECAY_TIME:
                    mParams.decayTime = value * 5.0f; // 0-5 seconds
                    break;
                case PARAM_WET_DRY:
                    mParams.wetDry = value;
                    break;
                case PARAM_TEMPO_SYNC:
                    mParams.tempoSync = value;
                    break;
                case PARAM_CROSSFADE:
                    mParams.crossfade = value;
                    break;
                case PARAM_LAYER_COUNT:
                    mParams.layerCount = 2.0f + (value * 6.0f); // 2-8 layers
                    break;
                case PARAM_FEEDBACK:
                    mParams.feedback = value * 0.95f; // 0-0.95 feedback
                    break;
                case PARAM_ROTARY_SPEED:
                    mParams.rotarySpeed = 0.1f + (value * 1.9f); // 0.1-2.0 Hz
                    break;
                case PARAM_ROTARY_DEPTH:
                    mParams.rotaryDepth = value; // 0-1.0
                    break;
                case PARAM_LFO_RATE:
                    mParams.lfoRate = 0.1f + (value * 9.9f); // 0.1-10 Hz
                    break;
                case PARAM_LFO_DEPTH:
                    mParams.lfoDepth = value; // 0-1.0
                    break;
                case PARAM_LFO_WAVEFORM:
                    mParams.lfoWaveform = static_cast<int>(value * 4.0f); // 0-4
                    break;
                case PARAM_LFO_TEMPO_SYNC:
                    mParams.lfoTempoSync = value;
                    break;
                case PARAM_LFO_TARGET:
                    mParams.lfoTarget = static_cast<int>(value * 7.0f); // 0-7 (8 parameters)
                    break;
                case PARAM_LFO_ENABLED:
                    mParams.lfoEnabled = value;
                    break;
            }
        }
    }
}

void Processor::updateTiming(double tempo, int32_t sampleRate) {
    mLastBPM = tempo;
    // Calculate samples per bar (assuming 4/4 time)
    mSamplesPerBar = static_cast<int32_t>(sampleRate * 60.0 * 4.0 / tempo);
}

void Processor::processAudio(float** inputs, float** outputs, 
                            int32_t numSamples, int32_t numChannels) {
    if (!mReverb || !mLFO) return;

    for (int32_t i = 0; i < numSamples; ++i) {
        // Get current parameters with optional LFO modulation
        float decayTime = mParams.decayTime;
        float rotarySpeed = mParams.rotarySpeed;
        float rotaryDepth = mParams.rotaryDepth;
        
        // Apply LFO if enabled
        if (mParams.lfoEnabled > 0.5f) {
            float lfoMod = mLFO->process(mParams.lfoRate, mParams.lfoDepth,
                                        static_cast<LFOEngine::Waveform>((int)mParams.lfoWaveform),
                                        mParams.lfoTempoSync > 0.5f);
            
            // Apply modulation to selected target parameter
            int targetParam = (int)mParams.lfoTarget;
            float modAmount = (lfoMod - 0.5f) * 2.0f;  // -1 to 1
            
            switch (targetParam) {
                case PARAM_DECAY_TIME:
                    decayTime = mParams.decayTime + (modAmount * mParams.decayTime * 0.5f);
                    decayTime = std::clamp(decayTime, 0.1f, 5.0f);
                    break;
                case PARAM_ROTARY_SPEED:
                    rotarySpeed = mParams.rotarySpeed + (modAmount * mParams.rotarySpeed * 0.5f);
                    rotarySpeed = std::clamp(rotarySpeed, 0.1f, 2.0f);
                    break;
                case PARAM_ROTARY_DEPTH:
                    rotaryDepth = mParams.rotaryDepth + (modAmount * 0.3f);
                    rotaryDepth = std::clamp(rotaryDepth, 0.0f, 1.0f);
                    break;
                // Add more modulation targets as needed
            }
        }
        
        // Check if we've completed a bar
        if (mSamplesPerBar > 0 && mSamplesSinceLastBar >= mSamplesPerBar) {
            mSamplesSinceLastBar = 0;
            
            // Add new layer (cycle through)
            unsigned int layerIdx = mReverb->getBarCount() % static_cast<int>(mParams.layerCount);
            mReverb->setNewLayer(layerIdx);
            mReverb->incrementBar();
        }

        // Process each channel
        float stereoSum = 0.0f;
        for (int32_t ch = 0; ch < numChannels; ++ch) {
            float input = inputs[ch][i];
            
            // Process through reverb with rotary effect
            float reverbOut = mReverb->processWithRotary(input, mLastBPM, decayTime,
                                                         mParams.feedback, 
                                                         static_cast<unsigned int>(mParams.layerCount),
                                                         rotarySpeed, rotaryDepth);
            
            // Mix wet/dry
            float output = input * (1.0f - mParams.wetDry) + reverbOut * mParams.wetDry;
            outputs[ch][i] = output;
            stereoSum += output;
        }

        mSamplesSinceLastBar++;
    }
}

tresult PLUGIN_API Processor::getRoutingInfo(Vst::RoutingInfo& inInfo, 
                                            Vst::RoutingInfo& outInfo) {
    return kResultTrue;
}

} // namespace CarouselReverb
