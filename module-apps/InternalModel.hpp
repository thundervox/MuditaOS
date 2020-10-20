// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <module-gui/gui/widgets/ListItemProvider.hpp>
#include <vector>

#include "Application.hpp"

namespace app
{

    template <class T> class InternalModel
    {

      protected:
        int modelIndex              = 0;
        unsigned int internalOffset = 0;
        unsigned int internalLimit  = 0;
        std::vector<T> internalData;

        virtual ~InternalModel()
        {
            eraseInternalData();
        }

        void eraseInternalData()
        {
            for (auto item : internalData) {
                delete item;
            }
            internalData.clear();
        }

        void setupModel(const uint32_t offset, const uint32_t limit)
        {
            modelIndex     = 0;
            internalOffset = offset;
            internalLimit  = limit;
        }

        gui::ListItem *getRecord(gui::Order order)
        {
            unsigned int index = 0;
            if (order == gui::Order::Previous) {
                index = internalOffset + internalLimit - 1 - modelIndex;
            }
            if (order == gui::Order::Next) {
                index = internalOffset + modelIndex;
            }

            if (isValidIndex(index, order)) {

                return getNextInternalDataElement(index);
            }
            else {

                return nullptr;
            }
        }

        [[nodiscard]] bool isValidIndex(unsigned int index, gui::Order order) const
        {
            return (index < internalData.size()) || (order == gui::Order::Previous && index < internalOffset);
        }

        void clearItemProperties(T Item)
        {
            Item->setFocus(false);
            Item->setVisible(true);
            Item->clearNavigationItem(gui::NavigationDirection::UP);
            Item->clearNavigationItem(gui::NavigationDirection::DOWN);
        }

        gui::ListItem *getNextInternalDataElement(unsigned int index)
        {
            modelIndex++;
            clearItemProperties(internalData[index]);
            return internalData[index];
        }
    };

} /* namespace app */
