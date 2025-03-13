#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <SDL3/SDL_log.h>

using json = nlohmann::json;

static json loadJson(const char *file_name)
{

    std::ifstream file(file_name);
    if (!file.is_open()){
        SDL_Log("[loadJson] %s dind't open", file_name);
        return nullptr;
    }

    json json_data;
    file >> json_data;
    file.close();

    SDL_Log("[loadJson] %s opened, size: %zu", file_name, json_data.size());
    return json_data;
}

static void saveJson(const char *file_name, json json_data)
{
    std::ofstream file(file_name);
    file << json_data.dump(4) << std::endl;
    file.close();
}
