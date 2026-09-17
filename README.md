## 3D City Skyline
*in C++ and OpenFrameworks/OpenGL*

&nbsp;

### 📋 Project Description
---

Creation of a procedurally generated simple city skyline scene built in OpenFrameworks, featuring randomly generated buildings, a river, a carousel, and a full lighting setup with dynamic movement, multiple lights, materials, textures and a skybox.

&nbsp;

*Features*

1. Geometry & Scene
- Skyline of 4+ buildings, procedurally generated from unit meshes and cubes
- Custom vertex-positioned river/water plane
- Ground plane with correct object placement
- Carousel with rotating top and pillars built from cylinders/cones
- Front, top, and custom perspective/orthogonal camera views

2. Animation
- Rotating carousel top (continuous rotation)
- Animated train element with relative motion between components

3. Lighting
- 4 light sources: directional (sun/moon), point, spot, and an additional light
- Dynamic light: sun/moon direction animates automatically over time (day/night cycle)
- Keyboard/mouse toggles to turn each light on/off independently
- Keyboard/mouse toggles for ambient, diffuse, and specular components per light

4. Materials & Textures
- 4+ distinct OpenGL materials applied across different elements
- Textures on buildings, river and snow (image-based)
- Bump mapping support for added surface detail

5. Skybox & Views
- Starfield skybox, lit and textured
- Wireframe, base, top, and front views
- Ambient-light-only render mode

&nbsp;

*Running the Project*

1. Install Visual Studio (https://visualstudio.microsoft.com/downloads/)
- On the instaler add "Desktop development with C++"
2. Install OpenFrameworks (https://openframeworks.cc/download/)
- Get version 0.12.0 (get the folder: of_v0.12.0_vs_release)
- To create a project use: \of_v0.12.0_vs_release\projectGenerator and add the files
3. Build and Run

&nbsp;

---

**NOTE:** This project was developed within the scope of a Computer Science course by Cláudia Torres. Course materials, base camera implementations, and utility modules were provided by the Computer Graphics course instructors and are fully credited in the comments of their respective source files.
