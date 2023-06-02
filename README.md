# GLSL Explorer

Simple OpenGL based GLSL shader visualizer.

## What does it do?

GLSL Explorer simply allows you to import any GLSL fragment shader from a file, 
and visualize it on a 2D plane. It supports viewport navigation with panning and zooming,
custom uniforms, multiple render passes and framebuffer export.

## How do I build it?

The project is self contained, all dependencies (glew, glfw, ImGui and stb_image) are directly integrated into it. Clone this repo anywhere you like, then open the folder in your favorite terminal and do:
```
cmake -S . -B build
cmake --build ./build
```

## How do I use it?

### Writing a shader

All you technically need is to write the main function for your fragment shader, like this:
```glsl
void main(){
    frag0 = vec4(1);
}
```

The fragment shader has 8 outputs, frag0 to frag7. It also provides you with the following inputs:
```glsl
in vec4 pos; // the fragment position
in vec2 uv; // normalized screen space coordinates
uniform float scale; // the viewport scale parameter
uniform vec4 offset; // the viewport position parameter
uniform float time; // runtime
uniform vec2 res; // dimensions of the screen, in pixels
uniform sampler2D texNoise; // a white noise texture
```

To add your own inputs, simply add a uniform in your shader file, it will automatically appear in the UI,
in the "Shader parameters" section.
Only the following data types are supported: float, vec2, vec3, vec4, int, ivec2, ivec3, ivec4, bool,
bvec2, bvec3, bvec4.

### General settings

In this section your can find the general setup of the program:
- **Shader file**: the path to the GLSL file to load
- **Scale**: The global viewport scale level (decreasing the value "zooms in")
- **Position**: 4D offset to pan around
- **Mouse sensitivity**: Scale and position adjustment sensitivity
- **Slider sensitivity**: How sensitive should the sliders be for the custom shader parameters
- **Render pass**: Selects the output texture currently being displayed on screen

### View options

UI can be toggled by pressing tab. Fullscreen mode can also be toggled by pressing F11.
Both options are accessible in the View menu, at the top of the screen when UI is visible.

### Navigation

To pan the view, simply click and drag. To zoom in and out, hold shift and click and drag to the right/left.

### Screenshots

The "Save screenshot" button exports the contents of the current frame to file named "out.png" in the current working directory.

## Going further

The [examples](examples) folder contains a few premade shaders.