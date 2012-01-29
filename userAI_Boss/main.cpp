#include "gameclient.h"

int main() {
    GameClient gameClient(SERVER_ADDR, SERVER_PORT, BOSS);
    gameClient.run();
    return 0;
}
