# Audio Filter Plugin

A real-time audio processing plugin implementing Low Pass Filter (LPF) and High Pass Filter (HPF) with GUI controls.

## Features

- **Real-time audio processing** using PyAudio
- **Low Pass Filter (LPF)**: Allows frequencies below the cutoff frequency to pass through
- **High Pass Filter (HPF)**: Allows frequencies above the cutoff frequency to pass through
- **Interactive GUI** with Tkinter for real-time parameter control
- **Adjustable cutoff frequency** from 50 Hz to 8000 Hz
- **Filter type selection** between Low Pass and High Pass modes

## Filter Implementations

### Low Pass Filter (LPF)
```
y[n] = (1-α)·x[n] + α·y[n-1]
where α = 1 / (1 + fc/sample_rate)
```

### High Pass Filter (HPF)
```
y[n] = α·(y[n-1] + x[n] - x[n-1])
where α = fc / (fc + sample_rate)
```

## Requirements

- Python 3.7+
- PyAudio
- NumPy
- SciPy
- Tkinter (usually included with Python)

## Installation

1. Install the required packages:
```bash
pip install -r requirements.txt
```

2. If you encounter issues with PyAudio on Windows, you may need to install it using conda:
```bash
conda install pyaudio
```

## Usage

1. Run the plugin:
```bash
python audio_filter_plugin.py
```

2. The GUI will open with the following controls:
   - **Filter Type**: Choose between Low Pass Filter (LPF) or High Pass Filter (HPF)
   - **Cutoff Frequency Slider**: Adjust the cutoff frequency from 50 Hz to 8000 Hz
   - **Start/Stop Buttons**: Control audio processing

3. Connect a microphone to your computer's audio input
4. Click "Start Audio Processing" to begin real-time filtering
5. Adjust the filter type and cutoff frequency to hear the effects
6. Click "Stop Audio Processing" to stop

## How It Works

1. **Audio Input**: The plugin captures audio from your default microphone using PyAudio
2. **Digital Filtering**: Each audio sample is processed through the selected filter (LPF or HPF)
3. **Real-time Output**: The filtered audio is played back through your speakers/headphones
4. **Parameter Control**: Filter parameters can be adjusted in real-time using the GUI

## Technical Specifications

- **Sample Rate**: 44,100 Hz
- **Audio Format**: 32-bit float
- **Channels**: Mono (1 channel)
- **Buffer Size**: 1024 samples
- **Filter Order**: First-order digital filters

## Notes

- Make sure your microphone and speakers are properly configured
- Use headphones to avoid feedback between input and output
- The plugin processes audio in real-time with minimal latency
- Filter state is reset when switching between filter types

## Troubleshooting

### Audio Issues
- Ensure your microphone is connected and working
- Check Windows audio settings for default input/output devices
- Try using headphones to prevent audio feedback

### Installation Issues
- If PyAudio installation fails, try using conda instead of pip
- Ensure you have the latest version of pip: `pip install --upgrade pip`

### Performance Issues
- Close other audio applications that might be using the microphone
- Reduce the buffer size in the code if you experience latency (CHUNK parameter)

## Audio Unit Version (macOS)

A professional Audio Unit (.au) version is available in the `AudioUnit/` directory for use in Logic Pro, GarageBand, and other macOS DAWs.

### Building the Audio Unit (macOS only)

```bash
cd AudioUnit
make install
```

This will create a `FilterAudioUnit.component` bundle and install it to `~/Library/Audio/Plug-Ins/Components/`.

### Using in DAW

1. **Logic Pro**: Insert > Audio FX > Demo > Filter
2. **GarageBand**: Smart Controls > Plug-ins > Audio Units > Demo > Filter
3. **Pro Tools**: Insert > Audio Units > Demo > Filter

For detailed Audio Unit build instructions, see `AudioUnit/README_AU.md`.

## License

This project is for educational purposes and demonstrates real-time audio processing concepts.
