module utils;

import std;

std::vector<std::string> ReadLines(const std::filesystem::path& filename)
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
    return lines;
}