# DX-Hieroglyph3-HLSL

![DirectX_Test.png](https://github.com/M35S/DX-Hieroglyph3-HLSL/blob/main/DX%20GitHub%20pics/DirectX_Test.PNG)

## Contents
- Introduction
- Application Flow

### Introduction

This is a 3D renderering engine called Hieroglyph3 that was developed by by Jason Zink. It utilizes DirectX_TK and the LuaLib libraries.

I was first exposed to this framework during my time at university. I decided to revisit this project and break it down to see how everything works together.

Please note that I have only included several source files from the project's source code due to the shear size of the project. 

Hopefully with what I have provided can help give some context. Hieroglyph3 can be downloaded via other GitHub users on here should you wish to use it.

### Application Flow

![DX_App_Flow.png](https://github.com/M35S/DX-Hieroglyph3-HLSL/blob/main/DX%20GitHub%20pics/DX_App_Flow.PNG)

The above diargam aims to simplify the application's flow from start to finish. 

You can see the core data structures that drive the application's rendering process are the following:
- Win32RenderWindow*: object window
- RendererDX11*: object renderer

The connections and arrow directions between the data structures illustrate how data is fed into the other data containers. 

You may notice that the object renderer plays a central role to the rendering process. 
