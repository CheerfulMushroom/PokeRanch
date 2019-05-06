#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <string.h>
#include </usr/local/include/aruco/aruco.h>

using namespace std;
using namespace cv;
int main(int argc, char* argv[]) {    
    const float marker_size = 0.04;  // Размер маркера (в метрах)
    Mat in_frame, out_frame;
    const char win1[]="Захват...";
    //const char win2[]="Запись...";
    double fps=30; // Число кадров в секунду
    //char file_out[]="recorded.avi";
    
    VideoCapture inVid(stoi(argv[1])); // Открыть камеру по умолчанию (первый аргумент командной сроки)
    if (!inVid.isOpened()) { // Проверка ошибок
        cout << "Ошибка! Камера не готова...\n";
        return -1;
    }

    // Получаем ширину и высоту входного видео
    int width = (int)inVid.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)inVid.get(CAP_PROP_FRAME_HEIGHT);
    
    /*VideoWriter recVid(file_out,
	            VideoWriter::fourcc('M','J','P','G'),
		        fps,
                Size(width, height));
    if (!recVid.isOpened()) {
	    cout << "Ошибка! Видеофайл не открыт...\n";
	    return -1;
    }*/

    // Создаем два окна: для исходного и конечного видео
	namedWindow(win1);
	//namedWindow(win2);
    
    // Интерфейс (типа)
    printf("****************\n");
    printf("Make photo - f\nStop - s\n");
    printf("****************\n");
    
    int wait_key_button;
	int number = 1;
    string number_s;
    char* photo_name = new char[10];
    //char* photo_name;
    char* ending = ".png";
    
    while (true) {
    // Читаем кадр с камеры (захват и декодирование)
	inVid >> in_frame;
    
    // Преобразуем кадр в полутоновый формат (чета меняет соотношение сторон)
    //cvtColor(in_frame, out_frame, COLOR_BGR3GRAY);
    //resize(out_frame, out_frame, Size(width, height*3)); // что-то вроде костыля
    
    aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[2]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
    aruco::MarkerDetector Detector;
    Detector.setDictionary("ARUCO_MIP_36h12");
    auto markers=Detector.detect(in_frame, camera, marker_size);
    for (auto m:markers) {
        aruco::CvDrawingUtils::draw3dAxis(in_frame,m,camera);
        cout << m.Rvec << " " << m.Tvec << endl;
    }
    // Записываем кадр в видеофайл (кодирование и сохранение)
	//recVid << in_frame;
    imshow(win1, in_frame);  // Показываем кадр в окне
    //imshow(win2, in_frame); // Показываем кадр в окне
	wait_key_button = waitKey(1000/fps);

     if ((wait_key_button == 70) || (wait_key_button == 102))
        {
        number_s = to_string(number);  // Перевод числа в строку
        strcpy(photo_name, number_s.c_str());  //Перевод string в char*
        imwrite(strcat(photo_name, ending), in_frame);
        number++;
        }

    if ((wait_key_button == 83) || (wait_key_button == 115))
	    {
        break;
        }
    }
	inVid.release(); // Закрываем камеру
    return 0;
}
