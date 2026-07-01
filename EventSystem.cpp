#include "EventSystem.h"

void EventSystem::subscribe(GameEvent event, Callback callback) {
    listeners[event].push_back(callback);
}

void EventSystem::emit(GameEvent event) {
    for (auto& cb : listeners[event])
        cb();
}