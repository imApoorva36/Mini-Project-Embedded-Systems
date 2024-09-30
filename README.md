# Smart Rehabilitation Glove for Hand Therapy

**Mini Project in Embedded Systems**  
**Guide:** Prof. Aparna P.  
**Team Members:** Apoorva Agrawal, Omar Sulemani, Nishchay Pallav

## Problem Statement

Hand disabilities due to neurological disorders and conditions like arthritis are increasingly prevalent. Traditional rehabilitation methods can be time-consuming and costly. This project aims to develop a smart rehabilitation glove to assist in hand therapy by providing real-time feedback and personalized exercise programs, enhancing the rehabilitation process for individuals with hand disabilities.

## Objective

To develop a smart rehabilitation glove that assists in hand therapy by providing real-time feedback and personalized exercise programs.

## Proposed Model

The smart rehabilitation glove is equipped with flex sensors and actuators controlled by an ESP32 microcontroller. It monitors hand movements and adjusts finger positions using servo motors based on sensor data. A Django-based web app provides real-time monitoring and feedback.

### Components

- **Microcontroller:** ESP32
- **Flex Sensors:** 5
- **Servo Motors:** 10
- **Tendons/Cables:** For connecting servos to finger joints
- **Power Supply:** Battery pack (Li-ion or Li-Po)
- **Wires and Connectors**
- **Glove:** Fabric glove for mounting sensors and actuators
- **Laptop:** For running the web app
- **Software:** Arduino IDE for ESP32 code, Django for the web app

## Repository Structure

- `esp32/code.ino`: Arduino code for the ESP32 microcontroller.
- `rehabglove/minniproj/`: Django project folder containing the web application.

## Installation

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/imApoorva36/Mini-Project-Embedded-Systems.git
   cd Mini-Project-Embedded-Systems
   ```

2. **Setup the Django Project:**

   - Navigate to the Django project folder:
     ```bash
     cd rehabglove
     ```
   - Create a virtual environment and install dependencies:
     ```bash
     python -m venv env
     source env/bin/activate  # On Windows use `env\Scripts\activate`
     pip install -r ../requirements.txt
     ```
   - Run the Django server:
     ```bash
     python manage.py makemigrations
     python manage.py migrate
     python manage.py runserver
     ```

3. **Upload Code to ESP32:**
   - Open the Arduino IDE.
   - Load the code from `esp32/code.ino`.
   - Select the correct board and port in the Arduino IDE.
   - Upload the code to the ESP32.

## Usage

1. **Wear the Glove:** Ensure the glove is properly fitted and sensors are correctly positioned.
2. **Connect to the Web App:** Open the web app in your browser (usually `http://localhost:8000`).
3. **Perform Exercises:** Follow the exercise routines provided by the app and monitor your progress.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
