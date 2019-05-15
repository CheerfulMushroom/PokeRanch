#include <iostream>
#include <GameWindow.h>



int main() {
    GameWindow screen(800, 600, 25);

    while (!glfwWindowShouldClose(screen.window)) {
        screen.frame_start_time = glfwGetTime();

        glfwPollEvents();

        glfwSwapBuffers(screen.window);
        screen.lock_frame_rate();
    }

    return 0;
}

