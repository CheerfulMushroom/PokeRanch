#include <iomanip>
#include <ctime>

#include "ButtonFunctions.h"

void change_to_ranch() {
    game_object->change_state(std::make_unique<RanchState>(game_object));
}

void change_to_pokedex(int pokemon_id) {
    game_object->change_state(std::make_unique<PokedexState>(game_object, pokemon_id));
}

void change_to_pause() {
    game_object->change_state(std::make_unique<PauseState>(game_object));
}

void swap_cam() {
    game_object->cam_id = (game_object->cam_id + 1) % 3;
    game_object->change_state(std::make_unique<RanchState>(game_object));
}

void take_screenshot(int width, int height) {
    cv::Mat img(height, width, CV_8UC3);

    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step / img.elemSize());

    glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);

    cv::Mat flipped(img);
    cv::flip(img, flipped, 0);

    // Получаем дату и время
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    auto date = oss.str();

    std::string file_name = "project/photos/" + date + ".png";

    cv::imwrite(file_name, img);
}

void close_window(){
    glfwWindowShouldClose(game_object->get_window());
}