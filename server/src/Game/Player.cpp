#include "Player.hpp"
#include "../Utils/Scheduling.hpp"
#include "../Utils/Instruction.hpp"
#include "Room.hpp"

Player::Player(Room &room, std::shared_ptr<Client> client, u_char id):
    _room(room),
    _client(client),
    _pos(0, 0),
    _id(id),
    _score(0)
{
    _timerMissileRefresh = NOW;
    _timerTotMissilesRefresh = 0;
}

void Player::fireMissile(size_t &missilesIds)
{
    if (NOW - _lastFire >= FIRE_TIME) {
        ++missilesIds;
        _missiles.push_back(std::make_unique<Missiles>(_pos.first + PLAYER_WIDTH, _pos.second + PLAYER_HEIGHT / 2, missilesIds, Missiles::ALlY, _room));
        _lastFire = NOW;
    }
}

void Player::refreshMissiles()
{
    _timerTotMissilesRefresh += NOW - _timerMissileRefresh;

    while (_timerTotMissilesRefresh >= MISSILE_PROGRESS_TIME) {
        for (auto i = _missiles.begin(); i != _missiles.end(); i++) {
            if (!(**i).Refresh()) {
                _missiles.erase(i);
                i--;
            }
        }
        _timerTotMissilesRefresh -= MISSILE_PROGRESS_TIME;
    }
    _timerMissileRefresh = NOW;
    
}

void Player::move(int dx, int dy)
{
    _pos.first += dx;
    _pos.second += dy;
}

std::shared_ptr<Client> Player::client() const
{
    return _client;
}

const std::pair<int, int> &Player::position() const
{
    return _pos;
}

const std::vector<std::unique_ptr<Missiles>> &Player::missiles() const
{
    return _missiles;
}

u_char Player::id() const
{
    return _id;
}

int Player::score() const
{
    return _score;
}

size_t Player::getLastMove()
{
    return _lastMove;
}

void Player::setLastMove(size_t lastMove)
{
    this->_lastMove = lastMove;
}
