//
// Created by souffle on 23-12-20.
//

#ifndef TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ARG_H_
#define TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ARG_H_
#include "cxxopts/cxxopts.h"
#include <iostream>

struct CommandArgs {
  std::optional<std::string> config_file;
};

//! cmd parse
//! \param argc
//! \param argv
//! \return
CommandArgs parse_command_args(int argc, char *argv[]) {
  cxxopts::Options options("program", "Description");
  options.add_options()
      ("h,help", "Print help")
      ("conf,config", "Config File", cxxopts::value<std::string>());
  options.allow_unrecognised_options();

  try {
    if (argc == 1) {
      std::cout << options.help() << "\n";
      exit(0);
    }

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << "\n";
      exit(0);
    }

    CommandArgs args;
    if (result.count("config")) {
      args.config_file = result["config"].as<std::string>();
      fprintf(stderr, "--config: %s\n", args.config_file.value().data());
    }
    return args;
  } catch (const cxxopts::exceptions::exception &e) {
    std::cerr << "Error parsing command-line options: " << e.what() << std::endl;
    std::cerr << options.help() << std::endl;
    exit(-1);
  }

}

#endif //TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ARG_H_
