# Homespan Temperature Sensor

### Goals:
- To create a minimal lightweight temperature and humidity sensor that can be placed anywhere with wifi reception and 5V power
- To test the abilities of Claude Sonnet 4.5 given minimal information

### Description:
This is an ESP-WROOM-32 ESP32 ESP-32S based HomeKit temperature and humidity sensor using the AM2302 module. It uses the Homespan framework. The AM2302 data pin is gpio 15 for easy io testing. When added to homekit, the device shows up as a temperature and humidity sensor.

### Prompt used:
>Read the readme and develop the project in the most professional, robust, minimal, clean, and stable way possible. Use camelCase and no line breaks before open curlies.

Everything line 10 and below was added after the prompt was given to Claude Sonnet 4.5. It was not able to correctly identify that the latest version of HomeSpan requires a specific platform. It did add a 30 update interval despite that not being in the instructions. Other than those two issues, this worked exactly as I described.
