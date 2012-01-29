#include "gameclient.h"

int main() {
    GameClient gameClient(SERVER_ADDR, SERVER_PORT, PLANE);
    gameClient.run();
    return 0;
}

