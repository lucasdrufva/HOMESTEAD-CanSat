import time
import logging
from multiprocessing import Queue, Process
from typing import Optional
from serial import Serial, SerialException, EIGHTBITS, PARITY_NONE

import time

class SerialFileLora(Process):
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
        file1 = open('raw/data9.txt', 'r')
        Lines = file1.readlines()

        ind = 0

        last = time.time_ns()
        while True:
            if time.time_ns() - 1000000*5 > last and ind < len(Lines):  
                print("sent" + Lines[ind])
                self.lora_radio_payloads.put(Lines[ind])
                ind += 1
                last = time.time_ns()

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
