#pragma once

#include "math/Vector.hpp"
#include "core/Renderer.hpp"
#include <GLFW/glfw3.h>
#include <chrono>

namespace GLBase {
    class Application
    {
    private:
        Renderer _renderer;
        void newFrame() const;
        std::pair<int, int> _winSize;
        void render() const;
    public:
        GLFWwindow *_window;
        std::pair<int, int> windowSize() const { return _winSize; }
        Application(int width, int height, const std::string_view& title, bool resizeable = false, bool maximized = false);
        void run();
        virtual bool processEvents();
        virtual void update(std::chrono::duration<double> delta) = 0;
        virtual void draw(const Renderer&) const = 0;
        virtual ~Application();
    };
}