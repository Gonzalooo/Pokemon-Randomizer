#ifndef SV_TRAINERS_H
#define SV_TRAINERS_H

#include "sv_shared_class.h"

class svTrainers: public QObject, public SVShared{
    Q_OBJECT
    public:
        svTrainers();
        ~svTrainers();
        json trainersData;
        QList<int> seeds;
        bool checkRandomization(bool& paldea, bool& kita, bool& blueberry);
        void noSoftlockeParadise();
        void noSoftlockeTerapagos();
        int getMaxNumberOfChanges(json trainerEntry, int& level, bool NULLS=false);
        void randomizeTrainers(trainerSettings trainer);
        void randomize(QRandomGenerator* r, bool paldea, bool kitakami, bool blueberry, bool boss = false);

        bool paldeaForAll = false;
        QRandomGenerator* localRand;

        trainerSettings allTrainers;
        trainerSettings rivalTrainers;
        trainerSettings gymTrainers;
        trainerSettings e4Trainers;
        trainerSettings championTrainers;
        trainerSettings paradisePokemon;
        trainerSettings routeTrainers;
        trainerSettings raidTrainers;

        trainerSettings allKitakamiTrainers;
        trainerSettings kitakamiRivals;
        trainerSettings ogreClanTrainers;
        trainerSettings kitakamiRouteTrainers;
        trainerSettings kitakamiRaidTrainers;

        trainerSettings allBlueberryTrainers;
        trainerSettings blueberryRivals;
        trainerSettings bb4Trainers;
        trainerSettings hiddenTreasure;
        trainerSettings blueberryRouteTrainers;
        trainerSettings blueberryRaidTrainers;
        
        // Main Game
        QList<int> area1 = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14           
        };
        
        QList<int> area15 = {
            15, 246, 247, 248, 249, 250, 251, 252, 253, 254,
            255, 256, 257, 258, 259, 260, 261, 262, 263, 264,
            265, 266, 267, 268, 269, 270, 271
        };
        
        QList<int> area3 = {
            16, 17          
        };
        
        QList<int> area4 = {
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41          
        };
        
        QList<int> area5 = {
            42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
            54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64         
        };
        
        QList<int> area6 = {
            65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
            77, 78, 79, 80, 81, 82, 83, 84         
        };
        
        QList<int> area7 = {
            85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
            97, 98, 99, 100, 101, 102, 103, 104, 105        
        };
        
        QList<int> area8 = {
            106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
            116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
            126, 127, 128, 129         
        };
        
        QList<int> area9 = {
            130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
            150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
            160, 161, 162, 163, 164, 165        
        };
        
        QList<int> area10 = {
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
            176, 177, 178, 179, 180, 181, 182, 183       
        };
        
        QList<int> area11 = {
            184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
            194, 195, 196       
        };
        
        QList<int> area12 = {
            197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
            207, 208, 209, 210, 211      
        };
        
        QList<int> area13 = {
            212, 213, 214    
        };
        
        QList<int> area14 = {
            215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
            225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
            235, 236, 237, 238, 239, 240, 241, 242, 243, 244,
            245    
        };
        
        QList<int> area16 = {
            272, 273, 274, 275, 276, 277, 278, 279, 280, 281,
            282
        };

        QList<int> area17 = {
            283, 284, 285, 286, 287, 288, 289, 290, 291, 292,
            293, 294, 295, 296, 297, 298, 299, 300, 301, 302,
            303, 304, 305, 306, 307
        };

        QList<int> area18 = {
            308, 309, 310, 311, 312, 313, 314, 315, 316
        };

        QList<int> area19 = {
            317, 318, 319, 320, 321, 322, 323, 324, 325, 326,
            327, 328, 329
        };

        QList<int> area20 = {
            330, 331, 332, 333, 334, 335
        };

        QList<int> area21 = {
            336, 337, 338, 339, 340, 341, 342, 343, 344, 345,
            346
        };

        QList<int> area22 = {
            347, 348, 349, 350, 351, 352
        };

        QList<int> area24 = {
            353
        };

        QList<int> area25 = {
            354
        };

        QList<int> area26 = {
            355
        };

        QList<int> penny = {356, 357, 358, 677};
        QList<int> geeta = {359, 360, 361};
        QList<int> clavell_fire = {363, 366};
        QList<int> clavell_grass = {364, 367};
        QList<int> clavell_water = {365, 368};
        QList<int> starDark = {369, 370, 371}; // 370 -> max is 5
        QList<int> starPoison = {372, 373, 374}; // 373 -> max is 5
        QList<int> starFairy = {375, 376, 377}; // 375 -> max is 5
        QList<int> starFire = {378, 379, 380}; // 379 -> max is 5
        QList<int> starFight = {381, 382, 383}; // 382 -> max is 5
        QList<int> importantTrainers = {384, 385, 493, 494, 498, 502, 509, 513, 528};
        QList<int> e4Dragon = {386, 387};
        QList<int> e4Steel = {388};
        QList<int> e4Flying = {389};
        QList<int> e4Ground = {390};
        QList<int> electricGym = {391, 392, 393, 394, 395, 396};
        QList<int> psychicGym = {397, 398, 399, 400};
        QList<int> ghostGym = {401, 402, 403, 404, 405};
        QList<int> iceGym = {406, 407};
        QList<int> grassGym = {408, 409};
        QList<int> waterGym = {410, 411, 412};
        QList<int> bugGym = {413, 414, 415, 416, 417};
        QList<int> normalGym = {418, 419, 420, 421, 422};
        QList<int> dendra = {423, 424};
        QList<int> miriam = {425};
        QList<int> arven = {426, 427, 428, 429, 430, 701}; // 427 -> Mabostiff
        QList<int> arvenTitans = {431, 432, 433, 434, 435};
        QList<int> professors = {436, 438};
        QList<int> paradaiseProtocol = {437, 439};
        QList<int> paldeaRaids = {
            440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450,
            451, 452, 453, 454, 455
        };
        QList<int> kitakamiRaids = {
            456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466,
            467, 468, 469, 470, 471, 472, 473
        };
        QList<int> blueberryRaids = {
            474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484,
            485, 486, 487, 488, 489, 490, 491
        };
        QList<int> raifort = {492};
        QList<int> saguaro = {526};
        QList<int> salvatore = {527};
        QList<int> ryme = {747};
        QList<int> zinia = {754, 755};
        QList<int> nemona_fire = {495, 499, 503, 506, 510, 514, 517, 520, 523, 703};
        QList<int> nemona_grass = {496, 500, 504, 507, 511, 515, 518, 521, 524, 704};
        QList<int> nemona_water = {497, 501, 505, 508, 512, 516, 519, 522, 525, 705};

        // DLC1
        QList<int> area1_dlc1 = {529, 530, 531, 532, 533, 534, 535, 536};
        QList<int> area2_dlc1 = {537, 538, 539, 540, 541, 542, 543, 544, 545, 546};
        QList<int> area3_dlc1 = {547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560};
        QList<int> area4_dlc1 = {561, 562};
        QList<int> area7_dlc1 = {563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574};
        QList<int> area8_dlc1 = {575, 576};
        QList<int> area9_dlc1 = {577, 578};
        QList<int> area10_dlc1 = {579, 580, 581, 582, 583, 584};
        QList<int> kieran_dlc1 = {585, 587, 589, 591, 593};
        QList<int> carmine_dlc1 = {599, 601, 603, 605, 607};
        QList<int> kieran_stront_dlc1 = {586, 588, 590, 592, 594};
        QList<int> carmine_strong_dlc1 = {600, 602, 604, 606, 608};
        QList<int> ogreClan = {609, 610, 611, 612, 613, 614, 615};
        QList<int> ogreClanBoss = {616};
        QList<int> perrin = {595};
        QList<int> oNareFamily = {596, 597, 598};

        // DLC2 - ignore fight 711 to prevent softlock
        QList<int> area1_dlc2 = {};
        QList<int> area2_dlc2 = {};
        QList<int> area3_dlc2 = {};
        QList<int> area4_dlc2 = {};
        QList<int> schoolwars = {678, 682, 702, 706, 707, 708}; // 706/714 -> honoo, 707/715 -> kusa, 708/716 -> mizu
        QList<int> kieran = {679, 681};
        QList<int> bb4DragonTrainers = {684, 685, 686};
        QList<int> bb4Dragon = {683};
        QList<int> bb4FairyTrainers = {689, 690, 691, 692, 693};
        QList<int> bb4Farity = {687, 688};
        QList<int> epilogue = {694, 695, 745, 746};
        QList<int> bb4Steel = {696};
        QList<int> bb4FireTrainers = {698, 699, 700};
        QList<int> bb4Fire = {697};
        QList<int> terapagos = {680};
        QList<int> carmine = {710, 711};
        QList<int> shiano = {709};

        QList<int> raid_npc_index = {440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459,
                                     460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479,
                                     480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 751};
        QList<int> multi_battles_index = {358, 430, 431, 432, 433, 434, 435, 701, 523, 524, 525, 681, 682, 605, 606,
                                          607, 608, 711, 694, 695, 745, 746};

};

#endif // SV_TRAINERS_H
