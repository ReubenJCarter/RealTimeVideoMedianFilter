#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdint.h>
#include <math.h>
#include <random>
#include <stdlib.h> 

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.hpp>

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

}