#include "gameserver.h"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Usage: platform.exe gui webkernel replay rounds time\n");
        printf("Gui:\n");
        printf("  0         no gui\n");
        printf("  1         has gui\n");
        printf("Webkernel:\n");
        printf("  0         not web kernel version\n");
        printf("  1         web kernel version\n");
        printf("Replay:\n");
        printf("  fileName  write replay to fileName\n");
        printf("Rounds:\n");
        printf("  num       set the total rounds to num\n");
        printf("Time:\n");
        printf("  time      set the time interval of every two rounds, counts in milliseconds\n");
    } else {
        int gui = atoi(argv[1]);
        int web = atoi(argv[2]);
        char* replay = argv[3];
        int rounds = atoi(argv[4]);
        int time = atoi(argv[5]);
        GameServer gameServer(gui, web, replay, rounds, time);
        gameServer.run();
    }
    return 0;
}
