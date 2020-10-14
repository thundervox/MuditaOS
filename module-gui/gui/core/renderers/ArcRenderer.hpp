#pragma once

#include "Color.hpp"
#include "Common.hpp"

#include "DrawCommand.hpp"

#include <module-utils/math/Math.hpp>

namespace gui
{
    class Context;
} // namespace gui

namespace gui::renderer
{
    class ArcRenderer
    {
      public:
        struct DrawableStyle
        {
            Length penWidth{1};
            Color color{ColorFullBlack};

            static auto from(const CommandArc &command) -> DrawableStyle;
        };

        static void draw(Context *ctx,
                         Point center,
                         Length radius,
                         trigonometry::Degrees begin,
                         trigonometry::Degrees sweep,
                         const DrawableStyle &style);

      private:
        static void draw(Context *ctx,
                         Point center,
                         Length radius,
                         trigonometry::Degrees begin,
                         trigonometry::Degrees sweep,
                         Color color);

        static void draw(Context *ctx,
                         Point center,
                         Length radius,
                         trigonometry::Degrees begin,
                         trigonometry::Degrees sweep,
                         Color color,
                         Length width);
    };
} // namespace gui::renderer
