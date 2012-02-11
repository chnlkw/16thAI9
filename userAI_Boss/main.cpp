#include "gameclient.h"

int main(int argc, char* argv[]) {
    GameClient gameClient(SERVER_ADDR, SERVER_PORT, BOSS, argv[1]);
    gameClient.run();
    return 0;
}
