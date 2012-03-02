#include "gameserver.h"

int main(int argc, char* argv[]) {
    GameServer gameServer(argc - 1);
    gameServer.run();
    return 0;
}
