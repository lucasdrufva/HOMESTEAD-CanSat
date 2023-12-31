import React, { useRef, useState } from "react";
import { Canvas, useFrame } from "@react-three/fiber";
import { ReactThreeFiber } from "@react-three/fiber";

// Hooks
import { useStorage } from "../../hooks/useStorage";

function CanSat(props) {
  // This reference will give us direct access to the mesh
  const mesh = useRef();
  // Set up state for the hovered and active state
  const [hovered, setHover] = useState(false);
  const [active, setActive] = useState(false);

  const get_rotation_real = (data) => {
    if(data.rotation == undefined){
      return [];
    }
    return data.rotation.map((packet) => packet.real);
  };

  const get_rotation_i = (data) => {
    if(data.rotation == undefined){
      return [];
    }
    return data.rotation.map((packet) => packet.i);
  };

  const get_rotation_j = (data) => {
    if(data.rotation == undefined){
      return [];
    }
    return data.rotation.map((packet) => packet.j);
  };

  const get_rotation_k = (data) => {
    if(data.rotation == undefined){
      return [];
    }
    return data.rotation.map((packet) => packet.k);
  };


  const real = useStorage(get_rotation_real);
  const i = useStorage(get_rotation_i);
  const j = useStorage(get_rotation_j);
  const k = useStorage(get_rotation_k);

  // Subscribe this component to the render-loop, rotate the mesh every frame
//   useFrame((state, delta) => (mesh.current.rotation.x += delta));

  // Return view, these are regular three.js elements expressed in JSX
  return (
    <mesh
      ref={mesh}
      quaternion={[real[real.length-1], i[i.length-1], j[j.length-1], k[k.length-1]]}
      scale={active ? 1.5 : 1}
      onClick={(event) => setActive(!active)}
      onPointerOver={(event) => setHover(true)}
      onPointerOut={(event) => setHover(false)}
    >
      <cylinderGeometry args={[1, 1, 2]} />
      <meshStandardMaterial color={hovered ? "hotpink" : "orange"} />
    </mesh>
  );
}
export default function Visual(props) {
  

  return (
    <div className="satellite">
      <Canvas>
        <ambientLight />
        <pointLight position={[10, 10, 10]} />
        {/* <CanSat rotation={[real, i, j, k]}></CanSat> */}
        <CanSat></CanSat>
      </Canvas>
    </div>
  );
}
