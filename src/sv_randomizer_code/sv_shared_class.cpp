#include "headers/sv_randomizer_headers/sv_shared_class.h"

json cleanSceneJSON;

/*
    Constructor that defines the necessary variables for SV Randomizer that are used
    Globally
 */
SVShared::SVShared(){
    pokemonMapping = readJsonQFile("SV_FLATBUFFERS/pokemon_mapping.json");
    pokemonPersonalData = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/personal_array_clean.json");
    qBaseDir = QDir(QDir::current().absoluteFilePath("SV_FLATBUFFERS"));
    maxGeneration = 9;
    maxAllowedId = 1026; // 1025 + 1 (egg counts as a pokemon so it needs to be index 0)
    allowedMoves = {
                    1, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31, 32, 33,
                    34, 35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
                    62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89,
                    90, 91, 92, 93, 94, 95, 97, 98, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 113, 114,
                    115, 116, 118, 120, 122, 123, 124, 126, 127, 129, 133, 135, 136, 137, 138, 139, 141, 143, 144, 147,
                    150, 151, 152, 153, 154, 156, 157, 160, 161, 162, 163, 164, 166, 167, 168, 172, 173, 174, 175,
                    176, 177, 178, 179, 180, 181, 182, 183, 184, 186, 187, 188, 189, 191, 192, 194, 195, 196, 197, 198,
                    199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 217, 219, 220,
                    221, 223, 224, 225, 226, 227, 229, 230, 231, 232, 234, 235, 236, 238, 239, 240, 241, 242, 243, 244,
                    245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 259, 260, 261, 262, 263, 264, 266,
                    268, 269, 270, 271, 272, 273, 275, 276, 278, 280, 281, 282, 283, 284, 285, 286, 291, 292, 294, 295,
                    296, 297, 298, 299, 303, 304, 305, 306, 307, 308, 309, 310, 311, 313, 314, 315, 317, 319, 321, 322,
                    323, 325, 326, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344,
                    345, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 359, 360, 361, 362, 364, 365, 366, 367, 368,
                    369, 370, 371, 372, 374, 379, 380, 383, 384, 385, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396,
                    397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416,
                    417, 418, 419, 420, 421, 422, 423, 424, 425, 427, 428, 430, 432, 433, 434, 435, 436, 437, 438, 439,
                    440, 441, 442, 444, 446, 447, 449, 450, 451, 452, 453, 454, 455, 457, 458, 459, 460, 461, 462, 463, 464, 467,
                    465, 468, 469, 470, 471, 472, 473, 474, 476, 478, 479, 482, 483, 484, 486, 487, 488, 489, 490, 491, 492,
                    493, 494, 495, 496, 497, 499, 500, 501, 502, 503, 504, 505, 506, 508, 509, 510, 511, 512, 513, 514,
                    515, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 532, 533, 534, 535, 538, 539,
                    540, 541, 542, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 562, 564, 565,
                    566, 568, 570, 571, 572, 573, 574, 575, 576, 577, 580, 581, 583, 584, 585, 586, 587, 589, 590, 591,
                    592, 593, 594, 595, 596, 597, 598, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 618, 619, 620,
                    621, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 672, 675, 676, 678, 679, 680, 681,
                    682, 683, 684, 686, 688, 689, 690, 691, 692, 693, 694, 705, 706, 707, 709, 710, 711, 713, 714, 715,
                    716, 722, 744, 745, 746, 747, 748, 749, 750, 751, 752, 756, 775, 776, 777, 778, 780, 781, 782, 783,
                    784, 785, 786, 787, 788, 789, 790, 791, 793, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806,
                    807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826,
                    827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846,
                    847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866,
                    867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886,
                    887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906,
                    907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919
    };
    bannedItems = {
                   2311, 2313, 2314, 2315, 2316, 2317, 2318, 2319, 2320, 2321, 2322, 2323, 2324, 2325, 2326, 2327, 2329,
                   2330, 2331, 2332, 2333, 2334, 2335, 2336, 2337, 2338, 2339, 2340, 2341, 2342, 2348, 2349, 2350, 2351,
                   2352, 2353, 2354, 2355, 2356, 2357, 2358, 2359, 2360, 2361, 2362, 2363, 2364, 2365, 2366, 2367, 2368,
                   2369, 2370, 2371, 2372, 2373, 2374, 2375, 2376, 2377, 2378, 2379, 2380, 2381, 2382, 2383, 2384, 2385,
                   2386, 2387, 2388, 2389, 2390, 2391, 2392, 2393, 2394, 2395, 2396, 1857, 1858, 1889, 1890, 1891, 1892,
                   1893, 1894, 1895, 1896, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1909, 1910, 1911, 1912, 1913, 1914,
                   1915, 1916, 1917, 1918, 1919, 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927, 1928, 1929, 1930, 1931,
                   1932, 1933, 1934, 1935, 1936, 1937, 1938, 1939, 1940, 1941, 1942, 1943, 1944, 1945, 1946, 2417, 2418,
                   2419, 2420, 2421, 2422, 2423, 2424, 2425, 2426, 2427, 2428, 2429, 2430, 2431, 2432, 2433, 2434, 2435,
                   2436, 2437, 2551, 2552
    };
    pokeballNames = {
                     "Poke Ball", "Great Ball", "Ultra Ball", "Master Ball", "Beast Ball",
                     "Cherish Ball", "Luxury Ball", "Timer Ball", "Net Ball", "Nest Ball", "Dive Ball", "Dusk Ball",
                     "Repeat Ball", "Premier Ball", "Heal Ball", "Quick Ball", "Fast Ball", "Level Ball", "Lure Ball",
                     "Heavy Ball","Love Ball","Friend Ball","Moon Ball","Sport Ball","Safari Ball","Dream Ball"
    };
    bannedPokemon = {
        {3, {1}}, {6, {1, 2}}, {9, {1}}, {10, {0}}, {11, {0}}, {12, {0}}, {13, {0}}, {14, {0}}, {15, {0, 1}}, {16, {0}},
        {17, {0}}, {18, {0, 1}}, {19, {0, 1}}, {20, {0, 1}}, {21, {0}}, {22, {0}}, {25, {8}}, {29, {0}}, {30, {0}}, {31, {0}},
        {32, {0}}, {33, {0}}, {34, {0}}, {41, {0}}, {42, {0}}, {46, {0}}, {47, {0}}, {63, {0}}, {64, {0}}, {65, {0, 1}},
        {66, {0}}, {67, {0}}, {68, {0}}, {77, {0, 1}}, {78, {0, 1}}, {80, {1}}, {83, {0, 1}}, {94, {1}}, {95, {0}}, {98, {0}},
        {99, {0}}, {104, {0}}, {105, {0, 1}}, {108, {0}}, {114, {0}}, {115, {0, 1}}, {118, {0}}, {119, {0}}, {120, {0}}, {121, {0}},
        {122, {0, 1}}, {124, {0}}, {127, {0, 1}}, {130, {1}}, {133, {1}}, {138, {0}}, {139, {0}}, {140, {0}}, {141, {0}}, {142, {0, 1}},
        {150, {1, 2}}, {165, {0}}, {166, {0}}, {169, {0}}, {175, {0}}, {176, {0}}, {177, {0}}, {178, {0}}, {181, {1}}, {201, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27}},
        {202, {0}}, {208, {0, 1}}, {212, {1}}, {213, {0}}, {214, {1}}, {222, {0, 1}}, {223, {0}}, {224, {0}}, {226, {0}}, {229, {1}},
        {238, {0}}, {241, {0}}, {248, {1}}, {251, {0}}, {254, {1}}, {257, {1}}, {260, {1}}, {263, {0, 1}}, {264, {0, 1}}, {265, {0}},
        {266, {0}}, {267, {0}}, {268, {0}}, {269, {0}}, {276, {0}}, {277, {0}}, {282, {1}}, {290, {0}}, {291, {0}}, {292, {0}},
        {293, {0}}, {294, {0}}, {295, {0}}, {300, {0}}, {301, {0}}, {302, {1}}, {303, {0, 1}}, {304, {0}}, {305, {0}}, {306, {0, 1}},
        {308, {1}}, {309, {0}}, {310, {0, 1}}, {315, {0}}, {318, {0}}, {319, {0, 1}}, {320, {0}}, {321, {0}}, {323, {1}}, {327, {0}},
        {334, {1}}, {337, {0}}, {338, {0}}, {343, {0}}, {344, {0}}, {345, {0}}, {346, {0}}, {347, {0}}, {348, {0}}, {351, {0, 1, 2, 3}},
        {352, {0}}, {354, {1}}, {359, {0, 1}}, {360, {0}}, {362, {1}}, {363, {0}}, {364, {0}}, {365, {0}}, {366, {0}}, {367, {0}},
        {368, {0}}, {369, {0}}, {373, {1}}, {376, {1}}, {380, {1}}, {381, {1}}, {382, {1}}, {383, {1}}, {384, {1}}, {399, {0}},
        {400, {0}}, {406, {0}}, {407, {0}}, {412, {0, 1, 2}}, {413, {0, 1, 2}}, {414, {0, 1, 2}}, {420, {0}}, {421, {0, 1}}, {427, {0}}, {428, {0, 1}},
        {431, {0}}, {432, {0}}, {439, {0}}, {441, {0}}, {445, {1}}, {448, {1}}, {451, {0}}, {452, {0}}, {455, {0}}, {458, {0}},
        {460, {1}}, {463, {0}}, {465, {0}}, {468, {0}}, {475, {1}}, {494, {0}}, {504, {0}}, {505, {0}}, {506, {0}}, {507, {0}},
        {508, {0}}, {509, {0}}, {510, {0}}, {511, {0}}, {512, {0}}, {513, {0}}, {514, {0}}, {515, {0}}, {516, {0}}, {517, {0}},
        {518, {0}}, {519, {0}}, {520, {0}}, {521, {0}}, {524, {0}}, {525, {0}}, {526, {0}}, {527, {0}}, {528, {0}}, {531, {0, 1}},
        {535, {0}}, {536, {0}}, {537, {0}}, {538, {0}}, {539, {0}}, {543, {0}}, {544, {0}}, {545, {0}}, {554, {0, 1}}, {555, {0, 1, 2, 3}},
        {556, {0}}, {557, {0}}, {558, {0}}, {561, {0}}, {562, {0, 1}}, {563, {0}}, {564, {0}}, {565, {0}}, {566, {0}}, {567, {0}},
        {568, {0}}, {569, {0}}, {582, {0}}, {583, {0}}, {584, {0}}, {587, {0}}, {588, {0}}, {589, {0}}, {592, {0}}, {593, {0}},
        {597, {0}}, {598, {0}}, {599, {0}}, {600, {0}}, {601, {0}}, {605, {0}}, {606, {0}}, {616, {0}}, {617, {0}}, {618, {0, 1}},
        {621, {0}}, {626, {0}}, {631, {0}}, {632, {0}}, {649, {0, 1, 2, 3, 4}}, {658, {2}}, {659, {0}}, {660, {0}}, {670, {5}}, {674, {0}},
        {675, {0}}, {676, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}, {679, {0}}, {680, {0}}, {681, {0, 1}}, {682, {0}}, {683, {0}}, {684, {0}}, {685, {0}}, {688, {0}},
        {689, {0}}, {694, {0}}, {695, {0}}, {696, {0}}, {697, {0}}, {698, {0}}, {699, {0}}, {710, {0, 1, 2, 3}}, {711, {0, 1, 2, 3}}, {716, {0, 1}},
        {717, {0}}, {718, {0, 1, 2, 3, 4}}, {719, {1}}, {746, {0, 1}}, {755, {0}}, {756, {0}}, {759, {0}}, {760, {0}}, {767, {0}}, {768, {0}},
        {771, {0}}, {772, {0}}, {773, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}}, {776, {0}}, {777, {0}}, {780, {0}}, {781, {0}}, {785, {0}}, {786, {0}}, {787, {0}},
        {788, {0}}, {793, {0}}, {794, {0}}, {795, {0}}, {796, {0}}, {797, {0}}, {798, {0}}, {799, {0}}, {800, {3}}, {802, {0}},
        {803, {0}}, {804, {0}}, {805, {0}}, {806, {0}}, {807, {0}}, {808, {0}}, {809, {0}}, {824, {0}}, {825, {0}}, {826, {0}},
        {827, {0}}, {828, {0}}, {829, {0}}, {830, {0}}, {831, {0}}, {832, {0}}, {835, {0}}, {836, {0}}, {850, {0}}, {851, {0}},
        {852, {0}}, {853, {0}}, {862, {0}}, {864, {0}}, {865, {0}}, {866, {0}}, {867, {0}}, {880, {0}}, {881, {0}}, {882, {0}},
        {883, {0}}, {890, {1}}, {1007, {1,2,3,4}}, {1008, {1,2,3,4}}, {1017, {4,5,6,7}}, {1024, {2}}
    };
}

SVShared::~SVShared(){

}

int SVShared::generateBinary(const std::string& schema, const std::string& jsonFile, const std::string& path, bool debug) {
    // Determine the platform and architecture
    // Determine the platform and architecture
    std::string platform;
    std::string architecture;

    #ifdef _WIN32
        platform = "Windows";
    #ifdef _WIN64
        architecture = "Win64";
    #else
        architecture = "Win32";
    #endif
    #elif __APPLE__
        platform = "macOS";
    #ifdef __aarch64__
        architecture = "ARM";
    #else
        architecture = "Intel";
    #endif
    #elif __linux__
        platform = "Linux";
    #ifdef __clang__
        architecture = "Clang";
    #else
        architecture = "GCC";
    #endif
    #else
        platform = "Unknown";
        architecture = "Unknown";
    #endif

    // Print detected platform and architecture if debug is enabled
    qDebug() << QString("Platform %1 with Architecture %2").arg(QString::fromStdString(platform), QString::fromStdString(architecture));

    // Determine the binary path based on platform and architecture
    QString flatc;
    if (platform == "Windows") {
        flatc = QDir::toNativeSeparators("thirdparty/flatbuffers/windows/flatc.exe");
    } else if (platform == "macOS") {
        flatc = (architecture == "ARM")
        ? QDir::toNativeSeparators("thirdparty/flatbuffers/mac_ant/flatc")
        : QDir::toNativeSeparators("thirdparty/flatbuffers/mac_intel/flatc");
    } else if (platform == "Linux") {
        flatc = (architecture == "Clang")
        ? QDir::toNativeSeparators("thirdparty/flatbuffers/linux_clang/flatc")
        : QDir::toNativeSeparators("thirdparty/flatbuffers/linux_gcc/flatc");
    } else {
        throw std::runtime_error("Unsupported platform or architecture");
    }

    // Create the folder hierarchy using QDir
    QDir outputDir(QStringLiteral("output/romfs/%1/").arg(QString::fromStdString(path)));
    if (!outputDir.exists()) {
        if (!outputDir.mkpath(".")) {
            qCritical() << "Failed to create directory hierarchy.";
            return -1;
        }
    }

    // Prepare the output path
    QString outpath = QDir::toNativeSeparators(outputDir.absolutePath());

    // Use QProcess to execute the command
    QProcess process;
    QString program = flatc;

    // Prepare the arguments
    QStringList arguments;
    arguments << "-b"
              << "-o" << outpath
              << QString::fromStdString(schema)
              << QString::fromStdString(jsonFile)
              << "--no-warnings";

    // Debug the command
    if (debug) {
        qDebug() << QString("Executing: %1 with Arguments: %2").arg(program, arguments.join(' '));
    }

    // Start the process
    process.start(program, arguments);
    process.waitForFinished();

    // Check the process result
    int returnCode = process.exitCode();
    if (debug) {
        if (returnCode != 0) {
            qCritical() << "Error executing command. Return code: " << returnCode;
            qCritical() << "Error output: " << process.readAllStandardError();
        } else {
            qDebug() << "Program Ended Successfully";
        }
    }

    return returnCode;
}

uint64_t SVShared::fnv1a_64(const std::string& str, uint64_t basis) {
    uint64_t hash = basis;
    for (char c : str) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 0x100000001B3;
    }
    return hash;
}

void SVShared::patchFileDescriptor() {
    // Load the JSON file
    json data = readJsonQFile("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data_clean.json");

    // Define the prefix and modPath
    QDir modDir("output/romfs");
    QString prefix = QDir::currentPath() + "/output/romfs/";

    // Use QDirIterator for recursive iteration
    QDirIterator it(modDir.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo(it.filePath());
        if (fileInfo.isFile()) {
            QString name = fileInfo.absoluteFilePath();
            name.replace("\\", "/"); // Replace backslashes with forward slashes

            // Ensure prefix length is correct
            if (name.startsWith(prefix)) {
                name = name.mid(prefix.length()); // Remove the prefix
            } else {
                qWarning() << "Unexpected path format: " << name;
                continue;
            }

            // Compute the hash
            uint64_t hash = fnv1a_64(name.toStdString());

            // Check if the hash exists in data["file_hashes"]
            auto& file_hashes = data["file_hashes"];
            auto it = std::find(file_hashes.begin(), file_hashes.end(), hash);
            if (it != file_hashes.end()) {
                size_t index = std::distance(file_hashes.begin(), it);
                file_hashes.erase(it); // Remove hash
                data["files"].erase(data["files"].begin() + index); // Remove corresponding file entry
            }
        }
    }

    // Write the modified data back to a JSON file
    QFile outfile("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data.json");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qFatal() << "Error: Could not open data.json for writing!";
        return;
    }

    QTextStream out(&outfile);
    out << QString::fromStdString(data.dump(2)); // Write JSON with pretty format
    outfile.close();

    qDebug() << "Patched trpfd!";
}


int modelCount = 0;
int fieldCount = 0;

void SVShared::recursiveFindOfPokemonSceneTable(json& sceneFile, QVector<int> devId, QVector<int> formId, QVector<int> gender, QVector<bool> rare){
    if (!sceneFile.is_array()) {
        return;
    }
    for(unsigned long long i =0; i<sceneFile.size(); i++){
        if(sceneFile[i]["Type"] == "ti_PokemonModelComponent" || sceneFile[i]["Type"] == "ti_FieldPokemonComponent"){
            qDebug()<<"Type: "<< QString::fromUtf8(sceneFile[i]["Type"].get<std::string>().c_str());

            const auto& dataArray = sceneFile[i]["Data"];
            std::vector<uint8_t> data;
            for (const auto& byteValue : dataArray) {
                if (byteValue.is_number_unsigned()) {
                    data.push_back(static_cast<uint8_t>(byteValue.get<uint32_t>()));
                } else {
                    qFatal() << "Invalid byte value in Data array.\n";
                }
            }

            if(sceneFile[i]["Type"] == "ti_PokemonModelComponent"){
                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                qDebug() << "TIPokemonModelComponent:";
                printSceneTables(originalTable);

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[modelCount];
                uint16_t form_id = formId[modelCount];
                int8_t sex_num = gender[modelCount];
                int8_t rare_bool = rare[modelCount];
                modelCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIPokemonModelComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneFile[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                qDebug() << "TIPokemonModelComponent (Updated)";
                printSceneTables(modifiedTale);


            }else if(sceneFile[i]["Type"] == "ti_FieldPokemonComponent"){

                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent:\n";
                printSceneTables(originalTable);

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[fieldCount];
                uint16_t form_id = formId[fieldCount];
                int8_t sex_num = gender[fieldCount];
                int8_t rare_bool = rare[fieldCount];
                fieldCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIFieldPokemonComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneFile[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent (Updated):";
                printSceneTables(modifiedTale);
            }
        }
        recursiveFindOfPokemonSceneTable(sceneFile[i]["SubObjects"], devId, formId, gender, rare);
    }
}
void SVShared::modifyPokemonScene(QVector<int> devId, QVector<int> formId, QVector<int> gender, QVector<bool> rare, QString input, QString output){

    QString binaryFilePath = input;
    QString outputFilePath = output;

    modelCount = 0;
    fieldCount = 0;

    cleanSceneJSON = readJsonQFile(binaryFilePath);

    qDebug()<<QString::fromUtf8(cleanSceneJSON["ObjectTemplateName"].get<std::string>().c_str());

    recursiveFindOfPokemonSceneTable(cleanSceneJSON["Objects"], devId, formId, gender, rare);

    modelCount = 0;
    fieldCount = 0;

    // Write the modified JSON back to the output file
    QFile outFile(outputFilePath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qFatal() << "Error: Could not open output file for writing!";
        return;
    }

    QTextStream outStream(&outFile);
    outStream << QString::fromStdString(cleanSceneJSON.dump(2)); // Pretty print JSON
    outFile.close();
}

/*
 * This function returns the specific tera type a pokemon need; otherwise it will return
 * a value to get it randomnly on child object (to use the seed they have)
 *
 * True if there is a list of pokemon
 * False if all for the generation are banned
 */
bool SVShared::getAllowedPokemon(allowedPokemonLimiter limiter, QMap<int, QList<int>>& allowedList, QString check){
    int totalRemoved = 0;

    for(int i = 0; i<maxGeneration; i++){
        if(limiter.gens[i] == false){
            totalRemoved++;
            switch(i){
                case 0:
                    allowedList = removeValuesFromQMap(allowedList, gen1);
                    break;
                case 1:
                    allowedList = removeValuesFromQMap(allowedList, gen2);
                    break;
                case 2:
                    allowedList = removeValuesFromQMap(allowedList, gen3);
                    break;
                case 3:
                    allowedList = removeValuesFromQMap(allowedList, gen4);
                    break;
                case 4:
                    allowedList = removeValuesFromQMap(allowedList, gen5);
                    break;
                case 5:
                    allowedList = removeValuesFromQMap(allowedList, gen6);
                    break;
                case 6:
                    allowedList = removeValuesFromQMap(allowedList, gen7);
                    break;
                case 7:
                    allowedList = removeValuesFromQMap(allowedList, gen8);
                    break;
                case 8:
                    allowedList = removeValuesFromQMap(allowedList, gen9);
                    break;
            }
        }
        else{
            switch(i){
                case 0:
                    allowedList = mergeQMaps(allowedList, gen1, bannedPokemon);
                    break;
                case 1:
                    allowedList = mergeQMaps(allowedList, gen2, bannedPokemon);
                    break;
                case 2:
                    allowedList = mergeQMaps(allowedList, gen3, bannedPokemon);
                    break;
                case 3:
                    allowedList = mergeQMaps(allowedList, gen4, bannedPokemon);
                    break;
                case 4:
                    allowedList = mergeQMaps(allowedList, gen5, bannedPokemon);
                    break;
                case 5:
                    allowedList = mergeQMaps(allowedList, gen6, bannedPokemon);
                    break;
                case 6:
                    allowedList = mergeQMaps(allowedList, gen7, bannedPokemon);
                    break;
                case 7:
                    allowedList = mergeQMaps(allowedList, gen8, bannedPokemon);
                    break;
                case 8:
                    allowedList = mergeQMaps(allowedList, gen9, bannedPokemon);
                    break;
            }
        }

        if(limiter.genLegends[i] == false){
            totalRemoved++;
            switch(i){
                case 0:
                    allowedList = removeValuesFromQMap(allowedList, gen1_legends);
                    break;
                case 1:
                    allowedList = removeValuesFromQMap(allowedList, gen2_legends);
                    break;
                case 2:
                    allowedList = removeValuesFromQMap(allowedList, gen3_legends);
                    break;
                case 3:
                    allowedList = removeValuesFromQMap(allowedList, gen4_legends);
                    break;
                case 4:
                    allowedList = removeValuesFromQMap(allowedList, gen5_legends);
                    break;
                case 5:
                    allowedList = removeValuesFromQMap(allowedList, gen6_legends);
                    break;
                case 6:
                    allowedList = removeValuesFromQMap(allowedList, gen7_legends);
                    break;
                case 7:
                    allowedList = removeValuesFromQMap(allowedList, gen8_legends);
                    break;
                case 8:
                    allowedList = removeValuesFromQMap(allowedList, gen9_legends);
                    break;
            }
        }
        else{
            switch(i){
                case 0:
                    allowedList = mergeQMaps(allowedList, gen1_legends, bannedPokemon);
                    break;
                case 1:
                    allowedList = mergeQMaps(allowedList, gen2_legends, bannedPokemon);
                    break;
                case 2:
                    allowedList = mergeQMaps(allowedList, gen3_legends, bannedPokemon);
                    break;
                case 3:
                    allowedList = mergeQMaps(allowedList, gen4_legends, bannedPokemon);
                    break;
                case 4:
                    allowedList = mergeQMaps(allowedList, gen5_legends, bannedPokemon);
                    break;
                case 5:
                    allowedList = mergeQMaps(allowedList, gen6_legends, bannedPokemon);
                    break;
                case 6:
                    allowedList = mergeQMaps(allowedList, gen7_legends, bannedPokemon);
                    break;
                case 7:
                    allowedList = mergeQMaps(allowedList, gen8_legends, bannedPokemon);
                    break;
                case 8:
                    allowedList = mergeQMaps(allowedList, gen9_legends, bannedPokemon);
                    break;
            }
        }
    }

    allowedList = removeValuesFromQMap(allowedList, megas);
    allowedList = removeValuesFromQMap(allowedList, UB);

    if(limiter.stage1 == false){
        totalRemoved++;
        allowedList = removeValuesFromQMap(allowedList, stage1);
    }else{
        allowedList = mergeQMaps(allowedList, stage1, bannedPokemon);
    }

    if(limiter.stage2 == false){
        totalRemoved++;
        allowedList = removeValuesFromQMap(allowedList, stage2);
    }else{
        allowedList = mergeQMaps(allowedList, stage2, bannedPokemon);
    }

    if(limiter.stage3 == false){
        totalRemoved++;
        allowedList = removeValuesFromQMap(allowedList, stage3);
    }else{
        allowedList = mergeQMaps(allowedList, stage3, bannedPokemon);
    }

    if(limiter.singleStage == false){
        totalRemoved++;
        allowedList = removeValuesFromQMap(allowedList, singleStage);
    }else{
        allowedList = mergeQMaps(allowedList, singleStage, bannedPokemon);
    }

    if(limiter.paradox == false){
        totalRemoved++;
        allowedList = removeValuesFromQMap(allowedList, paradox);
    }else{
        allowedList = mergeQMaps(allowedList, paradox, bannedPokemon);
    }

    if(totalRemoved == 23){
        // Set up emit message for check
        return false;
    }

    return true;
}

std::string SVShared::getTeraType(){
    return teraTypes[localRand->bounded(0, teraTypes.length())].toUpper().toStdString();
}

int SVShared::getTeraTypeInt(std::string teraType){
    if(teraType == "DEFAULT"){
        return -1;
    }

    for(int i =0; i<teraTypes.length(); i++){
        if(teraTypes[i].toUpper().toStdString()==teraType){
            return i;
        }
    }

    return 0;
}

void SVShared::randomizePokemon(QMap<int, QList<int>>& allowedPokemon, int& devid, int& form, int& gender, bool& rare, int shinyrate, json& changedFile){
    std::string genderStd;
    if (!localRand) {
        qFatal("localRand is null - randomize!");
    }
    qDebug()<<"Starting Randomizing Of Pokemon";
    int random = localRand->bounded(1, maxAllowedId);
    // Checks if this is an allowed Pokemon
    while(!allowedPokemon.contains(pokemonMapping["pokemons"][random]["natdex"]))
        random = localRand->bounded(1, maxAllowedId);
    qDebug()<<"Selected Random Id: "<<std::string(pokemonMapping["pokemons"][random]["name"]) << "("<<random<<")";

    // Checks if its an allowed pokemon form
    int formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
    while(!allowedPokemon[random].contains(formRandom)){
        formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
    }
    qDebug()<<"Selected Form: "<< form;

    // Save devid and not nationaldex to correctly change scene
    devid = int(pokemonMapping["pokemons"][random]["devid"]);
    form = formRandom;

    // Set Pokemon DevId - changedFile["values"][index]["pokeData"]["devId"]
    changedFile["devId"] = pokemonMapping["pokemons"][random]["devName"];

    // Set Pokemon Form
    changedFile["formId"] = formRandom;

    // Set Pokemon Gender - gets the pokemon name in Utf8 format and not ASCII
    QString form_Check = QString::fromUtf8(pokemonMapping["pokemons"][random]["name"].get<std::string>().c_str());
    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){
        gender = 0;
        if(genderlessPokemon.contains(form_Check)){
            genderStd = "DEFAULT";
        }else{
            genderStd = "MALE";
        }
    }else if(femaleOnlyPokemon.contains(form_Check)){
        gender = 1;
        genderStd = "FEMALE";
    } else if(genderForms.contains(form_Check)){
        if(form == 0){
            gender = 0;
            genderStd = "MALE";
        }else{
            gender = 1;
            genderStd = "FEMALE";
        }
    }else{
        int rand_gender = localRand->bounded(0, 100);
        if(rand_gender > int(pokemonPersonalData["entry"][int(pokemonMapping["pokemons"][random]["devid"])]["gender"]["ratio"])){
            gender = 0;
            genderStd = "MALE";
        }else{
            gender = 1;
            genderStd = "FEMALE";
        }
    }
    changedFile["sex"] = genderStd;
    qDebug()<<"Selected gender: "<< genderStd;

    //Shiny Pokemon Randomizing
    if(changedFile["rareType"] != "RARE"){
        int chance = localRand->bounded(0, 100);
        if(chance < shinyrate){
            changedFile["rareType"] = "RARE";
            rare = true;
        } else{
            changedFile["rareType"] = "NO_RARE";
            rare = false;
        }
    }else{
        changedFile["rareType"] = "RARE";
        rare = true;
    }
    qDebug()<<"Selected rare: "<< rare;

}

void SVShared::randomizePokemonMoves(){

}

void SVShared::patchScenes(const QList<QPair<QString, QString>>& jsonFile, QList<QString> outputs,
                           QVector<int> devId, QVector<int> formId, QVector<int> gender, QVector<bool> rare){

    for(int i =0; i<jsonFile.length(); i++){
        for(int j = 0; j<=1; j++){
            // Initial step is to get the actual scene json
            // extensions is the same length as the rests
            QString schema = jsonFile[i].first;
            QString binaryScene = QStringLiteral("%1_%2_clean.%3").arg(jsonFile[i].second).arg(j).arg(schema.section('/', -1).section('.', 0, 0));
            QString dirPath = QFileInfo(binaryScene).absolutePath() + "/";
            generateJson(jsonFile[i].first.toStdString(),
                         binaryScene.toStdString(),
                         dirPath.toStdString(), true);

            // First step is to traverse through the files and modify them
            // Note: jsonFile and outputs will always be the same length
            QString cleanScene = QStringLiteral("%1_%2_clean.json").arg(jsonFile[i].second).arg(j);
            QString output = QStringLiteral("%1_%2.json").arg(jsonFile[i].second).arg(j);

            modifyPokemonScene(devId, formId, gender, rare, cleanScene, output);

            // Second step is to generate the binary
            generateBinary(schema.toStdString(),
                            output.toStdString(),
                            outputs[i].toStdString(), true);

            // Lastly, we will delete the scene file and its clean json
            QDir().remove(output);
            QDir().remove(cleanScene);
        }
    }
 }

void SVShared::patchOneScene(const QList<QPair<QString, QString>>& jsonFile, QList<QString> outputs,
                           QVector<int> devId, QVector<int> formId, QVector<int> gender, QVector<bool> rare, QString fileNum){

    for(int i =0; i<jsonFile.length(); i++){
        // Initial step is to get the actual scene json
        // extensions is the same length as the rests
        QString schema = jsonFile[i].first;
        QString binaryScene = QStringLiteral("%1_%2_clean.%3").arg(jsonFile[i].second).arg(fileNum).arg(schema.section('/', -1).section('.', 0, 0));
        QString dirPath = QFileInfo(binaryScene).absolutePath() + "/";
        generateJson(jsonFile[i].first.toStdString(),
                     binaryScene.toStdString(),
                     dirPath.toStdString(), true);

        // First step is to traverse through the files and modify them
        // Note: jsonFile and outputs will always be the same length
        QString cleanScene = QStringLiteral("%1_%2_clean.json").arg(jsonFile[i].second).arg(fileNum);
        QString output = QStringLiteral("%1_%2.json").arg(jsonFile[i].second).arg(fileNum);

        modifyPokemonScene(devId, formId, gender, rare, cleanScene, output);

        // Second step is to generate the binary
        generateBinary(schema.toStdString(),
                        output.toStdString(),
                       outputs[i].toStdString(), true);

        // Lastly, we will delete the scene file and its clean json
        QDir().remove(output);
        QDir().remove(cleanScene);
    }
}

int SVShared::generateJson(const std::string& schema, const std::string& inputFile, const std::string& path, bool debug) {
    // Determine the platform and architecture
    // Determine the platform and architecture
    std::string platform;
    std::string architecture;

    #ifdef _WIN32
        platform = "Windows";
    #ifdef _WIN64
        architecture = "Win64";
    #else
        architecture = "Win32";
    #endif
    #elif __APPLE__
        platform = "macOS";
    #ifdef __aarch64__
        architecture = "ARM";
    #else
        architecture = "Intel";
    #endif
    #elif __linux__
        platform = "Linux";
    #ifdef __clang__
        architecture = "Clang";
    #else
        architecture = "GCC";
    #endif
    #else
        platform = "Unknown";
        architecture = "Unknown";
    #endif

    // Print detected platform and architecture if debug is enabled
    qDebug() << QString("Platform %1 with Architecture %2").arg(QString::fromStdString(platform), QString::fromStdString(architecture));

    // Determine the binary path based on platform and architecture
    QString flatc;
    if (platform == "Windows") {
        flatc = QDir::toNativeSeparators("thirdparty/flatbuffers/windows/flatc.exe");
    } else if (platform == "macOS") {
        flatc = (architecture == "ARM")
        ? QDir::toNativeSeparators("thirdparty/flatbuffers/mac_ant/flatc")
        : QDir::toNativeSeparators("thirdparty/flatbuffers/mac_intel/flatc");
    } else if (platform == "Linux") {
        flatc = (architecture == "Clang")
        ? QDir::toNativeSeparators("thirdparty/flatbuffers/linux_clang/flatc")
        : QDir::toNativeSeparators("thirdparty/flatbuffers/linux_gcc/flatc");
    } else {
        throw std::runtime_error("Unsupported platform or architecture");
    }

    // Use QProcess to execute the command
    QProcess process;
    QString program = flatc;

    // Prepare the arguments
    QStringList arguments;
    arguments << "-t"
              << "--strict-json"
              << "-o"
              << QString::fromStdString(path)
              << QString::fromStdString(schema)
              << "--"
              << QString::fromStdString(inputFile)
              << "--raw-binary";

    // Debug the command
    if (debug) {
        qDebug() << QString("Executing: %1 with Arguments: %2").arg(program, arguments.join(' '));
    }

    // Start the process
    process.start(program, arguments);
    process.waitForFinished();

    // Check the process result
    int returnCode = process.exitCode();
    if (debug) {
        if (returnCode != 0) {
            qCritical() << "Error executing command. Return code: " << returnCode;
            qCritical() << "Error output: " << process.readAllStandardError();
        } else {
            qDebug() << "Program Ended Successfully";
        }
    }

    return returnCode;
}

void SVShared::randomizeTeraType(json& checkFile, bool randomizeTeraType, int devId, int formId){
    if(randomizeTeraType == true){
        checkFile["gemType"] = getTeraType();
    }

    if(set_tera_type_pokemon.contains(pokemonMapping["pokemons_devid"][devId]["natdex"])){
        checkFile["gemType"] = selectTeraTypes(pokemonMapping["pokemons_devid"][devId]["natdex"], formId);
    }

    qDebug()<<"Selected Tera Type: "<< std::string(checkFile["gemType"]);
}
