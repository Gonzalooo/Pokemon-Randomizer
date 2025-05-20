#include "headers/sv_randomizer_headers/sv_starters.h"

svStarters::svStarters(){

}

svStarters::~svStarters(){

}

void svStarters::obtainSelectedStarters(int index, QString starterName, int form, QString gender, bool shiny, QString ball){
    int devid = 1;
    int formId = 0;
    int genderId = 0;
    bool rareId = false;

    if(!pokemonMapping["pokemons_name"].contains(starterName)){
        do{
            randomizePokemon(usableStarterPokemon, devid, formId, genderId, rareId, startersRates, startersMaps["values"][index]["pokeData"]);
        }while(softlockedStarters.contains(devid));

        if(shiny == true){
            startersMaps["values"][index]["pokeData"]["rareType"] = "RARE";
            rareId = true;
            qDebug()<<"Set Rare (overwrite) to: "<<rareId;
        }

        devIds.push_back(devid);
        forms.push_back(formId);
        genders.push_back(genderId);
        rares.push_back(rareId);

        startersMaps["values"][index]["pokeData"]["ballId"] = pokeballDevNames[ball][0].toStdString();

        randomizeTeraType(startersMaps["values"][index]["pokeData"], randomizeStartersTeraTypes, devid, formId);
    }else{
        std::string starterNameStd = starterName.toUtf8().constData();
        std::string genderStd = gender.toStdString();
        devid = int(pokemonMapping["pokemons_name"][starterNameStd]["devid"]);
        formId = form;

        startersMaps["values"][index]["pokeData"]["devId"] = pokemonMapping["pokemons_name"][starterNameStd]["devName"];
        startersMaps["values"][index]["pokeData"]["formId"] = formId;

        qDebug()<<"Set Pokemon to: "<<starterNameStd;
        qDebug()<<"Set Form to: "<<formId;

        devIds.push_back(devid);
        forms.push_back(formId);

        // Get Gender
        if(genderStd == "MALE" || genderStd == "GENDERLESS"){
            genders.push_back(0);
            genderId = 0;
            if(genderStd == "GENDERLESS")
                genderStd = "DEFAULT";
        }else if(genderStd == "FEMALE"){
            genders.push_back(1);
            genderId = 1;
        } else if(genderStd == "DEFAULT"){
            if(form == 0){
                genders.push_back(0);
                genderId = 0;
                genderStd = "MALE";
            }else{
                genders.push_back(1);
                genderId = 1;
                genderStd = "FEMALE";
            }
        }
        qDebug()<<"Set Gender to: "<<genderStd;

        // Get Shiny
        if(shiny == true){
            startersMaps["values"][index]["pokeData"]["rareType"] = "RARE";
            rares.push_back(true);
            rareId = true;
        }else{
            startersMaps["values"][index]["pokeData"]["rareType"] = "NO_RARE";
            rares.push_back(false);
            rareId = false;
        }

        qDebug()<<"Set Rare to: "<<rareId;

        // Set Tera Type - ["values"][index]["pokeData"]
        randomizeTeraType(startersMaps["values"][index]["pokeData"], randomizeStartersTeraTypes, devid, formId);

        startersMaps["values"][index]["pokeData"]["ballId"] = pokeballDevNames[ball][0].toStdString();
        qDebug()<<"Set Poke Ball To: "<< std::string(startersMaps["values"][index]["pokeData"]["ballId"]);
    }

    QString keyVal = "";
    if(index == 0){
        keyVal = "hono";
    }else if(index == 1){
        keyVal = "kusa";
    }else if(index == 2){
        keyVal = "mizu";
    }

    startersMaps["values"][index]["pokeData"]["item"] = getPokemonItemId(pokemonMapping["pokemons"][int(pokemonMapping["pokemons_devid"][devid]["natdex"])]["natdex"], formId);
    currectlySelectedStarters[keyVal]["id"] = pokemonMapping["pokemons"][int(pokemonMapping["pokemons_devid"][devid]["natdex"])]["devid"];
    currectlySelectedStarters[keyVal]["form"] = formId;
    currectlySelectedStarters[keyVal]["gender"] = genderId;
    currectlySelectedStarters[keyVal]["rare"] = (rareId) ? 1 : 0;
    currectlySelectedStarters[keyVal]["gem"] = getTeraTypeInt(std::string(startersMaps["values"][index]["pokeData"]["gemType"]));
    qDebug()<<"Saved "<<keyVal<<"(Devid:  "<< currectlySelectedStarters[keyVal]["id"]
             <<") - (Form: "<<currectlySelectedStarters[keyVal]["form"] << ") - (Gender: "<< currectlySelectedStarters[keyVal]["gender"]
             <<") - (Rare: "<<currectlySelectedStarters[keyVal]["rare"] << ") - (Tera: "<<currectlySelectedStarters[keyVal]["gem"] <<")";
}

void svStarters::randomize(QRandomGenerator* r){
    startersMaps = readJsonQFile("SV_FLATBUFFERS/SV_STARTERS_FLATBUFFERS/eventAddPokemon_array_clean.json");
    localRand = r;
    setRandNum(localRand);

    if(randomizeStarters == true){
        if(allStartersShiny == true){
            startersShiny[0] = true;
            startersShiny[1] = true;
            startersShiny[2] = true;
        }else if(forceShinyStarter == true){
            int force = localRand->bounded(0, 3);
            qDebug()<<"Started Forced to be Shiny: "<<force;
            startersShiny[force] = true;
        }

        qDebug()<<"List of Allowed Pokemon for Starters: "<<startersPokemon.toString();

        qDebug()<<"Getting Allowed Pokemon for Starters";
        bool sizeCheck = getAllowedPokemon(startersPokemon, usableStarterPokemon, "Starters");
        if(sizeCheck == false){
            qFatal()<<"Not Enough usable Pokemon for Starters";
        }
        // Sprigattito
        qDebug()<<"Randomizing Sprigatito";
        obtainSelectedStarters(1, starters[0], pokemonFormsIntsInGame[starters[0]][startersForms[0]], startersGenders[0], startersShiny[0], startersPokeballs[0]);

        // Quaxly
        qDebug()<<"Randomizing Quaxly";
        obtainSelectedStarters(2, starters[2], pokemonFormsIntsInGame[starters[2]][startersForms[2]], startersGenders[2], startersShiny[2], startersPokeballs[2]);

        // Fuecoco
        qDebug()<<"Randomizing Fuecoco";
        obtainSelectedStarters(0, starters[1], pokemonFormsIntsInGame[starters[1]][startersForms[1]], startersGenders[1], startersShiny[1], startersPokeballs[1]);

        QList<QPair<QString, QString>> files = {
            {qBaseDir.filePath("SV_SCENES/trscn.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/d030")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main")},
        };

        QList<QString> outputs = {
            "world/scene/parts/demo/ev/d030_/",
            "world/scene/parts/event/event_scenario/main_scenario/common_0060_/",
            "world/scene/parts/event/event_scenario/main_scenario/common_0060_/",
            "world/scene/parts/event/event_scenario/main_scenario/common_0070_/",
            "world/scene/parts/event/event_scenario/main_scenario/common_0088_/",
            "world/scene/parts/event/event_scenario/main_scenario/common_0090_/"
        };

        qDebug()<<"Patching Starter Scene Files";
        patchScenes(files, outputs, devIds, forms, genders, rares);

        devIds.clear();
        forms.clear();
        genders.clear();
        rares.clear();
    }

    // For later
    if(randomizeGifts == true){
        qDebug()<<"List of Allowed Pokemon for Gifts: "<<giftsPokemon.toString();

        qDebug()<<"Getting Allowed Pokemon for Gifts";
        bool sizeCheck = getAllowedPokemon(giftsPokemon, usableGiftPokemon, "Gifts");
        if(sizeCheck == false){
            qFatal()<<"Not Enough usable Pokemon for Kitakami Wilds";
        }

        int devid = 1;
        int formId = 0;
        int genderId = 0;
        bool rareId = false;


        for(unsigned long long i = 3; i<startersMaps["values"].size(); i++){
            qDebug()<<"Randomizing "<<std::string(pokemonMapping["pokemons_dev"][std::string(startersMaps["values"][i]["pokeData"]["devId"])]["name"]) << " Gift";
            randomizePokemon(usableStarterPokemon, devid, formId, genderId, rareId, startersRates, startersMaps["values"][i]["pokeData"]);
            randomizeTeraType(startersMaps["values"][i]["pokeData"], randomizeGiftsTeraTypes, devid, formId);
            if(i == 8){
                devIds.push_back(devid);
                forms.push_back(formId);
                genders.push_back(genderId);
                rares.push_back(rareId);
            }
        }

        QList<QPair<QString, QString>> files = {
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s1_side02_0000_always")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s1_side02_0030_always")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s1_side02_0060_pre_start")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s2_side02_0000_pre_start")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s2_side02_0005_pre_start")},
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Growlithe/s2_side02_0030_always")},
        };

        QList<QString> outputs = {
            "world/scene/parts/event/event_scenario/sub_scenario/s1_side02_0000_/",
            "world/scene/parts/event/event_scenario/sub_scenario/s1_side02_0030_/",
            "world/scene/parts/event/event_scenario/sub_scenario/s1_side02_0060_/",
            "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0000_/",
            "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0005_/",
            "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0030_/"
        };

        qDebug()<<"Patching Growlithe Scene Files";
        patchScenes(files, outputs, devIds, forms, genders, rares);

        devIds.clear();
        forms.clear();
        genders.clear();
        rares.clear();
    }

    // Write the modified JSON back to the output file
    QFile outFile(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json"));
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qFatal() << "Error: Could not open output file for writing!";
        return;
    }

    QTextStream outStream(&outFile);
    outStream << QString::fromStdString(startersMaps.dump(2)); // Pretty print JSON
    outFile.close();

    generateBinary(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.bfbs").toStdString(),
                   qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json").toStdString(),
                   "world/data/event/event_add_pokemon/eventAddPokemon/", true);

    QDir().remove(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json"));
}
