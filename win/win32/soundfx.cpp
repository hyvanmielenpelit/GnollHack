/*
 * soundfx.cpp
 * GnollHack for Windows Sound Effects Using FMOD
 *
 * Copyright (c) Janne Gustafsson, 2020
 *
 */

#include <windows.h>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "resource.h"

using namespace FMOD;
static System* fmod_core_system = (System*)0;
static Studio::System* fmod_studio_system = (Studio::System*)0;

struct GNHSoundInstance {
    Studio::EventInstance* eventInstance;
    enum ghsound_types ghsound;
    float normalVolume;
    int sound_type;
    boolean queued;
    boolean finished_playing; /* Used just for queable dialogues */
    unsigned int dialogue_mid;
    struct GNHSoundInstance* next_instance;
};

#define NUM_IMMEDIATE_SOUND_INSTANCES 32
#define NUM_LONG_IMMEDIATE_SOUND_INSTANCES 16

static GNHSoundInstance musicInstances[2] = { 0 };
static GNHSoundInstance levelAmbientInstances[2] = { 0 };
static GNHSoundInstance occupationAmbientInstances[2] = { 0 };
static GNHSoundInstance effectAmbientInstances[2] = { 0 };
static GNHSoundInstance environmentAmbientInstances[2] = { 0 };
static GNHSoundInstance immediateSoundInstances[NUM_IMMEDIATE_SOUND_INSTANCES] = { 0 };
static GNHSoundInstance longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES] = { 0 };
static GNHSoundInstance* ambient_base = NULL; /* for sound source ambients */

static float general_volume = 1.0f;
static float general_music_volume = 0.5f;
static float general_ambient_volume = 0.5f;
static float general_dialogue_volume = 0.5f;
static float general_sfx_volume = 0.5f;
static float general_ui_volume = 0.5f;
static boolean quieterMode = FALSE;

#define QUIETER_MODE_MULTIPLIER 0.5f
#define current_mode_volume (quieterMode ? QUIETER_MODE_MULTIPLIER : 1.0f)

extern "C" 
{

#include "soundset.h"
    int set_quieter_mode(int state);

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
        FMOD::Studio::Bank* bank[3] = { 0 };
        HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);

        for (int i = 0; i < 4; i++)
        {
            int rid[4] = { IDR_RCDATA_MASTER, IDR_RCDATA_STRINGS, IDR_RCDATA_AUXILIARY, IDR_RCDATA_INTRO };
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

    boolean
    load_fmod_bank_from_resource(HINSTANCE hInstance, int rid)
    {
        FMOD_RESULT       result;
        FMOD::Studio::Bank* bank = 0;

        HRSRC res = FindResource(hInstance, MAKEINTRESOURCE(rid), RT_RCDATA);

        if (res)
        {
            HGLOBAL mem = LoadResource(hInstance, res);
            void* data = LockResource(mem);
            size_t len = SizeofResource(hInstance, res);

            result = fmod_studio_system->loadBankMemory((const char*)data, (int)len, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
            if (result != FMOD_OK)
                return FALSE;
        }

        return TRUE;
    }

    boolean
    load_fmod_bank_from_file(HINSTANCE hInstance, char* filename)
    {
        FMOD_RESULT       result;
        FMOD::Studio::Bank* bank = 0;

        result = fmod_studio_system->loadBankFile(filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
        if (result != FMOD_OK)
            return FALSE;

        return TRUE;
    }

    boolean
    close_fmod_studio()
    {
        FMOD_RESULT result;

        result = fmod_studio_system->unloadAll();
        result = fmod_studio_system->release();

        fmod_studio_system = (Studio::System*)0;
        return 1;
    }

    boolean 
    fmod_play_music(struct ghsound_music_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;
        if (soundid <= GHSOUND_NONE || soundid >= MAX_GHSOUNDS)
        {
            if (musicInstances[0].eventInstance && musicInstances[0].normalVolume > 0.0f)
            {
                /* Stop music */
                result = musicInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
                musicInstances[0].normalVolume = 0.0f;
                result = fmod_studio_system->update();
                return (result == FMOD_OK);
            }

            /* Nothing to do */
            return TRUE;
        }


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
                /* Already playing, just check the difference in volume */
                if (musicInstances[0].normalVolume == 0.0f && info.volume > 0.0f)
                {
                    /* Restart the music and adjust volume */
                    result = musicInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_music_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;

                    result = musicInstances[0].eventInstance->start();
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (musicInstances[0].normalVolume > 0.0f && info.volume == 0.0f)
                {
                    /* Stop music */
                    result = musicInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (musicInstances[0].normalVolume != info.volume)
                {
                    /* Adjust volume */
                    result = musicInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_music_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;
                }

                /* Finally, update */
                if (musicInstances[0].normalVolume != info.volume)
                {
                    musicInstances[0].normalVolume = info.volume;
                    result = fmod_studio_system->update();
                    if (result != FMOD_OK)
                        return FALSE;
                }

                return TRUE;
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* musicInstance = NULL;
        result = musicDescription->createInstance(&musicInstance);
        if (result != FMOD_OK)
            return FALSE;

        musicInstance->setVolume(min(1.0f, info.volume * event_volume * general_music_volume * general_volume * current_mode_volume));
        if (info.volume > 0.0f)
        {
            result = musicInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }

        /* If starting succeeded, stop the old music in musicInstances[0] by fading */
        if (musicInstances[0].eventInstance)
        {
            musicInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to musicInstances[1] for later release, and before that, release existing musicInstances[1] */
            if (musicInstances[1].eventInstance)
                musicInstances[1].eventInstance->release();

            musicInstances[1] = musicInstances[0];
            /*
            musicInstances[1].eventInstance = musicInstances[0].eventInstance;
            musicInstances[1].ghsound = musicInstances[0].ghsound;
            musicInstances[1].normalVolume = musicInstances[0].normalVolume;
            musicInstances[1].sound_type = musicInstances[0].sound_type;
            */
        }

        /* Set the new instance as musicInstances[0] */
        musicInstances[0].eventInstance = musicInstance;
        musicInstances[0].ghsound = info.ghsound;
        musicInstances[0].normalVolume = info.volume;
        musicInstances[0].sound_type = 0;

        result = fmod_studio_system->update();
        return (result == FMOD_OK);
    }


    boolean
    fmod_play_level_ambient_sound(struct ghsound_environment_ambient_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;

        if (soundid <= GHSOUND_NONE || soundid >= MAX_GHSOUNDS)
        {
            if (levelAmbientInstances[0].eventInstance && levelAmbientInstances[0].normalVolume > 0.0f)
            {
                /* Stop ambient sound */
                result = levelAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
                levelAmbientInstances[0].normalVolume = 0.0f;
                result = fmod_studio_system->update();
                return (result == FMOD_OK);
            }

            /* Nothing to do */
            return TRUE;
        }

        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;

        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* occupationAmbientDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &occupationAmbientDescription);

        if (result != FMOD_OK)
            return FALSE;

        if (levelAmbientInstances[0].eventInstance)
        {
            Studio::EventInstance* earlierLevelAmbientInstance = levelAmbientInstances[0].eventInstance;
            Studio::EventDescription* earlierLevelAmbientDescription = NULL;
            earlierLevelAmbientInstance->getDescription(&earlierLevelAmbientDescription);
            if (earlierLevelAmbientDescription && occupationAmbientDescription == earlierLevelAmbientDescription)
            {
                /* Already playing, just check the difference in volume */
                if (levelAmbientInstances[0].normalVolume == 0.0f && info.volume > 0.0f)
                {
                    /* Restart the sound and adjust volume */
                    result = levelAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;

                    result = levelAmbientInstances[0].eventInstance->start();
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (levelAmbientInstances[0].normalVolume > 0.0f && info.volume == 0.0f)
                {
                    /* Stop ambient sound */
                    result = levelAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (levelAmbientInstances[0].normalVolume != info.volume)
                {
                    /* Adjust volume */
                    result = levelAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;
                }

                /* Finally, update */
                if (levelAmbientInstances[0].normalVolume != info.volume)
                {
                    levelAmbientInstances[0].normalVolume = info.volume;
                    result = fmod_studio_system->update();
                    if (result != FMOD_OK)
                        return FALSE;
                }

                return TRUE;
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* levelAmbientInstance = NULL;
        result = occupationAmbientDescription->createInstance(&levelAmbientInstance);
        if (result != FMOD_OK)
            return FALSE;

        result = levelAmbientInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
        if (result != FMOD_OK)
            return FALSE;

        if (info.volume > 0.0f)
        {
            result = levelAmbientInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }
        /* If starting succeeded, stop the old ambient sound in levelAmbientInstances[0] by fading */
        if (levelAmbientInstances[0].eventInstance)
        {
            if(levelAmbientInstances[0].ghsound != GHSOUND_NONE)
                levelAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to levelAmbientInstances[1] for later release, and before that, release existing levelAmbientInstances[1] */
            if (levelAmbientInstances[1].eventInstance)
                levelAmbientInstances[1].eventInstance->release();

            levelAmbientInstances[1] = levelAmbientInstances[0];
            /*
            levelAmbientInstances[1].eventInstance = levelAmbientInstances[0].eventInstance;
            levelAmbientInstances[1].ghsound = levelAmbientInstances[0].ghsound;
            levelAmbientInstances[1].normalVolume = levelAmbientInstances[0].normalVolume;
            levelAmbientInstances[1].sound_type = levelAmbientInstances[0].sound_type;
            */
        }

        /* Set the new instance as levelAmbientInstances[0] */
        levelAmbientInstances[0].eventInstance = levelAmbientInstance;
        levelAmbientInstances[0].ghsound = info.ghsound;
        levelAmbientInstances[0].normalVolume = info.volume;
        levelAmbientInstances[0].sound_type = 0;

        result = fmod_studio_system->update();
        return (result == FMOD_OK);
    }

    boolean
    fmod_play_environment_ambient_sound(struct ghsound_level_ambient_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;

        if (soundid <= GHSOUND_NONE || soundid >= MAX_GHSOUNDS)
        {
            if (environmentAmbientInstances[0].eventInstance && environmentAmbientInstances[0].normalVolume > 0.0f)
            {
                /* Stop ambient sound */
                result = environmentAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
                environmentAmbientInstances[0].normalVolume = 0.0f;
                result = fmod_studio_system->update();
                return (result == FMOD_OK);
            }

            /* Nothing to do */
            return TRUE;
        }

        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;

        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* occupationAmbientDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &occupationAmbientDescription);

        if (result != FMOD_OK)
            return FALSE;

        if (environmentAmbientInstances[0].eventInstance)
        {
            Studio::EventInstance* earlierEnvironmentAmbientInstance = environmentAmbientInstances[0].eventInstance;
            Studio::EventDescription* earlierEnvironmentAmbientDescription = NULL;
            earlierEnvironmentAmbientInstance->getDescription(&earlierEnvironmentAmbientDescription);
            if (earlierEnvironmentAmbientDescription && occupationAmbientDescription == earlierEnvironmentAmbientDescription)
            {
                /* Already playing, just check the difference in volume */
                if (environmentAmbientInstances[0].normalVolume == 0.0f && info.volume > 0.0f)
                {
                    /* Restart the sound and adjust volume */
                    result = environmentAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;

                    result = environmentAmbientInstances[0].eventInstance->start();
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (environmentAmbientInstances[0].normalVolume > 0.0f && info.volume == 0.0f)
                {
                    /* Stop ambient sound */
                    result = environmentAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (environmentAmbientInstances[0].normalVolume != info.volume)
                {
                    /* Adjust volume */
                    result = environmentAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;
                }

                /* Finally, update */
                if (environmentAmbientInstances[0].normalVolume != info.volume)
                {
                    environmentAmbientInstances[0].normalVolume = info.volume;
                    result = fmod_studio_system->update();
                    if (result != FMOD_OK)
                        return FALSE;
                }

                return TRUE;
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* environmentAmbientInstance = NULL;
        result = occupationAmbientDescription->createInstance(&environmentAmbientInstance);
        if (result != FMOD_OK)
            return FALSE;

        result = environmentAmbientInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
        if (result != FMOD_OK)
            return FALSE;

        if (info.volume > 0.0f)
        {
            result = environmentAmbientInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }
        /* If starting succeeded, stop the old ambient sound in environmentAmbientInstances[0] by fading */
        if (environmentAmbientInstances[0].eventInstance)
        {
            if (environmentAmbientInstances[0].ghsound != GHSOUND_NONE)
                environmentAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to environmentAmbientInstances[1] for later release, and before that, release existing environmentAmbientInstances[1] */
            if (environmentAmbientInstances[1].eventInstance)
                environmentAmbientInstances[1].eventInstance->release();

            environmentAmbientInstances[1] = environmentAmbientInstances[0];
            /*
            environmentAmbientInstances[1].eventInstance = environmentAmbientInstances[0].eventInstance;
            environmentAmbientInstances[1].ghsound = environmentAmbientInstances[0].ghsound;
            environmentAmbientInstances[1].normalVolume = environmentAmbientInstances[0].normalVolume;
            environmentAmbientInstances[1].sound_type = environmentAmbientInstances[0].sound_type;
            */
        }

        /* Set the new instance as environmentAmbientInstances[0] */
        environmentAmbientInstances[0].eventInstance = environmentAmbientInstance;
        environmentAmbientInstances[0].ghsound = info.ghsound;
        environmentAmbientInstances[0].normalVolume = info.volume;
        environmentAmbientInstances[0].sound_type = 0;

        result = fmod_studio_system->update();
        return (result == FMOD_OK);
    }

    boolean
    fmod_play_occupation_ambient_sound(struct ghsound_occupation_ambient_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;

        if (soundid <= GHSOUND_NONE || soundid >= MAX_GHSOUNDS)
        {
            if (occupationAmbientInstances[0].eventInstance && occupationAmbientInstances[0].normalVolume > 0.0f)
            {
                /* Stop ambient sound */
                result = occupationAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
                occupationAmbientInstances[0].normalVolume = 0.0f;
                result = fmod_studio_system->update();
                return (result == FMOD_OK);
            }

            /* Nothing to do */
            return TRUE;
        }

        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;

        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* occupationAmbientDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &occupationAmbientDescription);

        if (result != FMOD_OK)
            return FALSE;

        if (occupationAmbientInstances[0].eventInstance)
        {
            Studio::EventInstance* earlierOccupationAmbientInstance = occupationAmbientInstances[0].eventInstance;
            Studio::EventDescription* earlierOccupationAmbientDescription = NULL;
            earlierOccupationAmbientInstance->getDescription(&earlierOccupationAmbientDescription);
            if (earlierOccupationAmbientDescription && occupationAmbientDescription == earlierOccupationAmbientDescription)
            {
                /* Already playing, just check the difference in volume */
                if (occupationAmbientInstances[0].normalVolume == 0.0f && info.volume > 0.0f)
                {
                    /* Restart the sound and adjust volume */
                    result = occupationAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;

                    result = occupationAmbientInstances[0].eventInstance->start();
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (occupationAmbientInstances[0].normalVolume > 0.0f && info.volume == 0.0f)
                {
                    /* Stop ambient sound */
                    result = occupationAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                    if (result != FMOD_OK)
                        return FALSE;
                }
                else if (occupationAmbientInstances[0].normalVolume != info.volume)
                {
                    /* Adjust volume */
                    result = occupationAmbientInstances[0].eventInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
                    if (result != FMOD_OK)
                        return FALSE;
                }

                /* Finally, update */
                if (occupationAmbientInstances[0].normalVolume != info.volume)
                {
                    occupationAmbientInstances[0].normalVolume = info.volume;
                    result = fmod_studio_system->update();
                    if (result != FMOD_OK)
                        return FALSE;
                }

                return TRUE;
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* occupationAmbientInstance = NULL;
        result = occupationAmbientDescription->createInstance(&occupationAmbientInstance);
        if (result != FMOD_OK)
            return FALSE;

        result = occupationAmbientInstance->setVolume(min(1.0f, info.volume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
        if (result != FMOD_OK)
            return FALSE;

        if (info.volume > 0.0f)
        {
            result = occupationAmbientInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }
        /* If starting succeeded, stop the old ambient sound in occupationAmbientInstances[0] by fading */
        if (occupationAmbientInstances[0].eventInstance)
        {
            if (occupationAmbientInstances[0].ghsound != GHSOUND_NONE)
                occupationAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to occupationAmbientInstances[1] for later release, and before that, release existing occupationAmbientInstances[1] */
            if (occupationAmbientInstances[1].eventInstance)
                occupationAmbientInstances[1].eventInstance->release();

            occupationAmbientInstances[1] = occupationAmbientInstances[0];
            /*
            occupationAmbientInstances[1].eventInstance = occupationAmbientInstances[0].eventInstance;
            occupationAmbientInstances[1].ghsound = occupationAmbientInstances[0].ghsound;
            occupationAmbientInstances[1].normalVolume = occupationAmbientInstances[0].normalVolume;
            occupationAmbientInstances[1].sound_type = occupationAmbientInstances[0].sound_type;
            */
        }

        /* Set the new instance as occupationAmbientInstances[0] */
        occupationAmbientInstances[0].eventInstance = occupationAmbientInstance;
        occupationAmbientInstances[0].ghsound = info.ghsound;
        occupationAmbientInstances[0].normalVolume = info.volume;
        occupationAmbientInstances[0].sound_type = 0;

        result = fmod_studio_system->update();
        return (result == FMOD_OK);
    }




    boolean
    fmod_set_effect_ambient_volume(struct effect_ambient_volume_info info)
    {
        FMOD_RESULT result;
        if (effectAmbientInstances[0].eventInstance)
        {
            struct ghsound_eventmapping eventmap = ghsound2event[effectAmbientInstances[0].ghsound];
            float event_volume = eventmap.volume;
            float volume = info.volume;

            /* Adjust volume */
            effectAmbientInstances[0].eventInstance->setVolume(min(1.0f, volume * event_volume * general_sfx_volume * general_volume));

            if (volume == 0.0f && effectAmbientInstances[0].normalVolume > 0.0f)
            {
                /* Stop ambient */
                result = effectAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
            }
            else if (volume > 0.0f && effectAmbientInstances[0].normalVolume == 0.0f)
            {
                /* Start ambient */
                result = effectAmbientInstances[0].eventInstance->start();
                if (result != FMOD_OK)
                    return FALSE;

            }

            effectAmbientInstances[0].normalVolume = volume;
            result = fmod_studio_system->update();
            return (result == FMOD_OK);
        }

        /* Nothing's playing */
        return FALSE;
    }

    boolean
    fmod_play_effect_ambient_sound(struct ghsound_effect_ambient_info info)
    {
        FMOD_RESULT result;

        enum ghsound_types soundid = info.ghsound;

        if (soundid <= GHSOUND_NONE || soundid >= MAX_GHSOUNDS)
        {
            if (effectAmbientInstances[0].eventInstance)
            {
                /* Stop ambient sound */
                result = effectAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                    return FALSE;
                effectAmbientInstances[0].ghsound = GHSOUND_NONE;
                effectAmbientInstances[0].normalVolume = 0.0f;
                effectAmbientInstances[0].sound_type = 0;
                result = fmod_studio_system->update();
                return (result == FMOD_OK);
            }

            /* Nothing to do */
            return TRUE;
        }

        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;

        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        Studio::EventDescription* effectAmbientDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &effectAmbientDescription);

        if (result != FMOD_OK)
            return FALSE;

        if (effectAmbientInstances[0].eventInstance)
        {
            Studio::EventInstance* earlierEffectAmbientInstance = effectAmbientInstances[0].eventInstance;
            Studio::EventDescription* earlierEffectAmbientDescription = NULL;
            earlierEffectAmbientInstance->getDescription(&earlierEffectAmbientDescription);
            if (earlierEffectAmbientDescription && effectAmbientDescription == earlierEffectAmbientDescription)
            {
                /* Already selected, but may be stopped; just set the volume, which may start or stop the ambient */
                struct effect_ambient_volume_info vinfo = { 0 };
                vinfo.volume = info.volume;
                return fmod_set_effect_ambient_volume(vinfo);
            }
        }

        /* Not playing yet, so make a new instance */
        Studio::EventInstance* effectAmbientInstance = NULL;
        result = effectAmbientDescription->createInstance(&effectAmbientInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Note: effect ambient uses sfx volume instead of ambient volume */
        effectAmbientInstance->setVolume(min(1.0f, info.volume * event_volume * general_sfx_volume * general_volume));
        if (info.volume > 0.0f)
        {
            result = effectAmbientInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }

        /* If starting succeeded, stop the old ambient sound in effectAmbientInstances[0] by fading */
        if (effectAmbientInstances[0].eventInstance)
        {
            if (effectAmbientInstances[0].ghsound != GHSOUND_NONE && effectAmbientInstances[0].normalVolume > 0.0f)
                effectAmbientInstances[0].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            /* move to effectAmbientInstances[1] for later release, and before that, release existing effectAmbientInstances[1] */
            if (effectAmbientInstances[1].eventInstance)
                effectAmbientInstances[1].eventInstance->release();

            effectAmbientInstances[1] = effectAmbientInstances[0];
            /*
            effectAmbientInstances[1].eventInstance = effectAmbientInstances[0].eventInstance;
            effectAmbientInstances[1].ghsound = effectAmbientInstances[0].ghsound;
            effectAmbientInstances[1].normalVolume = effectAmbientInstances[0].normalVolume;
            effectAmbientInstances[1].sound_type = effectAmbientInstances[0].sound_type;
            */
        }

        /* Set the new instance as effectAmbientInstances[0] */
        effectAmbientInstances[0].eventInstance = effectAmbientInstance;
        effectAmbientInstances[0].ghsound = info.ghsound;
        effectAmbientInstances[0].normalVolume = info.volume;
        effectAmbientInstances[0].sound_type = 0;

        result = fmod_studio_system->update();
        return (result == FMOD_OK);
    }

    FMOD_RESULT F_CALLBACK GNHImmediateEventCallback(
        FMOD_STUDIO_EVENT_CALLBACK_TYPE type,
        FMOD_STUDIO_EVENTINSTANCE* event,
        void* parameters)
    {
        if (type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED)
        {
            void* ptr = (void*)event;
            for (int i = 0; i < NUM_IMMEDIATE_SOUND_INSTANCES; i++)
            {
                if (immediateSoundInstances[i].eventInstance == (Studio::EventInstance*)ptr)
                {
                    immediateSoundInstances[i].finished_playing = 1;
                    break;
                }
            }
            for (int i = 0; i < NUM_LONG_IMMEDIATE_SOUND_INSTANCES; i++)
            {
                if (longImmediateSoundInstances[i].eventInstance == (Studio::EventInstance*)ptr)
                {
                    longImmediateSoundInstances[i].finished_playing = 1;
                    break;
                }
            }

        }
        return FMOD_OK;
    }


    FMOD_RESULT F_CALLBACK GNHDialogueEventCallback(
        FMOD_STUDIO_EVENT_CALLBACK_TYPE type,
        FMOD_STUDIO_EVENTINSTANCE* event,
        void* parameters)
    {
        FMOD_RESULT result;
        if (type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED)
        {
            void* ptr = (void*)event;
            for (int i = 0; i < NUM_IMMEDIATE_SOUND_INSTANCES; i++)
            {
                if (immediateSoundInstances[i].eventInstance == (Studio::EventInstance*)ptr)
                {
                    immediateSoundInstances[i].finished_playing = 1;
                    for (int j = i - 1; j >= 0; j--)
                    {
                        if (immediateSoundInstances[j].queued)
                        {
                            immediateSoundInstances[j].queued = 0;
                            result = immediateSoundInstances[j].eventInstance->start();
                            result = fmod_studio_system->update();
                            return FMOD_OK;
                        }
                    }
                    (void)set_quieter_mode(FALSE);
                    break;
                }
            }
            for (int i = 0; i < NUM_LONG_IMMEDIATE_SOUND_INSTANCES; i++)
            {
                if (longImmediateSoundInstances[i].eventInstance == (Studio::EventInstance*)ptr)
                {
                    longImmediateSoundInstances[i].finished_playing = 1;
                    for (int j = i - 1; j >= 0; j--)
                    {
                        if (longImmediateSoundInstances[j].queued)
                        {
                            longImmediateSoundInstances[j].queued = 0;
                            result = longImmediateSoundInstances[j].eventInstance->start();
                            result = fmod_studio_system->update();
                            return FMOD_OK;
                        }
                    }
                    (void)set_quieter_mode(FALSE);
                    break;
                }
            }

        }
        return FMOD_OK;
    }
   
#if 0
    boolean fmod_stop_all_immediate_sounds()
    {
        FMOD_RESULT result;
        boolean stop_failed = FALSE;
        boolean update_failed = FALSE;

        for (int i = 0; i < NUM_IMMEDIATE_SOUND_INSTANCES; i++)
        {
            if (immediateSoundInstances[i].eventInstance && immediateSoundInstances[i].normalVolume > 0.0f)
            {
                /* Stop sound */
                result = immediateSoundInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                {
                    stop_failed = TRUE;
                }
                immediateSoundInstances[i].normalVolume = 0.0f;
            }
        }
        for (int i = 0; i < NUM_LONG_IMMEDIATE_SOUND_INSTANCES; i++)
        {
            if (longImmediateSoundInstances[i].eventInstance && longImmediateSoundInstances[i].normalVolume > 0.0f)
            {
                /* Stop sound */
                result = longImmediateSoundInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                if (result != FMOD_OK)
                {
                    stop_failed = TRUE;
                }
                longImmediateSoundInstances[i].normalVolume = 0.0f;
            }
        }

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            update_failed = TRUE;

        return !update_failed && !result;
    }
#endif

    boolean
    fmod_play_immediate_sound(struct ghsound_immediate_info info)
    {
        FMOD_RESULT result;

        enum sound_play_groups play_group = info.play_group;
        enum ghsound_types soundid = info.ghsound;
        struct ghsound_eventmapping eventmap = ghsound2event[soundid];
        float event_volume = eventmap.volume;
        if (!eventmap.eventPath || !strcmp(eventmap.eventPath, ""))
            return FALSE;

        if (info.play_flags & PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED)
        {
            boolean include_playing = (info.play_flags & PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING) != 0;
            boolean include_queued = (info.play_flags & PLAY_FLAGS_NO_PLAY_IF_ALREADY_QUEUED) != 0;
            if (play_group == SOUND_PLAY_GROUP_LONG)
            {
                for (int i = 0; i < NUM_LONG_IMMEDIATE_SOUND_INSTANCES; i++)
                {
                    if (longImmediateSoundInstances[i].ghsound == soundid && longImmediateSoundInstances[i].normalVolume > 0
                        && !longImmediateSoundInstances[i].finished_playing && ((include_queued && longImmediateSoundInstances[i].queued) || (include_playing && !longImmediateSoundInstances[i].queued)))
                    {
                        return TRUE;
                    }
                }
            }
            else
            {
                for (int i = 0; i < NUM_IMMEDIATE_SOUND_INSTANCES; i++)
                {
                    if (immediateSoundInstances[i].ghsound == soundid && immediateSoundInstances[i].normalVolume > 0
                        && !immediateSoundInstances[i].finished_playing && ((include_queued && immediateSoundInstances[i].queued) || (include_playing && !immediateSoundInstances[i].queued)))
                    {
                        return TRUE;
                    }
                }
            }
        }

        float relevant_general_volume = (info.sound_type == IMMEDIATE_SOUND_UI ? general_ui_volume : info.sound_type == IMMEDIATE_SOUND_DIALOGUE ? general_dialogue_volume : general_sfx_volume);

        Studio::EventDescription* immediateSoundDescription = NULL;
        result = fmod_studio_system->getEvent(eventmap.eventPath, &immediateSoundDescription);
        if (result != FMOD_OK)
            return FALSE;

        Studio::EventInstance* immediateSoundInstance = NULL;
        result = immediateSoundDescription->createInstance(&immediateSoundInstance);
        if (result != FMOD_OK)
            return FALSE;

        /* Set volume */
        result = immediateSoundInstance->setVolume(min(1.0f, info.volume * event_volume * relevant_general_volume * general_volume));
        if (result != FMOD_OK)
            return FALSE;

        if (result != FMOD_OK)
            return FALSE;

        /* Set parameters */
        FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesc;
        for (int i = 0; i < MAX_SOUND_PARAMETERS; i++)
        {
            if(!info.parameter_names[i] || strcmp(info.parameter_names[i], "") == 0)
                break;

            result = immediateSoundDescription->getParameterDescriptionByName(info.parameter_names[i], &paramDesc);
            if (result != FMOD_OK)
                return FALSE;

            FMOD_STUDIO_PARAMETER_ID paramID = paramDesc.id;
            float parameterValue = info.parameter_values[i];
            result = immediateSoundInstance->setParameterByID(paramID, parameterValue);
            if (result != FMOD_OK)
                return FALSE;
        }


        boolean queue_sound = FALSE;
        if (play_group == SOUND_PLAY_GROUP_LONG)
        {
            queue_sound = (info.sound_type == IMMEDIATE_SOUND_DIALOGUE && longImmediateSoundInstances[0].sound_type == IMMEDIATE_SOUND_DIALOGUE && !longImmediateSoundInstances[0].finished_playing);

            /* Long play group */
            if (longImmediateSoundInstances[0].eventInstance)
            {
                /* Delete the last one */
                if (longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance)
                {
                    /* Stop playing just in case */
                    longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
                    /* Release */
                    longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->release();
                }

                /* Move all others back */
                for (int i = NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1; i >= 1; i--)
                {
                    longImmediateSoundInstances[i] = longImmediateSoundInstances[i - 1];
                }
            }

            /* Set the new instance as longImmediateSoundInstances[0] */
            longImmediateSoundInstances[0].eventInstance = immediateSoundInstance;
            longImmediateSoundInstances[0].ghsound = info.ghsound;
            longImmediateSoundInstances[0].normalVolume = info.volume;
            longImmediateSoundInstances[0].sound_type = info.sound_type;
            longImmediateSoundInstances[0].dialogue_mid = info.dialogue_mid;
            longImmediateSoundInstances[0].queued = queue_sound;
            longImmediateSoundInstances[0].finished_playing = 0;

            if (info.sound_type == IMMEDIATE_SOUND_DIALOGUE)
                result = longImmediateSoundInstances[0].eventInstance->setCallback(GNHDialogueEventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
            else
                result = longImmediateSoundInstances[0].eventInstance->setCallback(GNHImmediateEventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);

            /* Fallback if queued for too long */
            if (longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].queued && !longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].finished_playing)
            {
                longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].queued = 0;
                (void)longImmediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->start();
            }
        }
        else
        {
            queue_sound = (info.sound_type == IMMEDIATE_SOUND_DIALOGUE && immediateSoundInstances[0].sound_type == IMMEDIATE_SOUND_DIALOGUE && !immediateSoundInstances[0].finished_playing);

            /* Normal play group */
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
                    immediateSoundInstances[i] = immediateSoundInstances[i - 1];
                }
            }

            /* Set the new instance as movementInstances[0] */
            immediateSoundInstances[0].eventInstance = immediateSoundInstance;
            immediateSoundInstances[0].ghsound = info.ghsound;
            immediateSoundInstances[0].normalVolume = info.volume;
            immediateSoundInstances[0].sound_type = info.sound_type;
            immediateSoundInstances[0].dialogue_mid = info.dialogue_mid;
            immediateSoundInstances[0].queued = queue_sound;
            immediateSoundInstances[0].finished_playing = 0;

            if (info.sound_type == IMMEDIATE_SOUND_DIALOGUE)
                result = immediateSoundInstances[0].eventInstance->setCallback(GNHDialogueEventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
            else
                result = immediateSoundInstances[0].eventInstance->setCallback(GNHImmediateEventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);

            /* Fallback if queued for too long */
            if (immediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].queued && !immediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].finished_playing)
            {
                immediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].queued = 0;
                (void)immediateSoundInstances[NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1].eventInstance->start();
            }
        }

        boolean play_sound = TRUE;
        if (queue_sound)
        {
            if (play_group == SOUND_PLAY_GROUP_LONG)
            {
                /* Is the previous still playing? */
                if (((longImmediateSoundInstances[1].eventInstance && longImmediateSoundInstances[1].ghsound > 0 && longImmediateSoundInstances[1].normalVolume > 0.0f) || longImmediateSoundInstances[1].queued) && !longImmediateSoundInstances[1].finished_playing)
                {
                    play_sound = FALSE;
                }
                else
                {
                    longImmediateSoundInstances[0].queued = 0;
                }
            }
            else
            {
                /* Is the previous still playing? */
                if (((immediateSoundInstances[1].eventInstance && immediateSoundInstances[1].ghsound > 0 && immediateSoundInstances[1].normalVolume > 0.0f) || immediateSoundInstances[1].queued) && !immediateSoundInstances[1].finished_playing)
                {
                    play_sound = FALSE;

                }
                else
                {
                    immediateSoundInstances[0].queued = 0;
                }
            }
        }

        if(play_sound)
        {
            /* Play sound */
            if (info.sound_type == IMMEDIATE_SOUND_DIALOGUE)
                (void)set_quieter_mode(TRUE);

            result = immediateSoundInstance->start();
            if (result != FMOD_OK)
                return FALSE;
        }

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        return TRUE;
    }

    FMOD_RESULT
    adjust_volume_type(GNHSoundInstance* soundList, float typeVolume)
    {
        FMOD_RESULT result = FMOD_OK;
        for (int i = 0; i <= 1; i++)
        {
            if (soundList[i].eventInstance)
            {
                enum ghsound_types soundid = soundList[i].ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = soundList[i].eventInstance->setVolume(min(1.0f, soundList[i].normalVolume * event_volume * typeVolume * general_volume * current_mode_volume));
            }
        }
        return result;
    }

    FMOD_RESULT
    set_music_and_ambient_volumes_without_update(void)
    {
        FMOD_RESULT result = FMOD_OK;
        result = adjust_volume_type(musicInstances, general_music_volume);
        result = adjust_volume_type(levelAmbientInstances, general_ambient_volume);
        result = adjust_volume_type(environmentAmbientInstances, general_ambient_volume);
        result = adjust_volume_type(occupationAmbientInstances, general_ambient_volume);
        result = adjust_volume_type(effectAmbientInstances, general_ambient_volume);
        for (GNHSoundInstance* curr = ambient_base; curr; curr = curr->next_instance)
        {
            if (curr->eventInstance)
            {
                enum ghsound_types soundid = curr->ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                result = curr->eventInstance->setVolume(min(1.0f, curr->normalVolume * event_volume * general_ambient_volume * general_volume * current_mode_volume));
            }
        }
        return result;
    }
    
    FMOD_RESULT
    adjust_immediate_volume_type(GNHSoundInstance* soundList, int soundListSize)
    {
        FMOD_RESULT result = FMOD_OK;
        for (int i = 0; i < soundListSize; i++)
        {
            if (soundList[i].eventInstance)
            {
                enum ghsound_types soundid = soundList[i].ghsound;
                struct ghsound_eventmapping eventmap = ghsound2event[soundid];
                float event_volume = eventmap.volume;
                float relevant_general_volume = (soundList[i].sound_type == IMMEDIATE_SOUND_UI ? general_ui_volume : soundList[i].sound_type == IMMEDIATE_SOUND_DIALOGUE ? general_dialogue_volume : general_sfx_volume);
                result = soundList[i].eventInstance->setVolume(min(1.0f, soundList[i].normalVolume * event_volume * relevant_general_volume * general_volume));
            }
        }
        return result;
    }

    int
    fmod_adjust_ghsound_general_volumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_ui_volume)
    {
        general_volume = new_general_volume;
        general_music_volume = new_general_music_volume;
        general_ambient_volume = new_general_ambient_volume;
        general_dialogue_volume = new_general_dialogue_volume;
        general_sfx_volume = new_general_sfx_volume;
        general_ui_volume = new_general_ui_volume;

        FMOD_RESULT result = set_music_and_ambient_volumes_without_update();
        result = adjust_immediate_volume_type(immediateSoundInstances, NUM_IMMEDIATE_SOUND_INSTANCES);
        result = adjust_immediate_volume_type(longImmediateSoundInstances, NUM_LONG_IMMEDIATE_SOUND_INSTANCES);
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
        ambientInstance->setVolume(fmod_volume * event_volume * general_ambient_volume * general_volume * current_mode_volume);

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
        result = ghs_ptr->eventInstance->setVolume(fmod_volume * event_volume * general_ambient_volume * general_volume * current_mode_volume);
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


    boolean
    fmod_stop_all_sounds(struct stop_all_info info)
    {
        if (info.stop_flags == 0)
            info.stop_flags = STOP_SOUNDS_FLAGS_ALL;

        FMOD_RESULT result;
        boolean res = TRUE;
        if (info.stop_flags & STOP_SOUNDS_FLAGS_MUSIC)
        {
            for (int i = 0; i <= 1; i++)
            {
                if (musicInstances[i].eventInstance)
                {
                    if ((result = musicInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = musicInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    musicInstances[i].eventInstance = 0;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_LEVEL_AMBIENT)
        {
            for (int i = 0; i <= 1; i++)
            {
                if (levelAmbientInstances[i].eventInstance)
                {
                    if (levelAmbientInstances[i].normalVolume > 0.0f && (result = levelAmbientInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = levelAmbientInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    levelAmbientInstances[i].eventInstance = 0;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_ENVIRONMENT_AMBIENT)
        {
            for (int i = 0; i <= 1; i++)
            {
                if (environmentAmbientInstances[i].eventInstance)
                {
                    if (environmentAmbientInstances[i].normalVolume > 0.0f && (result = environmentAmbientInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = environmentAmbientInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    environmentAmbientInstances[i].eventInstance = 0;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_OCCUPATION_AMBIENT)
        {
            for (int i = 0; i <= 1; i++)
            {
                if (occupationAmbientInstances[i].eventInstance)
                {
                    if (occupationAmbientInstances[i].normalVolume > 0.0f && (result = occupationAmbientInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = occupationAmbientInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    occupationAmbientInstances[i].eventInstance = 0;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_EFFECT_AMBIENT)
        {
            for (int i = 0; i <= 1; i++)
            {
                if (effectAmbientInstances[i].eventInstance)
                {
                    if (effectAmbientInstances[i].normalVolume > 0.0f && (result = effectAmbientInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = effectAmbientInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    effectAmbientInstances[i].eventInstance = 0;
                }
            }
        }

        boolean only_mid = !!(info.stop_flags & STOP_SOUNDS_FLAGS_ONLY_DIALOGUE_MID);
        /* Reverse order because of possible queueing */
        if (info.stop_flags & STOP_SOUNDS_FLAGS_IMMEDIATE_NORMAL)
        {
            for (int i = NUM_IMMEDIATE_SOUND_INSTANCES - 1; i >= 0; i--)
            {
                if (immediateSoundInstances[i].eventInstance && (!only_mid || (only_mid && info.dialogue_mid == immediateSoundInstances[i].dialogue_mid)))
                {
                    if (immediateSoundInstances[i].normalVolume > 0.0f && (result = immediateSoundInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = immediateSoundInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    immediateSoundInstances[i].eventInstance = 0;
                    immediateSoundInstances[i].normalVolume = 0.0f;
                    immediateSoundInstances[i].queued = 0;
                }
            }

            for (int i = NUM_IMMEDIATE_SOUND_INSTANCES - 1; i >= 0; i--)
            {
                if (immediateSoundInstances[i].eventInstance && !immediateSoundInstances[i].finished_playing)
                {
                    if (immediateSoundInstances[i].queued)
                    {
                        immediateSoundInstances[i].queued = 0;
                        result = immediateSoundInstances[i].eventInstance->start();
                        result = fmod_studio_system->update();
                    }
                    break;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_IMMEDIATE_LONG)
        {
            for (int i = NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1; i >= 0; i--)
            {
                if (longImmediateSoundInstances[i].eventInstance && (!only_mid || (only_mid && info.dialogue_mid == longImmediateSoundInstances[i].dialogue_mid)))
                {
                    if (longImmediateSoundInstances[i].normalVolume > 0.0f && (result = longImmediateSoundInstances[i].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT)) != FMOD_OK)
                        res = FALSE;
                    if ((result = longImmediateSoundInstances[i].eventInstance->release()) != FMOD_OK)
                        res = FALSE;
                    longImmediateSoundInstances[i].eventInstance = 0;
                    longImmediateSoundInstances[i].normalVolume = 0.0f;
                    longImmediateSoundInstances[i].queued = 0;
                }
            }

            for (int i = NUM_LONG_IMMEDIATE_SOUND_INSTANCES - 1; i >= 0; i--)
            {
                if (longImmediateSoundInstances[i].eventInstance && !longImmediateSoundInstances[i].finished_playing)
                {
                    if (longImmediateSoundInstances[i].queued)
                    {
                        longImmediateSoundInstances[i].queued = 0;
                        result = longImmediateSoundInstances[i].eventInstance->start();
                        result = fmod_studio_system->update();
                    }
                    break;
                }
            }
        }

        if (info.stop_flags & STOP_SOUNDS_FLAGS_SOUND_SOURCES)
        {
            /* These we just set volume to zero, since they are linked with sound_sources */
            for (GNHSoundInstance* curr = ambient_base; curr; curr = curr->next_instance)
            {
                if (curr->eventInstance)
                {
                    if (curr->normalVolume > 0.0f)
                        (void)fmod_set_ambient_ghsound_volume((void*)curr, 0.0f);
                }
            }
        }

        result = fmod_studio_system->update();
        if (result != FMOD_OK)
            return FALSE;

        return res;
    }

    FMOD_RESULT
    adjust_music_and_ambient_volumes()
    {
        FMOD_RESULT result = set_music_and_ambient_volumes_without_update();
        result = fmod_studio_system->update();
        return result;
    }

    int
    set_quieter_mode(int state)
    {
        if (quieterMode == (boolean)state)
            return FMOD_OK;

        quieterMode = (boolean)state;
        return (int)adjust_music_and_ambient_volumes();
    }

}




/* soundfx.cpp */
