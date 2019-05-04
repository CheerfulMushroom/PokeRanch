TARGET = open_cv_test.out

HDRS = \
    /usr/local/include/opencv4/

LIBS = \
    /usr/local/lib/

MODULE = \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_videoio

SRCS = \
    ./open_cv_test.cpp

.PHONY: all clean

all: $(SRCS) 
	$(CXX) -std=c++17 -I $(HDRS) -o $(TARGET) -L $(LIBS) $(SRCS) $(MODULE) -Wl,-rpath,/usr/local/lib/

clean:
	rm -rf $(TARGET)
