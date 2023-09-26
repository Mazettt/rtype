#include "Player.hpp"
#include "../Utils/Scheduling.hpp"
#include "../Utils/Instruction.hpp"

Player::Player(Room &room, Notifier sendToAll, std::shared_ptr<Client> client, u_char id):
    _room(room),
    _sendToAll(sendToAll),
    _client(client),
    _pos(0, 0),
    _id(id),
    _score(0)
{}

void Player::fireMissile(size_t &missilesIds)
{
    ++missilesIds;
    _missiles.push_back(std::make_unique<Missiles>(_pos.first, _pos.second, missilesIds, Missiles::ALlY, _room));
    (_room.*_sendToAll)("Missile fired");
}

void Player::refreshMissiles()
{
    for (auto i = _missiles.begin(); i != _missiles.end(); i++) {
        if (!(**i).Refresh()) {
            _missiles.erase(i);
            i--;
        }
    }
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