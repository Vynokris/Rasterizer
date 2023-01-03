# Rasterizer

## General Information

This project was done for the ISART Digital school by Rémi Serra and Alexandre Perché. <br>
The end goal of the project was to create a program that renders objects in 3D, computes lighting, and has a camera from which the scene is viewed. This is all done on one CPU thread, so the performance isn't ideal, although the code is somewhat optimised.

<br>

## Preview

<img src="./screenshots/showcase.gif" style="width:700px;"/>

<br>

## Features

### Objects:
- Rasterization of triangles in 3D space.
- Drawing functions for:
    - Quads
    - Cubes
    - Subdivided cubesW
    - Spheres
- Object depth buffer.
- Alpha-blending between objects.
- Back-face culling.
- Object manager to edit the scene's objects in the engine.

### 3D mathematics:
- Translation, rotation and scaling using matrices.
- Transformation of vertices through local, world, view, clip, ndc and screen spaces.

### Colors and textures:
- Interpolation of vertex colors.
- Loading textures from .bmp files.
- Perspective-correct application of textures onto objects (nearest-neighboor).
- Applying vertex hues to textures.

### Lights:
- Phong and Blinn-Phong lighting models.
- Materials that dictate how lighting is applied to objects.
- Light manager to edit the scene's lights in the engine.

### Camera:
- Perspective camera.
- 1st and 3rd person view modes.
- Unlit, lit, wireframe and z-buffer rendering modes.
- Controls:
    - Press ```right click``` to take control.
    - In 1st person: 
        - use ```WASD+shift/space``` to move and ```mouse``` to look around.
    - In 3rd person (2 options): 
        - use ```mouse``` to rotate around the taget point and ```mouse wheel``` to change the camera distance.
        - use ```WASD+shift/space``` to rotate around the target point and change the camera distance.

<br>

## Shortcomings
The following features were not added because of lack of time, or bad compatibility with the current state of the codebase.
- Meshes.
- Anti-aliasing.
- Clipping.

<br>

## How to build

### Linux
- Execute the ```make``` command in the project root.
- Launch the ```Rasterizer``` executable file.

### Windows
- You need to have clang installed (run ```clang -v``` to check).
- Execute ```build.bat``` to build the project using clang compiler.
- Or execute the ```make``` command in Cygwin.
- Launch the ```Rasterizer.exe``` file.
