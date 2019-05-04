#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main(int, char **) {    
    Mat in_frame, out_frame;
    const char win1[]="Захват...", win2[]="Запись...";
    double fps=30; // Число кадров в секунду
    char file_out[]="recorded.avi";
    
    VideoCapture inVid(2); // Открыть камеру по умолчанию
    if (!inVid.isOpened()) { // Проверка ошибок
        cout << "Ошибка! Камера не готова...\n";
        return -1;
    }

    // Получаем ширину и высоту входного видео
    int width = (int)inVid.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)inVid.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter recVid(file_out,
	            VideoWriter::fourcc('M','J','P','G'),
		        fps,
                Size(width, height));
    if (!recVid.isOpened()) {
	    cout << "Ошибка! Видеофайл не открыт...\n";
	    return -1;
    }
    // Создаем два окна: для исходного и конечного видео
	namedWindow(win1);
	namedWindow(win2);
	while (true) {
    // Читаем кадр с камеры (захват и декодирование)
	inVid >> in_frame;
    // Преобразуем кадр в полутоновый формат
    //cvtColor(in_frame, out_frame, COLOR_BGR3GRAY);
    //resize(out_frame, out_frame, Size(width, height*3));
    // Записываем кадр в видеофайл (кодирование и сохранение)
	recVid << in_frame;
    imshow(win1, in_frame);  // Показываем кадр в окне
    imshow(win2, in_frame); // Показываем кадр в окне
	if (waitKey(1000/fps) >= 0)
	    break;
    }
	inVid.release(); // Закрываем камеру
    return 0;
}
