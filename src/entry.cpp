#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "base/source/fobject.h"
#include "processor.h"
#include "controller.h"
#include "constants.h"

namespace CarouselReverb {

using namespace Steinberg;

// Define class IDs
DEF_CLASS_IID(Processor)
DEF_CLASS_IID(Controller)

} // namespace CarouselReverb

// Entry point for the plugin
bool InitModule() {
    return true;
}

bool DeinitModule() {
    return true;
}

extern "C" {

// EXPORT statement for Windows DLL
#ifdef _WIN32
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT
#endif

// Factory method
PLUGIN_EXPORT Steinberg::IPluginFactory* PLUGIN_API GetPluginFactory() {
    static Steinberg::FClassFactory factory;
    
    factory.registerClass(
        Steinberg::Vst::ProcessorClassIID(),
        Steinberg::PClassInfo::kNoFlags,
        Steinberg::Vst::ControllerClassIID(),
        CarouselReverb::PLUGIN_NAME,
        Steinberg::Vst::kDistributable,
        CarouselReverb::PLUGIN_VENDOR,
        CarouselReverb::PLUGIN_URL,
        CarouselReverb::PLUGIN_VERSION,
        CarouselReverb::Processor::createInstance
    );

    factory.registerClass(
        Steinberg::Vst::ControllerClassIID(),
        Steinberg::PClassInfo::kNoFlags,
        0,
        CarouselReverb::PLUGIN_NAME,
        Steinberg::Vst::kDistributable,
        CarouselReverb::PLUGIN_VENDOR,
        CarouselReverb::PLUGIN_URL,
        CarouselReverb::PLUGIN_VERSION,
        CarouselReverb::Controller::createInstance
    );

    return &factory;
}

} // extern "C"
