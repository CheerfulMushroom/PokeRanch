TARGET = test.out

HDRS_CV = \
    /usr/local/include/opencv4/
HDRS_GL = \
    /usr/local/include
HDRS = \
    project/include/

LIBS = \
    /usr/local/lib/


MODULES = \
    -lGLEW \
    -lGL \
    -lglfw3 \
    -lX11 \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_videoio \
    -laruco \
    -lassimp

SRCS = \
    ./project/src/GameWindow.cpp \
    ./project/src/ShaderProgram.cpp \
    ./project/src/model_main.cpp \
    ./project/src/Mesh.cpp \
    ./project/src/Model.cpp

all: $(SRCS)
	$(CXX) -std=c++17 -I $(HDRS_CV) -I $(HDRS) -I $(HDRS_GL) -o $(TARGET) -L $(LIBS) $(SRCS) $(MODULES) -pthread -ldl -Wl,-rpath,/usr/local/lib/

clean:
	rm -rf $(TARGET)
