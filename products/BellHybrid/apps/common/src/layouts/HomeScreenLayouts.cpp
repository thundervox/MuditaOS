// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md
#include "HomeScreenLayoutsBase.hpp"

namespace gui::factory
{
    std::map<std::string, LayoutGenerator> getLayoutsFormat24h()
    {
        return getLayoutsFormat24hBase();
    }

    std::map<std::string, LayoutGenerator> getLayoutsFormat12h()
    {
        return getLayoutsFormat12hBase();
    }

    std::map<std::string, LayoutGenerator> getAllLayouts()
    {
        return getLayoutsFormat12h();
    };

    std::string getDefaultLayout()
    {
        return "ClassicWithBattery";
    }
} // namespace gui::factory
