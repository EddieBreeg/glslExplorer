#include <screenshot.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <GLBase/core/glErrors.h>
#include <GL/glew.h>
#include <memory>

struct pixel {uint8_t r, g, b;};
bool saveFrame(const std::string& path, unsigned width, unsigned height, void *buf){
    std::unique_ptr<pixel> p = nullptr;
    if(!buf){ // if the user didn't provide us with a buffer to store the data, allocate it ourselves
        p = std::unique_ptr<pixel>(new pixel[width * height]);
        buf = p.get();
    }
    glCheckCall(
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf));
    stbi_flip_vertically_on_write(1);
    return stbi_write_png(path.c_str(), width, height, 3, buf, width*sizeof(pixel));
    
}