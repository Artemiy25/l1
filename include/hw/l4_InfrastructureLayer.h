#ifndef HW_L4_INFRASTRUCTURE_LAYER_H
#define HW_L4_INFRASTRUCTURE_LAYER_H

#include <string>
#include <vector>

// работа с файлом, про суда ничего не знает
class FileStorage
{
public:
    bool saveLines(const std::string &filename, std::vector<std::string> lines);
    bool loadLines(const std::string &filename, std::vector<std::string> &lines);
};

#endif
