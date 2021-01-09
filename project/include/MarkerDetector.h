#ifndef PREP_POKEMON_RANCH_MARKERDETECTOR_H
#define PREP_POKEMON_RANCH_MARKERDETECTOR_H

#include <glm/glm.hpp>
#include <aruco/aruco.h>
#include <vector>

#include "Interfaces.h"


class GameState;
class VideoStream;

class MarkerDetector: public Updatable {
public:
    MarkerDetector() = default;
    MarkerDetector(VideoStream* stream, std::string path_to_calibraion_info, int width, int height, GameState* state);
    void update() override;


    bool get_marker(int id, aruco::Marker* marker);
    glm::mat4 projection;


private:
    VideoStream* stream;
    GameState* state;
    aruco::MarkerDetector marker_detector;
    aruco::CameraParameters params;
    std::map<int, aruco::Marker> marker_map;
    std::set<int> found_markers;

};


#endif //PREP_POKEMON_RANCH_MARKERDETECTOR_H
