import speech_recognition as voice_recog  # For voice recognition
import RPi.GPIO as gpio_control  # For GPIO control on Raspberry Pi
import time

# Setup GPIO mode and pin for the light
gpio_control.setmode(gpio_control.BCM)
led_pin = 18  # GPIO pin for the light
gpio_control.setup(led_pin, gpio_control.OUT)

# Function to switch the light ON or OFF based on the voice command
def control_led(command_input):
    if "on" in command_input:
        gpio_control.output(led_pin, gpio_control.HIGH)  # Turn light ON
        print("Light turned ON")
    elif "off" in command_input:
        gpio_control.output(led_pin, gpio_control.LOW)  # Turn light OFF
        print("Light turned OFF")
    else:
        print("Command not recognized")

# Function to listen for a voice command
def capture_voice_command():
    recognizer = voice_recog.Recognizer()
    mic_device = voice_recog.Microphone()
    
    with mic_device as audio_source:
        print("Listening...")
        recognizer.adjust_for_ambient_noise(audio_source)
        audio_data = recognizer.listen(audio_source)
    
    try:
        voice_input = recognizer.recognize_google(audio_data).lower()  # Recognize speech
        print("You said: " + voice_input)
        control_led(voice_input)
    except voice_recog.UnknownValueError:
        print("Sorry, I could not understand the audio.")
    except voice_recog.RequestError as req_err:
        print(f"Could not request results from Google Speech Recognition service; {req_err}")

# Main loop to continuously listen for voice commands
if __name__ == "__main__":
    try:
        while True:
            capture_voice_command()
            time.sleep(1)
    except KeyboardInterrupt:
        gpio_control.cleanup()  # Clean up GPIO on exit
