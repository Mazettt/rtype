#include "Little.hpp"
#include "../../Room.hpp"

LittleMonster::LittleMonster(Room &room, u_int id, short x, short y):
    ArmedEntity(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{}

LittleMonster::LittleMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{}

LittleMonster::~LittleMonster()
{

}

void LittleMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= ENEMY_FIRE_TIME) {
        fireMissile(Missile::Type::LITTLE_MONSTER);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= ENEMY_MOVE_TIME) {
        move(-1, 0);
        _room.sendToAll(Stream::toMonsterPos(_id, _box.x, _box.y));
        _lastMove = now;
    }
}

bool LittleMonster::collide(const IEntity &other)
{
    return AEntity::collide(other) || missilesCollide(other);
}
