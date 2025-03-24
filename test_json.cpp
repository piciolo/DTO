#include "json/json.h"
#include <iostream>

int main() {
    Json::Value root;
    root["message"] = "ciao";
    std::cout << root.toStyledString() << std::endl;
    return 0;
}
