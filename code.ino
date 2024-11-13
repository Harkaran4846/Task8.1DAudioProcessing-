import speech_recognition as voice_recog  # Import speech recognition library for voice commands
import RPi.GPIO as gpio_control           # Import GPIO library for controlling pins on Raspberry Pi
import time                               # Import time library for delays

# Set up GPIO mode and define the pin for the LED light
gpio_control.setmode(gpio_control.BCM)    # Use BCM numbering for GPIO
led_pin = 18                              # Define GPIO pin 18 for the LED
gpio_control.setup(led_pin, gpio_control.OUT)  # Set up the pin as an output

# Function to control the LED based on voice command
def control_led(command_input):
    if "on" in command_input:                    # If command contains "on"
        gpio_control.output(led_pin, gpio_control.HIGH)  # Turn LED ON
        print("Light turned ON")
    elif "off" in command_input:                 # If command contains "off"
        gpio_control.output(led_pin, gpio_control.LOW)   # Turn LED OFF
        print("Light turned OFF")
    else:
        print("Command not recognized")          # If command is neither "on" nor "off"

# Function to listen and capture a voice command
def capture_voice_command():
    recognizer = voice_recog.Recognizer()        # Create a Recognizer object for audio processing
    mic_device = voice_recog.Microphone()        # Use the default microphone

    # Capture audio input from the microphone
    with mic_device as audio_source:
        print("Listening...")
        recognizer.adjust_for_ambient_noise(audio_source)  # Adjust for background noise
        audio_data = recognizer.listen(audio_source)        # Capture the audio

    try:
        # Use Google's recognizer to convert audio to text and make it lowercase
        voice_input = recognizer.recognize_google(audio_data).lower()
        print("You said: " + voice_input)  # Print recognized command
        control_led(voice_input)           # Call function to control LED based on command
    except voice_recog.UnknownValueError:
        print("Sorry, I could not understand the audio.")   # Error for unrecognized audio
    except voice_recog.RequestError as req_err:
        print(f"Could not request results from Google Speech Recognition service; {req_err}")  # Error if service fails

# Main loop to continuously listen for voice commands
if __name__ == "__main__":
    try:
        while True:
            capture_voice_command()       # Listen for voice command
            time.sleep(1)                 # Wait for 1 second before listening again
    except KeyboardInterrupt:
        gpio_control.cleanup()            # Clean up GPIO pins on exit
