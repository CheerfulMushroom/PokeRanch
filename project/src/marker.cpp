#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "string.h"
#include "pokemon_model_path.h"
#include <cmath>

#include <string.h>
#include </usr/local/include/aruco/aruco.h>


using namespace std;
using namespace cv;
int main(int argc, char* argv[]) {    
    if (argc != 3) {
        cout << "[camera number] [camera_calibration_file.yml]" << endl;
        return 0;
    }
    const float marker_size = 0.071;  // Размер маркера (в метрах)
    Mat in_frame, out_frame;
    const char win1[]="Захват...";
    //const char win2[]="Запись...";
    double fps=25; // Число кадров в секунду
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
    
    // Настраиваем режим определения маркеров и минимальный размер маркера
    aruco::MarkerDetector MDetector;
    MDetector.setDictionary("ARUCO_MIP_36h12");
    aruco::MarkerDetector::Params &params = MDetector.getParameters();
    aruco::DetectionMode dm = aruco::DM_FAST;
    float min_size = 0;
    MDetector.setDetectionMode(dm, min_size); // setDetectionMode(DetectionMode dm, float minMarkerSize=0)
    
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
    
    //vector<aruco::Marker> TheMarkers;

    aruco::CameraParameters camera;
    float storona;
    while (true) {
    // Читаем кадр с камеры (захват и декодирование)
	inVid >> in_frame;
    
    // Преобразуем кадр в полутоновый формат (чета меняет соотношение сторон)
    //cvtColor(in_frame, out_frame, COLOR_BGR3GRAY);
    //resize(out_frame, out_frame, Size(width, height*3)); // что-то вроде костыля
    
    camera.readFromXMLFile(argv[2]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
    MDetector.setDictionary("ARUCO_MIP_36h12");
    auto markers=MDetector.detect(in_frame, camera, marker_size);
    for (auto m:markers) {
        //aruco::CvDrawingUtils::draw3dCube(in_frame,m,camera);
        //if (m.id == 1) cout << "Pikachu" << endl;
        //if (m.id == 2) cout << "Charizard" << endl;
        //cout << pokemon_name(m.id) << endl;

        //storona = m[0].x - m[1].x;
        cv::putText(in_frame, pokemon_model_path(m.id), m.getCenter(), FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,0,245), 3);
        cout << m.Rvec.at<float>(0) << endl;
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