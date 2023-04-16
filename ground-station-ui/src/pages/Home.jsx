import React from "react";
import "./Home.css";

// Components
import DashboardGraph from "../components/main-dash/DashboardGraph";
import GaugeGraph from "../components/main-dash/GaugeGraph";
import GNSSMeta from "../components/main-dash/GNSSMeta";

export default function Home() {
  // Altitude data callbacks
  const get_altitude_mission_time = (data) => {
    return data.bme.map((packet) => packet.mission_time / 1000);
  };
  const get_altitude_y = (data) => {
    return data.bme.map((packet) => packet.altitude);
  };

  // Pressure data callbacks
  const get_pressure_y = (data) => {
    return data.bme.map((packet) => packet.pressure / 1000);
  };

  // Temperature data callbacks
  const get_temp_y = (data) => {
    return data.bme.map((packet) => packet.temperature);
  };

  // Battery voltge data callbacks
  const get_voltage_y = (data) => {
    return data.battery.map((packet) => packet.voltage / 1000);
  };

  const get_voltage_mission_time = (data) => {
    return data.battery.map((packet) => packet.mission_time / 1000);
  };


  const get_humidity_y = (data) => {
    return data.bme.map((packet) => packet.humidity);
  }

  // const get_acceleration_mission_time = (data) => {
  //   return data.acceleration_data.map((packet) => packet.mission_time / 1000);
  // }

  return (
    <main id="home" className="page-main">
      <h1>Main Dashboard</h1>
      <section id="graphs">
        <GaugeGraph
          title="Temperature"
          x_cb={get_altitude_mission_time}
          y_cb={get_temp_y}
          unit="°C"
          min={-20}
          max={50}
          inner_colour_1="red"
          inner_colour_2="blue"
          outer_colour="blue"
          className="card"
        />

        <GNSSMeta className="card" />

        <GaugeGraph
          title="Pressure"
          x_cb={get_altitude_mission_time}
          y_cb={get_pressure_y}
          unit="KPa"
          min={60}
          max={120}
          inner_colour_1="red"
          inner_colour_2="green"
          outer_colour="green"
          className="card"
        />

        <DashboardGraph
          title="Altitude"
          x_title="Time"
          x_unit="s"
          y_title="Altitude"
          y_unit="m"
          x_cb={get_altitude_mission_time}
          y_cb={get_altitude_y}
          line_colour={2}
          className="card"
        />

        <DashboardGraph
          title="Battery Voltage"
          x_title="Time"
          x_unit="s"
          y_title="Volt"
          y_unit="V"
          x_cb={get_voltage_mission_time}
          y_cb={get_voltage_y}
          line_colour={1}
          className="card"
        />

        <DashboardGraph
          title="Humidity"
          x_title="Time"
          x_unit="s"
          y_title="Humidity"
          y_unit="%"
          x_cb={get_altitude_mission_time}
          y_cb={get_humidity_y}
          line_colour={1}
          className="card"
        />
      </section>
    </main>
  );
}
