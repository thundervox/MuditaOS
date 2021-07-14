// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <BasePresenter.hpp>
#include <ListItemProvider.hpp>

class TechnicalWindowContract
{
  public:
    class View
    {
      public:
        virtual ~View() noexcept = default;
    };
    class Presenter : public app::BasePresenter<TechnicalWindowContract::View>
    {
      public:
        virtual ~Presenter() noexcept = default;

        virtual std::shared_ptr<gui::ListItemProvider> getTechnicalInformationProvider() const = 0;
    };
};

class TechnicalWindowPresenter : public TechnicalWindowContract::Presenter
{
  public:
    explicit TechnicalWindowPresenter(std::shared_ptr<gui::ListItemProvider> technicalInformationProvider);

    std::shared_ptr<gui::ListItemProvider> getTechnicalInformationProvider() const override;

  private:
    std::shared_ptr<gui::ListItemProvider> technicalInformationProvider;
};
