#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <tuple>

class Room;

using namespace std::chrono_literals;
using chronoTime = std::chrono::system_clock::time_point;
#define chronoNow std::chrono::system_clock::now()
#define chronoDiff(unit, a, b) std::chrono::duration_cast<unit>(a - b).count()
using chronoUs = std::chrono::microseconds;
using chronoMs = std::chrono::milliseconds;
using chronoSec = std::chrono::seconds;
using chronoMin = std::chrono::minutes;

class Levels
{
public:

    class Level {
        public:
            class EntityEvents
            {
                private:
                    std::vector<std::pair<size_t, std::vector<size_t>>> _spawns;
                    unsigned char _entity;
                    std::vector<std::pair<size_t, std::vector<size_t>>>::iterator _it;
                    bool _init = true;
                public:
                    EntityEvents(unsigned char entity);
                    ~EntityEvents();
                    std::vector<size_t> getSpawns(size_t currentTimecode);
                    void addSpawn(size_t timecode, size_t pos);
                    unsigned char getEntity() const;
                    void sort();
            };

            class StrobeEvent
            {
                private:
                    std::vector<std::tuple<size_t, unsigned char, bool>> _strobe;
                    std::vector<std::tuple<size_t, unsigned char, bool>>::iterator _it;
                    bool _init = true;
                public:
                    StrobeEvent();
                    ~StrobeEvent();
                    std::vector<std::tuple<size_t, unsigned char, bool>> getEvents(size_t currentTimecode);
                    void addColor(size_t timecode, unsigned char color, size_t duration);
                    void sort();

                    enum {
                        RED = 1,
                        GREEN,
                        BLUE,
                        YELLOW,
                        PURPLE,
                        CYAN,
                        WHITE
                    };
            };

            enum Songs {
                SOUND_OF_SPACE = 1
            };

            Level(const std::string &path);
            ~Level();
            std::vector<EntityEvents> &getEvents();
            StrobeEvent &getStrobes();
            size_t getDuration() const;
            unsigned char getStage() const;
            unsigned char getSong() const;

            class OpenError : public std::exception {
                public:
                    std::string _msg;
                    const char *what() const noexcept override { return _msg.c_str(); }
            };
            class ParsError : public std::exception {
                public:
                    std::string _msg;
                    const char *what() const noexcept override { return _msg.c_str(); }
            };
        private:
            void parsStage(const std::string &line, const std::string &path);
            void parsSong(const std::string &line, const std::string &path);
            void parsDuration(const std::string &line, const std::string &path);
            void parsEvents(const std::string &line, const std::string &path);
            std::vector<EntityEvents> _events;
            StrobeEvent _strobes;
            unsigned char _stage = 0;
            unsigned char _song = 0;
            size_t _duration = 0;

            char _parserEntity = -1;
    };
    Levels();
    ~Levels();

    void start();
    void update(Room &room);
    size_t lvl() const { return 1;}
    const Level &getLevel() const;

private:
    size_t _currentLvl;
    chronoTime _lastRefresh;
    chronoTime _lvlStart;
    std::vector<Levels::Level> _levels;

};