"""
Plugin Host Bridge - Makes Python plugin available to DAWs
This creates a bridge that allows DAWs to communicate with our Python filter
"""

import socket
import json
import threading
import time
import numpy as np
from audio_filter_plugin import AudioFilterPlugin

class PluginHostBridge:
    def __init__(self, port=8888):
        self.port = port
        self.server_socket = None
        self.is_running = False
        self.clients = []
        
        # Initialize our filter plugin
        self.filter_plugin = FilterPlugin()
        
    def start_server(self):
        """Start the plugin host bridge server"""
        try:
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.server_socket.bind(('localhost', self.port))
            self.server_socket.listen(5)
            
            self.is_running = True
            print(f"🎵 Plugin Host Bridge started on port {self.port}")
            print("Ready to receive audio from DAW...")
            
            while self.is_running:
                try:
                    client_socket, address = self.server_socket.accept()
                    print(f"✅ DAW connected from {address}")
                    
                    client_thread = threading.Thread(
                        target=self.handle_client,
                        args=(client_socket,),
                        daemon=True
                    )
                    client_thread.start()
                    self.clients.append(client_socket)
                    
                except socket.error as e:
                    if self.is_running:
                        print(f"Server error: {e}")
                    break
                    
        except Exception as e:
            print(f"Failed to start server: {e}")
        finally:
            self.cleanup()
    
    def handle_client(self, client_socket):
        """Handle communication with a DAW client"""
        try:
            while self.is_running:
                # Receive data from DAW
                data = client_socket.recv(8192)
                if not data:
                    break
                
                try:
                    # Parse audio data from DAW
                    message = json.loads(data.decode('utf-8'))
                    
                    if message['type'] == 'audio_block':
                        # Process audio block
                        processed_audio = self.process_audio_block(message)
                        
                        # Send processed audio back
                        response = {
                            'type': 'processed_audio',
                            'data': processed_audio,
                            'status': 'success'
                        }
                        
                    elif message['type'] == 'parameter_change':
                        # Handle parameter changes
                        self.update_parameters(message['parameters'])
                        response = {
                            'type': 'parameter_ack',
                            'status': 'success'
                        }
                    
                    elif message['type'] == 'ping':
                        # Health check
                        response = {
                            'type': 'pong',
                            'status': 'alive'
                        }
                    
                    else:
                        response = {
                            'type': 'error',
                            'message': 'Unknown message type'
                        }
                    
                    # Send response
                    response_data = json.dumps(response).encode('utf-8')
                    client_socket.send(response_data)
                    
                except json.JSONDecodeError:
                    error_response = {
                        'type': 'error',
                        'message': 'Invalid JSON'
                    }
                    client_socket.send(json.dumps(error_response).encode('utf-8'))
                
                except Exception as e:
                    error_response = {
                        'type': 'error',
                        'message': str(e)
                    }
                    client_socket.send(json.dumps(error_response).encode('utf-8'))
                    
        except Exception as e:
            print(f"Client handler error: {e}")
        finally:
            client_socket.close()
            if client_socket in self.clients:
                self.clients.remove(client_socket)
    
    def process_audio_block(self, message):
        """Process an audio block from the DAW"""
        audio_data = message['audio_data']
        sample_rate = message.get('sample_rate', 44100)
        
        # Convert to numpy array
        samples = np.array(audio_data, dtype=np.float32)
        
        # Apply our filter
        if len(samples.shape) == 1:
            # Mono
            processed = self.filter_plugin.process_mono(samples)
        else:
            # Stereo
            processed = self.filter_plugin.process_stereo(samples)
        
        # Convert back to list for JSON serialization
        return processed.tolist()
    
    def update_parameters(self, parameters):
        """Update filter parameters"""
        if 'cutoff_frequency' in parameters:
            self.filter_plugin.cutoff_frequency = parameters['cutoff_frequency']
        
        if 'filter_type' in parameters:
            self.filter_plugin.filter_type = parameters['filter_type']
            # Reset filter state when changing type
            self.filter_plugin.prev_input = 0.0
            self.filter_plugin.prev_output = 0.0
    
    def stop_server(self):
        """Stop the plugin host bridge"""
        self.is_running = False
        
        # Close all client connections
        for client in self.clients:
            try:
                client.close()
            except:
                pass
        
        # Close server socket
        if self.server_socket:
            try:
                self.server_socket.close()
            except:
                pass
        
        print("Plugin Host Bridge stopped")
    
    def cleanup(self):
        """Cleanup resources"""
        self.stop_server()

class FilterPlugin:
    """Simplified filter class for the bridge"""
    def __init__(self):
        self.cutoff_frequency = 1000.0
        self.filter_type = "lowpass"  # "lowpass" or "highpass"
        self.sample_rate = 44100.0
        
        # Filter state
        self.prev_input = 0.0
        self.prev_output = 0.0
    
    def calculate_alpha(self, cutoff_freq, sample_rate, filter_type):
        """Calculate filter coefficient"""
        if filter_type == "lowpass":
            return 1.0 / (1.0 + cutoff_freq / sample_rate)
        else:  # highpass
            return cutoff_freq / (cutoff_freq + sample_rate)
    
    def apply_lowpass_filter(self, input_sample):
        """Apply low-pass filter"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, "lowpass")
        output = (1 - alpha) * input_sample + alpha * self.prev_output
        self.prev_output = output
        return output
    
    def apply_highpass_filter(self, input_sample):
        """Apply high-pass filter"""
        alpha = self.calculate_alpha(self.cutoff_frequency, self.sample_rate, "highpass")
        output = alpha * (self.prev_output + input_sample - self.prev_input)
        self.prev_input = input_sample
        self.prev_output = output
        return output
    
    def process_mono(self, samples):
        """Process mono audio"""
        processed = np.zeros_like(samples)
        
        for i, sample in enumerate(samples):
            if self.filter_type == "lowpass":
                processed[i] = self.apply_lowpass_filter(sample)
            else:
                processed[i] = self.apply_highpass_filter(sample)
        
        return processed
    
    def process_stereo(self, samples):
        """Process stereo audio"""
        if len(samples.shape) == 2:
            left_channel = self.process_mono(samples[:, 0])
            right_channel = self.process_mono(samples[:, 1])
            return np.column_stack([left_channel, right_channel])
        else:
            return self.process_mono(samples)

def main():
    """Main function to start the plugin host bridge"""
    print("🎵 Filter Plugin Host Bridge")
    print("=" * 40)
    
    bridge = PluginHostBridge()
    
    try:
        bridge.start_server()
    except KeyboardInterrupt:
        print("\nShutting down...")
        bridge.stop_server()

if __name__ == "__main__":
    main()
