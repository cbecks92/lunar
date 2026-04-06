#pragma once

#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "base/source/fobject.h"
#include "carousel.h"
#include "constants.h"
#include <vector>
#include <memory>

namespace CarouselReverb {

using namespace Steinberg;

class Processor : public Vst::AudioEffect {
public:
    Processor();
    ~Processor() SMTG_OVERRIDE;

    // IPluginBase
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;

    // AudioEffect
    tresult PLUGIN_API setIoMode(Vst::IoMode mode) SMTG_OVERRIDE;
    tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;

    // Parameter management
    tresult PLUGIN_API getRoutingInfo(Vst::RoutingInfo& inInfo, 
                                      Vst::RoutingInfo& outInfo) SMTG_OVERRIDE;

    static FUnknown* createInstance(void*) {
        return (Vst::IAudioProcessor*)new Processor();
    }

    static DECLARE_CLASS_IID()

private:
    // Parameters
    struct Parameters {
        float decayTime = DEFAULT_DECAY_TIME;
        float wetDry = DEFAULT_WET_DRY;
        float tempoSync = DEFAULT_TEMPO_SYNC;
        float crossfade = DEFAULT_CROSSFADE;
        float layerCount = 4.0f;
        float feedback = 0.5f;
        float rotarySpeed = DEFAULT_ROTARY_SPEED;
        float rotaryDepth = DEFAULT_ROTARY_DEPTH;
    } mParams;

    // Audio processing
    std::unique_ptr<CarouselReverb> mReverb;
    std::vector<float> mInputBuffer;
    std::vector<float> mOutputBuffer;

    // Timing
    int32_t mSamplesSinceLastBar = 0;
    double mLastBPM = 120.0;
    int32_t mSamplesPerBar = 0;

    void processParameterChanges(Vst::IParameterChanges* changes);
    void updateTiming(double tempo, int32_t sampleRate);
    void processAudio(float** inputs, float** outputs, 
                     int32_t numSamples, int32_t numChannels);
};

} // namespace CarouselReverb
