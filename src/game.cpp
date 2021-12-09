#include "game.h"
#include "event/event.h"

#include <iostream>

#ifdef __linux__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#elif
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#endif

simpleengine::Game::Game(int w, int h, const std::string& window_name, const int& gl_profile, const int& major_version,
        const int& minor_version, const bool& resizeable, const int& forward_compat) : window_resizeable(resizeable) {
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
}

void simpleengine::Game::enable_default_gl_options() const {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

void simpleengine::Game::update() {
    handle_input();

    // Update items
    for (const std::shared_ptr<Event>& event : events) {
        event->update(0.f);
    }
}

void simpleengine::Game::handle_input() {

}

void simpleengine::Game::render_window() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    render_items();
}

void simpleengine::Game::render_items() {
    for (const std::shared_ptr<Event>& event : events) {
        event->render(window);
    }
}

int simpleengine::Game::run() {
    while (!glfwWindowShouldClose(window)) {
        // Update input
        glfwPollEvents();

        update();

        render_window();

        // End draw
        glfwSwapBuffers(window);
        glFlush();
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