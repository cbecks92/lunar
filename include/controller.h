#pragma once

#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstunits.h"
#include "pluginterfaces/base/fstring.h"
#include "base/source/fobject.h"
#include "constants.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace CarouselReverb {

using namespace Steinberg;

class Controller : public Vst::EditController, 
                   public Vst::IUnitInfo {
public:
    Controller();
    ~Controller() SMTG_OVERRIDE;

    // IPluginBase
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;

    // EditController
    tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getParameterInfo(int32 paramIndex, 
                                        Vst::ParameterInfo& info) SMTG_OVERRIDE;

    IPlugView* PLUGIN_API createView(const char* name) SMTG_OVERRIDE;

    // IUnitInfo
    int32 PLUGIN_API getUnitCount() SMTG_OVERRIDE { return 1; }
    tresult PLUGIN_API getUnitInfo(int32 unitIndex, 
                                   Vst::UnitInfo& info) SMTG_OVERRIDE;

    static FUnknown* createInstance(void*) {
        return (Vst::IEditController*)new Controller();
    }

    static DECLARE_CLASS_IID()

private:
    void addParameter(const char* title, const char* units,
                     double defaultValue, double minValue, 
                     double maxValue, int32 stepCount, 
                     Vst::ParameterInfo::ParameterFlags flags = 0);
};

} // namespace CarouselReverb
