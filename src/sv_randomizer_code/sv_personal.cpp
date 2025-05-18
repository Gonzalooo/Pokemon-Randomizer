#include "headers/sv_randomizer_headers/sv_personal.h"

svPersonal::svPersonal(){
    sortedMoves = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/sorted_move_list.json");
}

svPersonal::~svPersonal(){

}

void svPersonal::randomizeStats(){
    auto entries = personalMaps["entry"].get<std::vector<json>>();
    QList<int> seeds;
    for(unsigned long long i = 0; i<personalMaps["entry"].size(); i++){
        int threadSeed = localRand->generate();
        while(seeds.contains(threadSeed)){
            threadSeed = localRand->generate();
        }
        seeds.append(threadSeed);
    }

    QFuture<void> future = QtConcurrent::map(entries, [&](json& entry){
        if(entry["is_present"] == true){
            json localPokemon = entry;
            json localMapping = pokemonMapping;

            int speciesCheck = localPokemon["species"]["model"];
            int formCheck = localPokemon["species"]["form"];

            int index = &entry - &entries[0];
            QRandomGenerator randGen(seeds[index]);

            // Print out details about the data being worked on
            qDebug() << "Thread" << QThread::currentThreadId() << "working on species:"
                     << std::string(pokemonMapping["pokemons"][speciesCheck]["name"])
                     << "form:" << formCheck;

            if(fixEvolutions == true){
                qDebug()<<"Checking if there is an evolutin fix for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                fixEvolution(localPokemon, speciesCheck, formCheck);
            }

            if(randomizeEvolutions == true){
                qDebug()<<"Randomizing Evolutions for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                randomizeEvos(localPokemon, randGen);
            }

            if(randomizeAbilities == true){
                qDebug()<<"Randomizing Abilities for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                randomizeAbility(localPokemon, speciesCheck, formCheck, randGen);
            }

            if(randomizeTypes == true){
                qDebug()<<"Randomizing Types for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                randomizeType(localPokemon, speciesCheck, formCheck, randGen);;
            }

            if(randomizeMoveset == true){
                qDebug()<<"Randomizing Moveset for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                randomizeMoves(localPokemon,
                               int(localPokemon["type_1"]),
                               int(localPokemon["type_2"]),
                               speciesCheck, formCheck, randGen);
            }

            if(randomizeBST == true){
                qDebug()<<"Randomizing BST for: "<< std::string(localMapping["pokemons"][speciesCheck]["name"]);
                randomizeBSTs(localPokemon, speciesCheck, formCheck, randGen);
            }

            entry = localPokemon;
        }

    });

    future.waitForFinished();
    personalMaps["entry"] = entries;
}

void svPersonal::randomize(QRandomGenerator* r){
    personalMaps = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/personal_array_clean.json");
    movesMaps = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/waza_array_clean.json");
    localRand = r;
    setRandNum(localRand);

    if(banWonderGuard == true){
        bannedAbilities.append(25);
    }

    if(banExitAbilities == true){
        bannedAbilities.append(193);
        bannedAbilities.append(194);
    }

    if(randomizeTMs == true){
        if(noAnimationTMs == true){
            qDebug()<<"Allowing all Pokemon Moves";
            addAllMoves();

            qDebug()<<"Randomizing TMs";
            randomizeTM(true);
        }else{
            qDebug()<<"Randomizing TMs";
            randomizeTM();
        }
    }

    randomizeStats();

    bannedAbilities = {278, 307};
    selectedTMs = {};

    closeFileAndDelete("SV_PERSONAL/personal_array.json",
                       "SV_PERSONAL/personal_array.fbs",
                       "avalon/data/",
                       personalMaps, true);

    closeFileAndDelete("SV_PERSONAL/waza_array.json",
                       "SV_PERSONAL/Waza.fbs",
                       "avalon/data/",
                       movesMaps, true);
}

void svPersonal::addAllMoves(){
    for(unsigned long long i = 0; i<movesMaps["Table"].size(); i++){
        // if (!movesMaps["Table"][i].contains("MoveID")) {
        //     movesMaps["Table"][i]["MoveID"] = 0;
        // }

        if (!movesMaps["Table"][i].contains("CanUseMove")) {
            movesMaps["Table"][i]["CanUseMove"] = true;
        }

        if(!movesMaps["Table"][i].contains("Power") || movesMaps["Table"][i]["Power"] == 1)
            movesMaps["Table"][i]["Power"] = int(65);

    }
}

void svPersonal::randomizeTM(bool allTMs){
    json itemMaps = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/itemdata_array_clean.json");

    for(unsigned long long i = 0; i<itemMaps["values"].size(); i++){
        if(itemMaps["values"][i]["ItemType"] == "ITEMTYPE_WAZA"){
            int selectedMove = 1;
            if(allTMs == true){
                selectedMove = localRand->bounded(1, 920);
                while(selectedTMs.contains(selectedMove) || !allowedTMMove(selectedMove)){
                    selectedMove = localRand->bounded(1, 920);
                }
                itemMaps["values"][i]["MachineWaza"] = sortedMoves["moves"][selectedMove]["devName"];
                selectedTMs.append(int(sortedMoves["moves"][selectedMove]["id"]));
            }else{
                selectedMove = localRand->bounded(0, allowedMoves.size());
                while(selectedTMs.contains(allowedMoves[selectedMove]) || !allowedTMMove(allowedMoves[selectedMove])){
                    selectedMove = localRand->bounded(0, allowedMoves.size());
                }
                itemMaps["values"][i]["MachineWaza"] = sortedMoves["moves"][allowedMoves[selectedMove]]["devName"];
                selectedTMs.append(int(sortedMoves["moves"][allowedMoves[selectedMove]]["id"]));
            }
        }
    }

    closeFileAndDelete("SV_PERSONAL/itemdata_array.json",
                       "SV_PERSONAL/itemdata_array.bfbs",
                       "world/data/item/itemdata/",
                       itemMaps, true);
}

void svPersonal::randomizeAbility(json& pokemon, int speciesCheck, int formCheck, QRandomGenerator& rng){
    // Preliminary Checks
    if(speciesCheck == 964) { // If Palafin
        int palafinAbility = rng.bounded(1, 311);
        while(bannedAbilities.contains(palafinAbility)){
            palafinAbility = rng.bounded(1, 311);
        }
        pokemon["ability_hidden"] = palafinAbility;
        if(palafinAbility == 193 || palafinAbility == 194){
            exitAbilitiesPokemon.append({{"id", speciesCheck}, {"form", formCheck}});
        }
    }
    else if(speciesCheck == 1024 && formCheck == 0){
        // Skip Terapagos Base Forms
    }
    else{
        bool wgAdded = false;
        bool eaAdded = false;
        for(int j = 1; j<=3; j++){
            int pokemonAbility = rng.bounded(1, 311);
            while(bannedAbilities.contains(pokemonAbility)){
                pokemonAbility = rng.bounded(1, 311);
            }

            if((pokemonAbility == 193 || pokemonAbility == 194) && eaAdded == false){
                exitAbilitiesPokemon.append({{"id", speciesCheck}, {"form", formCheck}});
                eaAdded = true;
            }

            if(pokemonAbility == 25 && wgAdded == false){
                wonderGuardPokemon.append({{"id", speciesCheck}, {"form", formCheck}});
                wgAdded = true;
            }

            if(j == 3){
                pokemon["ability_hidden"] = pokemonAbility;
            }else{
                pokemon[QStringLiteral("ability_%1").arg(j).toStdString()] = pokemonAbility;
            }
        }
    }
}

void svPersonal::randomizeType(json& pokemon, int speciesCheck, int formCheck, QRandomGenerator& rng){
    if(speciesCheck == 1024 && formCheck !=0){
        // Add checks for battle form only later
    }

    int type_selection_1 = rng.bounded(0, 18);
    int oldType = pokemon["type_1"];
    pokemon["type_1"] = type_selection_1;

    if(pokemon["type_2"] == oldType && grantExtraTypes == true){
        int newType = rng.bounded(0, 100);
        if(newType < 60){
            int type_selection_2 = rng.bounded(0, 18);
            while(type_selection_2 == type_selection_1){
                type_selection_2 = rng.bounded(0, 18);
            }
            pokemon["type_2"] = type_selection_2;
        }
    }else if(pokemon["type_2"] != oldType){
        int type_selection_2 = rng.bounded(0, 18);
        while(type_selection_2 == type_selection_1){
            type_selection_2 = rng.bounded(0, 18);
        }
        pokemon["type_2"] = type_selection_2;
    }
}

void svPersonal::randomizeBSTs(json& pokemon, int species, int form, QRandomGenerator& rng){
    if(species == 1024 && form == 0){
        // Do something with base forms of battle only forms.
    }

    if(keepSameBST != true){
        pokemon["base_stats"]["hp"] = 15 + rng.bounded(0, 241);
        pokemon["base_stats"]["atk"] = 15 + rng.bounded(0, 241);
        pokemon["base_stats"]["def"] = 15 + rng.bounded(0, 241);
        pokemon["base_stats"]["spa"] = 15 + rng.bounded(0, 241);
        pokemon["base_stats"]["spd"] = 15 + rng.bounded(0, 241);
        pokemon["base_stats"]["spe"] = 15 + rng.bounded(0, 241);
    }
    else{
        int bstTotal = 0;
        bstTotal += int(pokemon["base_stats"]["hp"]);
        bstTotal += int(pokemon["base_stats"]["atk"]);
        bstTotal += int(pokemon["base_stats"]["def"]);
        bstTotal += int(pokemon["base_stats"]["spa"]);
        bstTotal += int(pokemon["base_stats"]["spd"]);
        bstTotal += int(pokemon["base_stats"]["spe"]);

        QList<int> bstList = {15, 15, 15, 15, 15, 15};
        bstTotal -= (15*6);

        bool randomizeStats = true;
        while(randomizeStats == true){
            QList<int> statsChecked = {};
            while(bstTotal !=0){
                int noInfiniteLoop = 1;
                int checkedTotal = bstTotal;
                int checkingStat = rng.bounded(0, 6);

                while(statsChecked.contains(checkingStat)){
                    checkingStat = rng.bounded(0, 6);
                    if(noInfiniteLoop == 6){
                        break;
                    }

                    noInfiniteLoop++;
                }

                if(noInfiniteLoop==6){
                    break;
                }

                statsChecked.append(checkingStat);

                int newBaseStat = rng.bounded(0, 241);
                while(checkedTotal - newBaseStat < 0){
                    newBaseStat = rng.bounded(0, bstTotal+1);
                }
                while(bstList[checkingStat] + newBaseStat > 255){
                    newBaseStat = rng.bounded(0, 241);
                }

                bstList[checkingStat] += newBaseStat;
                bstTotal -= newBaseStat;

                if(bstTotal == 0){
                    randomizeStats = false;
                }
            };
        };

        pokemon["base_stats"]["hp"] = bstList[0];
        pokemon["base_stats"]["atk"] = bstList[1];
        pokemon["base_stats"]["def"] = bstList[2];
        pokemon["base_stats"]["spa"] = bstList[3];
        pokemon["base_stats"]["spd"] = bstList[4];
        pokemon["base_stats"]["spe"] = bstList[5];
    }
}

void fixEvo(json& pokemon, int species, int form, int level, int condition, int index, bool push){
    if(push == true){
        pokemon["evolutions"].push_back({{"level", level}, {"condition", condition}, {"parameter", 0}, {"reserved3", 0},
                                         {"reserved4", 0}, {"reserved5", 0}, {"species", species},
                                         {"form", form}});
    }else{
        pokemon["evolutions"][index]["level"] = level;
        pokemon["evolutions"][index]["form"] = form;
        pokemon["evolutions"][index]["species"] = species;
        pokemon["evolutions"][index]["condition"] = condition;
        pokemon["evolutions"][index]["parameter"] = 0;
        pokemon["evolutions"][index]["reserved3"] = 0;
        pokemon["evolutions"][index]["reserved4"] = 0;
        pokemon["evolutions"][index]["reserved5"] = 0;
    }
}

// Add Stone/Item Based Evolutions later
// https://bulbapedia.bulbagarden.net/wiki/Evolution_stone (except Eevee)
// https://bulbapedia.bulbagarden.net/wiki/Location-based_Evolution (except Eevee)
// https://bulbapedia.bulbagarden.net/wiki/Category:Pok%C3%A9mon_that_evolve_by_leveling_up_with_a_held_item
// https://bulbapedia.bulbagarden.net/wiki/Methods_of_Evolution#Evolution_stone (look to see which aren't possible in a randomizer)
void svPersonal::fixEvolution(json& pokemon, int species, int form){
    switch(species){
    case 25:
        if(form == 0){
            fixEvo(pokemon, 26, 0, 36, 32, 1, true); // Raichu
            fixEvo(pokemon, 26, 0, 36, 33, 1, true); // Raichu - Alolan
        }
        break;
    case 61: // Poliwhirl
        fixEvo(pokemon, 186, 0, 36, 4, 1, false); // Politoed
        fixEvo(pokemon, 62, 0, 36, 4, 0, true); // Poliwrath
        break;
    case 64: // Kadabra
        fixEvo(pokemon, 65, 0, 36, 4, 0, false); // Alakazam
        break;
    case 67: // Machoke
        fixEvo(pokemon, 68, 0, 36, 4, 0, false); // Machamp
        break;
    case 75: // Graveler (Kanto and Alola)
        fixEvo(pokemon, 76, form, 36, 4, 0, false); // Golem - Kanto/Alola
        break;
    case 79: // Slowpoke
        fixEvo(pokemon, 80, 0, 36, 32, 0, false); // Slowbro - in morning
        fixEvo(pokemon, 199, 0, 36, 33, 1, false); // Slowking - at night
        break;
    case 93: // Haunter
        fixEvo(pokemon, 94, 0, 36, 4, 0, false); // Gengar
        break;
    case 95: // Onix
        fixEvo(pokemon, 208, 0, 36, 4, 0, false); // Steelix
        break;
    case 102: // Exeggcute
        fixEvo(pokemon, 103, 0, 36, 32, 1, true); // Exeggcutor
        fixEvo(pokemon, 103, 1, 36, 33, 1, true); // Exeggcutor - Alola
        break;
    case 109: // Koffing
        fixEvo(pokemon, 110, 0, 35, 32, 0, false); // Weezing
        fixEvo(pokemon, 110, 1, 35, 33, 1, true); // Weezing - Galar
        break;
    case 112: // Rhydon
        fixEvo(pokemon, 464, 0, 36, 4, 0, false); // Rhyperior
        break;
    case 117: // Seadra
        fixEvo(pokemon, 230, 0, 36, 4, 0, false); // Slowking
        break;
    case 123: // Scyther
        fixEvo(pokemon, 212, 0, 36, 32, 0, false); // Scizor - in morning
        fixEvo(pokemon, 900, 0, 36, 33, 1, false); // Kleavor - at night
        break;
    case 125: // Electabuzz
        fixEvo(pokemon, 466, 0, 36, 4, 0, false); // Electivire
        break;
    case 126: // Magmar
        fixEvo(pokemon, 467, 0, 36, 4, 0, false); // Magmotar
        break;
    case 137: // Porygon
        fixEvo(pokemon, 233, 0, 16, 4, 0, false); // Porygon2
        break;
    case 156: // Quilava
        fixEvo(pokemon, 157, 0, 36, 32, 0, false); // Typholosion
        fixEvo(pokemon, 157, 1, 36, 33, 0, true); // Typholosion - Hisuian
        break;
    case 217: // Ursaring
        fixEvo(pokemon, 901, 0, 36, 32, 0, false); // Ursaluna
        fixEvo(pokemon, 901, 1, 36, 33, 0, true); // Ursaluna - Bloodmoon
        break;
    case 233: // Porygon2
        fixEvo(pokemon, 474, 0, 36, 4, 0, false); // PorygonZ
        break;
    case 234: // Standler
        fixEvo(pokemon, 899, 0, 36, 4, 0, true); // Wyrdeer
        break;
    case 349: // Feebas
        fixEvo(pokemon, 350, 0, 36, 4, 1, false); // Milotic
        break;
    case 356: // Dusclops
        fixEvo(pokemon, 477, 0, 36, 4, 1, false); // Dusknoir
        break;
    case 366: // Clamperl
        fixEvo(pokemon, 368, 0, 36, 32, 0, false); // Gorebyss
        fixEvo(pokemon, 367, 0, 36, 33, 1, false); // Huntail
        break;
    case 502: // Dewott
        fixEvo(pokemon, 503, 0, 36, 32, 0, false); // Samurott
        fixEvo(pokemon, 503, 1, 36, 33, 0, true); // Samurott - Hisui
        break;
    case 525: // Boldore
        fixEvo(pokemon, 526, 0, 36, 4, 1, false); // Milotic
        break;
    case 533: // Gurdurr
        fixEvo(pokemon, 534, 0, 36, 4, 1, false); // Conkeldurr
        break;
    case 548: // Petilil
        fixEvo(pokemon, 548, 0, 36, 32, 0, true); // Lilligant
        fixEvo(pokemon, 548, 1, 36, 33, 0, true); // Lilligant - Hisui
        break;
    case 588: // Karrablast
        fixEvo(pokemon, 589, 0, 36, 4, 1, false); // Escavalier
        break;
    case 616: // Shelmet
        fixEvo(pokemon, 617, 0, 36, 4, 1, false); // Accelgor
        break;
    case 627:
        fixEvo(pokemon, 628, 0, 36, 32, 0, false); // Braviary
        fixEvo(pokemon, 628, 1, 36, 33, 0, true); // Braviary - Hisui
        break;
    case 682: // Spritzee
        fixEvo(pokemon, 683, 0, 36, 4, 1, false); // Aromatisse
        break;
    case 684: // Swirlix
        fixEvo(pokemon, 685, 0, 36, 4, 1, false); // Slurpuff
        break;
    case 686: // Inkay
        fixEvo(pokemon, 687, 0, 30, 33, 1, true); // Malamar
        break;
    case 704: // Goomy
        fixEvo(pokemon, 705, 0, 36, 32, 0, false); // Sliggo
        fixEvo(pokemon, 705, 1, 36, 33, 0, true); // Sliggo - Hisui
        break;
    case 708: // Phantump
        fixEvo(pokemon, 709, 0, 36, 4, 1, false); // Trevenant
        break;
    case 710: // Pumpkaboo
        fixEvo(pokemon, 711, form, 36, 4, 1, false); // Gourgeist
        break;
    case 712: // Bergmite
        fixEvo(pokemon, 713, 0, 36, 32, 0, false); // Avalugg
        fixEvo(pokemon, 713, 1, 36, 33, 0, true); // Avalugg - Hisui
        break;
    case 723: // Dartrix
        fixEvo(pokemon, 724, 0, 36, 32, 0, false); // Decidueye
        fixEvo(pokemon, 724, 1, 36, 33, 0, true); // Decidueye - Hisui
        break;
    }
}

void svPersonal::randomizeEvos(json& pokemon, QRandomGenerator& localRand){
    json maps = pokemonMapping;
    QMap<int, QList<int>> localBanned = bannedPokemon;
    int localMax = maxAllowedId;
    if(evolveEveryLevel == true){
        json newEvos = json::array();
        for(int z = 2; z<=100; z++){
            int selectedPokemon = localRand.bounded(1, localMax);
            int selectedForm = localRand.bounded(0, static_cast<int>(maps["pokemons"][selectedPokemon]["forms"].size()));

            while(localBanned.contains(selectedPokemon) && localBanned[selectedPokemon].contains(selectedForm)){
                selectedPokemon = localRand.bounded(1, localMax);
                selectedForm = localRand.bounded(0, static_cast<int>(maps["pokemons"][selectedPokemon]["forms"].size()));
            }

            newEvos.push_back({
                {"level", z}, {"condition", 4}, {"parameter", 0},
                {"reserved3", 0}, {"reserved4", 0}, {"reserved5", 0},
                {"species", int(maps["pokemons"][selectedPokemon]["devid"])},
                {"form", selectedForm}
            });
        }

        pokemon["evolutions"] = newEvos;
    }else{
        for (auto& evolution : pokemon["evolutions"]) {
            int selectedPokemon = localRand.bounded(1, localMax);
            int selectedForm = localRand.bounded(0, static_cast<int>(maps["pokemons"][selectedPokemon]["forms"].size()));

            while(localBanned.contains(selectedPokemon) && localBanned[selectedPokemon].contains(selectedForm)){
                selectedPokemon = localRand.bounded(1, localMax);
                selectedForm = localRand.bounded(0, static_cast<int>(maps["pokemons"][selectedPokemon]["forms"].size()));
            }

            evolution["species"] = int(maps["pokemons"][selectedPokemon]["devid"]);
            evolution["form"] = selectedForm;
        }
    }
}

void svPersonal::randomizeMoves(json& pokemon, int type1, int type2, int species, int form, QRandomGenerator& rng){

    if(type1 || type2){
         // Do something
    }

    QStringList keys = {"levelup_moves", "reminder_moves", "egg_moves"};
    for(int k = 0; k<keys.size(); k++){
        QList<int> currentMoves;
        qDebug()<<"Randomizing - "<<keys[k];
        for(unsigned long long j = 0; j<pokemon[keys[k].toStdString()].size(); j++){
            if(noAnimationTMs == true){
                int selectedMove = rng.bounded(1, 920);
                while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species, form)){
                    selectedMove = rng.bounded(1, 920);
                }
                currentMoves.append(selectedMove);
                if(keys[k] == "levelup_moves"){
                    pokemon[keys[k].toStdString()][j]["move"] = selectedMove;
                }else{
                   pokemon[keys[k].toStdString()][j] = selectedMove;
                }
            }else{
                int selectedMove = rng.bounded(0, allowedMoves.size());
                while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species, form)){
                    selectedMove = rng.bounded(0, allowedMoves.size());
                }
                currentMoves.append(selectedMove);
                if(keys[k] == "levelup_moves"){
                    pokemon[keys[k].toStdString()][j]["move"] = allowedMoves[selectedMove];
                }else{
                    pokemon[keys[k].toStdString()][j] = allowedMoves[selectedMove];
                }
            }
        }
    }

    qDebug()<<"Randomizing - "<<"TMs";
    if(species != 151){
        QList<int> currentMoves;
        for(unsigned long long j = 0; j<pokemon["tm_moves"].size(); j++){
            if(randomizeTMs == false){
                int selectedMove = rng.bounded(0, defaultTMs.size());
                while(currentMoves.contains(selectedMove)){
                    selectedMove = rng.bounded(0, defaultTMs.size());
                }
                currentMoves.append(selectedMove);
                pokemon["tm_moves"][j] = defaultTMs[selectedMove];
            }
            else{
                int selectedMove = rng.bounded(0, selectedTMs.size());
                while(currentMoves.contains(selectedMove)){
                    selectedMove = rng.bounded(0, selectedTMs.size());
                }
                currentMoves.append(selectedMove);
                pokemon["tm_moves"][j] = selectedTMs[selectedMove];
            }
        }
    }else{
        json mewTms = json::array();
        QList<int> currentMoves;
        if(randomizeTMs == false){
            for(int j =0; j<defaultTMs.size(); j++){
                mewTms.push_back(int(defaultTMs[j]));
            }
            pokemon["tm_moves"] = mewTms;
        }
        else{
            for(int j =0; j<selectedTMs.size(); j++){
                mewTms.push_back(int(selectedTMs[j]));
            }
            pokemon["tm_moves"] = mewTms;
        }
    }
}
