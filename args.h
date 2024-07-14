#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

class ArgsParser {
public:
    ArgsParser(int argc, char* argv[]) {
        parse(argc, argv);
    }

    std::vector<std::string> getVector() const {
        return argsVector;
    }

    std::string get(const std::string& key) const {
        if (argsMap.find(key) != argsMap.end()) {
            return argsMap.at(key);
        } else {
            throw std::runtime_error("Key not found: " + key);
        }
    }

    bool getBool(const std::string& key) const {
        return argsMap.find(key) != argsMap.end() && argsMap.at(key) == "true";
    }

private:
    std::vector<std::string> argsVector;
    std::unordered_map<std::string, std::string> argsMap;

    void parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            argsVector.push_back(arg);

            if (arg.rfind("--", 0) == 0) {
                std::string key = arg.substr(2);
                std::string value = "true";

                if ((i + 1) < argc && argv[i + 1][0] != '-') {
                    value = argv[++i];
                }

                argsMap[key] = value;
            }
        }
    }
};

#endif