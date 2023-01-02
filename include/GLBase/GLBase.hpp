#pragma once

#include "math/Vector.hpp"
#include "core/Renderer.hpp"
#include <GLFW/glfw3.h>

namespace GLBase {
    class Application
    {
    private:
        Renderer _renderer;
        GLFWwindow *_window;
        void newFrame() const;
        std::pair<int, int> _winSize;
        void render() const;
    public:
        std::pair<int, int> windowSize() const { return _winSize; }
        Application(int width, int height, const std::string_view& title, bool resizeable = false, bool maximized = false);
        void run();
        virtual bool processEvents();
        virtual void update() = 0;
        virtual void draw(const Renderer&) const = 0;
        virtual ~Application();
    };
}