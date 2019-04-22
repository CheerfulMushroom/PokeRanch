#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
    cv::Mat in_image, out_image;
    in_image = imread(argv[1], cv::IMREAD_UNCHANGED);
    if (in_image.empty()) {
        std::cout << "Ошибка" << std::endl;
        return 1;
    }

    cv::namedWindow(argv[1], cv::WINDOW_AUTOSIZE);
    cv::namedWindow(argv[2], cv::WINDOW_AUTOSIZE);

    cv::imshow(argv[1], in_image);
    cv::cvtColor(in_image, out_image, cv::COLOR_BGR2GRAY);
    cv::imshow(argv[2], out_image);
    std::cout << "Кнопку для выхода" << std::endl;
    cv::waitKey();

    imwrite(argv[2], out_image);
    return 0;
}
