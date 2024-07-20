#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <httplib.h>
#include <regex>
#include "args.h"

using namespace std;
using json = nlohmann::json;

string repo = "http://files.deema.help";
string version = "0.0.3";

string str_replace(string source, string find, string replace) {
    size_t pos = 0;

    while ((pos = source.find(find, pos)) != string::npos) {
         source.replace(pos, find.length(), replace);
         pos += replace.length();
    }

    return source;
}

void help() {
    cout << "Deema CLI for dc-api-core" << endl << endl;

    cout << "deema gen/g controller/c <ControllerName> [--ws]" << endl;
    cout << "deema gen/g validator/v <ValidatorName>" << endl;
    cout << "deema gen/g project/p <ProjectName> [--typescript, --ts, --here, --install]" << endl;
    cout << "deema serve" << endl << endl;

    cout << "GitHub: https://github.com/mayerdev/deema" << endl;
    cout << "Docs: https://deema.help" << endl << endl;

    cout << "Version: v" << version << endl;
}

int main(int argc, char* argv[]) {
    ArgsParser args(argc, argv);

    vector<string> vec = args.getVector();
    if(vec.size() < 1) {
        help();
        return 0;
    }

    if(vec[0] == "gen" || vec[0] == "g") {
        ifstream cfg_file("config.json");

        bool load_config = false;

        if(vec[1] != "project" && vec[1] != "p") {
            load_config = true;

            if(!cfg_file.good()) {
                cout << "Error: config.json not found" << endl;
                return 0;
            }
        }

        json config;

        if(load_config) config = json::parse(cfg_file);
        httplib::Client http(repo);

        if(vec[1] == "controller" || vec[1] == "c") {
            if(vec.size() < 3) {
                help();
                return 0;
            }

            string controller_name = vec[2];
            string ext = ".js";

            if(config["typescript"].is_boolean() && config["typescript"].get<bool>()) ext = ".ts";

            if(!filesystem::exists("controllers")) filesystem::create_directory("controllers");

            if(filesystem::exists("controllers/" + vec[2] + ext)) {
                cout << "Error: Controller already exists" << endl;
                return 0;
            }

            string remote_name = "controller";
            if(args.getBool("ws")) remote_name = "controller.ws";

            auto res = http.Get("/" + remote_name + ext);
            string result = str_replace(res->body, "$ControllerName", controller_name);
            
            ofstream controller("controllers/" + vec[2] + ext);
            controller << result;
            controller.close();

            cout << "Created controller: " << vec[2] + ext << endl;
        } else if(vec[1] == "validator" || vec[1] == "v") {
            if(vec.size() < 3) {
                help();
                return 0;
            }

            if(!config["typescript"].is_boolean() || !config["typescript"].get<bool>()) {
                cout << "Error: Validators requires TypeScript" << endl;
                return 0;
            }

            string validator_name = vec[2];

            if(!filesystem::exists("validators")) filesystem::create_directory("validators");

            if(filesystem::exists("validators/" + vec[2] + ".ts")) {
                cout << "Error: Validator already exists" << endl;
                return 0;
            }

            auto res = http.Get("/validator.ts");
            string result = str_replace(res->body, "$ValidatorName", validator_name);
            
            ofstream validator("validators/" + vec[2] + ".ts");
            validator << result;
            validator.close();

            cout << "Created validator: " << vec[2] + ".ts" << endl;
        } else if(vec[1] == "project" || vec[1] == "p") {
            if(vec.size() < 3) {
                help();
                return 0;
            }

            string project_name = vec[2];

            if(filesystem::exists(vec[2])) {
                cout << "Error: Project already exists" << endl;
                return 0;
            }


            string prefix = "";

            if(args.getBool("--here")) {
                if(filesystem::exists("./controllers") || filesystem::exists("./validators") || filesystem::exists("./package.json")) {
                    cout << "Error: Project already exists" << endl;
                    return 0;
                }

                prefix = "./";
            } else {
                filesystem::create_directory(vec[2]);
                prefix = "./" + vec[2] + "/";
            }

            auto package = http.Get("/project/package.json");
            string package_json = str_replace(package->body, "$ProjectName", project_name);

            ofstream package_file(prefix + "/package.json");
            package_file << package_json;
            package_file.close();

            auto config = http.Get("/project/config.json");
            string config_json = str_replace(config->body, "$ProjectName", project_name);

            if(args.getBool("typescript") || args.getBool("ts")) config_json = str_replace(config_json, "$typescript", "true");
            else config_json = str_replace(config_json, "$typescript", "false");

            ofstream config_file(prefix + "/config.json");
            config_file << config_json << endl;
            config_file.close();

            filesystem::create_directory(prefix + "/controllers");
            filesystem::create_directory(prefix + "/validators");
            filesystem::create_directory(prefix + "/models");
            filesystem::create_directory(prefix + "/models/mongo");

            if(args.getBool("install")) {
                string cmd = "cd " + prefix + " && npm install yarn -g && yarn";
                cout << cmd << endl;
                system(cmd.c_str());
            }

            cout << "Created project: " << vec[2] << endl;
        }
    } else if(vec[0] == "serve") {
        cout << "Starting dev server..." << endl;

        // todo: check if node > version 14 && node < version 18 exists
        // todo: check if node_modules is installed

        string v_cmd = "node --version";
        
        auto result = system(v_cmd.c_str());

        regex r("v(1[5-9])\\.");
        smatch match;
        if (regex_search(result, match, r)) {
            cout << "Node version: " << match[1] << endl;
        } else {
            cout << "ERROR: Node version not supported" << endl;
            return 1;
        }

        if (filesystem::exists("node_modules") != 0) {
            cout << "ERROR: node_modules not installed" << endl;
            return 1;
        }

        string cmd = "node node_modules/.bin/dc-api-core --dev";
        system(cmd.c_str());
    } else help();

    return 0;
}
