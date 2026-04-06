#include "pluginterfaces/base/ipluginbase.h"
#include "public.sdk/source/main/pluginfactory.h"
#include "processor.h"
#include "controller.h"
#include "constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

BEGIN_FACTORY_DEF(CarouselReverb::PLUGIN_VENDOR,
                  CarouselReverb::PLUGIN_URL,
                  CarouselReverb::PLUGIN_EMAIL)

    DEF_CLASS2(INLINE_UID_FROM_FUID(CarouselReverb::ProcessorUID),
               PClassInfo::kManyInstances,
               kAudioEffectClass,
               CarouselReverb::PLUGIN_NAME,
               Vst::kDistributable,
               CarouselReverb::PLUGIN_SUBCATEGORY,
               CarouselReverb::PLUGIN_VERSION,
               kVstVersionString,
               CarouselReverb::Processor::createInstance)

    DEF_CLASS2(INLINE_UID_FROM_FUID(CarouselReverb::ControllerUID),
               PClassInfo::kManyInstances,
               kEditControllerClass,
               CarouselReverb::PLUGIN_NAME " Controller",
               0,
               "",
               CarouselReverb::PLUGIN_VERSION,
               kVstVersionString,
               CarouselReverb::Controller::createInstance)

END_FACTORY
