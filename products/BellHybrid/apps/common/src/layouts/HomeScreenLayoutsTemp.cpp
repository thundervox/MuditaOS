// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md
#include "HomeScreenLayoutsBase.hpp"

#include <common/layouts/HomeScreenLayoutClassicWithTemp.hpp>

namespace gui::factory
{
    std::map<std::string, LayoutGenerator> getLayoutsFormat24h()
    {
        auto layouts = getLayoutsFormat24hBase();
        layouts.insert({"ClassicWithTemp", []() { return new HomeScreenLayoutClassicWithTemp("ClassicWithTemp"); }});
        return layouts;
    }

    std::map<std::string, LayoutGenerator> getLayoutsFormat12h()
    {
        auto layouts = getLayoutsFormat24hBase();
        layouts.insert({"ClassicWithTemp", []() { return new HomeScreenLayoutClassicWithTemp("ClassicWithTemp"); }});

        return layouts;
    }

} // namespace gui::factory
