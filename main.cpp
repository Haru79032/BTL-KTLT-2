
#include "eniesLobby.h"

using namespace std;

int main(int argc, const char * argv[]) {
   	
    string inputFile = "sample_input.txt";
    EniesLobbyBattle battle(inputFile);
    battle.runBattle();
    cout << battle.getResult() << endl;
}
