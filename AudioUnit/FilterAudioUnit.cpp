#include "FilterAudioUnit.h"
#include <cmath>
#include <algorithm>

// Component registration
static const ComponentDescription kAudioUnitDescription = {
    .componentType = kAudioUnitType_Effect,
    .componentSubType = 'Fltr',  // 4-character unique ID
    .componentManufacturer = 'Demo',  // Your manufacturer ID
    .componentFlags = 0,
    .componentFlagsMask = 0
};

FilterAudioUnit::FilterAudioUnit(AudioUnit inAudioUnit)
    : mAudioUnit(inAudioUnit)
    , mCutoffFrequency(1000.0f)
    , mFilterType(kFilterType_LowPass)
    , mSampleRate(44100.0)
    , mInitialized(false)
{
    // Initialize filter state
    ResetFilter();
    
    // Set default stream format
    mStreamFormat.mSampleRate = 44100.0;
    mStreamFormat.mFormatID = kAudioFormatLinearPCM;
    mStreamFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
    mStreamFormat.mBytesPerPacket = sizeof(Float32);
    mStreamFormat.mFramesPerPacket = 1;
    mStreamFormat.mBytesPerFrame = sizeof(Float32);
    mStreamFormat.mChannelsPerFrame = 2;
    mStreamFormat.mBitsPerChannel = 32;
}

FilterAudioUnit::~FilterAudioUnit() {
}

OSStatus FilterAudioUnit::Initialize() {
    if (mInitialized) {
        return noErr;
    }
    
    ResetFilter();
    mInitialized = true;
    return noErr;
}

OSStatus FilterAudioUnit::Uninitialize() {
    mInitialized = false;
    return noErr;
}

OSStatus FilterAudioUnit::GetPropertyInfo(AudioUnitPropertyID inID,
                                         AudioUnitScope inScope,
                                         AudioUnitElement inElement,
                                         UInt32& outDataSize,
                                         Boolean& outWritable) {
    switch (inID) {
        case kAudioUnitProperty_StreamFormat:
            outDataSize = sizeof(AudioStreamBasicDescription);
            outWritable = true;
            return noErr;
            
        case kAudioUnitProperty_SampleRate:
            outDataSize = sizeof(Float64);
            outWritable = true;
            return noErr;
            
        case kAudioUnitProperty_MaximumFramesPerSlice:
            outDataSize = sizeof(UInt32);
            outWritable = true;
            return noErr;
            
        case kAudioUnitProperty_ParameterList:
            outDataSize = kNumberOfParameters * sizeof(AudioUnitParameterID);
            outWritable = false;
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

OSStatus FilterAudioUnit::GetProperty(AudioUnitPropertyID inID,
                                     AudioUnitScope inScope,
                                     AudioUnitElement inElement,
                                     void* outData,
                                     UInt32& ioDataSize) {
    switch (inID) {
        case kAudioUnitProperty_StreamFormat:
            if (ioDataSize < sizeof(AudioStreamBasicDescription)) {
                return kAudioUnitErr_InvalidPropertyValue;
            }
            *(AudioStreamBasicDescription*)outData = mStreamFormat;
            ioDataSize = sizeof(AudioStreamBasicDescription);
            return noErr;
            
        case kAudioUnitProperty_SampleRate:
            if (ioDataSize < sizeof(Float64)) {
                return kAudioUnitErr_InvalidPropertyValue;
            }
            *(Float64*)outData = mSampleRate;
            ioDataSize = sizeof(Float64);
            return noErr;
            
        case kAudioUnitProperty_ParameterList:
            if (ioDataSize < kNumberOfParameters * sizeof(AudioUnitParameterID)) {
                return kAudioUnitErr_InvalidPropertyValue;
            }
            AudioUnitParameterID* paramList = (AudioUnitParameterID*)outData;
            paramList[0] = kParam_FilterType;
            paramList[1] = kParam_CutoffFrequency;
            ioDataSize = kNumberOfParameters * sizeof(AudioUnitParameterID);
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

OSStatus FilterAudioUnit::SetProperty(AudioUnitPropertyID inID,
                                     AudioUnitScope inScope,
                                     AudioUnitElement inElement,
                                     const void* inData,
                                     UInt32 inDataSize) {
    switch (inID) {
        case kAudioUnitProperty_StreamFormat:
            if (inDataSize < sizeof(AudioStreamBasicDescription)) {
                return kAudioUnitErr_InvalidPropertyValue;
            }
            mStreamFormat = *(const AudioStreamBasicDescription*)inData;
            mSampleRate = mStreamFormat.mSampleRate;
            return noErr;
            
        case kAudioUnitProperty_SampleRate:
            if (inDataSize < sizeof(Float64)) {
                return kAudioUnitErr_InvalidPropertyValue;
            }
            mSampleRate = *(const Float64*)inData;
            mStreamFormat.mSampleRate = mSampleRate;
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

OSStatus FilterAudioUnit::GetParameterInfo(AudioUnitParameterID inID,
                                          AudioUnitScope inScope,
                                          AudioUnitElement inElement,
                                          AudioUnitParameterInfo& outParameterInfo) {
    outParameterInfo.flags = kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
    
    switch (inID) {
        case kParam_FilterType:
            strncpy(outParameterInfo.name, "Filter Type", sizeof(outParameterInfo.name));
            outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
            outParameterInfo.minValue = 0;
            outParameterInfo.maxValue = 1;
            outParameterInfo.defaultValue = 0;
            return noErr;
            
        case kParam_CutoffFrequency:
            strncpy(outParameterInfo.name, "Cutoff Frequency", sizeof(outParameterInfo.name));
            outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
            outParameterInfo.minValue = 50.0f;
            outParameterInfo.maxValue = 8000.0f;
            outParameterInfo.defaultValue = 1000.0f;
            outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidParameter;
    }
}

OSStatus FilterAudioUnit::GetParameter(AudioUnitParameterID inID,
                                      AudioUnitScope inScope,
                                      AudioUnitElement inElement,
                                      AudioUnitParameterValue& outValue) {
    switch (inID) {
        case kParam_FilterType:
            outValue = mFilterType;
            return noErr;
            
        case kParam_CutoffFrequency:
            outValue = mCutoffFrequency;
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidParameter;
    }
}

OSStatus FilterAudioUnit::SetParameter(AudioUnitParameterID inID,
                                      AudioUnitScope inScope,
                                      AudioUnitElement inElement,
                                      AudioUnitParameterValue inValue,
                                      UInt32 inBufferOffsetInFrames) {
    switch (inID) {
        case kParam_FilterType:
            mFilterType = (int)inValue;
            ResetFilter();  // Reset filter state when changing type
            return noErr;
            
        case kParam_CutoffFrequency:
            mCutoffFrequency = std::max(50.0f, std::min(8000.0f, inValue));
            return noErr;
            
        default:
            return kAudioUnitErr_InvalidParameter;
    }
}

OSStatus FilterAudioUnit::Render(AudioUnitRenderActionFlags& ioActionFlags,
                                const AudioTimeStamp& inTimeStamp,
                                UInt32 inFramesToProcess,
                                AudioBufferList& ioData) {
    if (!mInitialized) {
        return kAudioUnitErr_Uninitialized;
    }
    
    // Process each channel
    for (UInt32 channel = 0; channel < ioData.mNumberBuffers; ++channel) {
        Float32* channelData = (Float32*)ioData.mBuffers[channel].mData;
        UInt32 numSamples = inFramesToProcess;
        
        // Process each sample
        for (UInt32 sample = 0; sample < numSamples; ++sample) {
            float input = channelData[sample];
            float output;
            
            if (mFilterType == kFilterType_LowPass) {
                output = ApplyLowPassFilter(input, channel);
            } else {
                output = ApplyHighPassFilter(input, channel);
            }
            
            channelData[sample] = output;
        }
    }
    
    return noErr;
}

float FilterAudioUnit::ApplyLowPassFilter(float input, int channel) {
    // y[n] = (1-α)·x[n] + α·y[n-1]
    // where α = 1 / (1 + fc/sample_rate)
    float alpha = CalculateAlpha(mCutoffFrequency, mSampleRate, kFilterType_LowPass);
    float output = (1.0f - alpha) * input + alpha * mPrevOutput[channel];
    mPrevOutput[channel] = output;
    return output;
}

float FilterAudioUnit::ApplyHighPassFilter(float input, int channel) {
    // y[n] = α·(y[n-1] + x[n] - x[n-1])
    // where α = fc / (fc + sample_rate)
    float alpha = CalculateAlpha(mCutoffFrequency, mSampleRate, kFilterType_HighPass);
    float output = alpha * (mPrevOutput[channel] + input - mPrevInput[channel]);
    mPrevInput[channel] = input;
    mPrevOutput[channel] = output;
    return output;
}

void FilterAudioUnit::ResetFilter() {
    mPrevOutput[0] = mPrevOutput[1] = 0.0f;
    mPrevInput[0] = mPrevInput[1] = 0.0f;
}

float FilterAudioUnit::CalculateAlpha(float cutoffFreq, float sampleRate, int filterType) {
    if (filterType == kFilterType_LowPass) {
        return 1.0f / (1.0f + cutoffFreq / sampleRate);
    } else {
        return cutoffFreq / (cutoffFreq + sampleRate);
    }
}

// Component dispatch functions
extern "C" {
    
OSStatus FilterAudioUnitEntry(ComponentParameters* params) {
    switch (params->what) {
        case kComponentOpenSelect:
            return FilterAudioUnitOpen((ComponentInstance)params->params[0]);
        case kComponentCloseSelect:
            return FilterAudioUnitClose((ComponentInstance)params->params[0]);
        case kAudioUnitInitializeSelect:
            return FilterAudioUnitInitialize((ComponentInstance)params->params[0]);
        case kAudioUnitUninitializeSelect:
            return FilterAudioUnitUninitialize((ComponentInstance)params->params[0]);
        case kAudioUnitGetPropertyInfoSelect:
            return FilterAudioUnitGetPropertyInfo((ComponentInstance)params->params[0],
                                                 (AudioUnitPropertyID)params->params[1],
                                                 (AudioUnitScope)params->params[2],
                                                 (AudioUnitElement)params->params[3],
                                                 (UInt32*)params->params[4],
                                                 (Boolean*)params->params[5]);
        case kAudioUnitGetPropertySelect:
            return FilterAudioUnitGetProperty((ComponentInstance)params->params[0],
                                             (AudioUnitPropertyID)params->params[1],
                                             (AudioUnitScope)params->params[2],
                                             (AudioUnitElement)params->params[3],
                                             (void*)params->params[4],
                                             (UInt32*)params->params[5]);
        case kAudioUnitSetPropertySelect:
            return FilterAudioUnitSetProperty((ComponentInstance)params->params[0],
                                             (AudioUnitPropertyID)params->params[1],
                                             (AudioUnitScope)params->params[2],
                                             (AudioUnitElement)params->params[3],
                                             (const void*)params->params[4],
                                             (UInt32)params->params[5]);
        case kAudioUnitGetParameterSelect:
            return FilterAudioUnitGetParameter((ComponentInstance)params->params[0],
                                              (AudioUnitParameterID)params->params[1],
                                              (AudioUnitScope)params->params[2],
                                              (AudioUnitElement)params->params[3],
                                              (AudioUnitParameterValue*)params->params[4]);
        case kAudioUnitSetParameterSelect:
            return FilterAudioUnitSetParameter((ComponentInstance)params->params[0],
                                              (AudioUnitParameterID)params->params[1],
                                              (AudioUnitScope)params->params[2],
                                              (AudioUnitElement)params->params[3],
                                              (AudioUnitParameterValue)params->params[4],
                                              (UInt32)params->params[5]);
        case kAudioUnitRenderSelect:
            return FilterAudioUnitRender((ComponentInstance)params->params[0],
                                        (AudioUnitRenderActionFlags*)params->params[1],
                                        (const AudioTimeStamp*)params->params[2],
                                        (UInt32)params->params[3],
                                        (AudioBufferList*)params->params[4]);
        default:
            return badComponentSelector;
    }
}

static FilterAudioUnit* GetFilterAudioUnit(ComponentInstance inInstance) {
    return (FilterAudioUnit*)GetComponentInstanceStorage(inInstance);
}

OSStatus FilterAudioUnitOpen(ComponentInstance inInstance) {
    FilterAudioUnit* audioUnit = new FilterAudioUnit((AudioUnit)inInstance);
    SetComponentInstanceStorage(inInstance, (Handle)audioUnit);
    return noErr;
}

OSStatus FilterAudioUnitClose(ComponentInstance inInstance) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    delete audioUnit;
    return noErr;
}

OSStatus FilterAudioUnitInitialize(ComponentInstance inInstance) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->Initialize();
}

OSStatus FilterAudioUnitUninitialize(ComponentInstance inInstance) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->Uninitialize();
}

OSStatus FilterAudioUnitGetPropertyInfo(ComponentInstance inInstance,
                                       AudioUnitPropertyID inID,
                                       AudioUnitScope inScope,
                                       AudioUnitElement inElement,
                                       UInt32* outDataSize,
                                       Boolean* outWritable) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->GetPropertyInfo(inID, inScope, inElement, *outDataSize, *outWritable);
}

OSStatus FilterAudioUnitGetProperty(ComponentInstance inInstance,
                                   AudioUnitPropertyID inID,
                                   AudioUnitScope inScope,
                                   AudioUnitElement inElement,
                                   void* outData,
                                   UInt32* ioDataSize) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->GetProperty(inID, inScope, inElement, outData, *ioDataSize);
}

OSStatus FilterAudioUnitSetProperty(ComponentInstance inInstance,
                                   AudioUnitPropertyID inID,
                                   AudioUnitScope inScope,
                                   AudioUnitElement inElement,
                                   const void* inData,
                                   UInt32 inDataSize) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->SetProperty(inID, inScope, inElement, inData, inDataSize);
}

OSStatus FilterAudioUnitGetParameterInfo(ComponentInstance inInstance,
                                        AudioUnitParameterID inID,
                                        AudioUnitScope inScope,
                                        AudioUnitElement inElement,
                                        AudioUnitParameterInfo* outParameterInfo) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->GetParameterInfo(inID, inScope, inElement, *outParameterInfo);
}

OSStatus FilterAudioUnitGetParameter(ComponentInstance inInstance,
                                    AudioUnitParameterID inID,
                                    AudioUnitScope inScope,
                                    AudioUnitElement inElement,
                                    AudioUnitParameterValue* outValue) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->GetParameter(inID, inScope, inElement, *outValue);
}

OSStatus FilterAudioUnitSetParameter(ComponentInstance inInstance,
                                    AudioUnitParameterID inID,
                                    AudioUnitScope inScope,
                                    AudioUnitElement inElement,
                                    AudioUnitParameterValue inValue,
                                    UInt32 inBufferOffsetInFrames) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->SetParameter(inID, inScope, inElement, inValue, inBufferOffsetInFrames);
}

OSStatus FilterAudioUnitRender(ComponentInstance inInstance,
                              AudioUnitRenderActionFlags* ioActionFlags,
                              const AudioTimeStamp* inTimeStamp,
                              UInt32 inFramesToProcess,
                              AudioBufferList* ioData) {
    FilterAudioUnit* audioUnit = GetFilterAudioUnit(inInstance);
    return audioUnit->Render(*ioActionFlags, *inTimeStamp, inFramesToProcess, *ioData);
}

} // extern "C"
