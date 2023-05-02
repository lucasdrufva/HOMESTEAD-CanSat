import React, { useEffect } from "react";
import "./Controls.css";

// Hooks
import { useState } from "react";
import { useKey } from "../../hooks/useKey";
import { useStorage } from "../../hooks/useStorage";

// SVG components
import { ReactComponent as Play } from "../../assets/Play.svg";
import { ReactComponent as Pause } from "../../assets/Pause.svg";
import { ReactComponent as Forward } from "../../assets/Forward.svg";
import { ReactComponent as Stop } from "../../assets/Stop.svg";

// Utils
import { clear_telemetry } from "../../utils/storage";
import GaugeGraph from "../main-dash/GaugeGraph";

export default function Controls({ websocketRef }) {
  const minSpeed = 0.25; // Minimum speed
  const maxSpeed = 4; // Maximum speed
  const [speed, setSpeed] = useState(1); // Tracks replay speed; normal speed of 1 by default

  // let mode;

  // const modeStuff = useStorage((data) => {data.status.map((packet) => packet.status)})

  // mode = modeStuff[0];
  const get_altitude_mission_time = (data) => {
    return data.bme.map((packet) => packet.mission_time / 1000);
  };
  const get_mode_y = (data) => {
    //return data.bme.map((packet) => packet.altitude);
    return data.status.map((packet) => packet.status);
  };

  var y_pos;

  const y = useStorage(get_mode_y);

  y_pos = y[y.length - 1];

  useEffect(()=>{
    if(y_pos == "start"){
      console.log("Clearing everything!")
      clear_telemetry();
    }
  }, [y]);


  // Sends the speed command to fast forward
  const fast_forward = () => {
    if (speed < maxSpeed) {
      setSpeed((prevSpeed) => prevSpeed * 2);
    }
    websocketRef.current.send(`telemetry replay speed ${speed}`);
  };

  // Sends the speed command to slow forward
  const slow_forward = () => {
    if (speed > minSpeed) {
      setSpeed((prevSpeed) => prevSpeed / 2);
    }
    websocketRef.current.send(`telemetry replay speed ${speed}`);
  };

  // Sends the play command
  const play = () => {
    websocketRef.current.send(`telemetry replay play`);
  };

  // Sends the pause command
  const pause = () => {
    websocketRef.current.send(`telemetry replay pause`);
  };

  // Sends the stop command
  const stop = () => {
    websocketRef.current.send(`telemetry replay stop`);
    clear_telemetry();
  };

  // When Shift + C is pressed, toggle controls
  const [hideControls, setHideControls] = useState(false);
  useKey("KeyC", "shift", () => {
    setHideControls((prevState) => !prevState);
  });

  return (
    <>
      <div className={hideControls ? "buttons" : "buttons hide-controls"}>
      <Forward
        id="backward"
        style={{ transform: "rotate(180deg)" }} // Flip the forward button to make it point backward
        onClick={slow_forward}
      />
      <Stop id="stop" onClick={stop} />
      <Play id="play" onClick={play} />
      <p>{speed}X</p>
      <Pause id="pause" onClick={pause} />
      <Forward id="forward" onClick={fast_forward} />
      </div>
      <div>
        <button onClick={()=>websocketRef.current.send(`serial lora_radio mode PAD`)}>Set mode PAD</button>
        <button onClick={()=>websocketRef.current.send(`serial lora_radio mode FLIGHT`)}>Set mode FLIGHT</button>
        <button onClick={()=>websocketRef.current.send(`serial lora_radio mode DROP`)}>Set mode DROP</button>
        <button onClick={()=>websocketRef.current.send(`serial lora_radio mode RETRIEVAL`)}>Set mode RETRIEVAL</button>
        <p>Current mode: {y_pos}</p>
        {/* <GaugeGraph title="Mode" x_cb={get_altitude_mission_time} y_cb={get_mode_y} unit="°C"
          min={-20}
          max={50}
          inner_colour_1="red"
          inner_colour_2="blue"
          outer_colour="blue"
          className="card"></GaugeGraph> */}
      </div>
    </>
    
  );
}
