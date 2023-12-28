#pragma once

#include "beatsaber-hook/shared/utils/utils.h"

//#define LOG_INFO(value...)
#define LOG_INFO(value...) getLogger().info(value)
//#define LOG_DEBUG(value...)
#define LOG_DEBUG(value...) getLogger().debug(value)
//#define LOG_ERROR(value...)
#define LOG_ERROR(value...) getLogger().error(value)

Logger& getLogger();
