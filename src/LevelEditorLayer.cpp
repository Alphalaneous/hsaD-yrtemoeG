#include "LevelEditorLayer.hpp"

void RELevelEditorLayer::objectMoved_(GameObject* object) {
    if (!object) return;

    constexpr std::array<int, 51> effectObjects = {
        22, 23, 24, 25, 26, 27, 28, 31, 32, 33,
        55, 56, 57, 58, 59, 1915, 2067,2903, 
        2904, 2905, 2907, 2909, 2910, 2911, 2912, 
        2913, 2914, 2915, 2916, 2917, 2919, 2920,
        2921, 2922, 2923, 2924, 3006, 3007, 3008, 
        3009, 3010, 3016, 3017, 3018, 3019, 3020, 
        3021, 3022, 3023, 3024, 3660
    };

    constexpr std::array<int, 6> colorObjects = {
        29, 30, 105, 899, 900, 915
    };

    if (std::find(effectObjects.begin(), effectObjects.end(), object->m_objectID) != effectObjects.end()) {
        m_drawGridLayer->m_sortEffects = true;
    }
    else if (std::find(colorObjects.begin(), colorObjects.end(), object->m_objectID) != colorObjects.end()) {
        m_colorTriggersChanged = true;
    }
    else if (object->m_objectID == 1007) {
        m_alphaTriggersChanged = true;
    }
    else if (object->m_objectID == 1006) {
        m_pulseTriggersChanged = true;
    }
    else if (object->m_objectID == 1268 || object->m_objectID == 2068) {
        m_spawnTriggersChanged = true;
    }

    if (object->m_isSpawnOrderTrigger) {
        m_spawnOrderObjectsChanged = true;
    }
    if (object->m_dontIgnoreDuration) {
        static_cast<EffectGameObject*>(object)->m_endPosition = CCPoint{0, 0};
    }
}