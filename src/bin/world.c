#include <Ether.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  ether_system_startup();
  ether_wld_load("sample.wld");
  //ether_wld_load("chess.wld");

  ether_application_init();
  ether_system_request_refresh();

  ether_system_run();

  return 0;
}
