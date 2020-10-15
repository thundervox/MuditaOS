#pragma once

#include <list>
#include "Rect.hpp"
#include "BoxLayout.hpp"
#include "Span.hpp"
#include "ListItemProvider.hpp"
#include "Style.hpp"

namespace gui
{
    class ListItemProvider;

    class ListViewScroll : public Rect
    {
      public:
        ListViewScroll(Item *parent, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

        bool shouldShowScroll(unsigned int listPageSize, unsigned int elementsCount);
        void update(unsigned int startIndex, unsigned int listPageSize, unsigned int elementsCount, int topMargin);
    };

    class ListView : public Rect
    {
      protected:
        unsigned int startIndex                    = 0;
        unsigned int storedFocusIndex              = 0;
        unsigned int elementsCount                 = 1;
        std::shared_ptr<ListItemProvider> provider = nullptr;
        VBox *body                                 = nullptr;
        ListViewScroll *scroll                     = nullptr;
        std::list<std::pair<style::listview::RebuildType, unsigned int>> rebuildRequests;

        unsigned int currentPageSize = 0;
        bool pageLoaded              = true;
        bool focusOnLastItem         = false;
        int scrollTopMargin          = style::margins::big;

        style::listview::Boundaries boundaries   = style::listview::Boundaries::Fixed;
        style::listview::Direction direction     = style::listview::Direction::Bottom;
        style::listview::Orientation orientation = style::listview::Orientation::TopBottom;

        void clearItems();
        virtual void addItemsOnPage();
        void setFocus();
        void refresh();
        void resizeWithScroll();
        void recalculateStartIndex();
        void fillFirstPage();
        void setStartIndex();
        void recalculateOnBoxRequestedResize();
        unsigned int calculateMaxItemsOnPage();
        unsigned int calculateLimit(style::listview::Direction value = style::listview::Direction::Bottom);
        Order getOrderFromDirection();
        virtual bool requestNextPage();
        virtual bool requestPreviousPage();
        void setup(style::listview::RebuildType rebuildType, unsigned int dataOffset = 0);

      public:
        ListView();
        ListView(Item *parent, uint32_t x, uint32_t y, uint32_t w, uint32_t h, std::shared_ptr<ListItemProvider> prov);
        ~ListView();

        void setElementsCount(unsigned int count);
        void setProvider(std::shared_ptr<ListItemProvider> provider);
        void rebuildList(style::listview::RebuildType rebuildType = style::listview::RebuildType::Full,
                         unsigned int dataOffset                  = 0,
                         bool forceRebuild                        = false);
        void clear();
        std::shared_ptr<ListItemProvider> getProvider();
        void setOrientation(style::listview::Orientation value);
        void setBoundaries(style::listview::Boundaries value);
        void setScrollTopMargin(int value);
        void setAlignment(const Alignment &value) override;
        void onProviderDataUpdate();

        // virtual methods from Item
        std::list<DrawCommand *> buildDrawList() override;
        bool onInput(const InputEvent &inputEvent) override;
        bool onDimensionChanged(const BoundingBox &oldDim, const BoundingBox &newDim) override;
        auto handleRequestResize(const Item *, unsigned short request_w, unsigned short request_h) -> Size override;
    };

} /* namespace gui */
