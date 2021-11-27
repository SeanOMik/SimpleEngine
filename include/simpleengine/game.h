#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "event/event.h"

namespace simpleengine {
    class Game {
    private:
        using self = simpleengine::Game;
    public:
        /**
         * @brief Construct a new Game object. Initializes GLEW and OpenGL
         * 
         * @param w Width of viewport
         * @param h Height of viewport
         * @param window_name The name of the window
         */
        Game(int w, int h, const std::string& window_name, const int& gl_profile = GLFW_OPENGL_CORE_PROFILE, const int& major_version = 4,
            const int& minor_version = 4, const bool& resizeable = false, const int& forward_compat = GL_TRUE);
        virtual ~Game();

        void add_event(std::shared_ptr<simpleengine::Event> event);

        void update();
        void handle_input();
        void render_window();
        void render_items();
        void exit();
        int run();

        //void AddEvent(Event* event);
        std::shared_ptr<GLFWwindow> get_window();
    private:
        static void framebuffer_resize_callback(GLFWwindow*, int fbW, int fbH);

        void initialize(const int& gl_profile, const int& major_version, const int& minor_version,
            const bool& resizeable, const int& forward_compat = GL_TRUE);

        std::shared_ptr<GLFWwindow> window;
        std::vector<std::shared_ptr<simpleengine::Event>> events;
        const bool& window_resizeable;
    };
}