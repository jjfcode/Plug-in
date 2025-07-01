"""
Test script to verify audio plugin dependencies and basic functionality
"""

import sys

def test_imports():
    """Test if all required modules can be imported"""
    print("Testing imports...")
    
    try:
        import numpy
        print("✓ NumPy imported successfully")
    except ImportError as e:
        print(f"✗ NumPy import failed: {e}")
        return False
    
    try:
        import scipy
        print("✓ SciPy imported successfully")
    except ImportError as e:
        print(f"✗ SciPy import failed: {e}")
        return False
    
    try:
        import pyaudio
        print("✓ PyAudio imported successfully")
    except ImportError as e:
        print(f"✗ PyAudio import failed: {e}")
        print("Try installing with: pip install pyaudio")
        return False
    
    try:
        import tkinter
        print("✓ Tkinter imported successfully")
    except ImportError as e:
        print(f"✗ Tkinter import failed: {e}")
        return False
    
    return True

def test_audio_devices():
    """Test available audio devices"""
    try:
        import pyaudio
        
        print("\nTesting audio devices...")
        audio = pyaudio.PyAudio()
        
        print(f"Available audio devices: {audio.get_device_count()}")
        
        # List input devices
        print("\nInput devices:")
        for i in range(audio.get_device_count()):
            info = audio.get_device_info_by_index(i)
            if info['maxInputChannels'] > 0:
                print(f"  {i}: {info['name']} (Channels: {info['maxInputChannels']})")
        
        # List output devices
        print("\nOutput devices:")
        for i in range(audio.get_device_count()):
            info = audio.get_device_info_by_index(i)
            if info['maxOutputChannels'] > 0:
                print(f"  {i}: {info['name']} (Channels: {info['maxOutputChannels']})")
        
        audio.terminate()
        return True
        
    except Exception as e:
        print(f"Audio device test failed: {e}")
        return False

def test_filter_calculations():
    """Test filter coefficient calculations"""
    print("\nTesting filter calculations...")
    
    try:
        import numpy as np
        
        # Test parameters
        sample_rate = 44100
        cutoff_freq = 1000
        
        # Low pass filter alpha
        alpha_lp = 1.0 / (1.0 + cutoff_freq / sample_rate)
        print(f"✓ Low pass alpha calculated: {alpha_lp:.6f}")
        
        # High pass filter alpha
        alpha_hp = cutoff_freq / (cutoff_freq + sample_rate)
        print(f"✓ High pass alpha calculated: {alpha_hp:.6f}")
        
        # Test with sample data
        input_sample = 0.5
        prev_output = 0.1
        prev_input = 0.2
        
        # Low pass filter test
        lp_output = (1 - alpha_lp) * input_sample + alpha_lp * prev_output
        print(f"✓ Low pass filter output: {lp_output:.6f}")
        
        # High pass filter test
        hp_output = alpha_hp * (prev_output + input_sample - prev_input)
        print(f"✓ High pass filter output: {hp_output:.6f}")
        
        return True
        
    except Exception as e:
        print(f"Filter calculation test failed: {e}")
        return False

def main():
    """Run all tests"""
    print("Audio Filter Plugin - Dependency Test")
    print("=" * 40)
    
    success = True
    
    success &= test_imports()
    success &= test_audio_devices()
    success &= test_filter_calculations()
    
    print("\n" + "=" * 40)
    if success:
        print("✓ All tests passed! You can run the audio plugin.")
        print("Run: python audio_filter_plugin.py")
    else:
        print("✗ Some tests failed. Please check the error messages above.")
        print("Make sure all dependencies are installed correctly.")
    
    return success

if __name__ == "__main__":
    main()
