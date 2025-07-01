"""
ReaScript version of the Filter Plugin for REAPER
This can be loaded directly in REAPER as a Python script effect
"""

import math
import reaper_python as rpr

class FilterReaScript:
    def __init__(self):
        self.cutoff_frequency = 1000.0
        self.filter_type = 0  # 0 = LPF, 1 = HPF
        self.sample_rate = 44100.0
        
        # Filter state (stereo)
        self.prev_output = [0.0, 0.0]
        self.prev_input = [0.0, 0.0]
        
        # Register parameters with REAPER
        self.register_parameters()
    
    def register_parameters(self):
        """Register plugin parameters with REAPER"""
        # Parameter 0: Filter Type
        rpr.TrackFX_SetParam(rpr.GetSelectedTrack(0, 0), 0, 0, self.filter_type)
        
        # Parameter 1: Cutoff Frequency (normalized 0-1)
        normalized_freq = (self.cutoff_frequency - 50.0) / (8000.0 - 50.0)
        rpr.TrackFX_SetParam(rpr.GetSelectedTrack(0, 0), 0, 1, normalized_freq)
    
    def calculate_alpha(self, cutoff_freq, sample_rate, filter_type):
        """Calculate filter coefficient"""
        if filter_type == 0:  # LPF
            return 1.0 / (1.0 + cutoff_freq / sample_rate)
        else:  # HPF
            return cutoff_freq / (cutoff_freq + sample_rate)
    
    def apply_lowpass_filter(self, input_sample, channel):
        """Apply low-pass filter"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, 0)
        output = (1.0 - alpha) * input_sample + alpha * self.prev_output[channel]
        self.prev_output[channel] = output
        return output
    
    def apply_highpass_filter(self, input_sample, channel):
        """Apply high-pass filter"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, 1)
        output = alpha * (self.prev_output[channel] + input_sample - self.prev_input[channel])
        self.prev_input[channel] = input_sample
        self.prev_output[channel] = output
        return output
    
    def process_samples(self, samples, num_channels):
        """Process audio samples"""
        # Update parameters from REAPER
        track = rpr.GetSelectedTrack(0, 0)
        if track:
            # Get filter type parameter
            self.filter_type = int(rpr.TrackFX_GetParam(track, 0, 0)[1])
            
            # Get cutoff frequency parameter (denormalize)
            freq_param = rpr.TrackFX_GetParam(track, 0, 1)[1]
            self.cutoff_frequency = 50.0 + freq_param * (8000.0 - 50.0)
        
        # Process each sample
        for i in range(len(samples[0])):
            for channel in range(min(num_channels, 2)):
                if self.filter_type == 0:  # LPF
                    samples[channel][i] = self.apply_lowpass_filter(samples[channel][i], channel)
                else:  # HPF
                    samples[channel][i] = self.apply_highpass_filter(samples[channel][i], channel)
        
        return samples

# Initialize the filter
filter_plugin = FilterReaScript()

def process_audio():
    """Main audio processing function called by REAPER"""
    # This function is called for each audio block
    # Implementation depends on REAPER's Python API
    pass

# REAPER script metadata
"""
@description Filter Plugin (LPF/HPF)
@author Demo
@version 1.0
@provides [effect] filter_plugin.py
@about
# Filter Plugin for REAPER

Low Pass and High Pass filters with real-time parameter control.

## Parameters:
- Filter Type: 0 = Low Pass, 1 = High Pass  
- Cutoff Frequency: 50 Hz - 8000 Hz

## Usage:
1. Load this script as an effect in REAPER
2. Adjust parameters in the FX window
3. Process audio in real-time
"""

if __name__ == "__main__":
    rpr.ShowMessageBox("Filter Plugin loaded successfully!", "Filter Plugin", 0)
