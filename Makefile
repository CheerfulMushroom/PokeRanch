TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   project/include

SRCS = \
       project/src/main.cpp \
       project/src/GameObject.cpp \
       project/src/GameWindow.cpp \
       project/src/ShaderProgram.cpp \
       project/src/States.cpp \
       project/src/Button.cpp \
       project/src/ButtonFunctions.cpp \

.PHONY: all clean

all: $(SRCS)
	$(CXX) $(addprefix -I,$(HDRS)) -o $(TARGET) $(CXXFLAGS) $(SRCS) -lGLEW -lglfw  -lGL -lX11 -lpthread -lXrandr -lXi

clean:
	rm -rf $(TARGET)
