/*
 * soundfx.cpp
 * GnollHack Sound Effects
 *
 * Copyright (c) Janne Gustafsson, 2020
 *
 */

#include <windows.h>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "resource.h"

using namespace FMOD;
static System* fmod_system = (System*)0;
static Studio::System* fmod_studio_system = (Studio::System*)0;
static Sound* sound1 = (Sound*)0;
static Sound* sound2 = (Sound*)0;
static Sound* sound3 = (Sound*)0;

extern "C" 
{
    boolean
    initialize_fmod()
    {
        FMOD_RESULT result;
        unsigned int version;
        void* extradriverdata = 0;

        result = System_Create(&fmod_system);
        result = fmod_system->getVersion(&version);

        if (version < FMOD_VERSION)
        {
//            Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
            return FALSE;
        }

        result = fmod_system->init(32, FMOD_INIT_NORMAL, extradriverdata);
        return TRUE;
    }

    void
    fmod_play_sound_example()
    {
        if (!fmod_system)
            return;

        Channel* channel = 0;
        FMOD_RESULT       result;

        //result = fmod_system->createSound("C:\\Users\\janne\\Test\\Sound Test\\Tower-Defense.wav", FMOD_DEFAULT, 0, &sound2);
        //result = sound2->setMode(FMOD_LOOP_NORMAL);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
        //result = fmod_system->playSound(sound2, 0, false, &channel);

        Sleep(500);

        result = fmod_system->createSound("C:\\Users\\janne\\Test\\Sound Test\\PowerUp20.wav", FMOD_DEFAULT, 0, &sound1);
        result = sound1->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
        result = fmod_system->playSound(sound1, 0, false, &channel);

        Sleep(500);

        HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);
        HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(IDR_RCDATA_OGG), RT_RCDATA);

        if (res)
        {
            HGLOBAL mem = LoadResource(hResInstance, res);
            void* data = LockResource(mem);
            size_t len = SizeofResource(hResInstance, res);

            FMOD_CREATESOUNDEXINFO exinfo;
            memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
            exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
            exinfo.length = (unsigned int)len;

            result = fmod_system->createSound((const char*)data, FMOD_OPENMEMORY, &exinfo, &sound3);
            result = sound3->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
            result = fmod_system->playSound(sound3, 0, false, &channel);
        }
    }



    boolean
    initialize_fmod_studio()
    {
        void* extraDriverData = NULL;

        Studio::System::create(&fmod_studio_system);

        fmod_studio_system->getCoreSystem(&fmod_system);
        fmod_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0);

        FMOD_RESULT result = fmod_studio_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

        return (result == FMOD_OK);
    }

    void
    fmod_event_example()
    {
        FMOD_RESULT       result;
        FMOD::Studio::Bank* bank[3] = { 0 };
        HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);

        for (int i = 0; i < 3; i++)
        {
            int rid[3] = { IDR_RCDATA_MASTER, IDR_RCDATA_STRINGS, IDR_RCDATA_BANK };
            HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(rid[i]), RT_RCDATA);

            if (res)
            {
                HGLOBAL mem = LoadResource(hResInstance, res);
                void* data = LockResource(mem);
                size_t len = SizeofResource(hResInstance, res);

                result = fmod_studio_system->loadBankMemory((const char*)data, (int)len, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank[i]);
            }
        }

        Studio::EventDescription* footstepDescription = NULL;
        result = fmod_studio_system->getEvent("event:/Character/Player Footsteps", &footstepDescription);

        Studio::EventInstance* footstepInstance = NULL;
        result = footstepDescription->createInstance(&footstepInstance);

        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        result = footstepDescription->getParameterDescriptionByName("Surface", &paramDesc);

        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;
        float surfaceParameterValue = 1.0f;
        result = footstepInstance->setParameterByID(surfaceID, surfaceParameterValue);

        for(int i = 0; i < 12; i++)
        {
            result = footstepInstance->start();

            if((i % 3) == 0)
                surfaceParameterValue = surfaceParameterValue + 1.0f;

            if (surfaceParameterValue > paramDesc.maximum)
                surfaceParameterValue = paramDesc.minimum;

            result = footstepInstance->setParameterByID(surfaceID, surfaceParameterValue);
            result = fmod_studio_system->update();
            Sleep(300);
        }

        footstepInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        for(int i = 0; i < 3;i++)
            bank[i]->unload();

        result = fmod_studio_system->release();
        fmod_studio_system = (Studio::System*)0;

    }

    void
    fmod_event_example_from_file()
    {
        FMOD_RESULT result;
        FMOD::Studio::Bank* masterBank = NULL;
        fmod_studio_system->loadBankFile("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
        FMOD::Studio::Bank* stringsBank = NULL;
        fmod_studio_system->loadBankFile("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);
        FMOD::Studio::Bank* sfxBank = NULL;
        fmod_studio_system->loadBankFile("SFX.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &sfxBank);

        int ecnt = 0;
        result = sfxBank->getEventCount(&ecnt);

        int ewritecnt = 0;
        Studio::EventDescription* ed_array[50];

        result = sfxBank->getEventList(ed_array, 50, &ewritecnt);

        FMOD_STUDIO_LOADING_STATE lstate;
        result = sfxBank->getLoadingState(&lstate);

        for (int i = 0; i < ewritecnt; i++)
        {
            Studio::EventDescription* ed = ed_array[i];
            char buf[512];
            int retr = 0;
            result = ed->getPath(buf, 512, &retr);
            result = result;
        }
        Studio::EventDescription* eventDescription = NULL;
        result = fmod_studio_system->getEvent("event:/Character/Player Footsteps", &eventDescription);

        // Find the parameter once and then set by handle
        // Or we can just find by name every time but by handle is more efficient if we are setting lots of parameters
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        eventDescription->getParameterDescriptionByName("Surface", &paramDesc);
        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;

        FMOD::Studio::EventInstance* eventInstance = NULL;
        eventDescription->createInstance(&eventInstance);

        // Make the event audible to start with
        float surfaceParameterValue = 1.0f;
        eventInstance->setParameterByID(surfaceID, surfaceParameterValue);

        eventInstance->start();

        for (int i = 0; i < 4; i++)
        {
            surfaceParameterValue = surfaceParameterValue + 1.0f;
            if (surfaceParameterValue > paramDesc.maximum)
                surfaceParameterValue = paramDesc.minimum;

            eventInstance->setParameterByID(surfaceID, surfaceParameterValue);
            fmod_studio_system->update();
            Sleep(200);
        }

        eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        sfxBank->unload();
        fmod_studio_system->release();
        fmod_studio_system = (Studio::System*)0;
    }


    void fmod_orig_example()
    {
#define ERRCHECK(x) x

            void* extraDriverData = NULL;
            //Common_Init(&extraDriverData);

            FMOD::Studio::System* system = NULL;
            ERRCHECK(FMOD::Studio::System::create(&system));

            // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
            FMOD::System* coreSystem = NULL;
            ERRCHECK(system->getCoreSystem(&coreSystem));
            ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

            ERRCHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData));

            FMOD::Studio::Bank* masterBank = NULL;
            ERRCHECK(system->loadBankFile("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

            FMOD::Studio::Bank* stringsBank = NULL;
            ERRCHECK(system->loadBankFile("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

            FMOD::Studio::Bank* sfxBank = NULL;
            ERRCHECK(system->loadBankFile("SFX.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &sfxBank));

            // Get the Looping Ambience event
            FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
            ERRCHECK(system->getEvent("event:/Ambience/Country", &loopingAmbienceDescription));

            FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;
            ERRCHECK(loopingAmbienceDescription->createInstance(&loopingAmbienceInstance));

            // Get the 4 Second Surge event
            FMOD::Studio::EventDescription* cancelDescription = NULL;
            ERRCHECK(system->getEvent("event:/UI/Cancel", &cancelDescription));

            FMOD::Studio::EventInstance* cancelInstance = NULL;
            ERRCHECK(cancelDescription->createInstance(&cancelInstance));

            // Get the Single Explosion event
            FMOD::Studio::EventDescription* explosionDescription = NULL;
            ERRCHECK(system->getEvent("event:/Weapons/Explosion", &explosionDescription));

            // Start loading explosion sample data and keep it in memory
            ERRCHECK(explosionDescription->loadSampleData());

                    // One-shot event
                    FMOD::Studio::EventInstance* eventInstance = NULL;
                    ERRCHECK(explosionDescription->createInstance(&eventInstance));

                    ERRCHECK(eventInstance->start());
                    Sleep(500);
                    // Release will clean up the instance when it completes
                    ERRCHECK(eventInstance->release());

                    ERRCHECK(loopingAmbienceInstance->start());
                    ERRCHECK(system->update());
                    Sleep(500);
                    ERRCHECK(loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE));

                    ERRCHECK(cancelInstance->start());
                    ERRCHECK(system->update());
                    Sleep(500);

    
            ERRCHECK(sfxBank->unload());
            ERRCHECK(stringsBank->unload());
            ERRCHECK(masterBank->unload());
            ERRCHECK(system->release());

    }


    void
    fmod_stop_all_sounds()
    {
        FMOD_RESULT result;
        if (sound1)
        {
            result = sound1->release();
            sound1 = (Sound*)0;
        }
        if (sound2)
        {
            result = sound2->release();
            sound2 = (Sound*)0;
        }
        if (sound3)
        {
            result = sound3->release();
            sound3 = (Sound*)0;
        }
        if (fmod_system)
        {
            result = fmod_system->release();
            fmod_system = (System*)0;
        }
        if (fmod_studio_system)
        {
            result = fmod_studio_system->release();
            fmod_studio_system = (Studio::System*)0;
        }
    }

}

/* soundfx.cpp */
