#include "headers/sv_randomizer_headers/sv_trainers.h"

svTrainers::svTrainers(){

}

svTrainers::~svTrainers(){

}

void svTrainers::noSoftlockeParadise(){
    QMap<int, QList<int>> allowedPokemon;
    bool sizeCheck = getAllowedPokemon(paradisePokemon.allowedPokemons, allowedPokemon, "Koraidon");

    if(paradisePokemon.randomize == true){
        QList<int> protocol = paradisePokemon.randomizedIndex;
        for(int i = 0; i< protocol.length(); i++){
            QMap<QString, int> checkDict = {{"id", 0}, {"form", 0}};

            while(wonderGuardPokemon.contains(checkDict)){
                int random = localRand->bounded(1, maxAllowedId);

                while(!allowedPokemon.contains(int(pokemonMapping["pokemons"][random]["natdex"])))
                    random = localRand->bounded(1, maxAllowedId);

                int formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
                while(pokemonMapping["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                    formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
                }

                checkDict={{"id", int(random)}, {"form",int(formRandom)}};
            }

            trainersData["values"][protocol[i]]["poke1"]["devId"] = pokemonMapping["pokemons"][int(checkDict["id"])]["devName"];
            trainersData["values"][protocol[i]]["poke1"]["formId"] = int(checkDict["form"]);
        }
    }
}

void svTrainers::noSoftlockeTerapagos(){
    QMap<int, QList<int>> allowedPokemon;
    bool sizeCheck = getAllowedPokemon(hiddenTreasure.allowedPokemons, allowedPokemon, "Terapagos");

    if(hiddenTreasure.randomize == true){
        QList<int> protocol = hiddenTreasure.randomizedIndex;
        for(int i = 0; i< protocol.length(); i++){
            QMap<QString, int> checkDict = {{"id", 0}, {"form", 0}};

            while(wonderGuardPokemon.contains(checkDict)){
                int random = localRand->bounded(1, maxAllowedId);

                while(!allowedPokemon.contains(int(pokemonMapping["pokemons"][random]["natdex"])))
                    random = localRand->bounded(1, maxAllowedId);

                int formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
                while(pokemonMapping["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                    formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
                }

                checkDict={{"id", int(random)}, {"form",int(formRandom)}};
            }

            trainersData["values"][protocol[i]]["poke1"]["devId"] = pokemonMapping["pokemons"][int(checkDict["id"])]["devName"];
            trainersData["values"][protocol[i]]["poke1"]["formId"] = int(checkDict["form"]);
        }
    }
}

int svTrainers::getMaxNumberOfChanges(json trainerEntry, int& level, bool NULLS){
    int totalChanges = 0;

    for(int i = 1; i<=6; i++){
        std::string key = "poke"+std::to_string(i);
        if(NULLS == true){
            if(trainerEntry[key]["devId"] == "DEV_NULL"){
                totalChanges++;
            }
        }else{
            if(trainerEntry[key]["devId"] != "DEV_NULL"){
                level = int(trainerEntry[key]["level"]);
                totalChanges++;
            }
        }
    }

    return totalChanges;
}

void svTrainers::randomizeTrainers(trainerSettings trainer){
    if(trainer.randomize == true){
        auto entries = trainersData["values"].get<std::vector<json>>();
        QMap<int, QList<int>> allowedPokemon;

        bool sizeCheck = getAllowedPokemon(trainer.allowedPokemons, allowedPokemon, "Trainers");
        qDebug()<<"Total Number of randomized Trainers:"<< trainer.randomizedIndex.size();

        QFuture<void> future = QtConcurrent::map(entries, [&](json& entry){
            int index = &entry - &entries[0];
            int maxLevel = 100;
            // Only randomize if its within the allowed indexes
            if(trainer.randomizedIndex.contains(index)){
                json localMapping = pokemonMapping;

                QRandomGenerator randGen(seeds[index]);
                qDebug() << "Thread" << QThread::currentThreadId() << "working on trainer:"
                         << std::string(entry["trid"])<<" Index:"<<index;

                // Enable tera if selected
                if(trainer.allowTera == true){
                    entry["changeGem"] = true;
                }

                if(arvenTitans.contains(index)){
                    entry["changeGem"] = false;
                }

                // Sets the maximum number of pokemon to be randomized for the trainer
                int maxChanges = 0;
                if(trainer.extraPokemon == true){
                    int initial = getMaxNumberOfChanges(entry, maxLevel);

                    if(initial !=6){
                        maxChanges = randGen.bounded(initial+1, 7);
                    }else{
                        maxChanges = initial;
                    }

                    // Protective Case
                    if(maxChanges >=6){
                        maxChanges = 6;
                        if(entry["trid"] == "dan_aku_boss_01" ||
                            entry["trid"] == "dan_doku_boss_01" ||
                            entry["trid"] == "dan_fairy_boss_01" ||
                            entry["trid"] == "dan_hono_boss_01" ||
                            entry["trid"] == "dan_kakutou_boss_01"){

                            maxChanges = 5;
                            entry["aiDouble"] = false;
                            entry["battleType"] = "_1vs1";
                        }
                    }

                }else if (trainer.force6Pokemon == true){
                    maxChanges = 6;

                    // Set Team Star Bosses to 5 (6th is the Revevaroom)
                    if(entry["trid"] == "dan_aku_boss_01" ||
                        entry["trid"] == "dan_doku_boss_01" ||
                        entry["trid"] == "dan_fairy_boss_01" ||
                        entry["trid"] == "dan_hono_boss_01" ||
                        entry["trid"] == "dan_kakutou_boss_01"){

                        maxChanges = 5;
                        entry["aiDouble"] = false;
                        entry["battleType"] = "_1vs1";
                    }

                }else{
                    maxChanges = getMaxNumberOfChanges(entry, maxLevel);
                }

                // Set battles to doubles or randomnly select between them (ignoring multi and raid battles)
                if(!raid_npc_index.contains(index) && !multi_battles_index.contains(index)){
                    if(trainer.allDoubles == true){
                        entry["aiDouble"] = true;
                        entry["battleType"] = "_2vs2";
                        if(maxChanges < 2){
                            maxChanges = 2;
                        }
                    }
                    else if(trainer.singlesOrDoubles == true){
                        int battleType = randGen.bounded(1,101);
                        if(battleType>50){
                            entry["aiDouble"] = true;
                            entry["battleType"] = "_2vs2";
                            if(maxChanges < 2){
                                maxChanges = 2;
                            }
                        }else{
                            entry["aiDouble"] = false;
                            entry["battleType"] = "_1vs1";
                        }
                    }
                }

                // set Ai level
                if(trainer.makeAISmart == true){
                    entry["aiWeak"] = false;
                    entry["aiBasic"] = false;
                    entry["aiHigh"] = false;
                    entry["aiExpert"] = true;
                    entry["aiItem"] = true;
                    entry["aiChange"] = true;
                }

                // Protective Cases (prevents crashes or softlocks)
                if(entry["trid"] == "sister_01_01" || entry["trid"] == "sister_01_01_strong"){
                    entry["aiDouble"] = false;
                    entry["battleType"] = "_1vs1";
                }
                if(index >= 585 && index <= 594){
                    entry["aiDouble"] = false;
                    entry["battleType"] = "_1vs1";
                }

                if(paldeaRaids.contains(index) || kitakamiRaids.contains(index) || blueberryRaids.contains(index)
                    || arvenTitans.contains(index)){
                    maxChanges = 1;
                    entry["battleType"] = "_1vs1";
                    entry["changeGem"] = false;
                }

                if(entry["trid"] == "rival_01_hono" ||
                    entry["trid"] == "rival_01_kusa" ||
                    entry["trid"] == "rival_01_mizu" || index == 711 || index == 681){
                    entry["aiDouble"] = false;
                    entry["battleType"] = "_1vs1";
                    entry["changeGem"] = false;
                    maxChanges = 1;
                }

                // Randomize the Pokemon
                for(int i=1; i<=maxChanges; i++){
                    // Pokemon key
                    std::string key = "poke"+std::to_string(i);

                    // set IVs
                    if(trainer.forcePerfectIV == true){
                        entry[key]["talentType"] = "VALUE";
                        entry[key]["talentValue"]["hp"] = 31;
                        entry[key]["talentValue"]["atk"] = 31;
                        entry[key]["talentValue"]["def"] = 31;
                        entry[key]["talentValue"]["spAtk"] = 31;
                        entry[key]["talentValue"]["spDef"] = 31;
                        entry[key]["talentValue"]["agi"] = 31;
                    }

                    // Obtained Randomized Pokemon (Manual since using randGen
                    int pokemon = randGen.bounded(1, maxAllowedId);
                    while(!allowedPokemon.contains(pokemon)){
                        pokemon = randGen.bounded(1, maxAllowedId);
                    }

                    if(static_cast<int>(localMapping["pokemons"][pokemon]["forms"].size()) == 0){
                        qFatal()<<"Pokemon: "<<pokemon;
                    }
                    int form = randGen.bounded(0, static_cast<int>(localMapping["pokemons"][pokemon]["forms"].size()));
                    while(!allowedPokemon[pokemon].contains(form)){
                        form = randGen.bounded(0, static_cast<int>(localMapping["pokemons"][pokemon]["forms"].size()));
                    }

                    std::string genderStd = "MALE";
                    QString form_Check = QString::fromUtf8(localMapping["pokemons"][pokemon]["name"].get<std::string>().c_str());
                    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){
                        if(genderlessPokemon.contains(form_Check)){
                            genderStd = "DEFAULT";
                        }else{
                            genderStd = "MALE";
                        }
                    }else if(femaleOnlyPokemon.contains(form_Check)){
                        genderStd = "FEMALE";
                    } else if(genderForms.contains(form_Check)){
                        if(form == 0){
                            genderStd = "MALE";
                        }else{
                            genderStd = "FEMALE";
                        }
                    }else{
                        int rand_gender = localRand->bounded(0, 100);
                        if(rand_gender > int(pokemonPersonalData["entry"][int(localMapping["pokemons"][pokemon]["devid"])]["gender"]["ratio"])){
                            genderStd = "MALE";
                        }else{
                            genderStd = "FEMALE";
                        }
                    }
                    // Set Pokemon
                    entry[key]["devId"] = localMapping["pokemons"][pokemon]["devName"];
                    entry[key]["formId"] = form;
                    entry[key]["item"] = getPokemonItemId(pokemon, form);
                    entry[key]["sex"] = genderStd;
                    if(int(entry[key]["level"]) == 0){
                        entry[key]["level"] = maxLevel + randGen.bounded(1, 3);
                    }
                    randomizeTeraType(entry[key], trainer.randomizeTeras, localMapping["pokemons"][pokemon]["devid"], form);

                    // Set Shinyness
                    if(trainer.allowShinies == true){
                        if(entry[key]["rareType"] != "RARE"){
                            int shinyRate = randGen.bounded(1, 101);
                            if(shinyRate>50){
                                entry[key]["rareType"] = "RARE";
                            }
                        }
                    }

                    // Set moves
                    entry[key]["wazaType"] = "DEFAULT";
                    for(int k = 1; k<=4; k++){
                        std::string moveKey = "waza"+std::to_string(k);
                        entry[key][moveKey]["wazaId"] = "WAZA_NULL";
                    }
                }

            }

        });

        future.waitForFinished();
        trainersData["values"] = entries;
    }
}

void svTrainers::randomize(QRandomGenerator* r, bool paldea, bool kitakami, bool blueberry, bool boss){
    trainersData = readJsonQFile("SV_FLATBUFFERS/SV_TRAINERS/trdata_array_clean.json");
    localRand = r;
    setRandNum(localRand);

    for(unsigned long long i =0; i<trainersData["values"].size(); i++){
        int threadSeed = localRand->generate();
        while(seeds.contains(threadSeed)){
            threadSeed = localRand->generate();
        }
        seeds.append(threadSeed);
    }
    if(paldea == true){
        qDebug()<<"Randomizing All Trainers";
        randomizeTrainers(allTrainers);
        qDebug()<<"Randomizing Rivals";
        randomizeTrainers(rivalTrainers);
        qDebug()<<"Randomizing Gym Trainers";
        randomizeTrainers(gymTrainers);
        qDebug()<<"Randomizing E4 Trainers";
        randomizeTrainers(e4Trainers);
        qDebug()<<"Randomizing Koraidon/Miraidon Trainers";
        noSoftlockeParadise();
        qDebug()<<"Randomizing Champions Trainers";
        randomizeTrainers(championTrainers);
        qDebug()<<"Randomizing Route Trainers";
        randomizeTrainers(routeTrainers);
        qDebug()<<"Randomizing Raid Trainers";
        randomizeTrainers(raidTrainers);
    }

    if(kitakami == true){
        qDebug()<<"Randomizing All Trainers - Kitkami";
        randomizeTrainers(allKitakamiTrainers);
        qDebug()<<"Randomizing Rivals - Kitakami";
        randomizeTrainers(kitakamiRivals);
        qDebug()<<"Randomizing Ogre Clan";
        randomizeTrainers(ogreClanTrainers);
        qDebug()<<"Randomizing Route Trainers - Kitakami";
        randomizeTrainers(kitakamiRouteTrainers);
        qDebug()<<"Randomizing Raid Trainers - Kitakami";
        randomizeTrainers(kitakamiRaidTrainers);
    }

    if(blueberry == true){
        qDebug()<<"Randomizing All Trainers - Blueberry";
        randomizeTrainers(allBlueberryTrainers);
        qDebug()<<"Randomizing Rivals - Blueberry";
        randomizeTrainers(blueberryRivals);
        qDebug()<<"Randomizing BB4 Trainers";
        randomizeTrainers(bb4Trainers);
        qDebug()<<"Randomizing Terapagos";
        noSoftlockeTerapagos();
        qDebug()<<"Randomizing Route Trainers - Blueberry";
        randomizeTrainers(blueberryRouteTrainers);
        qDebug()<<"Randomizing Raid Trainers - Blueberry";
        randomizeTrainers(blueberryRaidTrainers);
    }

    qDebug()<<"Closing and finalizing file";
    closeFileAndDelete("SV_TRAINERS/trdata_array.json",
                       "SV_TRAINERS/trdata_array.bfbs",
                       "world/data/trainer/trdata/",
                       trainersData, !boss);
    qDebug()<<"Returning file";
}

bool svTrainers::checkRandomization(bool& paldea, bool& kita, bool& blueberry){
    qDebug()<<"Checking and Getting indexes for Trainers";
    bool trainersCanBeRandomized = false;

    if(allTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        allTrainers.randomizedIndex = area1;
        allTrainers.randomizedIndex += area15;
        allTrainers.randomizedIndex += area3;
        allTrainers.randomizedIndex += area4;
        allTrainers.randomizedIndex += area5;
        allTrainers.randomizedIndex += area6;
        allTrainers.randomizedIndex += area7;
        allTrainers.randomizedIndex += area8;
        allTrainers.randomizedIndex += area9;
        allTrainers.randomizedIndex += area10;
        allTrainers.randomizedIndex += area11;
        allTrainers.randomizedIndex += area12;
        allTrainers.randomizedIndex += area13;
        allTrainers.randomizedIndex += area14;
        allTrainers.randomizedIndex += area16;
        allTrainers.randomizedIndex += area17;
        allTrainers.randomizedIndex += area18;
        allTrainers.randomizedIndex += area19;
        allTrainers.randomizedIndex += area20;
        allTrainers.randomizedIndex += area21;
        allTrainers.randomizedIndex += area22;
        allTrainers.randomizedIndex += area24;
        allTrainers.randomizedIndex += area25;
        allTrainers.randomizedIndex += area26;
        allTrainers.randomizedIndex += penny;
        allTrainers.randomizedIndex += geeta;
        allTrainers.randomizedIndex += clavell_fire;
        allTrainers.randomizedIndex += clavell_grass;
        allTrainers.randomizedIndex += clavell_water;
        allTrainers.randomizedIndex += starDark;
        allTrainers.randomizedIndex += starPoison;
        allTrainers.randomizedIndex += starFairy;
        allTrainers.randomizedIndex += starFire;
        allTrainers.randomizedIndex += starFight;
        allTrainers.randomizedIndex += importantTrainers;
        allTrainers.randomizedIndex += e4Dragon;
        allTrainers.randomizedIndex += e4Steel;
        allTrainers.randomizedIndex += e4Flying;
        allTrainers.randomizedIndex += e4Ground;
        allTrainers.randomizedIndex += electricGym;
        allTrainers.randomizedIndex += psychicGym;
        allTrainers.randomizedIndex += ghostGym;
        allTrainers.randomizedIndex += iceGym;
        allTrainers.randomizedIndex += grassGym;
        allTrainers.randomizedIndex += waterGym;
        allTrainers.randomizedIndex += bugGym;
        allTrainers.randomizedIndex += normalGym;
        allTrainers.randomizedIndex += dendra;
        allTrainers.randomizedIndex += miriam;
        allTrainers.randomizedIndex += arven;
        allTrainers.randomizedIndex += professors;
        allTrainers.randomizedIndex += paldeaRaids;
        allTrainers.randomizedIndex += raifort;
        allTrainers.randomizedIndex += arvenTitans;
        allTrainers.randomizedIndex += saguaro;
        allTrainers.randomizedIndex += salvatore;
        allTrainers.randomizedIndex += nemona_fire;
        allTrainers.randomizedIndex += nemona_grass;
        allTrainers.randomizedIndex += nemona_water;
        allTrainers.randomizedIndex += ryme;
        allTrainers.randomizedIndex += zinia;
        if(paldeaForAll == true){
            allTrainers.randomizedIndex += area1_dlc1;
            allTrainers.randomizedIndex += area2_dlc1;
            allTrainers.randomizedIndex += area3_dlc1;
            allTrainers.randomizedIndex += area4_dlc1;
            allTrainers.randomizedIndex += area7_dlc1;
            allTrainers.randomizedIndex += area8_dlc1;
            allTrainers.randomizedIndex += area9_dlc1;
            allTrainers.randomizedIndex += area10_dlc1;
            allTrainers.randomizedIndex += kieran_dlc1;
            allTrainers.randomizedIndex += carmine_dlc1;
            allTrainers.randomizedIndex += kieran_stront_dlc1;
            allTrainers.randomizedIndex += carmine_strong_dlc1;
            allTrainers.randomizedIndex += ogreClan;
            allTrainers.randomizedIndex += ogreClanBoss;
            allTrainers.randomizedIndex += perrin;
            allTrainers.randomizedIndex += oNareFamily;
            allTrainers.randomizedIndex += kieran;
            allTrainers.randomizedIndex += carmine;
            allTrainers.randomizedIndex += shiano;
            allTrainers.randomizedIndex += schoolwars;
            allTrainers.randomizedIndex += bb4DragonTrainers;
            allTrainers.randomizedIndex += bb4FairyTrainers;
            allTrainers.randomizedIndex += bb4FireTrainers;
            allTrainers.randomizedIndex += bb4Fire;
            allTrainers.randomizedIndex += bb4Steel;
            allTrainers.randomizedIndex += bb4Farity;
            allTrainers.randomizedIndex += bb4Dragon;
            allTrainers.randomizedIndex += area1_dlc2;
            allTrainers.randomizedIndex += area2_dlc2;
            allTrainers.randomizedIndex += area3_dlc2;
            allTrainers.randomizedIndex += area4_dlc2;
            allTrainers.randomizedIndex += epilogue;
            allTrainers.randomizedIndex += blueberryRaids;
        }
    }

    if(rivalTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        rivalTrainers.randomizedIndex = penny;
        rivalTrainers.randomizedIndex += nemona_fire;
        rivalTrainers.randomizedIndex += nemona_grass;
        rivalTrainers.randomizedIndex += nemona_water;
        rivalTrainers.randomizedIndex += clavell_fire;
        rivalTrainers.randomizedIndex += clavell_grass;
        rivalTrainers.randomizedIndex += clavell_water;
        rivalTrainers.randomizedIndex += starDark;
        rivalTrainers.randomizedIndex += starPoison;
        rivalTrainers.randomizedIndex += starFairy;
        rivalTrainers.randomizedIndex += starFire;
        rivalTrainers.randomizedIndex += starFight;
        rivalTrainers.randomizedIndex += arven;
        rivalTrainers.randomizedIndex += arvenTitans;
        rivalTrainers.randomizedIndex += importantTrainers;
        if(paldeaForAll == true){
            rivalTrainers.randomizedIndex += kieran_dlc1;
            rivalTrainers.randomizedIndex += carmine_dlc1;
            rivalTrainers.randomizedIndex += kieran_stront_dlc1;
            rivalTrainers.randomizedIndex += carmine_strong_dlc1;
            rivalTrainers.randomizedIndex += perrin;
            rivalTrainers.randomizedIndex += oNareFamily;
            rivalTrainers.randomizedIndex += kieran;
            rivalTrainers.randomizedIndex += carmine;
            rivalTrainers.randomizedIndex += shiano;
        }
    }

    if(gymTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        gymTrainers.randomizedIndex = electricGym;
        gymTrainers.randomizedIndex += psychicGym;
        gymTrainers.randomizedIndex += ghostGym;
        gymTrainers.randomizedIndex += iceGym;
        gymTrainers.randomizedIndex += grassGym;
        gymTrainers.randomizedIndex += waterGym;
        gymTrainers.randomizedIndex += bugGym;
        gymTrainers.randomizedIndex += normalGym;
    }

    if(e4Trainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        e4Trainers.randomizedIndex = e4Dragon;
        e4Trainers.randomizedIndex += e4Steel;
        e4Trainers.randomizedIndex += e4Flying;
        e4Trainers.randomizedIndex += e4Ground;
        e4Trainers.randomizedIndex += dendra;
        e4Trainers.randomizedIndex += miriam;
        e4Trainers.randomizedIndex += raifort;
        e4Trainers.randomizedIndex += saguaro;
        e4Trainers.randomizedIndex += salvatore;
        e4Trainers.randomizedIndex += ryme;
        e4Trainers.randomizedIndex += zinia;
        if(paldeaForAll == true){
            e4Trainers.randomizedIndex += ogreClan;
            e4Trainers.randomizedIndex += schoolwars;
            e4Trainers.randomizedIndex += bb4DragonTrainers;
            e4Trainers.randomizedIndex += bb4FairyTrainers;
            e4Trainers.randomizedIndex += bb4FireTrainers;
            e4Trainers.randomizedIndex += bb4Fire;
            e4Trainers.randomizedIndex += bb4Steel;
            e4Trainers.randomizedIndex += bb4Farity;
            e4Trainers.randomizedIndex += bb4Dragon;
        }
    }

    if(championTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        championTrainers.randomizedIndex = geeta;
        championTrainers.randomizedIndex += professors;
        if(paldeaForAll == true){
            championTrainers.randomizedIndex += ogreClanBoss;
        }
    }

    if(routeTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        routeTrainers.randomizedIndex = area1;
        routeTrainers.randomizedIndex += area15;
        routeTrainers.randomizedIndex += area3;
        routeTrainers.randomizedIndex += area4;
        routeTrainers.randomizedIndex += area5;
        routeTrainers.randomizedIndex += area6;
        routeTrainers.randomizedIndex += area7;
        routeTrainers.randomizedIndex += area8;
        routeTrainers.randomizedIndex += area9;
        routeTrainers.randomizedIndex += area10;
        routeTrainers.randomizedIndex += area11;
        routeTrainers.randomizedIndex += area12;
        routeTrainers.randomizedIndex += area13;
        routeTrainers.randomizedIndex += area14;
        routeTrainers.randomizedIndex += area16;
        routeTrainers.randomizedIndex += area17;
        routeTrainers.randomizedIndex += area18;
        routeTrainers.randomizedIndex += area19;
        routeTrainers.randomizedIndex += area20;
        routeTrainers.randomizedIndex += area21;
        routeTrainers.randomizedIndex += area22;
        routeTrainers.randomizedIndex += area24;
        routeTrainers.randomizedIndex += area25;
        routeTrainers.randomizedIndex += area26;
        if(paldeaForAll == true){
            routeTrainers.randomizedIndex += area1_dlc1;
            routeTrainers.randomizedIndex += area2_dlc1;
            routeTrainers.randomizedIndex += area3_dlc1;
            routeTrainers.randomizedIndex += area4_dlc1;
            routeTrainers.randomizedIndex += area7_dlc1;
            routeTrainers.randomizedIndex += area8_dlc1;
            routeTrainers.randomizedIndex += area9_dlc1;
            routeTrainers.randomizedIndex += area10_dlc1;
            routeTrainers.randomizedIndex += area1_dlc2;
            routeTrainers.randomizedIndex += area2_dlc2;
            routeTrainers.randomizedIndex += area3_dlc2;
            routeTrainers.randomizedIndex += area4_dlc2;
            routeTrainers.randomizedIndex += epilogue;
        }
    }

    if(raidTrainers.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        raidTrainers.randomizedIndex = paldeaRaids;
        if(paldeaForAll == true){
            raidTrainers.randomizedIndex += kitakamiRaids;
            raidTrainers.randomizedIndex += blueberryRaids;
        }
    }

    if(paradisePokemon.randomize == true){
        paldea = true;
        trainersCanBeRandomized = true;
        paradisePokemon.randomizedIndex = paradaiseProtocol;
        if(paldeaForAll == true){
            paradisePokemon.randomizedIndex += terapagos;
        }
    }

    // Kitakami

    if(allKitakamiTrainers.randomize == true){
        kita = true;
        trainersCanBeRandomized = true;
        allKitakamiTrainers.randomizedIndex = area1_dlc1;
        allKitakamiTrainers.randomizedIndex += area2_dlc1;
        allKitakamiTrainers.randomizedIndex += area3_dlc1;
        allKitakamiTrainers.randomizedIndex += area4_dlc1;
        allKitakamiTrainers.randomizedIndex += area7_dlc1;
        allKitakamiTrainers.randomizedIndex += area8_dlc1;
        allKitakamiTrainers.randomizedIndex += area9_dlc1;
        allKitakamiTrainers.randomizedIndex += area10_dlc1;
        allKitakamiTrainers.randomizedIndex += kieran_dlc1;
        allKitakamiTrainers.randomizedIndex += carmine_dlc1;
        allKitakamiTrainers.randomizedIndex += kieran_stront_dlc1;
        allKitakamiTrainers.randomizedIndex += carmine_strong_dlc1;
        allKitakamiTrainers.randomizedIndex += ogreClan;
        allKitakamiTrainers.randomizedIndex += ogreClanBoss;
        allKitakamiTrainers.randomizedIndex += perrin;
        allKitakamiTrainers.randomizedIndex += oNareFamily;
    }

    if(kitakamiRivals.randomize == true){
        kita = true;
        trainersCanBeRandomized = true;
        kitakamiRivals.randomizedIndex = kieran_dlc1;
        kitakamiRivals.randomizedIndex += carmine_dlc1;
        kitakamiRivals.randomizedIndex += kieran_stront_dlc1;
        kitakamiRivals.randomizedIndex += carmine_strong_dlc1;
        kitakamiRivals.randomizedIndex += perrin;
        kitakamiRivals.randomizedIndex += oNareFamily;
    }

    if(ogreClanTrainers.randomize == true){
        kita = true;
        trainersCanBeRandomized = true;
        ogreClanTrainers.randomizedIndex = ogreClan;
        ogreClanTrainers.randomizedIndex += ogreClanBoss;
    }

    if(kitakamiRouteTrainers.randomize == true){
        kita = true;
        trainersCanBeRandomized = true;
        kitakamiRouteTrainers.randomizedIndex = area1_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area2_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area3_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area4_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area7_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area8_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area9_dlc1;
        kitakamiRouteTrainers.randomizedIndex += area10_dlc1;
    }

    if(kitakamiRaidTrainers.randomize == true){
        kita = true;
        trainersCanBeRandomized = true;
        kitakamiRaidTrainers.randomizedIndex = kitakamiRaids;
    }

    // Blueberry

    if(allBlueberryTrainers.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        allBlueberryTrainers.randomizedIndex = kieran;
        allBlueberryTrainers.randomizedIndex += carmine;
        allBlueberryTrainers.randomizedIndex += shiano;
        allBlueberryTrainers.randomizedIndex += schoolwars;
        allBlueberryTrainers.randomizedIndex += bb4DragonTrainers;
        allBlueberryTrainers.randomizedIndex += bb4FairyTrainers;
        allBlueberryTrainers.randomizedIndex += bb4FireTrainers;
        allBlueberryTrainers.randomizedIndex += bb4Fire;
        allBlueberryTrainers.randomizedIndex += bb4Steel;
        allBlueberryTrainers.randomizedIndex += bb4Farity;
        allBlueberryTrainers.randomizedIndex += bb4Dragon;
        allBlueberryTrainers.randomizedIndex += area1_dlc1;
        allBlueberryTrainers.randomizedIndex += area2_dlc1;
        allBlueberryTrainers.randomizedIndex += area3_dlc1;
        allBlueberryTrainers.randomizedIndex += area4_dlc1;
        allBlueberryTrainers.randomizedIndex += epilogue;
        allBlueberryTrainers.randomizedIndex += blueberryRaids;
    }

    if(blueberryRivals.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        blueberryRivals.randomizedIndex = kieran;
        blueberryRivals.randomizedIndex += carmine;
        blueberryRivals.randomizedIndex += shiano;
    }

    if(bb4Trainers.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        bb4Trainers.randomizedIndex = schoolwars;
        bb4Trainers.randomizedIndex += bb4DragonTrainers;
        bb4Trainers.randomizedIndex += bb4FairyTrainers;
        bb4Trainers.randomizedIndex += bb4FireTrainers;
        bb4Trainers.randomizedIndex += bb4Fire;
        bb4Trainers.randomizedIndex += bb4Steel;
        bb4Trainers.randomizedIndex += bb4Farity;
        bb4Trainers.randomizedIndex += bb4Dragon;
    }

    if(blueberryRouteTrainers.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        blueberryRouteTrainers.randomizedIndex = area1_dlc2;
        blueberryRouteTrainers.randomizedIndex += area2_dlc2;
        blueberryRouteTrainers.randomizedIndex += area3_dlc2;
        blueberryRouteTrainers.randomizedIndex += area4_dlc2;
        blueberryRouteTrainers.randomizedIndex += epilogue;
    }

    if(blueberryRaidTrainers.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        blueberryRaidTrainers.randomizedIndex = blueberryRaids;
    }

    if(hiddenTreasure.randomize == true){
        blueberry = true;
        trainersCanBeRandomized = true;
        hiddenTreasure.randomizedIndex = terapagos;
    }

    return trainersCanBeRandomized;
}
