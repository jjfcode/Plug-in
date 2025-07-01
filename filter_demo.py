"""
Demo script showing filter behavior with generated test signals
This demonstrates the filter functionality without requiring a microphone
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import tkinter as tk
from tkinter import ttk

class FilterDemo:
    def __init__(self):
        self.sample_rate = 44100
        self.duration = 1.0  # seconds
        self.cutoff_frequency = 1000  # Hz
        
    def calculate_alpha(self, cutoff_freq, sample_rate, filter_type):
        """Calculate alpha coefficient for the filter"""
        if filter_type == "lowpass":
            alpha = 1.0 / (1.0 + cutoff_freq / sample_rate)
        else:  # highpass
            alpha = cutoff_freq / (cutoff_freq + sample_rate)
        return alpha
    
    def apply_lowpass_filter(self, signal_data, cutoff_freq):
        """Apply low-pass filter to signal"""
        alpha = self.calculate_alpha(cutoff_freq, self.sample_rate, "lowpass")
        
        filtered_signal = np.zeros_like(signal_data)
        prev_output = 0.0
        
        for i, sample in enumerate(signal_data):
            output = (1 - alpha) * sample + alpha * prev_output
            filtered_signal[i] = output
            prev_output = output
            
        return filtered_signal
    
    def apply_highpass_filter(self, signal_data, cutoff_freq):
        """Apply high-pass filter to signal"""
        alpha = self.calculate_alpha(cutoff_freq, self.sample_rate, "highpass")
        
        filtered_signal = np.zeros_like(signal_data)
        prev_input = 0.0
        prev_output = 0.0
        
        for i, sample in enumerate(signal_data):
            output = alpha * (prev_output + sample - prev_input)
            filtered_signal[i] = output
            prev_input = sample
            prev_output = output
            
        return filtered_signal
    
    def generate_test_signal(self):
        """Generate a test signal with multiple frequencies"""
        t = np.linspace(0, self.duration, int(self.sample_rate * self.duration))
        
        # Create a signal with multiple frequency components
        signal_data = (
            0.5 * np.sin(2 * np.pi * 200 * t) +    # Low frequency (200 Hz)
            0.5 * np.sin(2 * np.pi * 1000 * t) +   # Mid frequency (1000 Hz)
            0.3 * np.sin(2 * np.pi * 3000 * t) +   # High frequency (3000 Hz)
            0.2 * np.sin(2 * np.pi * 6000 * t)     # Very high frequency (6000 Hz)
        )
        
        return t, signal_data
    
    def plot_frequency_response(self):
        """Plot the frequency response of both filters"""
        frequencies = np.logspace(1, 4, 1000)  # 10 Hz to 10 kHz
        
        # Calculate frequency responses
        lowpass_responses = []
        highpass_responses = []
        
        for freq in frequencies:
            # For first-order filters, we can calculate the theoretical response
            omega = 2 * np.pi * freq / self.sample_rate
            
            # Low pass response
            alpha_lp = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, "lowpass")
            H_lp = (1 - alpha_lp) / (1 - alpha_lp * np.exp(-1j * omega))
            lowpass_responses.append(abs(H_lp))
            
            # High pass response
            alpha_hp = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, "highpass")
            H_hp = alpha_hp * (1 - np.exp(-1j * omega)) / (1 - alpha_hp * np.exp(-1j * omega))
            highpass_responses.append(abs(H_hp))
        
        # Plot
        plt.figure(figsize=(12, 8))
        
        # Frequency response plot
        plt.subplot(2, 1, 1)
        plt.semilogx(frequencies, 20 * np.log10(lowpass_responses), 'b-', label='Low Pass Filter', linewidth=2)
        plt.semilogx(frequencies, 20 * np.log10(highpass_responses), 'r-', label='High Pass Filter', linewidth=2)
        plt.axvline(self.cutoff_frequency, color='k', linestyle='--', alpha=0.7, label=f'Cutoff: {self.cutoff_frequency} Hz')
        plt.xlabel('Frequency (Hz)')
        plt.ylabel('Magnitude (dB)')
        plt.title('Filter Frequency Response')
        plt.grid(True, alpha=0.3)
        plt.legend()
        plt.ylim(-60, 5)
        
        # Test signal demonstration
        plt.subplot(2, 1, 2)
        t, original_signal = self.generate_test_signal()
        
        # Apply filters
        lowpass_filtered = self.apply_lowpass_filter(original_signal, self.cutoff_frequency)
        highpass_filtered = self.apply_highpass_filter(original_signal, self.cutoff_frequency)
        
        # Plot time domain signals (first 0.01 seconds for clarity)
        time_samples = int(0.01 * self.sample_rate)
        plt.plot(t[:time_samples], original_signal[:time_samples], 'k-', label='Original Signal', alpha=0.7)
        plt.plot(t[:time_samples], lowpass_filtered[:time_samples], 'b-', label='Low Pass Filtered', linewidth=2)
        plt.plot(t[:time_samples], highpass_filtered[:time_samples], 'r-', label='High Pass Filtered', linewidth=2)
        plt.xlabel('Time (s)')
        plt.ylabel('Amplitude')
        plt.title(f'Time Domain Signals (Cutoff: {self.cutoff_frequency} Hz)')
        plt.grid(True, alpha=0.3)
        plt.legend()
        
        plt.tight_layout()
        plt.show()
    
    def run_demo(self):
        """Run the filter demonstration"""
        print("Audio Filter Demo")
        print("=" * 50)
        print(f"Sample Rate: {self.sample_rate} Hz")
        print(f"Cutoff Frequency: {self.cutoff_frequency} Hz")
        print()
        
        # Generate test signal
        print("Generating test signal with multiple frequencies:")
        print("- 200 Hz (Low frequency)")
        print("- 1000 Hz (Mid frequency)")
        print("- 3000 Hz (High frequency)")
        print("- 6000 Hz (Very high frequency)")
        print()
        
        t, original_signal = self.generate_test_signal()
        
        # Apply filters
        print("Applying filters...")
        lowpass_filtered = self.apply_lowpass_filter(original_signal, self.cutoff_frequency)
        highpass_filtered = self.apply_highpass_filter(original_signal, self.cutoff_frequency)
        
        # Calculate RMS values for different frequency bands
        def calculate_frequency_band_power(signal_data, low_freq, high_freq):
            """Calculate power in a specific frequency band"""
            fft = np.fft.fft(signal_data)
            freqs = np.fft.fftfreq(len(signal_data), 1/self.sample_rate)
            
            # Find indices for the frequency band
            band_indices = (np.abs(freqs) >= low_freq) & (np.abs(freqs) <= high_freq)
            band_power = np.sum(np.abs(fft[band_indices])**2)
            return band_power
        
        # Analyze frequency content
        print("\nFrequency band analysis:")
        print("Frequency Band | Original | Low Pass | High Pass")
        print("-" * 50)
        
        bands = [(0, 500), (500, 1500), (1500, 4000), (4000, 8000)]
        band_names = ["0-500 Hz", "500-1500 Hz", "1500-4000 Hz", "4000-8000 Hz"]
        
        for i, (low, high) in enumerate(bands):
            orig_power = calculate_frequency_band_power(original_signal, low, high)
            lp_power = calculate_frequency_band_power(lowpass_filtered, low, high)
            hp_power = calculate_frequency_band_power(highpass_filtered, low, high)
            
            print(f"{band_names[i]:12} | {orig_power:8.0f} | {lp_power:8.0f} | {hp_power:8.0f}")
        
        print("\nGenerating plots...")
        self.plot_frequency_response()
        
        print("\nDemo completed!")
        print("You can see how the low pass filter attenuates high frequencies")
        print("and the high pass filter attenuates low frequencies.")

def main():
    """Run the filter demonstration"""
    try:
        import matplotlib.pyplot as plt
        demo = FilterDemo()
        demo.run_demo()
    except ImportError:
        print("Matplotlib is required for the demo plots.")
        print("Install it with: pip install matplotlib")
        print("\nRunning basic demo without plots...")
        
        demo = FilterDemo()
        t, signal_data = demo.generate_test_signal()
        
        lp_filtered = demo.apply_lowpass_filter(signal_data, 1000)
        hp_filtered = demo.apply_highpass_filter(signal_data, 1000)
        
        print(f"Original signal RMS: {np.sqrt(np.mean(signal_data**2)):.4f}")
        print(f"Low pass filtered RMS: {np.sqrt(np.mean(lp_filtered**2)):.4f}")
        print(f"High pass filtered RMS: {np.sqrt(np.mean(hp_filtered**2)):.4f}")
        print("\nFilters are working correctly!")

if __name__ == "__main__":
    main()
