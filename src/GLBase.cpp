#include <GLBase/GLBase.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <stdexcept>
#include <chrono>

namespace GLBase{
    Application::Application(int width, int height, const std::string_view& title, bool resizeable, bool maximized)
    {
        /* inits glfw, OpenGL and ImGUI */
		if(!glfwInit()){
			const char *err;
			glfwGetError(&err);
			throw std::runtime_error(err);
		} 
		glfwWindowHint(GLFW_RESIZABLE, resizeable);
		glfwWindowHint(GLFW_MAXIMIZED, maximized);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_winSize = {width, height};

		_window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
		if(!_window) {
			const char *err;
			glfwGetError(&err);
			throw std::runtime_error(err);			
		}

		glfwMakeContextCurrent(_window);
    	glfwSwapInterval(0); // Disable vsync

		auto err = glewInit();
		if(err != GLEW_OK) throw std::runtime_error((const char*)glewGetErrorString(err));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_renderer = Renderer();

		ImGui::SetCurrentContext(ImGui::CreateContext());
		ImGuiIO &io = ImGui::GetIO();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init();
		/* Enable docking for the UI windows */
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;	
    }
    Application::~Application() {
		/* cleanup */
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();

		glfwDestroyWindow(_window);
		glfwTerminate();
	}
    void Application::newFrame() const {
        ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
    }
    void Application::render() const {
		ImGui::Render();
		glCheckCall(glViewport(0, 0, _winSize.first, _winSize.second));
		_renderer.clear(); // clear the screen
		draw(_renderer); // let the user render things out

		// draw the UI
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// flush everything
        glfwSwapBuffers(_window);
	}
	bool Application::processEvents() {
		glfwPollEvents();
        return  !(glfwWindowShouldClose(_window));
	}
	void Application::run() {
		/* simple loop */
		using std::chrono::steady_clock, std::chrono::duration;
		duration<double> delta;
		steady_clock::time_point t = steady_clock::now();
		while (processEvents()) {
			delta = steady_clock::now() - t;
			t = steady_clock::now();
			newFrame();
			update(delta);
			glfwGetFramebufferSize(_window, &_winSize.first, &_winSize.second);
			render();
		}
	}
}