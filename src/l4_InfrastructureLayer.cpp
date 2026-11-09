#include <fstream>
#include "hw/l4_InfrastructureLayer.h"

bool FileStorage::saveLines(const std::string &filename, std::vector<std::string> lines)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    for (unsigned int i = 0; i < lines.size(); i++)
        file << lines[i] << std::endl;

    file.close();
    return true;
}

bool FileStorage::loadLines(const std::string &filename, std::vector<std::string> &lines)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    lines.clear();
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return true;
}
