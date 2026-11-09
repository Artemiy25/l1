#ifndef HW_L2_APPLICATION_LAYER_H
#define HW_L2_APPLICATION_LAYER_H

#include <string>
#include <vector>
#include "hw/l3_DomainLayer.h"

class Application
{
public:
    Application(ShipRegistry &registry);

    std::string execute(std::vector<std::string> args);

private:
    std::string add(std::vector<std::string> args);
    std::string remove(std::vector<std::string> args);
    std::string update(std::vector<std::string> args);
    std::string view();
    std::string save(std::vector<std::string> args);
    std::string load(std::vector<std::string> args);

    ShipRegistry &_registry;
};

#endif
