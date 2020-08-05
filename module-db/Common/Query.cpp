#include "Query.hpp"

#include <memory>
#include <utility>

using namespace db;

Query::Query(Type type, QueryListener *listener) : type(type), listener(listener)
{}

QueryListener *Query::getQueryListener() const noexcept
{
    return listener;
}

void Query::setQueryListener(QueryListener *queryListener) noexcept
{
    listener = queryListener;
}

QueryResult::QueryResult(std::shared_ptr<Query> requestQuery) : requestQuery(std::move(requestQuery))
{}

void QueryResult::setRequestQuery(std::shared_ptr<Query> requestQueryToSet)
{
    requestQuery = requestQueryToSet;
}

std::shared_ptr<Query> QueryResult::getRequestQuery() const noexcept
{
    return requestQuery;
}

bool QueryResult::handle()
{
    if (requestQuery == nullptr || requestQuery->getQueryListener() == nullptr) {
        throw std::runtime_error("No listener to handle query");
    }

    return requestQuery->getQueryListener()->handleQueryResponse(this);
}
