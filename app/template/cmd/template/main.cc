//
// Created by souffle on 23-12-15.
//

#include "arg.h"
#include "environment.h"

int main(int argc, char *argv[]) {
  auto cmd = parse_command_args(argc, argv);
  Environment::NewBootConf(cmd.config_file.value());

  Environment::NewTrace(BootStrapLoad::GetInstance().experiment.trace);
  Environment::NewRegister(RegistryLoad::GetInstance().consul,
                           BootStrapLoad::GetInstance().app,
                           BootStrapLoad::GetInstance().server.http);

  auto Srv = Environment::NewServer();
  Srv->Start();
}