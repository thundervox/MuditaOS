// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <vector>
#include <string>
#include <type_traits>

/// Generic elements container.
/// By default, the container will choose the most optimal way of elements storage.
/// Fundamental types(ints, float, doubles, chars) are stored as simple variables. Compound types are stored in
/// std::vector.
/// Sometimes it is necessary to force the latter way of storing values even for fundamental types. For
/// instance, the user might want to store a fixed list of integers to iterate. In that case, set the force parameter to
/// true.
template <class ElementType, bool force = false, typename = void>
class Model
{
  public:
    enum class Boundaries
    {
        Fixed,     /// Stop scrolling upon reaching last/first element
        Continuous /// Jump to beginning/end upon reaching last/first element
    };
    using range      = std::vector<ElementType>;
    using value_type = ElementType;

    Model(range &&r, Boundaries boundaries) : elements{std::move(r)}, it{elements.begin()}, boundaries{boundaries}
    {}

    Model(const range &r, Boundaries boundaries) : elements{r}, it{elements.begin()}, boundaries{boundaries}
    {}

    auto get() const -> ElementType
    {
        return it == elements.end() ? ElementType{} : *it;
    }

    auto set(ElementType val) -> void
    {
        const auto e = std::find_if(elements.begin(), elements.end(), [&val](const auto &i) { return i == val; });
        if (e != elements.end()) {
            it = e;
        }
    }

    auto next() -> bool
    {
        bool ret{true};
        if (std::next(it) == elements.end()) {
            if (boundaries == Boundaries::Continuous) {
                it = elements.begin();
            }
            else {
                ret = false;
            }
        }
        else {
            it = std::next(it);
        }
        return ret;
    }

    auto previous() -> bool
    {
        bool ret{true};
        if (it == elements.begin()) {
            if (boundaries == Boundaries::Continuous) {
                it = std::prev(elements.end());
            }
            else {
                ret = false;
            }
        }
        else {
            it = std::prev(it);
        }
        return ret;
    }

    auto set_range(range newRange) -> void
    {
        if (elements != newRange) {
            elements = newRange;
            it       = elements.begin();
        }
    }

    [[nodiscard]] auto size() const -> std::size_t
    {
        return elements.size();
    }

    [[nodiscard]] auto is_min() const -> bool
    {
        return it == elements.begin();
    }

    [[nodiscard]] auto is_max() const -> bool
    {
        return std::next(it) == elements.end();
    }

    auto begin() const
    {
        return elements.begin();
    }

    auto end() const
    {
        return elements.end();
    }

  private:
    range elements;
    typename range::iterator it = elements.end();
    const Boundaries boundaries{};
};

template <typename ElementType, bool force>
class Model<ElementType, force, std::enable_if_t<std::is_fundamental_v<ElementType> and not force>>
{
    struct details
    {
        struct range
        {
            ElementType min{};
            ElementType max{};
            ElementType step{};

            constexpr auto operator!=(const range &oth) const -> bool
            {
                return min != oth.min || max != oth.max || step != oth.step;
            }
        };
    };

  public:
    enum class Boundaries
    {
        Fixed,
        Continuous
    };
    using range      = typename details::range;
    using value_type = ElementType;

    Model(range &&elements, Boundaries boundaries)
        : elements{std::move(elements)}, value{elements.min}, boundaries{boundaries}
    {}

    Model(range &elements, Boundaries boundaries) : elements{elements}, value{elements.min}, boundaries{boundaries}
    {}

    auto get() const -> ElementType
    {
        return value;
    }

    auto set(ElementType val) -> void
    {
        value = val;
    }

    auto next() -> bool
    {
        bool ret{true};
        if (value >= elements.max) {
            if (boundaries == Boundaries::Continuous) {
                value = elements.min;
            }
            else {
                value = elements.max;
                ret   = false;
            }
        }
        else {
            value += elements.step;
        }
        return ret;
    }

    auto previous() -> bool
    {
        bool ret{true};
        if (value <= elements.min) {
            if (boundaries == Boundaries::Continuous) {
                value = elements.max;
            }
            else {
                value = elements.min;
                ret   = false;
            }
        }
        else {
            value -= elements.step;
        }
        return ret;
    }

    auto set_range(range newRange) -> void
    {
        if (elements != newRange) {
            elements = newRange;
            value    = elements.min;
        }
    }

    [[nodiscard]] auto is_min() const -> bool
    {
        return value == elements.min;
    }

    [[nodiscard]] auto is_max() const -> bool
    {
        return value == elements.max;
    }

  private:
    range elements;
    ElementType value{};
    const Boundaries boundaries{};
};
