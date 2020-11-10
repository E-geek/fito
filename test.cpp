#include <iostream>
#include <cstdlib>
#include <stdio.h>
using namespace std;

int main()  {
  int addr[8];
  sscanf("09:b8:11:01:00:a3:ff:ae", "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", addr + 0, addr + 1, addr + 2, addr + 3, addr + 4, addr + 5, addr + 6, addr + 7);
  for (int i = 0; i < 8; i++) {
    printf("%i:", addr[i]);
  }
  return 0;
}