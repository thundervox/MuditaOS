﻿// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "DatabaseInitializer.hpp"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <array>
#include <log/log.hpp>

DatabaseInitializer::DatabaseInitializer(Database *db) : db(db)
{}

bool DatabaseInitializer::run(std::filesystem::path path, std::string ext)
{
    // Database name is database file path, need to strip off all filesystem related stuff(path, extension)
    std::filesystem::path dbpath = db->getName();
    std::string dbname           = dbpath.filename().replace_extension();

    for (int i = 1;; i++) {
        auto fname = std::make_unique<std::stringstream>();
        (*fname) << dbname << "_" << std::setfill('0') << std::setw(3) << i << '.' << ext;
        auto file = path / fname->str();
        LOG_DEBUG("Running db script: %s", file.c_str());
        auto commands = readCommands(file);
        if (commands.empty())
            break;
        if (!executeOnDb(commands)) {
            LOG_ERROR("Can't initialize database [%s] with [%s]", db->getName().c_str(), file.c_str());
            return false;
        }
    }
    return true;
}

std::string DatabaseInitializer::readContent(const char *filename) const noexcept
{
    std::unique_ptr<char[]> fcontent;
    long fsize = 0;

    auto fp = std::fopen(filename, "r");
    if (fp) {
        std::fseek(fp, 0, SEEK_END);
        fsize = std::ftell(fp);
        std::rewind(fp);

        fcontent = std::make_unique<char[]>(fsize + 1);

        std::fread(fcontent.get(), 1, fsize, fp);

        std::fclose(fp);
    }
    else
        return {};

    return std::string(fcontent.get());
}

std::vector<std::string> DatabaseInitializer::readCommands(std::filesystem::path filePath)
{
    auto fileContent = readContent(filePath.c_str());
    if (fileContent.empty())
        return {};

    std::string currentStatement{};
    std::vector<std::string> statements{};

    std::string line{};
    for (auto &c : fileContent) {
        if (c != '\n') {
            line += c;
        }
        else {
            if (line.empty() || starts_with(line, std::string("--"))) {
                line.clear();
                continue;
            }
            if (ends_with(line, std::string(";"))) {
                statements.push_back(currentStatement + line);
                currentStatement.clear();
                line.clear();
                continue;
            }
            currentStatement += line;

            line.clear();
        }
    }
    return statements;
}

std::array<std::string, 3> DatabaseInitializer::splitFilename(std::string filename)
{
    auto name    = filename.substr(0, filename.find("."));
    auto prefix  = name.substr(0, name.find_last_of("_"));
    auto postfix = name.substr(name.find_last_of("_") + 1, std::string::npos);

    return {name, prefix, postfix};
}

std::vector<std::filesystem::path> DatabaseInitializer::listFiles(std::filesystem::path path,
                                                                  std::string prefix,
                                                                  std::string ext)
{
    std::set<std::pair<int, std::filesystem::path>> orderedFiles;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (!entry.is_directory() && entry.path().has_filename()) {
            try {
                auto parts      = splitFilename(entry.path().filename().string());
                auto filePrefix = parts[1];
                if (filePrefix == prefix) {
                    auto num = std::stoi(parts[2]);
                    orderedFiles.insert({num, entry.path()});
                }
            }
            catch (std::invalid_argument &e) {
                LOG_INFO("Ignoring file: %s", entry.path().c_str());
            }
        }
    }

    std::vector<std::filesystem::path> files;
    std::for_each(orderedFiles.begin(), orderedFiles.end(), [&](auto item) { files.push_back(item.second); });
    return files;
}

bool DatabaseInitializer::executeOnDb(const std::vector<std::string> statements)
{
    for (auto st : statements) {
        if (!db->execute(st.c_str())) {
            return false;
        }
    }
    return true;
}
