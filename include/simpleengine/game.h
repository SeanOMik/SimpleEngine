#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <vector>

#ifdef __linux__
#include <GL/glew.h>
#else
#include <gl/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "event/event.h"
#include "simpleengine/renderable.h"

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
        void add_renderable(std::shared_ptr<simpleengine::Renderable> renderable_event);
        void set_fps_limit(const int& fps_limit);
        void set_enable_vsync(const bool& enabled);

        void update(const float& delta_time);
        void input_update(const float& delta_time);
        void render_window(const float& interpolate_alpha, const float& delta_time);
        void render_items(const float& interpolate_alpha, const float& delta_time);
        void exit();
        int run();

        //void AddEvent(Event* event);
        GLFWwindow* get_window();
    private:
        static void framebuffer_resize_callback(GLFWwindow*, int fbW, int fbH);

        void initialize(const int& gl_profile, const int& major_version, const int& minor_version,
            const bool& resizeable, const int& forward_compat = GL_TRUE);

        GLFWwindow* window;
        std::vector<std::shared_ptr<simpleengine::Event>> events;
        std::vector<std::shared_ptr<simpleengine::Renderable>> renderable_events;
        const bool& window_resizeable;

        // FPS related stuff
        void update_enabled_vsync() const;
        void limit_framerate(const float& delta_time) const; // Triggered at the end of a draw to help limit the FPS to `fps_limit`.
        int fps_limit = -1;
        bool enable_vsync = true;

        // Engine TPS related stuff
        int max_engine_tps = 120; // The maximum engine TPS
        float fixed_delta_time = 1.f / (float) max_engine_tps; // The delta time from fixed timestep
        float tps_accumulator = 0.f;

        float get_delta_time();

        std::chrono::high_resolution_clock::time_point last_frame_time;
    };
}