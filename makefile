TARGET = marker.out

HDRSCV = \
	/usr/local/include/opencv4/

HDRS = \
	./project/include/

LIBS = \
    /usr/local/lib/

MODULE = \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_videoio \
    -laruco

SRCS = \
    project/src/marker.cpp \
	project/src/pokemon_name.cpp

.PHONY: all clean

all: $(SRCS) 
	$(CXX) -std=c++17 -I $(HDRSCV) -I $(HDRS) -o $(TARGET) -L $(LIBS) $(SRCS) $(MODULE) -Wl,-rpath,/usr/local/lib/

clean:
	rm -rf $(TARGET)
