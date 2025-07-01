#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/base/ustring.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

class FilterVST3 : public AudioEffect
{
public:
    FilterVST3();
    virtual ~FilterVST3();

    // AudioEffect overrides
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
    tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup) SMTG_OVERRIDE;

    // Factory method
    static FUnknown* createInstance(void*) { return (IAudioProcessor*)new FilterVST3(); }

    // Parameters
    enum ParameterIds
    {
        kFilterTypeId = 0,
        kCutoffFreqId = 1
    };

private:
    // Filter state variables
    float m_sampleRate;
    float m_cutoffFreq;
    int m_filterType; // 0 = LPF, 1 = HPF
    
    // Filter memory
    float m_lastOutput[2]; // stereo
    float m_lastInput[2];  // for HPF
    
    // Filter functions
    float applyLowPassFilter(float input, int channel);
    float applyHighPassFilter(float input, int channel);
};