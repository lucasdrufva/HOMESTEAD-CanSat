# The unfortunate manager of serial ports
# Handles connections, specifying what serial port a radio should use and spawning the serial processes
#
# Authors:
# Thomas Selwyn (Devil)

import glob
import sys
import logging
from multiprocessing import Process, Queue, active_children
from serial import Serial, SerialException
from modules.serial.serial_lora_radio import SerialLoraRadio
from modules.serial.serial_file_lora import SerialFileLora
from signal import signal, SIGTERM


def shutdown_sequence():
    for child in active_children():
        child.terminate()
    exit(0)


class SerialManager(Process):
    def __init__(self, serial_status: Queue, serial_ws_commands: Queue, radio_signal_report: Queue,
                 lora_radio_input: Queue, lora_radio_payloads: Queue):
        super().__init__()

        self.serial_status = serial_status
        self.serial_ports = []
        self.serial_ws_commands = serial_ws_commands

        self.radio_signal_report = radio_signal_report

        self.lora_radio_input = lora_radio_input
        self.lora_radio_payloads = lora_radio_payloads
        self.lora_radio = None

        # Immediately find serial ports
        self.update_serial_ports()

        # Handle program closing to ensure no orphan processes
        signal(SIGTERM, shutdown_sequence)

        self.run()

    def run(self):
        while True:
            while not self.serial_ws_commands.empty():
                ws_cmd = self.serial_ws_commands.get()
                self.parse_ws_command(ws_cmd)

    def parse_ws_command(self, ws_cmd):
        print(ws_cmd)
        try:
            print("Huh!")
            print(ws_cmd[1:])
            match ws_cmd[0]:
                case "lora_radio":
                    print("mm")
                    self.parse_lora_radio_ws(ws_cmd[1:])
                case "update":
                    self.update_serial_ports()
                case _:
                    logging.error("Serial: Invalid device type.")
        except IndexError:
            logging.error("Serial: Error parsing ws command")
    
    def parse_lora_radio_ws(self, ws_cmd):
        radio_ws_cmd = ws_cmd[0]

        if radio_ws_cmd == "connect" and self.lora_radio is None:
            proposed_serial_port = ws_cmd[1]
            if proposed_serial_port != "test":
                self.lora_radio = Process(
                    target=SerialLoraRadio,
                    args=(
                        self.serial_status,
                        self.radio_signal_report,
                        self.lora_radio_input,
                        self.lora_radio_payloads,
                        proposed_serial_port
                    ),
                    daemon=True)
            else:
                self.lora_radio = Process(
                    target=SerialFileLora,
                    args=(
                        self.serial_status,
                        self.radio_signal_report,
                        self.lora_radio_input,
                        self.lora_radio_payloads,
                        proposed_serial_port
                    ),
                    daemon=True)
            
            self.lora_radio.start()
        elif radio_ws_cmd == "connect":
            logging.info(f"Serial: Already connected.")
        elif radio_ws_cmd == "disconnect" and self.lora_radio is not None:
            logging.info(f"Serial: lora Radio terminating")
            self.serial_status.put(f"lora_connected False")
            self.serial_status.put(f"lora_port null")
            self.lora_radio.terminate()
            self.lora_radio = None
        elif radio_ws_cmd == "disconnect":
            logging.warning("Serial: lora Radio already disconnected.")
        elif radio_ws_cmd == "mode":
            self.lora_radio_input.put(ws_cmd[1])

    def update_serial_ports(self) -> list[str]:
        """ Finds and updates serial ports on device

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        com_ports = [""]

        if sys.platform.startswith('win'):
            com_ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # '/dev/tty[A-Za-z]*'
            com_ports = glob.glob('/dev/ttyUSB*')
        elif sys.platform.startswith('darwin'):
            com_ports = glob.glob('/dev/cu.*')

        tested_com_ports = []

        # Checks ports if they are potential COM ports
        for test_port in com_ports:
            try:
                ser = Serial(test_port)
                ser.close()
                tested_com_ports.append(test_port)
            except (OSError, SerialException):
                pass

        tested_com_ports = tested_com_ports + ["test"]

        self.serial_ports = tested_com_ports
        self.serial_status.put(f"serial_ports {self.serial_ports}")

        return tested_com_ports
