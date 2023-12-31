import React from "react";

export default function Satellite({ ide, type, azimuth, elevation, snr }) {
  return (
    <div className="satellite">
      <h5>{type}</h5>
      {ide}
      {/* <p>
        <strong>Elevation: </strong>
        {elevation}
      </p> */}
      {/* <p>
        <strong>Azimuth: </strong>
        {azimuth}
      </p> */}
      <p>
        <strong>SNR: </strong>
        {snr}
      </p>
    </div>
  );
}
