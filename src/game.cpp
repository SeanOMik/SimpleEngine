//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#include "game.h"

#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <gl/gl.h>

simpleengine::Game::Game(int w, int h, const std::string& window_name, const bool& resizeable) {
    // Create a window
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, resizeable);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

simpleengine::Game::~Game() {
    
}

void simpleengine::Game::update() {
    
}

void simpleengine::Game::render_window() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    render_items();
}

void simpleengine::Game::render_items() {
    
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