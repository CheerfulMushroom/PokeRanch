#ifndef PREP_POKEMON_RANCH_MARKERDETECTOR_H
#define PREP_POKEMON_RANCH_MARKERDETECTOR_H

#include <glm/glm.hpp>
#include <aruco/aruco.h>

#include "Interfaces.h"

class VideoStream;

class MarkerDetector: public Updatable {
public:
    MarkerDetector() = default;
    MarkerDetector(VideoStream* stream, std::string path_to_calibraion_info, int width, int height);
    void update() override;


    bool get_model_view(int id, glm::mat4* model);
    glm::mat4 projection;


private:
    VideoStream* stream;
    aruco::MarkerDetector marker_detector;
    aruco::CameraParameters params;
    std::map<int, glm::mat4> model_view_map;
};


#endif //PREP_POKEMON_RANCH_MARKERDETECTOR_H
