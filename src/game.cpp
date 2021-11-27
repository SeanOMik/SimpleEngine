#include "game.h"
#include "event/event.h"

#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <gl/gl.h>

simpleengine::Game::Game(int w, int h, const std::string& window_name, const int& gl_profile, const int& major_version,
        const int& minor_version, const bool& resizeable, const int& forward_compat) : window_resizeable(resizeable) {
    initialize(gl_profile, major_version, minor_version, window_resizeable, forward_compat);

    // Create a window
    window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(w, h, window_name.c_str(), NULL, NULL));

    // If we're not resizeable, we need to set the viewport size.
    if (!resizeable) {
        int fbWidth;
        int fbHeight;
        glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    } else {
        glfwSetFramebufferSizeCallback(window.get(), simpleengine::Game::framebuffer_resize_callback);
    }

    glfwMakeContextCurrent(window.get());

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize glew!" << std::endl;
        glfwTerminate();
    }
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
    while (!glfwWindowShouldClose(window.get())) {
        // Update input
        glfwPollEvents();

        update();

        render_window();

        // End draw
        glfwSwapBuffers(window.get());
        glFlush();
    }

    return 0;
}

std::shared_ptr<GLFWwindow> simpleengine::Game::get_window() {
    return window;
}

void simpleengine::Game::exit() {
    glfwSetWindowShouldClose(window.get(), true);
    glfwTerminate();
}

void simpleengine::Game::framebuffer_resize_callback(GLFWwindow*, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}