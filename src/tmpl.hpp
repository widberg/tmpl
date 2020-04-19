#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::string> macro_map_t;

int replace_directory(const std::filesystem::path& template_path, const std::filesystem::path& output_path, macro_map_t& macros);
int replace_stream(std::istream& template_path, std::ostream& output_path, const macro_map_t& macros);
std::string replace_filename(const std::string& template_string, macro_map_t& macros);
