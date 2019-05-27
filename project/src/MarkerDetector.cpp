#define marker_size 0.071

#include <glm/ext.hpp>
#include <VideoStream.h>

#include "MarkerDetector.h"

MarkerDetector::MarkerDetector(VideoStream *stream, std::string path_to_calibraion_info) {
    this->stream = stream;

    // Setting up aruco detector
    marker_detector.setDictionary("ARUCO_MIP_36h12");
    aruco::DetectionMode mode = aruco::DM_FAST;
    marker_detector.setDetectionMode(mode, 0);

    params.readFromXMLFile(path_to_calibraion_info);

    cv::Size img_size(800, 600);

    double proj_mat[16];
    params.glGetProjectionMatrix(params.CamSize, img_size, proj_mat, 0.01, 100);
    projection = glm::make_mat4(proj_mat);


}

void MarkerDetector::update() {
    model_view_map.clear();

    cv::Mat frame = stream->frame;
    auto markers = marker_detector.detect(frame, params, marker_size);


    for (auto mrkr:markers) {
        cv::Mat rodrig;

        auto good_rvec = mrkr.Rvec;
        good_rvec.at<float>(0, 2) *= -1.0;


        Rodrigues(good_rvec, rodrig);


        GLfloat RTMat[16] = {rodrig.at<float>(0, 0), rodrig.at<float>(0, 1), rodrig.at<float>(0, 2), 0,
                             rodrig.at<float>(1, 0), rodrig.at<float>(1, 1), rodrig.at<float>(1, 2), 0,
                             rodrig.at<float>(2, 0), rodrig.at<float>(2, 1), rodrig.at<float>(2, 2), 0,
                             10 * mrkr.Tvec.at<float>(0), 10 * mrkr.Tvec.at<float>(1), 10 * -mrkr.Tvec.at<float>(2), 1};

        glm::mat4 model_view = glm::make_mat4(RTMat);

        model_view_map[mrkr.id] = model_view;
    }
}


glm::mat4 MarkerDetector::get_model_view(int id) {
    return model_view_map[id];
}
