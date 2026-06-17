#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(RELevelEditorLayer, LevelEditorLayer) {
    
    void objectMoved_(GameObject* object);
};