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
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

#endif //PCH_H
