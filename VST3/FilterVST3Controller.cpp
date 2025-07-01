#include "FilterVST3Controller.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

FilterVST3Controller::FilterVST3Controller()
: mFilterTypeParam(nullptr)
, mCutoffFreqParam(nullptr)
, mBypassParam(nullptr)
{
    setControllerClass(FilterVST3ControllerUID);
}

FilterVST3Controller::~FilterVST3Controller()
{
}

tresult FilterVST3Controller::initialize(FUnknown* context)
{
    tresult result = EditController::initialize(context);
    if (result == kResultTrue)
    {
        // Create parameters
        mFilterTypeParam = new RangeParameter(STR16("Filter Type"), kFilterTypeId, nullptr, 0, 1, 0, 0, ParameterInfo::kCanAutomate);
        mFilterTypeParam->setPrecision(0);
        parameters.addParameter(mFilterTypeParam);

        mCutoffFreqParam = new RangeParameter(STR16("Cutoff Frequency"), kCutoffFreqId, nullptr, 20, 20000, 1000, 0, ParameterInfo::kCanAutomate);
        mCutoffFreqParam->setPrecision(0);
        parameters.addParameter(mCutoffFreqParam);

        mBypassParam = new RangeParameter(STR16("Bypass"), kBypassId, nullptr, 0, 1, 0, 0, ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass);
        mBypassParam->setPrecision(0);
        parameters.addParameter(mBypassParam);
    }
    return result;
}

tresult FilterVST3Controller::terminate()
{
    return EditController::terminate();
}

tresult FilterVST3Controller::setComponentState(IBStream* state)
{
    if (!state) return kResultFalse;
    
    IBStreamer streamer(state, kLittleEndian);
    
    float savedCutoff = 0.0f;
    int savedType = 0;
    float savedBypass = 0.0f;
    
    if (streamer.readFloat(savedCutoff) == false) return kResultFalse;
    if (streamer.readInt32(savedType) == false) return kResultFalse;
    if (streamer.readFloat(savedBypass) == false) return kResultFalse;
    
    if (mCutoffFreqParam) mCutoffFreqParam->setNormalized(savedCutoff / 20000.0f);
    if (mFilterTypeParam) mFilterTypeParam->setNormalized(savedType);
    if (mBypassParam) mBypassParam->setNormalized(savedBypass);
    
    return kResultOk;
}

tresult FilterVST3Controller::setState(IBStream* state)
{
    return setComponentState(state);
}

tresult FilterVST3Controller::getState(IBStream* state)
{
    if (!state) return kResultFalse;
    
    IBStreamer streamer(state, kLittleEndian);
    
    float cutoff = mCutoffFreqParam ? mCutoffFreqParam->getNormalized() * 20000.0f : 1000.0f;
    int type = mFilterTypeParam ? (int)mFilterTypeParam->getNormalized() : 0;
    float bypass = mBypassParam ? mBypassParam->getNormalized() : 0.0f;
    
    streamer.writeFloat(cutoff);
    streamer.writeInt32(type);
    streamer.writeFloat(bypass);
    
    return kResultOk;
} 