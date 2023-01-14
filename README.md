# OpenGL Game Engine Project
OpenGL and SDL2 game engine written in C++

## Current features
* Built in Entity Component System[^1] (ECS) architecture
* 3D model loading and rendering with transformations
* Texture mapping
* Text rendering
* Access to the user input in multiple forms
* Collision detection and physics simulation with dynamics and collision resolution

## What we learned
Over the course of this project we learned how OpenGL works to render the graphics that many of us take for granted when we play video games, watch an animation, or use any kind of 3D software. We learned how to structure projects around the framework and [documented them](Source/Platform/OpenGL/OpenGLRenderDevice.cpp) so that they are usable, readable and maintainable.

We also learned how to debug, test and fix low-level issues using [RenderDoc](https://renderdoc.org/) and the Visual Studio C++ debugger and profiler.

## Demo
Here's a quick video showcase of the 3D model rendering, collision detection and physics engine simulation, text rendering and user input systems in action:
![Game Engine Physics Demo Video](Demo.gif)

## Installation
Before building this project, you will need Microsoft Visual Studio 2022 with the *Desktop development with C++* workload.

Next, clone the project and open `GLEngine.sln` using Visual Studio. The project should already be configured and necessary binaries are present; build in either `Release x64` or `Debug x64`.

Currently no binaries are released, this may change in the future.

## Credits
This project was created in collaboration with [Maxwell Hunt](https://github.com/Maxwell-Hunt).

* [Intro to Modern OpenGL Tutorial](https://www.youtube.com/watch?v=ftiKrP3gW3k&list=PLEETnX-uPtBXT9T-hD0Bj31DSnwio-ywh) by Benny Bobaganoosh "thebennybox"
* [3D Game Programming Tutorial](https://www.youtube.com/watch?v=0t91FvMJXAs&list=PLEETnX-uPtBUrfzE3Dxy3PWyApnW6YEMm) by Benny Bobaganoosh "thebennybox"
* [Texture Packing for Fonts](https://straypixels.net/texture-packing-for-fonts/) by Edward Lu "StrayPixels"
* [Designing a physics engine](https://blog.winter.dev/2020/designing-a-physics-engine/) by IainWinter

## Helpful links for understanding the project
* https://www.khronos.org/opengl/wiki (OpenGL Documentation)
* https://learnopengl.com (OpenGL Documentation and tutorials)
* https://docs.gl (OpenGL Documentation)

## License
Please see the [LICENSE](LICENSE) for more information.


[^1]: Software architecture pattern sometimes used for game development. It consists of, as the name would suggest, components, entities, and systems. Components refer to pieces of connected data, entities refer to collections of components which are stored together in memory and systems are processes which act on entities which have certain specified components.