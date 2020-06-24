/*
 * soundfx.cpp
 * GnollHack for Windows Sound Effects using FMOD
 *
 * Copyright (c) Janne Gustafsson, 2020
 *
 */

#include <windows.h>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "resource.h"
#include "soundset.h"

using namespace FMOD;
static System* fmod_core_system = (System*)0;
static Studio::System* fmod_studio_system = (Studio::System*)0;

struct GNHSoundInstance {
    Studio::EventInstance* eventInstance;
    enum ghsound_types ghsound;
    float normalVolume;
    struct GNHSoundInstance* next_instance;
};

static GNHSoundInstance musicInstances[2] = { 0 };
static GNHSoundInstance movementInstances[2] = { 0 };
static GNHSoundInstance hitInstances[2] = { 0 };

static float general_volume = 1.0f;
static float general_music_volume = 1.0f;
static float general_sound_effects_volume = 1.0f;

GNHSoundInstance* ambient_base = NULL;


/* GHSound -> FMOD event mapping here */
enum sound_bank_types {
    SOUND_BANK_NONE = 0,
    SOUND_BANK_MASTER,
    MAX_SOUND_BANKS
};

struct sound_bank_definition {
    boolean isresource;
    int resource_id;
    const char* filename;
};


struct ghsound_eventmapping {
    enum sound_bank_types bank_id;
    const char* eventPath;
    int within_event_id;
    float volume;
};


#define NoSound { SOUND_BANK_NONE,  "", 0, 0.0f}

const ghsound_eventmapping ghsound2event[MAX_GHSOUNDS] = {
    { SOUND_BANK_NONE, "" , 0, 0.0f },
    { SOUND_BANK_MASTER, "event:/Music-Normal-1", 0, 1.0f},
    { SOUND_BANK_MASTER, "event:/Player Footsteps", 0, 1.0f},
    NoSound,
    NoSound,
    NoSound,
    NoSound,
    NoSound,
    NoSound,
    NoSound,
    { SOUND_BANK_MASTER, "event:/Fountain Ambient" , 0, 1.0f},
    { SOUND_BANK_MASTER, "event:/Bee Ambient" , 0, 1.0f},
    { SOUND_BANK_MASTER, "event:/Fire Ambient" , 0, 1.0f}
};

#undef NoSound


extern "C" 
{
    boolean
    initialize_fmod_studio()
    {
        void* extraDriverData = NULL;

        Studio::System::create(&fmod_studio_system);

        fmod_studio_system->getCoreSystem(&fmod_core_system);
        fmod_core_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0);

        FMOD_RESULT result = fmod_studio_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

        boolean res = (result == FMOD_OK);
        return res;
    }

    boolean
    load_fmod_banks()
    {
        FMOD_RESULT       result;
        FMOD::Studio::Bank* bank[2] = { 0 };
        HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);

        for (int i = 0; i < 2; i++)
        {
            int rid[2] = { IDR_RCDATA_MASTER, IDR_RCDATA_STRINGS };
            HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(rid[i]), RT_RCDATA);

            if (res)
            {
                HGLOBAL mem = LoadResource(hResInstance, res);
                void* data = LockResource(mem);
                size_t len = SizeofResource(hResInstance, res);

                result = fmod_studio_system->loadBankMemory((const char*)data, (int)len, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank[i]);
                if (result != FMOD_OK)
                    return FALSE;
            }
        }

        return TRUE;
    }

    void
    close_fmod_studio()
    {
        FMOD_RESULT result;

        result = fmod_studio_system->unloadAll();
        result = fmod_studio_system->release();

        fmod_studio_system = (Studio::System*)0;
    }

    boolean 
    fmod_play_music(struct ghsound_music_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;

        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* musicDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &musicDescription);

        if (result != FMOD_OK)
            return FALSE;

        if (musicInstances[0].eventInstance)
        {
            Studio::EventInstance* earlierMusicInstance = musicInstances[0].eventInstance;
            Studio::EventDescription* earlierMusicDescription = NULL;
            earlierMusicInstance->getDescription(&earlierMusicDescription);
            if (earlierMusicDescription && musicDescription == earlierMusicDescription)
            {
                /* Already playing, no need to do anything, unless so specified */
                return 1;
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* musicInstance = NULL;
        result = musicDescription->createInstance(&musicInstance);
        if (result != FMOD_OK)
            return FALSE;

        musicInstance->setVolume(info.volume * event_volume * general_music_volume * general_volume);
        result = musicInstance->start();
        if (result != FMOD_OK)
            return FALSE;

        /* If starting succeeded, stop the old music in musicInstances[0] by fading */
        if (musicInstances[0].eventInstance)
        {
            musicInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to musicInstances[1] for later release, and before that, release existing musicInstances[1] */
            if (musicInstances[1].eventInstance)
                musicInstances[1].eventInstance->release();

            musicInstances[1].eventInstance = musicInstances[0].eventInstance;
            musicInstances[1].ghsound = musicInstances[0].ghsound;
            musicInstances[1].normalVolume = musicInstances[0].normalVolume;
        }

        /* Set the new instance as musicInstances[0] */
        musicInstances[0].eventInstance = musicInstance;
        musicInstances[0].ghsound = info.ghsound;
        musicInstances[0].normalVolume = info.volume;

        result = fmod_studio_system->update();
        return TRUE;
    }


    boolean
    fmod_play_movement_sound(struct ghsound_movement_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;
        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* movementDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &movementDescription);

        if (result != FMOD_OK)
            return FALSE;

        Studio::EventInstance* movementInstance = NULL;
        result = movementDescription->createInstance(&movementInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Set volume */
        movementInstance->setVolume(info.volume * event_volume * general_sound_effects_volume * general_volume);

        /* Set surface */
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        result = movementDescription->getParameterDescriptionByName("Surface", &paramDesc);

        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;
        float surfaceParameterValue = (float)info.floor;
        result = movementInstance->setParameterByID(surfaceID, surfaceParameterValue);

        /* Play sound */
        result = movementInstance->start();
        if (result != FMOD_OK)
            return FALSE;

        /* If starting succeeded, stop the old music in musicInstances[0] by fading */
        if (movementInstances[0].eventInstance)
        {
            movementInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to movementInstances[1] for later release, and before that, release existing movementInstances[1] */
            if (movementInstances[1].eventInstance)
                movementInstances[1].eventInstance->release();

            movementInstances[1].eventInstance = movementInstances[0].eventInstance;
            movementInstances[1].ghsound = movementInstances[0].ghsound;
            movementInstances[1].normalVolume = movementInstances[0].normalVolume;
        }

        /* Set the new instance as movementInstances[0] */
        movementInstances[0].eventInstance = movementInstance;
        movementInstances[0].ghsound = info.ghsound;
        movementInstances[0].normalVolume = info.volume;

        result = fmod_studio_system->update();
        return TRUE;
    }


    boolean
    fmod_play_hit_sound(struct ghsound_hit_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;
        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* hitDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &hitDescription);

        if (result != FMOD_OK)
            return FALSE;

        Studio::EventInstance* hitInstance = NULL;
        result = hitDescription->createInstance(&hitInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Set volume */
        hitInstance->setVolume(info.volume * event_volume * general_sound_effects_volume * general_volume);

        /* Set surface */
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        result = hitDescription->getParameterDescriptionByName("Surface", &paramDesc);

        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;
        float surfaceParameterValue = (float)info.strike_surface;
        result = hitInstance->setParameterByID(surfaceID, surfaceParameterValue);

        /* Play sound */
        result = hitInstance->start();
        if (result != FMOD_OK)
            return FALSE;

        /* If starting succeeded, stop the old music in musicInstances[0] by fading */
        if (hitInstances[0].eventInstance)
        {
            hitInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to movementInstances[1] for later release, and before that, release existing movementInstances[1] */
            if (hitInstances[1].eventInstance)
                hitInstances[1].eventInstance->release();

            hitInstances[1].eventInstance = hitInstances[0].eventInstance;
            hitInstances[1].ghsound = hitInstances[0].ghsound;
            hitInstances[1].normalVolume = hitInstances[0].normalVolume;
        }

        /* Set the new instance as movementInstances[0] */
        hitInstances[0].eventInstance = hitInstance;
        hitInstances[0].ghsound = info.ghsound;
        hitInstances[0].normalVolume = info.volume;

        result = fmod_studio_system->update();
        return TRUE;
    }


    int
    fmod_adjust_ghsound_general_volumes(float new_general_volume, float new_general_music_volume, float new_general_effects_volume)
    {
        general_volume = new_general_volume;
        general_music_volume = new_general_music_volume;
        general_sound_effects_volume = new_general_effects_volume;

        FMOD_RESULT result;
        for (int i = 0; i <= 1; i++)
        {
            if (musicInstances[i].eventInstance)
            {
                enum ghsound_types soundid = musicInstances[i].ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = musicInstances[i].eventInstance->setVolume(musicInstances[i].normalVolume * event_volume * general_music_volume * general_volume);
            }
        }
        for (int i = 0; i <= 1; i++)
        {
            if (movementInstances[i].eventInstance)
            {
                enum ghsound_types soundid = musicInstances[i].ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = movementInstances[i].eventInstance->setVolume(movementInstances[i].normalVolume * event_volume * general_sound_effects_volume * general_volume);
            }
        }

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        return TRUE;
    }

    int
    fmod_add_ambient_ghsound(enum ghsound_types ghsound, float fmod_volume, void** ambient_sound_ptr_ptr)
    {
        FMOD_RESULT result;

        struct ghsound_eventmapping eventmap = ghsound2event[ghsound];
        float event_volume = eventmap.volume;
        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* ambientDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &ambientDescription);

        if (result != FMOD_OK)
            return FALSE;

        Studio::EventInstance* ambientInstance = NULL;
        result = ambientDescription->createInstance(&ambientInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Set volume */
        ambientInstance->setVolume(fmod_volume * event_volume * general_sound_effects_volume * general_volume);

        /* Create new GHSoundInstance */
        GNHSoundInstance* new_ghs_instance = (GNHSoundInstance*)malloc(sizeof(GNHSoundInstance));
        if (!new_ghs_instance)
        {
            result = ambientInstance->release();
            return FALSE;
        }

        /* Play sound */
        result = ambientInstance->start();
        if (result != FMOD_OK)
        {
            result = ambientInstance->release();
            free(new_ghs_instance);
            return FALSE;
        }

        memset((void*)new_ghs_instance, 0, sizeof(new_ghs_instance));
        new_ghs_instance->next_instance = ambient_base;
        ambient_base = new_ghs_instance;
        
        new_ghs_instance->eventInstance = ambientInstance;
        new_ghs_instance->ghsound = ghsound;
        new_ghs_instance->normalVolume = fmod_volume;

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        *ambient_sound_ptr_ptr = (void*)new_ghs_instance;

        return TRUE;
    }

    int
    fmod_delete_ambient_ghsound(void* ambient_sound_ptr)
    {
        if (!ambient_sound_ptr)
            return FALSE;

        FMOD_RESULT result;
        boolean found = FALSE;
        GNHSoundInstance* ghs_ptr = (GNHSoundInstance*)ambient_sound_ptr;
        for (GNHSoundInstance* ghs_curr = ambient_base; ghs_curr; ghs_curr = ghs_curr->next_instance)
        {
            if (ghs_curr == ghs_ptr)
            {
                found = TRUE;
                break;
            }
        }

        if (!found)
            return FALSE;

        if (!ghs_ptr->eventInstance)
            return FALSE;

        result = ghs_ptr->eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        if (result != FMOD_OK)
            return FALSE;

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        result = ghs_ptr->eventInstance->release();
        if (result != FMOD_OK)
            return FALSE;

        GNHSoundInstance* prev = NULL;
        for (GNHSoundInstance* curr = ambient_base; curr; curr = curr->next_instance)
        {
            if (curr == ghs_ptr)
            {
                if (prev == NULL)
                    ambient_base = curr->next_instance;
                else
                    prev->next_instance = curr->next_instance;

                break;
            }
            prev = curr;
        }
        
        free(ghs_ptr);

        return TRUE;
    }

    int
    fmod_set_ambient_ghsound_volume(void* ambient_sound_ptr, float fmod_volume)
    {
        if (!ambient_sound_ptr)
            return FALSE;

        if (fmod_volume < 0.0f || fmod_volume > 1.0f)
            return FALSE;

        FMOD_RESULT result;
        GNHSoundInstance* ghs_ptr = (GNHSoundInstance*)ambient_sound_ptr;
        if (!ghs_ptr->eventInstance)
            return FALSE;

        float old_volume;
        result = ghs_ptr->eventInstance->getVolume(&old_volume);
        if (result != FMOD_OK)
            return FALSE;

        enum ghsound_types soundid = ghs_ptr->ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;
        result = ghs_ptr->eventInstance->setVolume(fmod_volume * event_volume * general_sound_effects_volume * general_volume);
        if (result != FMOD_OK)
            return FALSE;

        if (old_volume == 0.0f && fmod_volume > 0.0f)
            result = ghs_ptr->eventInstance->start();
        else if (old_volume > 0.0f && fmod_volume == 0.0f)
            result = ghs_ptr->eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

        if (result != FMOD_OK)
            return FALSE;

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        ghs_ptr->normalVolume = fmod_volume;

        return TRUE;
    }

#if 0
    /* OBSOLETE - Examples and core */
    static Sound* sound1 = (Sound*)0;
    static Sound* sound2 = (Sound*)0;
    static Sound* sound3 = (Sound*)0;

    boolean
    initialize_fmod_core()
    {
        FMOD_RESULT result;
        unsigned int version;
        void* extradriverdata = 0;

        result = System_Create(&fmod_core_system);
        result = fmod_core_system->getVersion(&version);

        if (version < FMOD_VERSION)
        {
            //            Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
            return FALSE;
        }

        result = fmod_core_system->init(32, FMOD_INIT_NORMAL, extradriverdata);
        return TRUE;
    }

    void
        fmod_play_sound_example()
    {
        if (!fmod_core_system)
            return;

        Channel* channel = 0;
        FMOD_RESULT       result;

        //result = fmod_system->createSound("C:\\Users\\janne\\Test\\Sound Test\\Tower-Defense.wav", FMOD_DEFAULT, 0, &sound2);
        //result = sound2->setMode(FMOD_LOOP_NORMAL);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
        //result = fmod_system->playSound(sound2, 0, false, &channel);

        Sleep(500);

        result = fmod_core_system->createSound("C:\\Users\\janne\\Test\\Sound Test\\PowerUp20.wav", FMOD_DEFAULT, 0, &sound1);
        result = sound1->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
        result = fmod_core_system->playSound(sound1, 0, false, &channel);

        Sleep(500);

        HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);
        HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(1 /*IDR_RCDATA_OGG*/), RT_RCDATA);

        if (res)
        {
            HGLOBAL mem = LoadResource(hResInstance, res);
            void* data = LockResource(mem);
            size_t len = SizeofResource(hResInstance, res);

            FMOD_CREATESOUNDEXINFO exinfo;
            memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
            exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
            exinfo.length = (unsigned int)len;

            result = fmod_core_system->createSound((const char*)data, FMOD_OPENMEMORY, &exinfo, &sound3);
            result = sound3->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
            result = fmod_core_system->playSound(sound3, 0, false, &channel);
        }
    }


    void
        fmod_test_event()
    {
        FMOD_RESULT       result;
        Studio::EventDescription* footstepDescription = NULL;
        result = fmod_studio_system->getEvent("event:/Character/Player Footsteps", &footstepDescription);

        Studio::EventInstance* footstepInstance = NULL;
        result = footstepDescription->createInstance(&footstepInstance);

        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        result = footstepDescription->getParameterDescriptionByName("Surface", &paramDesc);

        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;
        float surfaceParameterValue = 1.0f;
        result = footstepInstance->setParameterByID(surfaceID, surfaceParameterValue);

        result = footstepInstance->start();
        result = fmod_studio_system->update();
    }


    void
        fmod_event_example()
    {
        FMOD_RESULT       result;

        Studio::EventDescription* footstepDescription = NULL;
        result = fmod_studio_system->getEvent("event:/Character/Player Footsteps", &footstepDescription);

        Studio::EventInstance* footstepInstance = NULL;
        result = footstepDescription->createInstance(&footstepInstance);

        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        result = footstepDescription->getParameterDescriptionByName("Surface", &paramDesc);

        FMOD_STUDIO_PARAMETER_ID surfaceID = paramDesc.id;
        float surfaceParameterValue = 1.0f;
        result = footstepInstance->setParameterByID(surfaceID, surfaceParameterValue);

        for (int i = 0; i < 12; i++)
        {
            result = footstepInstance->start();

            if ((i % 3) == 0)
                surfaceParameterValue = surfaceParameterValue + 1.0f;

            if (surfaceParameterValue > paramDesc.maximum)
                surfaceParameterValue = paramDesc.minimum;

            result = footstepInstance->setParameterByID(surfaceID, surfaceParameterValue);
            result = fmod_studio_system->update();
            Sleep(300);
        }

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
        /*
        if (fmod_core_system)
        {
            result = fmod_core_system->release();
            fmod_core_system = (System*)0;
        }
        if (fmod_studio_system)
        {
            result = fmod_studio_system->release();
            fmod_studio_system = (Studio::System*)0;
        }
        */
    }
#endif

}

/* soundfx.cpp */
