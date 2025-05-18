#ifndef SV_PERSONAL_H
#define SV_PERSONAL_H

#include "sv_shared_class.h"

class svPersonal: public QObject, SVShared{
    Q_OBJECT

    public:
        svPersonal();
        ~svPersonal();

        json personalMaps;
        json movesMaps;
        json sortedMoves;
        QRandomGenerator* localRand;

        bool randomizeAbilities = false;
        bool banWonderGuard = false; // connected
        bool banExitAbilities = false; // connected

        bool randomizeTypes = false; // connected
        bool grantExtraTypes = false; // connected

        bool randomizeMoveset = false; // connected
        bool setSameMovesAsType = false;

        bool randomizeBST = false; // connected
        bool keepSameBST = false; // connected

        bool randomizeEvolutions = false;
        bool evolveEveryLevel = false; // connected
        bool fixEvolutions = false; // connected

        bool randomizeTMs= false; // connected
        bool noAnimationTMs = false; // connected

        QList<int> bannedAbilities = {278, 307};
        QList<int> selectedTMs = {};
        QList<int> defaultTMs = {
            1, 7, 8, 9, 14, 19, 34, 36, 38, 46,
            53, 56, 57, 58, 59, 60, 63, 67, 76, 83,
            85, 86, 87, 89, 91, 92, 94, 97, 101, 109,
            113, 114, 115, 118, 126, 127, 129, 133, 141, 144,
            156, 157, 162, 164, 168, 174, 179, 180, 182, 184,
            188, 189, 191, 196, 200, 201, 202, 203, 204, 206,
            214, 220, 226, 227, 232, 240, 241, 242, 244, 246,
            247, 248, 250, 253, 257, 261, 263, 264, 268, 269,
            270, 271, 280, 282, 283, 285, 286, 297, 304, 307,
            308, 311, 313, 314, 315, 317, 319, 328, 330, 331,
            332, 333, 334, 337, 338, 339, 341, 342, 345, 347,
            349, 350, 352, 356, 360, 366, 369, 370, 374, 390,
            394, 396, 398, 399, 402, 403, 404, 405, 406, 408,
            409, 410, 411, 412, 413, 414, 416, 417, 419, 421,
            422, 423, 424, 428, 430, 433, 434, 437, 441, 442,
            444, 446, 447, 450, 451, 473, 474, 479, 482, 484,
            486, 488, 490, 491, 492, 500, 503, 506, 512, 513,
            518, 519, 520, 521, 522, 523, 525, 527, 528, 529,
            535, 542, 555, 566, 572, 574, 577, 580, 581, 583,
            598, 604, 605, 667, 669, 675, 676, 678, 679, 684,
            706, 707, 710, 776, 784, 791, 796, 797, 799, 800,
            802, 803, 806, 807, 808, 809, 811, 812, 813, 814,
            815, 851, 861, 883, 884, 885, 886, 912, 913, 914,
            915, 916, 917, 918
        };

        void randomize(QRandomGenerator* r);

    private:
        void randomizeStats();
        void randomizeAbility(json& pokemon, int species, int form, QRandomGenerator& rng);
        void randomizeType(json& pokemon, int species, int form, QRandomGenerator& rng);
        void randomizeMoves(json& pokemon, int type1, int type2, int species, int form, QRandomGenerator& rng);
        void randomizeBSTs(json& pokemon, int species, int form, QRandomGenerator& rng);
        void randomizeEvos(json& pokemon, QRandomGenerator& rng);
        void fixEvolution(json& pokemon, int species, int form);

        void addAllMoves();
        void randomizeTM(bool all = false);

};

#endif // SV_PERSONAL_H
