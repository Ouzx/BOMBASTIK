#include <pitches.h>

// W330#4\nW165#4\nW330#4\nW294#3\n

int melody[][2] = {
    // 1st line
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 2nd line Dolap: Start
    {NOTE_E4, 2},
    {NOTE_B4, 4},
    {NOTE_A4, 4},
    {NOTE_B4, 2},
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 3rd line
    {NOTE_E4, 2},
    {0, 3},
    {NOTE_A4, 4},
    {NOTE_G4, 4},
    {NOTE_A4, 2},
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 4th line
    {NOTE_E4, 2},
    {0, 3},
    {NOTE_G4, 4},
    {NOTE_FS4, 4},
    {NOTE_G4, 2},
    {NOTE_E4, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 5th line First Dolap: End
    {NOTE_E4, 2},
    {0, 3},
    {NOTE_B5, 4},
    {NOTE_E4, 2},
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},
    {NOTE_E4, 2},
    {NOTE_B4, 4},
    {NOTE_A4, 4},
    {NOTE_B4, 2},
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 3rd line
    {NOTE_E4, 2},
    {0, 3},
    {NOTE_A4, 4},
    {NOTE_G4, 4},
    {NOTE_A4, 2},
    {NOTE_E4, 4},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 4th line
    {NOTE_E4, 2},
    {0, 3},
    {NOTE_G4, 4},
    {NOTE_FS4, 4},
    {NOTE_G4, 2},
    {NOTE_E4, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // 6th line Second Dolap: End
    {NOTE_E4, 3},
    {NOTE_B4, 4},
    {NOTE_G4, 4},
    {NOTE_G4, 4},
    {NOTE_FS4, 4},
    {NOTE_D5, 4},
    {NOTE_B4, 3},
    {NOTE_FS5, 4},
    {NOTE_D5, 3},
    {NOTE_A5, 4},
    {NOTE_FS5, 3},
    {NOTE_B5, 3},

    // 7th line
    {0, 2},
    {NOTE_E4, 3},
    {NOTE_B4, 3},
    {NOTE_B4, 4},
    {NOTE_C4, 4},
    {NOTE_B4, 4},
    {NOTE_A4, 4},
    {NOTE_B4, 2},

    // 8th line
    {NOTE_A4, 4},
    {NOTE_G4, 4},
    {NOTE_A4, 3},
    {NOTE_A4, 3},
    {NOTE_G4, 3},
    {NOTE_A4, 3},
    {NOTE_B4, 3},
    {NOTE_B4, 4},
    {NOTE_A4, 3},

    // 9th line
    {NOTE_G4, 4},
    {NOTE_FS4, 4},
    {NOTE_G4, 3},
    {NOTE_G4, 3},
    {NOTE_FS4, 3},
    {NOTE_G4, 4},
    {NOTE_A4, 4},
    {NOTE_G4, 4},
    {NOTE_FS4, 4},
    {NOTE_G4, 2},

    // 10th line
    {NOTE_FS4, 4},
    {NOTE_E4, 4},
    {NOTE_FS4, 3},
    {NOTE_FS4, 3},
    {NOTE_E4, 4},
    {NOTE_FS4, 4},
    {NOTE_G4, 3},
    {NOTE_E3, 4},
    {NOTE_E4, 4},
    {NOTE_D4, 3},

    // // 11th line
    // {NOTE_E4, 1},
    // {NOTE_E3, 16},
    // {NOTE_E4, 16},
    // {NOTE_D4, 8},

    // // 12th line
    // {NOTE_E4, 3},
    // {NOTE_E3, 16},
    // {NOTE_E4, 16},
    // {NOTE_D4, 8},

};

int rows = sizeof(melody) / sizeof(melody[0]);
