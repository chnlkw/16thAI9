#include "gameserver.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: platform.exe gui replay rounds time\n");
        printf("Gui:\n");
        printf("  0         No gui\n");
        printf("  1         Has gui\n");
        printf("Replay:\n");
        printf("  fileName  write replay to fileName\n");
        printf("Rounds:\n");
        printf("  num       set the total rounds to num\n");
        printf("Time:\n");
        printf("  time      set the time interval of every two rounds, counts in milliseconds\n");
    } else {
        int gui = atoi(argv[1]);
        char* replay = argv[2];
        int rounds = atoi(argv[3]);
        int time = atoi(argv[4]);
        GameServer gameServer(gui, replay, rounds, time);
        gameServer.run();
    }
    return 0;
}
