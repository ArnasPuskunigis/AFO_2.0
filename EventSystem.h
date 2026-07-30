#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

enum class GameEvent {
    EnemyKilled,
    PlayerHit,
    BulletFired,
    WaveStarted
};

class EventSystem {
public:
    using Callback = std::function<void()>;

    void subscribe(GameEvent event, Callback callback);
    void emit(GameEvent event);

private:
    std::unordered_map<GameEvent, std::vector<Callback>> listeners;
};