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

#define NUM_IMMEDIATE_SOUND_INSTANCES 8

static GNHSoundInstance musicInstances[2] = { 0 };
static GNHSoundInstance immediateSoundInstances[NUM_IMMEDIATE_SOUND_INSTANCES] = { 0 };
static GNHSoundInstance* ambient_base = NULL;

#if 0
static GNHSoundInstance movementInstances[2] = { 0 };
static GNHSoundInstance hitInstances[2] = { 0 };
#endif

static float general_volume = 1.0f;
static float general_music_volume = 1.0f;
static float general_ambient_volume = 1.0f;
static float general_sound_effects_volume = 1.0f;


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
    { SOUND_BANK_MASTER, "event:/Music-Splash", 0, 0.5f},
    { SOUND_BANK_MASTER, "event:/Music-Player-Selection", 0, 0.3f},
    { SOUND_BANK_MASTER, "event:/Music-Intro", 0, 1.0f},
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
    { SOUND_BANK_MASTER, "event:/Fire Ambient" , 0, 1.0f},
    { SOUND_BANK_MASTER, "event:/Quarterstaff Swing" , 0, 1.0f},
    { SOUND_BANK_MASTER, "event:/Quarterstaff Hit" , 0, 1.0f}
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
    fmod_play_immediate_sound(struct ghsound_immediate_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;
        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* immediateSoundDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &immediateSoundDescription);
        if (result != FMOD_OK)
            return FALSE;

        Studio::EventInstance* immediateSoundInstance = NULL;
        result = immediateSoundDescription->createInstance(&immediateSoundInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Set volume */
        result = immediateSoundInstance->setVolume(info.volume * event_volume * general_sound_effects_volume * general_volume);
        if (result != FMOD_OK)
            return FALSE;

        /* Set parameters */
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        for (int i = 0; i < MAX_SOUND_PARAMETERS; i++)
        {
            if(!info.parameter_names[i] || strcmp(info.parameter_names[i], "") == 0)
                break;

            result = immediateSoundDescription->getParameterDescriptionByName("Surface", &paramDesc);
            if (result != FMOD_OK)
                return FALSE;

            FMOD_STUDIO_PARAMETER_ID paramID = paramDesc.id;
            float parameterValue = info.parameter_values[i];
            result = immediateSoundInstance->setParameterByID(paramID, parameterValue);
            if (result != FMOD_OK)
                return FALSE;
        }

        /* Play sound */
        result = immediateSoundInstance->start();
        if (result != FMOD_OK)
            return FALSE;

        if (immediateSoundInstances[0].eventInstance)
        {
            /* Delete the last one */
            if (immediateSoundInstances[NUM_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance)
            {
                /* Stop playing just in case */
                immediateSoundInstances[NUM_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                /* Release */
                immediateSoundInstances[NUM_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->release();
            }

            /* Move all others back */
            for (int i = NUM_IMMEDIATE_SOUND_INSTANCES - 1; i >= 1; i--)
            {
                immediateSoundInstances[i].eventInstance = immediateSoundInstances[i - 1].eventInstance;
                immediateSoundInstances[i].ghsound = immediateSoundInstances[i - 1].ghsound;
                immediateSoundInstances[i].normalVolume = immediateSoundInstances[i - 1].normalVolume;
            }
        }

        /* Set the new instance as movementInstances[0] */
        immediateSoundInstances[0].eventInstance = immediateSoundInstance;
        immediateSoundInstances[0].ghsound = info.ghsound;
        immediateSoundInstances[0].normalVolume = info.volume;

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        return TRUE;
    }



    int
    fmod_adjust_ghsound_general_volumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_effects_volume)
    {
        general_volume = new_general_volume;
        general_music_volume = new_general_music_volume;
        general_ambient_volume = new_general_ambient_volume;
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
        for (int i = 0; i < NUM_IMMEDIATE_SOUND_INSTANCES; i++)
        {
            if (immediateSoundInstances[i].eventInstance)
            {
                enum ghsound_types soundid = immediateSoundInstances[i].ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = immediateSoundInstances[i].eventInstance->setVolume(immediateSoundInstances[i].normalVolume * event_volume * general_sound_effects_volume * general_volume);
            }
        }
        for (GNHSoundInstance* curr = ambient_base; curr; curr = curr->next_instance)
        {
            if (curr->eventInstance)
            {
                enum ghsound_types soundid = curr->ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = curr->eventInstance->setVolume(curr->normalVolume * event_volume * general_ambient_volume * general_volume);
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
        ambientInstance->setVolume(fmod_volume * event_volume * general_ambient_volume * general_volume);

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
        result = ghs_ptr->eventInstance->setVolume(fmod_volume * event_volume * general_ambient_volume * general_volume);
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


}

/* soundfx.cpp */
