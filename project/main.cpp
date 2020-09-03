#include <iostream>
#include "server/server.h"
int main() {
    Server s(2000);
    s.start();
    return 0;
}
