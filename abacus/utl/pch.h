#pragma once

#ifndef PCH_H
#define PCH_H

//vlk
#include "vulkan.h"

//sdl
#include "SDL.h"
#undef main
#include "SDL_vulkan.h"

//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_EXPERIMENTAL
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/hash.hpp>

//std
#include <stdio.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <cstdint> 
#include <limits> 
#include <algorithm> 
#include <fstream>
#include <array>
#include <unordered_map>
#include <chrono>

#endif //PCH_H
