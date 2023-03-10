TARGET  ?= $(shell $(CXX) -dumpmachine)

CXXFLAGS = -O0 -g -Wall -Wno-unused-variable -Wno-macro-redefined 
CPPFLAGS = -Iexternals/include -Iexternals/include/MyMath -Iinclude -MMD
LDFLAGS  = -Lexternals/libs-$(TARGET)

# CYGWIN SPECIFICS
ifeq ($(TARGET), x86_64-pc-cygwin)
LDLIBS = -lglfw3 -lgdi32 -lglu32
BIN    =  Rasterizer
externals/src/imgui_impl_opengl3.o: CXXFLAGS+="-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM"

# CLANG SPECIFICS
else ifeq ($(CXX),clang)
BIN         =  Rasterizer.exe
CPPVERSION  = -std=c++17
CXXFLAGS   += -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH=1 -nostdlib
LDLIBS      = -lglfw3 -lglu32 -luser32 -lshell32 -lgdi32
LDFLAGS     = -Lexternals/libs-vc2019

# LINUX SPECIFICS
else ifneq (,$(filter x86_64%linux-gnu,$(TARGET)))
LDLIBS = -lglfw3 -lm -ldl -lX11 -lssl -lpthread -lGLU
BIN    =  Rasterizer

endif

# PROGRAM OBJS
OBJS = src/main.o src/App.o src/Camera.o src/Framebuffer.o src/Renderer.o src/Scene.o src/Light.o src/Texture.o src/ShapeManager.o

# GLAD
OBJS += externals/src/gl.o
# IMGUI
OBJS += externals/src/imgui.o externals/src/imgui_demo.o externals/src/imgui_draw.o externals/src/imgui_widgets.o externals/src/imgui_tables.o
# IMGUI backends
OBJS += externals/src/imgui_impl_glfw.o externals/src/imgui_impl_opengl3.o
# MYMATH
OBJS   += externals/include/MyMath/my_math.o



DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(BIN)

-include $(DEPS)

%.o: %.cpp
	$(CXX) $(CPPVERSION) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(BIN) $(OBJS) $(DEPS) imgui.ini && clear