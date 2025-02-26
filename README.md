# Reflections of Paradise  

## Project Overview  
**Reflections of Paradise** is a 3D graphics project developed using **OpenGL** and **Blender**, designed to create a realistic and immersive island scene. The project explores various graphical techniques, including lighting, animations, fog, rain effects, and camera movement, showcasing the capabilities of **OpenGL** in rendering visually appealing environments.  

## Features  
- **Realistic 3D Island Scene** – Modeled in Blender and rendered using OpenGL.  
- **Lighting Effects** – Includes directional light (sunlight) and point lights (lanterns, torches).  
- **Camera System** – Supports free movement and an automated guided tour.  
- **Animation** – A rotating umbrella and dynamic environmental effects.  
- **Fog & Rain Simulation** – Weather effects controlled via keyboard input.  
- **Wireframe & Point Views** – Toggle different visualization modes for scene analysis.  
- **Transparent Objects** – Glass objects with realistic transparency effects.  

## Functionalities  
- **Camera Navigation:** Move around the scene using the keyboard and mouse.  
- **Lighting Controls:** Adjust the intensity and position of light sources dynamically.  
- **Weather Effects:** Toggle fog and rain effects for a more immersive experience.  
- **Scene Animation:** The umbrella rotates, and torches emit realistic lighting.  

## Controls  
| Key | Function |
|-----|----------|
| `W` / `S` | Move Forward / Backward |
| `A` / `D` | Move Left / Right |
| `Q` / `E` | Rotate Camera Left / Right |
| `C` | Start Automated Tour |
| `F` | Toggle Fog Effect |
| `M` | Toggle Rain Effect |
| `P` | Activate Point View |
| `O` | Activate Wireframe View |
| `I` | Activate Normal View |
| `L` | Toggle Main Point Light Source |
| `K` / `J` / `H` | Activate Each Torch |

## Implementation Details  
### **Camera Movement**  
Implemented using transformations and dynamic view matrix updates with `lookAt`. The camera allows both manual exploration and an automated tour to showcase the scene.  

### **Lighting System**  
- **Directional Light:** Simulates sunlight with adjustable intensity and dynamic movement.  
- **Point Light:** Used for torches and lanterns, with attenuation effects for realistic falloff.  

### **Fog & Weather Effects**  
- Fog is implemented in the fragment shader, adjusting density based on the camera distance.  
- Rain simulation uses randomly generated particles falling with different speeds and delays.  

### **Transparency & Fragment Discarding**  
- Transparent objects (glasses) blend colors for a realistic see-through effect.  
- Fragment discarding removes unwanted black edges in tree leaves, enhancing realism.  

## Getting Started  
### **Clone the Repository**  
```sh
git clone https://github.com/your-username/reflections-of-paradise.git
