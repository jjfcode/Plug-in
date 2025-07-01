"""
Audio Filter Plugin - Low Pass and High Pass Filters
Real-time audio processing with GUI controls for filter parameters
"""

import numpy as np
import pyaudio
import tkinter as tk
from tkinter import ttk
import threading
import queue
import time

class AudioFilterPlugin:
    def __init__(self):
        # Audio configuration
        self.CHUNK = 1024
        self.FORMAT = pyaudio.paFloat32
        self.CHANNELS = 1
        self.RATE = 44100
        
        # Filter parameters
        self.cutoff_frequency = 1000.0  # Hz
        self.filter_type = "lowpass"  # "lowpass" or "highpass"
        
        # Filter state variables
        self.prev_input = 0.0
        self.prev_output = 0.0
        
        # Audio stream objects
        self.audio = pyaudio.PyAudio()
        self.input_stream = None
        self.output_stream = None
        
        # Threading and control
        self.is_running = False
        self.audio_queue = queue.Queue()
        
        # GUI
        self.root = None
        self.setup_gui()
        
    def calculate_alpha(self, cutoff_freq, sample_rate, filter_type):
        """Calculate alpha coefficient for the filter"""
        if filter_type == "lowpass":
            # α = 1 / (1 + fc/sample_rate)
            alpha = 1.0 / (1.0 + cutoff_freq / sample_rate)
        else:  # highpass
            # α = fc / (fc + sample_rate)
            alpha = cutoff_freq / (cutoff_freq + sample_rate)
        return alpha
    
    def apply_lowpass_filter(self, input_sample):
        """Apply low-pass filter: y[n] = (1-α)·x[n] + α·y[n-1]"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.RATE, "lowpass")
        output = (1 - alpha) * input_sample + alpha * self.prev_output
        self.prev_output = output
        return output
    
    def apply_highpass_filter(self, input_sample):
        """Apply high-pass filter: y[n] = α·(y[n-1] + x[n] - x[n-1])"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.RATE, "highpass")
        output = alpha * (self.prev_output + input_sample - self.prev_input)
        self.prev_input = input_sample
        self.prev_output = output
        return output
    
    def process_audio_chunk(self, audio_data):
        """Process a chunk of audio data through the selected filter"""
        # Convert bytes to numpy array
        audio_array = np.frombuffer(audio_data, dtype=np.float32)
        
        # Apply filter to each sample
        filtered_array = np.zeros_like(audio_array)
        
        for i, sample in enumerate(audio_array):
            if self.filter_type == "lowpass":
                filtered_array[i] = self.apply_lowpass_filter(sample)
            else:  # highpass
                filtered_array[i] = self.apply_highpass_filter(sample)
        
        return filtered_array.astype(np.float32).tobytes()
    
    def audio_callback(self):
        """Audio processing thread"""
        while self.is_running:
            try:
                # Read audio input
                if self.input_stream:
                    audio_data = self.input_stream.read(self.CHUNK, exception_on_overflow=False)
                    
                    # Process audio
                    filtered_data = self.process_audio_chunk(audio_data)
                    
                    # Play processed audio
                    if self.output_stream:
                        self.output_stream.write(filtered_data)
                        
            except Exception as e:
                print(f"Audio processing error: {e}")
                time.sleep(0.001)
    
    def start_audio_processing(self):
        """Start audio input/output streams and processing"""
        try:
            # Initialize input stream (microphone)
            self.input_stream = self.audio.open(
                format=self.FORMAT,
                channels=self.CHANNELS,
                rate=self.RATE,
                input=True,
                frames_per_buffer=self.CHUNK
            )
            
            # Initialize output stream (speakers)
            self.output_stream = self.audio.open(
                format=self.FORMAT,
                channels=self.CHANNELS,
                rate=self.RATE,
                output=True,
                frames_per_buffer=self.CHUNK
            )
            
            # Start processing
            self.is_running = True
            self.audio_thread = threading.Thread(target=self.audio_callback)
            self.audio_thread.daemon = True
            self.audio_thread.start()
            
            print("Audio processing started...")
            
        except Exception as e:
            print(f"Error starting audio: {e}")
    
    def stop_audio_processing(self):
        """Stop audio processing and close streams"""
        self.is_running = False
        
        if self.input_stream:
            self.input_stream.stop_stream()
            self.input_stream.close()
            
        if self.output_stream:
            self.output_stream.stop_stream()
            self.output_stream.close()
            
        print("Audio processing stopped.")
    
    def update_cutoff_frequency(self, value):
        """Update cutoff frequency from GUI slider"""
        self.cutoff_frequency = float(value)
        self.cutoff_label.config(text=f"Cutoff Frequency: {self.cutoff_frequency:.0f} Hz")
    
    def update_filter_type(self):
        """Update filter type from GUI radio buttons"""
        # Reset filter state when changing filter type
        self.prev_input = 0.0
        self.prev_output = 0.0
        print(f"Filter type changed to: {self.filter_type}")
    
    def setup_gui(self):
        """Create the GUI interface"""
        self.root = tk.Tk()
        self.root.title("Audio Filter Plugin - Low Pass & High Pass")
        self.root.geometry("500x400")
        self.root.configure(bg='#2c3e50')
        
        # Title
        title_label = tk.Label(
            self.root, 
            text="Audio Filter Plugin", 
            font=("Arial", 18, "bold"),
            bg='#2c3e50',
            fg='white'
        )
        title_label.pack(pady=20)
        
        # Filter Type Selection
        filter_frame = tk.LabelFrame(
            self.root, 
            text="Filter Type", 
            font=("Arial", 12, "bold"),
            bg='#34495e',
            fg='white',
            padx=20,
            pady=10
        )
        filter_frame.pack(pady=10, padx=20, fill='x')
        
        self.filter_var = tk.StringVar(value="lowpass")
        
        lowpass_radio = tk.Radiobutton(
            filter_frame,
            text="Low Pass Filter (LPF)",
            variable=self.filter_var,
            value="lowpass",
            command=lambda: self.set_filter_type("lowpass"),
            bg='#34495e',
            fg='white',
            selectcolor='#2c3e50',
            font=("Arial", 10)
        )
        lowpass_radio.pack(anchor='w', pady=5)
        
        highpass_radio = tk.Radiobutton(
            filter_frame,
            text="High Pass Filter (HPF)",
            variable=self.filter_var,
            value="highpass",
            command=lambda: self.set_filter_type("highpass"),
            bg='#34495e',
            fg='white',
            selectcolor='#2c3e50',
            font=("Arial", 10)
        )
        highpass_radio.pack(anchor='w', pady=5)
        
        # Frequency Control
        freq_frame = tk.LabelFrame(
            self.root, 
            text="Frequency Control", 
            font=("Arial", 12, "bold"),
            bg='#34495e',
            fg='white',
            padx=20,
            pady=10
        )
        freq_frame.pack(pady=10, padx=20, fill='x')
        
        # Cutoff frequency label
        self.cutoff_label = tk.Label(
            freq_frame,
            text=f"Cutoff Frequency: {self.cutoff_frequency:.0f} Hz",
            bg='#34495e',
            fg='white',
            font=("Arial", 10, "bold")
        )
        self.cutoff_label.pack(pady=5)
        
        # Frequency slider
        self.freq_slider = tk.Scale(
            freq_frame,
            from_=20,
            to=20000,
            orient=tk.HORIZONTAL,
            command=self.update_cutoff_frequency,
            bg='#34495e',
            fg='white',
            highlightbackground='#2c3e50',
            troughcolor='#2c3e50',
            length=400
        )
        self.freq_slider.set(self.cutoff_frequency)
        self.freq_slider.pack(pady=10)
        
        # Control Buttons
        button_frame = tk.Frame(self.root, bg='#2c3e50')
        button_frame.pack(pady=20)
        
        self.start_button = tk.Button(
            button_frame,
            text="Start Audio Processing",
            command=self.start_audio_processing,
            bg='#27ae60',
            fg='white',
            font=("Arial", 12, "bold"),
            padx=20,
            pady=10
        )
        self.start_button.pack(side=tk.LEFT, padx=10)
        
        self.stop_button = tk.Button(
            button_frame,
            text="Stop Audio Processing",
            command=self.stop_audio_processing,
            bg='#e74c3c',
            fg='white',
            font=("Arial", 12, "bold"),
            padx=20,
            pady=10
        )
        self.stop_button.pack(side=tk.LEFT, padx=10)
        
        # Status information
        status_frame = tk.LabelFrame(
            self.root, 
            text="Filter Information", 
            font=("Arial", 10, "bold"),
            bg='#34495e',
            fg='white',
            padx=20,
            pady=10
        )
        status_frame.pack(pady=10, padx=20, fill='x')
        
        info_text = """
Low Pass Filter: Allows frequencies below cutoff to pass through
High Pass Filter: Allows frequencies above cutoff to pass through
        
Sample Rate: 44.1 kHz | Channels: Mono | Buffer Size: 1024 samples
        """
        
        info_label = tk.Label(
            status_frame,
            text=info_text,
            bg='#34495e',
            fg='white',
            font=("Arial", 9),
            justify=tk.LEFT
        )
        info_label.pack()
        
        # Handle window closing
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
    
    def set_filter_type(self, filter_type):
        """Set the filter type and update"""
        self.filter_type = filter_type
        self.update_filter_type()
    
    def on_closing(self):
        """Handle application closing"""
        self.stop_audio_processing()
        if self.audio:
            self.audio.terminate()
        self.root.destroy()
    
    def run(self):
        """Start the GUI application"""
        self.root.mainloop()

def main():
    """Main function to run the audio filter plugin"""
    print("Starting Audio Filter Plugin...")
    print("Make sure you have a microphone connected for input.")
    
    # Create and run the plugin
    plugin = AudioFilterPlugin()
    plugin.run()

if __name__ == "__main__":
    main()
