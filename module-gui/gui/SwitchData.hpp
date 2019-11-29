/*
 * @file SwitchData.hpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 15 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#ifndef MODULE_APPS_SWITCHDATA_HPP_
#define MODULE_APPS_SWITCHDATA_HPP_

#include <string>

namespace gui {

inline const std::string special_input = "AppSpecialInput";
inline const std::string char_select = "CharSelect";

//class template that stores information that was sent along with switch message
class SwitchData {
protected:
	std::string description = "";
public:

	SwitchData() = default;
	SwitchData( const std::string& description ) : description{description} {};
	virtual ~SwitchData() = default;
	virtual const std::string& getDescription() const { return description; };
	virtual void setDescription( const std::string desc ) { description = desc; };
};

class SwitchSpecialChar : public SwitchData
{
    public:
        SwitchSpecialChar(const std::string &description) : SwitchData(description) {}
        virtual ~SwitchSpecialChar() = default;
};

} /* namespace gui */

#endif /* MODULE_APPS_SWITCHDATA_HPP_ */
