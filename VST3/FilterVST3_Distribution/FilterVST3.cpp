#include "FilterVST3.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "base/source/fstreamer.h"
#include <cmath>

// Plugin UIDs - Generate unique IDs for your plugin
static const FUID FilterVST3ProcessorUID(0x12345678, 0x12345678, 0x12345678, 0x12345678);
static const FUID FilterVST3ControllerUID(0x87654321, 0x87654321, 0x87654321, 0x87654321);

FilterVST3::FilterVST3()
: m_sampleRate(44100.0f)
, m_cutoffFreq(1000.0f)
, m_filterType(0)
{
    // Initialize filter memory
    for (int i = 0; i < 2; ++i) {
        m_lastOutput[i] = 0.0f;
        m_lastInput[i] = 0.0f;
    }
    
    setControllerClass(FilterVST3ControllerUID);
}

FilterVST3::~FilterVST3()
{
}

tresult FilterVST3::initialize(FUnknown* context)
{
    tresult result = AudioEffect::initialize(context);
    if (result == kResultTrue)
    {
        addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
        addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
    }
    return result;
}

tresult FilterVST3::terminate()
{
    return AudioEffect::terminate();
}

tresult FilterVST3::setActive(TBool state)
{
    if (state)
    {
        // Reset filter state when activated
        for (int i = 0; i < 2; ++i) {
            m_lastOutput[i] = 0.0f;
            m_lastInput[i] = 0.0f;
        }
    }
    return AudioEffect::setActive(state);
}

tresult FilterVST3::setupProcessing(ProcessSetup& newSetup)
{
    m_sampleRate = static_cast<float>(newSetup.sampleRate);
    return AudioEffect::setupProcessing(newSetup);
}

tresult FilterVST3::process(ProcessData& data)
{
    // Process audio
    if (data.numInputs > 0 && data.numOutputs > 0)
    {
        AudioBusBuffers& input = data.inputs[0];
        AudioBusBuffers& output = data.outputs[0];
        
        int32 numChannels = input.numChannels;
        int32 numSamples = data.numSamples;
        
        for (int32 channel = 0; channel < numChannels && channel < 2; ++channel)
        {
            float* inputBuffer = input.channelBuffers32[channel];
            float* outputBuffer = output.channelBuffers32[channel];
            
            for (int32 sample = 0; sample < numSamples; ++sample)
            {
                float inputSample = inputBuffer[sample];
                float outputSample;
                
                if (m_filterType == 0) // Low Pass Filter
                {
                    outputSample = applyLowPassFilter(inputSample, channel);
                }
                else // High Pass Filter
                {
                    outputSample = applyHighPassFilter(inputSample, channel);
                }
                
                outputBuffer[sample] = outputSample;
            }
        }
    }
    
    return kResultTrue;
}

float FilterVST3::applyLowPassFilter(float input, int channel)
{
    // Low Pass Filter: y[n] = (1-α)·x[n] + α·y[n-1]
    // where α = 1 / (1 + fc/sample_rate)
    float alpha = 1.0f / (1.0f + m_cutoffFreq / m_sampleRate);
    float output = (1.0f - alpha) * input + alpha * m_lastOutput[channel];
    m_lastOutput[channel] = output;
    return output;
}

float FilterVST3::applyHighPassFilter(float input, int channel)
{
    // High Pass Filter: y[n] = α·(y[n-1] + x[n] - x[n-1])
    // where α = fc / (fc + sample_rate)
    float alpha = m_cutoffFreq / (m_cutoffFreq + m_sampleRate);
    float output = alpha * (m_lastOutput[channel] + input - m_lastInput[channel]);
    m_lastInput[channel] = input;
    m_lastOutput[channel] = output;
    return output;
}

tresult FilterVST3::setState(IBStream* state)
{
    if (!state) return kResultFalse;
    
    IBStreamer streamer(state, kLittleEndian);
    
    float savedCutoff = 0.0f;
    int savedType = 0;
    
    if (streamer.readFloat(savedCutoff) == false) return kResultFalse;
    if (streamer.readInt32(savedType) == false) return kResultFalse;
    
    m_cutoffFreq = savedCutoff;
    m_filterType = savedType;
    
    return kResultOk;
}

tresult FilterVST3::getState(IBStream* state)
{
    if (!state) return kResultFalse;
    
    IBStreamer streamer(state, kLittleEndian);
    
    streamer.writeFloat(m_cutoffFreq);
    streamer.writeInt32(m_filterType);
    
    return kResultOk;
}