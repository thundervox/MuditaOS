/*
 * @file ImageMessage.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 28 maj 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#include "EinkMessage.hpp"
#include "ImageMessage.hpp"

namespace seink {

ImageMessage::ImageMessage( uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t* data) :
	EinkMessage{ seink::MessageType::ImageData }, x{x}, y{y}, w{w}, h{h}, data{ data } {
}

ImageMessage::~ImageMessage() {
}

} /* namespace seink */
