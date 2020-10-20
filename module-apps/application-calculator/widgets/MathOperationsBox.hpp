// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once
#include "module-apps/Application.hpp"
#include <module-gui/gui/widgets/Item.hpp>
#include <module-gui/gui/widgets/GridLayout.hpp>

namespace gui
{
    class MathOperationsBox : public GridLayout
    {
        void fillInTheGrid();

      public:
        MathOperationsBox(
            gui::Item *parent, int offsetTop, uint32_t width, uint32_t height, uint32_t cellWidth, uint32_t cellHeight);

        ~MathOperationsBox() override = default;
    };
} // namespace gui
