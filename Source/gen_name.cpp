#include "gen_name.h"
#include <fstream>

using namespace std;

string CharacterName::grabRandomName() {
    ifstream fileOfNames;
    string tmpName;
    fileOfNames.open("./Settings/names.dat");
    if (fileOfNames.is_open()) {
        auto chosenSeed = randomNumber(1, 7732);
        for (auto i = 1; i < chosenSeed; i++) {
            fileOfNames.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        getline(fileOfNames, tmpName, '\n');
    } else {
        tmpName = "error with name file - check code";
    }
    return tmpName;
}
