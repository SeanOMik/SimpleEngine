#include "game.h"
#include "event/event.h"
#include "renderable.h"

#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>

#ifdef __linux__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#endif

simpleengine::Game::Game(int w, int h, const std::string& window_name, const int& gl_profile, const int& major_version,
        const int& minor_version, const bool& resizeable, const int& forward_compat) : window_resizeable(resizeable),
        tps_accumulator(0.f) {
    initialize(gl_profile, major_version, minor_version, window_resizeable, forward_compat);

    // Create a window
    window = glfwCreateWindow(w, h, window_name.c_str(), NULL, NULL);

    // If we're not resizeable, we need to set the viewport size.
    if (!resizeable) {
        int fbWidth;
        int fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    } else {
        glfwSetFramebufferSizeCallback(window, simpleengine::Game::framebuffer_resize_callback);
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize glew!" << std::endl;
        glfwTerminate();
    }

    enable_default_gl_options();

    last_frame_time = std::chrono::high_resolution_clock::now();
}

void simpleengine::Game::enable_default_gl_options() const {
    
    //glFrontFace(GL_CW);

    update_enabled_vsync();
}

void simpleengine::Game::enable_gl_option(GLenum option) const {
    glEnable(option);
}

void simpleengine::Game::initialize(const int& gl_profile, const int& major_version, const int& minor_version,
        const bool& resizeable, const int& forward_compat) {
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_RESIZABLE, window_resizeable);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forward_compat);
}

simpleengine::Game::~Game() {
    glfwDestroyWindow(window);
}

void simpleengine::Game::add_event(std::shared_ptr<simpleengine::Event> event) {
    events.push_back(event);
}

void simpleengine::Game::add_renderable(std::shared_ptr<simpleengine::Renderable> renderable_event) {
    renderable_events.push_back(renderable_event);
    
    // Also push to normal events to trigger updates.
    // Instead of this, we could instead loop renderable_events in the update function,
    // but that would cause multiple loops.
    events.push_back(renderable_event);
}

void simpleengine::Game::set_fps_limit(const int& fps_limit) {
    this->fps_limit = fps_limit;
}

void simpleengine::Game::set_enable_vsync(const bool& enabled) {
    this->enable_vsync = enabled;

    this->update_enabled_vsync();
}

void simpleengine::Game::update_enabled_vsync() const {
    if (enable_vsync) {
        //glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

void simpleengine::Game::input_update(const float& delta_time) {
    // TODO

    for (const std::shared_ptr<Event>& event : events) {
        event->input_update(delta_time);
    }
}

void simpleengine::Game::update(const float& delta_time) {
    // Update items
    for (const std::shared_ptr<Event>& event : events) {
        event->update(delta_time);
    }
}

void simpleengine::Game::render_window(const float& interpolate_alpha, const float& delta_time) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    render_items(interpolate_alpha, delta_time);
}

void simpleengine::Game::render_items(const float& interpolate_alpha, const float& delta_time) {
    for (const std::shared_ptr<Renderable>& renderable : renderable_events) {
        renderable->render(interpolate_alpha, delta_time);
    }
}

float simpleengine::Game::get_delta_time() {
    auto current_frame_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration delta_dur = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    return std::chrono::duration_cast<std::chrono::duration<float>>(delta_dur).count();
}

void simpleengine::Game::limit_framerate(const float& delta_time) const {
    if (!enable_vsync && fps_limit >= 15) {
        auto delta_time_duration = std::chrono::duration<float>(delta_time);
        auto limit_duration = std::chrono::duration<float>(1 / ( (float) fps_limit / 2));

        if (delta_time_duration < limit_duration) {
            auto sleep_duration = limit_duration - delta_time_duration;

            std::this_thread::sleep_for(sleep_duration);
        }
    }
}

int simpleengine::Game::run() {
    while (!glfwWindowShouldClose(window)) {
        // Get delta time first thing
        float frame_time = get_delta_time();

        // Poll input events
        glfwPollEvents();
        input_update(frame_time); // Update input on varying timestep

        tps_accumulator += frame_time;

        // https://gafferongames.com/post/fix_your_timestep/
        while (tps_accumulator >= fixed_delta_time) {
            update(fixed_delta_time);

            tps_accumulator -= fixed_delta_time;
        }
        
        // Alpha used for interpolating objects in rendering
        float interpolate_alpha = tps_accumulator / fixed_delta_time;

        render_window(interpolate_alpha, frame_time);

        // End draw
        glfwSwapBuffers(window);
        glFlush();
        
        limit_framerate(frame_time);
    }

    return 0;
}

GLFWwindow* simpleengine::Game::get_window() {
    return window;
}

void simpleengine::Game::exit() {
    glfwSetWindowShouldClose(window, true);
    glfwTerminate();
}

void simpleengine::Game::framebuffer_resize_callback(GLFWwindow*, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}