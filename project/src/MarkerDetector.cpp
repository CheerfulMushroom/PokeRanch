#define marker_size 0.020

#include <glm/ext.hpp>
#include <opencv2/calib3d.hpp>


#include "VideoStream.h"
#include "MarkerDetector.h"
#include "States.h"
#include "AnimModel.h"
#include "ButtonFunctions.h"

MarkerDetector::MarkerDetector(VideoStream *stream,
                               std::string path_to_calibraion_info,
                               int width, int height,
                               GameState *state) {
    this->stream = stream;
    this->state = state;

    // Setting up aruco detector
    marker_detector.setDictionary("ARUCO_MIP_36h12");
    aruco::DetectionMode mode = aruco::DM_FAST;
    marker_detector.setDetectionMode(mode, 0);

    params.readFromXMLFile(std::move(path_to_calibraion_info));

    cv::Size img_size(width, height);

    double proj_mat[16];
    params.glGetProjectionMatrix(params.CamSize, img_size, proj_mat, 0.01, 100);
    projection = glm::make_mat4(proj_mat);


}

void MarkerDetector::update() {
    marker_map.clear();

    cv::Mat frame = stream->frame;
    auto markers = marker_detector.detect(frame, params, marker_size);

    for (auto mrkr:markers) {
        marker_map[mrkr.id] = mrkr;

        if (found_markers.find(mrkr.id) == found_markers.end()) {
            auto model = std::make_unique<AnimModel>(mrkr.id, state, this,
                                                     std::bind(change_to_pokedex, mrkr.id));
            state->add_element(std::move(model));
            found_markers.insert(mrkr.id);
        }
    }
}


bool MarkerDetector::get_marker(int id, aruco::Marker *marker) {
    auto searched = marker_map.find(id);
    if (searched != marker_map.end()) {
        *marker = marker_map[id];
        return true;
    }
    return false;
}
