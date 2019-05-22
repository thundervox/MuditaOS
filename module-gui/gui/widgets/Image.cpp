/*
 * Image.cpp
 *
 *  Created on: 17 mar 2019
 *      Author: robert
 */

#include "../core/DrawCommand.hpp"
#include "../core/PixMapManager.hpp"
#include "../core/PixMap.hpp"
#include "Image.hpp"
#include "utf8/UTF8.hpp"

namespace gui {

Image::Image() : Rect(), pixMap { nullptr } {
	type = ItemType::IMAGE;
}

Image::Image( Item* parent, uint32_t x, uint32_t y, uint32_t w, uint32_t h, const UTF8 imageName ) :
	Rect( parent, x, y, w, h ),
	pixMap { nullptr }{
	type = ItemType::IMAGE;

	setPosition(x, y);
	updateDrawArea();
}

Image::~Image() {
}


bool Image::setImageWithID( int id ) {

	//get pixmap for selected ID
	pixMap = PixMapManager::getInstance().getPixMap( id );

	//set height and width and max and min dimensions
	uint16_t pixMapWidth = pixMap->getWidth();
	uint16_t pixMapHeight = pixMap->getHeight();

	minWidth = maxWidth = widgetArea.w = pixMapWidth;
	minHeight = maxHeight = widgetArea.h = pixMapHeight;

	return true;
}

std::list<DrawCommand*> Image::buildDrawList() {

	std::list<DrawCommand*> commands;

	//check if widget is visible
	if( visible == false )
		return commands;

	//get children draw commands
	std::list<DrawCommand*> childrenCommands = Item::buildDrawList();
	if( !childrenCommands.empty() )
		commands.merge( childrenCommands );

	//set local draw commands
	CommandImage* img = new CommandImage{};

	//image
	img->x = drawArea.x;
	img->y = drawArea.y;
	img->w = drawArea.w;
	img->h = drawArea.h;

	//cmd part
	img->areaX = img->x;
	img->areaY = img->y;
	img->areaW = img->w;
	img->areaH = img->h;

	img->imageID = this->pixMap->getID();

	commands.push_back( img );

	return commands;
}

} /* namespace gui */
