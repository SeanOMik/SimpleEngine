//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_GAME_H
#define SIMPLEENGINE_GAME_H

#include <string>
#include <memory>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace simpleengine {
    class Game {
    public:
        friend class CollisionHandler;
        
        /**
         * @brief Construct a new Game object. Initializes GLEW and OpenGL
         * 
         * @param w Width of viewport
         * @param h Height of viewport
         * @param window_name The name of the window
         */
        Game(int w, int h, const std::string& window_name, const bool& resizeable = false);
        virtual ~Game();

        void update();
        void render_window();
        void render_items();
        void exit();
        int run();

        //void AddEvent(Event* event);
        std::shared_ptr<GLFWwindow> get_window();
    private:
        static void framebuffer_resize_callback(GLFWwindow*, int fbW, int fbH);

        std::shared_ptr<GLFWwindow> window;
    };
}

#endif //GAMEENGINE_GAME_H
