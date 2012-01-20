#include "gameclient.h"
#include <iostream>

using namespace std;

int main() {
    GameClient gc("127.0.0.1", 80);
    cout << "ok" << endl;
    gc.run();
    return 0;
}
