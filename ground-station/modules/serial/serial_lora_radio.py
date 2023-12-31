import time
import logging
from multiprocessing import Queue, Process
from typing import Optional
from serial import Serial, SerialException, EIGHTBITS, PARITY_NONE

class SerialLoraRadio(Process):
    def __init__(
            self,
            serial_status: Queue,
            radio_signal_report: Queue,
            lora_radio_input: Queue,
            lora_radio_payloads: Queue,
            serial_port: str
    ):
        Process.__init__(self)

        self.serial_status = serial_status
        self.radio_signal_report = radio_signal_report
        self.lora_radio_input = lora_radio_input
        self.lora_radio_payloads = lora_radio_payloads

        self.serial_port = serial_port
        self.ser = None

        self.run()
    
    def run(self):
        while True:
            try:
                # initiate the USB serial connection
                logging.info(f"Lora Radio: Connecting to {self.serial_port}")
                # Settings matched to lora Transceiver Data Sheet's default UART settings
                self.ser = Serial(port=self.serial_port,
                                  timeout=1,
                                  baudrate=9600)
                logging.info(f"Lora Radio: Connected to {self.serial_port}")

                self.serial_status.put(f"lora_connected True")
                self.serial_status.put(f"lora_port {self.serial_port}")

                while True:
                    while not self.lora_radio_input.empty():
                        self.write_to_lora_radio(self.lora_radio_input.get())

                    self.check_for_transmissions()

            except SerialException:
                self.serial_status.put(f"lora_connected False")
                self.serial_status.put(f"lora_port null")
                logging.info("Lora Radio: Error communicating with serial device.")
                time.sleep(3)

    def check_for_transmissions(self) -> None:
        """Checks for new transmissions on the line."""

        message = self.ser.readline()

        message = message.decode('utf-8')

        if message == "":
            logging.info("Nothing received.")
            return
        
        logging.info("Got lora data")


        #message = message[2:-5]  # Trim unnecessary elements of the message
        message = message.strip()
        message = message[8:]
        self.lora_radio_payloads.put(message)  # Put serial message in data queue for telemetry

    def write_to_lora_radio(self, command_string: str) -> Optional[bool]:
        """
        Writes data to the RN2483 radio via UART.
        :author: Tarik
        :param command_string: The full command to be sent to the RN2483 radio

        >> write_to_rn2483_radio("radio set pwr 7")
        >> "ok"
        Above example sets the radio transmission power to 7
        """

        data = str(command_string)
        data += "\r\n"  # Must include carriage return for valid commands (see DS40001784B pg XX)
        self.ser.flush()  # Flush the serial port

        self.ser.write(data.encode('utf-8'))  # Encode command_string as bytes and then transmit over serial port

        print("Sent to radio:", data)