#pragma once

#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

// Audio Unit Component Entry Point
extern "C" {
    OSStatus FilterAudioUnitEntry(ComponentParameters* params);
}

// Parameter IDs
enum {
    kParam_FilterType = 0,
    kParam_CutoffFrequency = 1,
    kNumberOfParameters = 2
};

// Filter types
enum {
    kFilterType_LowPass = 0,
    kFilterType_HighPass = 1
};

class FilterAudioUnit {
public:
    FilterAudioUnit(AudioUnit inAudioUnit);
    ~FilterAudioUnit();
    
    // Audio Unit callbacks
    OSStatus Initialize();
    OSStatus Uninitialize();
    OSStatus GetPropertyInfo(AudioUnitPropertyID inID,
                           AudioUnitScope inScope,
                           AudioUnitElement inElement,
                           UInt32& outDataSize,
                           Boolean& outWritable);
    
    OSStatus GetProperty(AudioUnitPropertyID inID,
                        AudioUnitScope inScope,
                        AudioUnitElement inElement,
                        void* outData,
                        UInt32& ioDataSize);
    
    OSStatus SetProperty(AudioUnitPropertyID inID,
                        AudioUnitScope inScope,
                        AudioUnitElement inElement,
                        const void* inData,
                        UInt32 inDataSize);
    
    OSStatus GetParameterInfo(AudioUnitParameterID inID,
                             AudioUnitScope inScope,
                             AudioUnitElement inElement,
                             AudioUnitParameterInfo& outParameterInfo);
    
    OSStatus GetParameter(AudioUnitParameterID inID,
                         AudioUnitScope inScope,
                         AudioUnitElement inElement,
                         AudioUnitParameterValue& outValue);
    
    OSStatus SetParameter(AudioUnitParameterID inID,
                         AudioUnitScope inScope,
                         AudioUnitElement inElement,
                         AudioUnitParameterValue inValue,
                         UInt32 inBufferOffsetInFrames);
    
    OSStatus Render(AudioUnitRenderActionFlags& ioActionFlags,
                   const AudioTimeStamp& inTimeStamp,
                   UInt32 inFramesToProcess,
                   AudioBufferList& ioData);

private:
    // Filter implementation
    float ApplyLowPassFilter(float input, int channel);
    float ApplyHighPassFilter(float input, int channel);
    
    void ResetFilter();
    float CalculateAlpha(float cutoffFreq, float sampleRate, int filterType);
    
    // Audio Unit instance
    AudioUnit mAudioUnit;
    
    // Parameters
    float mCutoffFrequency;
    int mFilterType;
    
    // Filter state (stereo)
    float mPrevOutput[2];
    float mPrevInput[2];
    
    // Audio properties
    Float64 mSampleRate;
    bool mInitialized;
    
    // Audio stream format
    AudioStreamBasicDescription mStreamFormat;
};

// Component dispatch functions
extern "C" {
    OSStatus FilterAudioUnitOpen(ComponentInstance inInstance);
    OSStatus FilterAudioUnitClose(ComponentInstance inInstance);
    OSStatus FilterAudioUnitInitialize(ComponentInstance inInstance);
    OSStatus FilterAudioUnitUninitialize(ComponentInstance inInstance);
    OSStatus FilterAudioUnitGetPropertyInfo(ComponentInstance inInstance,
                                           AudioUnitPropertyID inID,
                                           AudioUnitScope inScope,
                                           AudioUnitElement inElement,
                                           UInt32* outDataSize,
                                           Boolean* outWritable);
    OSStatus FilterAudioUnitGetProperty(ComponentInstance inInstance,
                                       AudioUnitPropertyID inID,
                                       AudioUnitScope inScope,
                                       AudioUnitElement inElement,
                                       void* outData,
                                       UInt32* ioDataSize);
    OSStatus FilterAudioUnitSetProperty(ComponentInstance inInstance,
                                       AudioUnitPropertyID inID,
                                       AudioUnitScope inScope,
                                       AudioUnitElement inElement,
                                       const void* inData,
                                       UInt32 inDataSize);
    OSStatus FilterAudioUnitGetParameterInfo(ComponentInstance inInstance,
                                            AudioUnitParameterID inID,
                                            AudioUnitScope inScope,
                                            AudioUnitElement inElement,
                                            AudioUnitParameterInfo* outParameterInfo);
    OSStatus FilterAudioUnitGetParameter(ComponentInstance inInstance,
                                        AudioUnitParameterID inID,
                                        AudioUnitScope inScope,
                                        AudioUnitElement inElement,
                                        AudioUnitParameterValue* outValue);
    OSStatus FilterAudioUnitSetParameter(ComponentInstance inInstance,
                                        AudioUnitParameterID inID,
                                        AudioUnitScope inScope,
                                        AudioUnitElement inElement,
                                        AudioUnitParameterValue inValue,
                                        UInt32 inBufferOffsetInFrames);
    OSStatus FilterAudioUnitRender(ComponentInstance inInstance,
                                  AudioUnitRenderActionFlags* ioActionFlags,
                                  const AudioTimeStamp* inTimeStamp,
                                  UInt32 inFramesToProcess,
                                  AudioBufferList* ioData);
}
