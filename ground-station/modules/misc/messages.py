# Display for the console window
# Thomas Selwyn (Devil)

def print_cu_rocket(rocket_name: str, version: str, author: str) -> None:

    """Prints the information about the rocket and software version."""

    print(fr"""
            ooo
           / : \
          / o0o \
    _____"~~~~~~~"_____
    \+###|U * * U|###+/
     \...!(.>..<)!.../
      ^^^^o|   |o^^^^
   +=====):^^^^^:(=====+#
   .____  .|!!!|.  ____.
   |#####:/" " "\:#####|
   |#####=|  O  |=#####|
   |#####>\_____/<#####|
    ^^^^^   | |   ^^^^^
            o o

                            HomeStead Avionics Telemetry Server
                            {f"Rocket": <11}{rocket_name}
                            {f"Version": <11}{version}
                            {f"Author": <11}{author}
              """)
