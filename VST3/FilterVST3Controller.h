#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "base/source/fstreamer.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

class FilterVST3Controller : public EditController
{
public:
    FilterVST3Controller();
    virtual ~FilterVST3Controller();

    // EditController overrides
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

    // Factory method
    static FUnknown* createInstance(void*) { return (IEditController*)new FilterVST3Controller(); }

    // Parameters
    enum ParameterIds
    {
        kFilterTypeId = 0,
        kCutoffFreqId = 1,
        kBypassId = 2
    };

private:
    // Parameter objects
    Parameter* mFilterTypeParam;
    Parameter* mCutoffFreqParam;
    Parameter* mBypassParam;
}; 