#include <headers/sv_randomizer_headers/sv_raids.h>


svRaids::svRaids(){

}

svRaids::~svRaids(){

}

void svRaids::randomize(QStringList paths, QStringList clean, QStringList schema, QStringList folder, QString region,
                        QRandomGenerator* r){

    localRand = r;
    setRandNum(localRand);
    allowedPokemonLimiter raidPokemon;
    int rate = 10;
    int forceShiny = false;

    if(region == "Paldea" || paldeaForAll == true){
        raidPokemon = paldeaPokemon;
        rate = shinyRatePaldea;
        forceShiny = forceShinyRaidsPaldea;
    } else if(region == "Kitakami"){
        raidPokemon = kitakamiPokemon;
        rate = shinyRateKitakami;
        forceShiny = forceShinyRaidsKitakami;
    } else if(region == "Blueberry"){
        raidPokemon = blueberryPokemon;
        rate = shinyRateBlueberry;
        forceShiny = forceShinyRaidsBlueberry;
    }

    bool sizeCheck = getAllowedPokemon(raidPokemon, allowedPokemon, region+"-Raids");

    if(sizeCheck == false){
        qFatal()<<QString("Not Enough usable Pokemon for %1 Raids").arg(region);
    }

    for(unsigned int i =0; i<clean.size(); i++){
        json raids = readJsonQFile("SV_FLATBUFFERS/"+clean[i]);

        json newRaids;
        newRaids["values"] = json::array();
        QList<int> usedPokemon;
        QList<QMap<QString, int>> usedForms = {{{"id", 0}, {"form", 0}}};

        unsigned long long totalCount = allowedPokemon.size(); // Count of keys
        for (QMap<int, QList<int>>::const_iterator it = allowedPokemon.cbegin(); it != allowedPokemon.cend(); ++it) {
            totalCount += it.value().size(); // Accessing values without detaching
        }

        unsigned long long checkTotal = raids["values"].size();
        if(totalCount < raids["values"].size())
            checkTotal = totalCount;

        for(unsigned long long j =0; j<checkTotal; j++){
            QMap<QString, int> checkDict;

            int random = 1;
            int form = 0;
            int gender = 0;
            bool rare = false;
            bool check_loop = true;
            do{
                randomizePokemon(allowedPokemon, random, form, gender, rare, rate, raids["values"][i]["raidEnemyInfo"]["bossPokePara"]);
                checkDict = {{"id", int(random)}, {"form", int(form)}};
                if(!usedForms.contains(checkDict)){
                    if(!exitAbilitiesPokemon.contains(checkDict)){
                        usedForms.append(checkDict);
                        check_loop = false;
                    }
                }

                // Get new pokemon if its Terapagos
                if(int(pokemonMapping["pokemons_devid"][random]["natdex"])==1024){
                    check_loop=true;
                }

            }while(check_loop == true);

            // Check for pokemon Appending
            int check = 0;
            for(int k =0; k<usedForms.size(); k++){
                if(usedForms[k]["id"] == checkDict["id"]){
                    check++;
                }
            }

            int formTotal = 0;
            for(unsigned long long k =0; k < pokemonMapping["pokemons_devid"][random]["forms"].size(); k++){
                if(pokemonMapping["pokemons_devid"][random]["forms"][k]["is_present"] == true){
                    formTotal++;
                }
            }

            if(check == formTotal){
                usedPokemon.append(random);
            }

            // Data Changes
            raids["values"][j]["raidEnemyInfo"]["romVer"] = "BOTH";
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["item"] = getPokemonItemId(int(pokemonMapping["pokemons_devid"][random]["natdex"]), form);
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["wazaType"] = "DEFAULT";
            if(raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] == "NO_RARE"){
                raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "DEFAULT";
            }
            if(forceShiny == true){
                raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
            }
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza1"]["wazaId"] = "WAZA_NULL";
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza2"]["wazaId"] = "WAZA_NULL";
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza3"]["wazaId"] = "WAZA_NULL";
            raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza4"]["wazaId"] = "WAZA_NULL";

            // Removing Timer Moves and changing with Terablast usage
            raids["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["timming"] = "HP";
            raids["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["action"] = "WAZA";
            raids["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["value"] = int(85);

            for(int i = 2; i<=6; i++){
                std::string key = "extraAction"+std::to_string(i);

                if(raids["values"][j]["raidEnemyInfo"]["bossDesc"][key]["timming"] == "NONE"){
                    continue;
                }


                if(raids["values"][j]["raidEnemyInfo"]["bossDesc"][key]["action"] == "WAZA"){
                    raids["values"][j]["raidEnemyInfo"]["bossDesc"][key]["wazano"] = "WAZA_TERABAASUTO";
                }

            }

            // Ogerpon Hardset of TeraType
            if(int(pokemonMapping["pokemons_devid"][random]["natdex"]) == 1017){
                if(form == 0){
                    raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "KUSA";
                }
                if(form == 1){
                    raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "MIZU";
                }
                if(form == 2){
                    raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "HONOO";
                }
                if(form == 3){
                    raids["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "IWA";
                }
            }

            newRaids["values"].push_back(raids["values"][j]);

        }

        closeFileAndDelete(paths[i],
                           schema[i],
                           folder[i],
                           raids, false);
    }

}
