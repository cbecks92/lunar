#include "controller.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include <cstring>

namespace CarouselReverb {

using namespace Steinberg;
using namespace VSTGUI;

IMPLEMENT_REFCOUNT(Controller)

Controller::Controller() {
    // Reserve space for parameters
    // We'll add them during initialize
}

Controller::~Controller() {}

tresult PLUGIN_API Controller::initialize(FUnknown* context) {
    tresult result = EditController::initialize(context);
    if (result != kResultOk)
        return result;

    // Add parameters
    addParameter(STR16("Decay Time"), STR16("sec"), 
                 DEFAULT_DECAY_TIME / 5.0f, 0.0, 1.0, 100);
    
    addParameter(STR16("Wet/Dry"), STR16("%"), 
                 DEFAULT_WET_DRY, 0.0, 1.0, 100);
    
    addParameter(STR16("Tempo Sync"), STR16("bar"), 
                 DEFAULT_TEMPO_SYNC, 0.5, 4.0, 15);
    
    addParameter(STR16("Crossfade"), STR16(""), 
                 DEFAULT_CROSSFADE, 0.0, 1.0, 100);
    
    addParameter(STR16("Layer Count"), STR16(""), 
                 (4.0f - 2.0f) / 6.0f, 0.0, 1.0, 6);
    
    addParameter(STR16("Feedback"), STR16(""), 
                 0.5f / 0.95f, 0.0, 1.0, 95);

    addParameter(STR16("Rotary Speed"), STR16("Hz"), 
                 (DEFAULT_ROTARY_SPEED - 0.1f) / 1.9f, 0.0, 1.0, 100);
    
    addParameter(STR16("Rotary Depth"), STR16(""), 
                 DEFAULT_ROTARY_DEPTH, 0.0, 1.0, 100);

    return kResultTrue;
}

tresult PLUGIN_API Controller::terminate() {
    return EditController::terminate();
}

tresult PLUGIN_API Controller::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;
    return kResultTrue;
}

tresult PLUGIN_API Controller::getParameterInfo(int32 paramIndex, 
                                                Vst::ParameterInfo& info) {
    if (paramIndex < 0 || paramIndex >= getParameterCount())
        return kResultFalse;

    Vst::Parameter* param = getParameterObject(paramIndex);
    if (param) {
        info.id = param->getInfo().id;
        info.flags = param->getInfo().flags;
        info.stepCount = param->getInfo().stepCount;
        info.defaultNormalizedValue = param->getInfo().defaultNormalizedValue;
        UString(info.title, 128).fromAscii(param->getInfo().title);
        UString(info.units, 128).fromAscii(param->getInfo().units);
        return kResultTrue;
    }
    return kResultFalse;
}

tresult PLUGIN_API Controller::getUnitInfo(int32 unitIndex, 
                                          Vst::UnitInfo& info) {
    if (unitIndex != 0)
        return kResultFalse;

    info.id = 0;
    info.parentUnitId = kNoParentUnitId;
    info.programListId = kNoProgramListId;
    UString(info.name, 128).fromAscii("Carousel Reverb");
    return kResultTrue;
}

IPlugView* PLUGIN_API Controller::createView(const char* name) {
    if (strcmp(name, "editor") == 0) {
        return new VST3Editor(this, "CarouselReverbUI", "CarouselReverbUI.uidesc");
    }
    return nullptr;
}

void Controller::addParameter(const char* title, const char* units,
                             double defaultValue, double minValue, 
                             double maxValue, int32 stepCount, 
                             Vst::ParameterInfo::ParameterFlags flags) {
    Vst::RangeParameter* param = new Vst::RangeParameter(
        STR16(title),
        getParameterCount(),  // paramId
        STR16(units),
        minValue,
        maxValue,
        defaultValue,
        stepCount,
        flags
    );
    parameters.addParameter(param);
}

} // namespace CarouselReverb
