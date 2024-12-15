module utils;

import std;

std::string ReadText(const std::filesystem::path& filename)
{
    std::string data;
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        data = std::move(ss).str();
    }
    return data;
}

std::vector<std::string> ReadLines(const std::filesystem::path& filename, bool remove_last_empty)
{
    std::vector<std::string> lines;
	std::ifstream file(filename);
    if (file.is_open())
    {
        while (file) {
            lines.emplace_back();
            std::getline(file, lines.back());
        }
    }
    if (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    return lines;
}

std::vector<std::vector<std::string>> ReadLineBlocks(const std::filesystem::path& filename)
{
    std::vector<std::vector<std::string>> out;
    for (auto block : ReadLines(filename) | stdv::split("")) {
        out.emplace_back().assign_range(block);
    }
    return out;
}