import React from "react";
import Popup from "reactjs-popup";
import 'reactjs-popup/dist/index.css';
import { useWebsocket } from "../../hooks/useWebsocket";

function DeviceSelection({ports}){
    return (
        <Popup trigger={<button> Select device</button>} position="right center">
            <div style={{color: "Blue"}}>Select your device</div>
            <ul style={{color: "Blue"}}>
                {ports.map(port => <li>{port}</li>)}
            </ul>
            
        </Popup>
    );
}

export default DeviceSelection;