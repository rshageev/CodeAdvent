export module utils:File;

import std;

export
{
	std::string ReadText(const std::filesystem::path& filename);
	std::vector<std::string> ReadLines(const std::filesystem::path& filename, bool remove_last_empty = true);
	std::vector<std::vector<std::string>> ReadLineBlocks(const std::filesystem::path& filename);
}