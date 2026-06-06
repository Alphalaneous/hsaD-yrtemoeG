#include "EditorUI.hpp"

uint64_t REEditorUI::s_rngSeed = 0;

EditorUI* REEditorUI::create_(LevelEditorLayer* editorLayer) {
    auto ret = new EditorUI();
    if (ret->init(editorLayer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

int REEditorUI::getRandomStartKey(int specialTemplate) {
    s_rngSeed = s_rngSeed * 0x343FD + 0x269EC3;

    int rand15 = (s_rngSeed >> 16) & 0x7FFF;
    float r = static_cast<float>(rand15) / 32767.f;

    if (specialTemplate == 1461) {
        int idx = static_cast<int>(r * 4.f);

        if (idx == 1) return 1462;
        if (idx == 2) return 1463;
        if (idx != 0) return 1464;
        return 1461;
    }

    if (specialTemplate < 1462) {
        if (specialTemplate == 869) {
            int idx = static_cast<int>(r + r);
            return (idx != 0) ? 870 : 869;
        }

        if (specialTemplate == 1431) {
            int idx = static_cast<int>(r * 3.f);

            if (idx != 0) {
                if (idx != 1) return 1433;
                return 1432;
            }

            return 1431;
        }
    }
    else {
        if (specialTemplate == 2703) {
            int idx = static_cast<int>(r + r);
            return (idx != 0) ? 2704 : 2703;
        }

        if (specialTemplate == 2838) {
            int idx = static_cast<int>(r + r);
            return (idx != 0) ? 2840 : 2838;
        }

        if (specialTemplate == 1861) {
            int idx = static_cast<int>(r * 4.f);

            if (idx == 1) return 1862;
            if (idx == 2) return 1863;
            if (idx != 0) return 1864;
            return 1861;
        }
    }

    return specialTemplate;
}

int REEditorUI::getSmartObjectKey(int key, GJSmartDirection dir) {
    if (key == 1861) {
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::Right) return 1862;
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 1863;
        return 1864;
    }

    if (key == 869) return 870;
    if (key == 870) return 869;

    if (key == 1431 || key == 1432 || key == 1433) {
        s_rngSeed = s_rngSeed * 0x343FD + 0x269EC3;
        float r = ((s_rngSeed >> 16) & 0x7FFF) / 32767.f;
        int idx = static_cast<int>(floorf(r * 3.f));

        if (idx == 0) return 1431;
        if (idx == 1) return 1432;
        return 1433;
    }

    if (key == 1461) {
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::Right) return 1462;
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 1463;
        return 1464;
    }

    if (key == 1462) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 1464;
        if (dir != GJSmartDirection::Left && dir != GJSmartDirection::Right) return 1463;
        return 1461;
    }

    if (key == 1463) {
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::Right) return 1464;
        if (dir != GJSmartDirection::Top && dir != GJSmartDirection::Bottom) return 1462;
        return 1461;
    }

    if (key == 1464) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 1462;
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::Right) return 1463;
    }

    auto rotate4 = [&](int base) {
        int offset = key - base;

        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) {
            return base + ((offset + 2) & 3);
        }

        if (dir == GJSmartDirection::Left) {
            return base + ((offset + 3) & 3);
        }

        if (dir == GJSmartDirection::Right) {
            return base + ((offset + 3) & 3);
        }

        return base + ((offset + 1) & 3);
    };

    if (key >= 1862 && key <= 1864) return rotate4(1862);
    if (key >= 1865 && key <= 1868) return rotate4(1865);
    if (key >= 1869 && key <= 1872) return rotate4(1869);
    if (key >= 1874 && key <= 1877) return rotate4(1874);
    if (key >= 1878 && key <= 1881) return rotate4(1878);
    if (key >= 1882 && key <= 1885) return rotate4(1882);

    if (key == 2703 || key == 2704) {
        if (dir != GJSmartDirection::Left && dir != GJSmartDirection::Right) {
            return (key == 2703) ? 2704 : 2703;
        }

        return key;
    }

    if (key == 2838) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 2840;
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::TopRight || dir == GJSmartDirection::BottomRight) return 2841;
        return 2839;
    }

    if (key == 2839) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 2841;
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::TopRight || dir == GJSmartDirection::BottomRight) return 2838;
        return 2840;
    }

    if (key == 2840) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 2838;
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::TopRight || dir == GJSmartDirection::BottomRight) return 2839;
        return 2841;
    }

    if (key == 2841) {
        if (dir == GJSmartDirection::Top || dir == GJSmartDirection::Bottom) return 2839;
        if (dir == GJSmartDirection::Left || dir == GJSmartDirection::TopRight || dir == GJSmartDirection::BottomRight) return 2840;
        return 2838;
    }

    return 1461;
}

int REEditorUI::smartTypeForKey(int key) {
    switch(key) {
    case 0:
        return 1431;
    case 1:
        return 1461;
    case 2:
        return 1861;
    case 3:
        return 1865;
    case 4:
        return 1869;
    case 5:
        return 1874;
    case 6:
        return 1878;
    case 7:
        return 1882;
    default:
        return 2703;
    case 9:
        return 2704;
    case 10:
        return 2838;
    case 11:
        return 869;
    }
}

void REEditorUI::draw() {
    if (m_swipeActive) {
        ccDrawColor4B({0, 255, 0, 255});
        ccDrawRect(m_swipeStart, m_swipeEnd);
    }
}

bool REEditorUI::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (m_rotationTouchID == -1 && m_rotationControl && m_rotationControl->isVisible() && m_rotationControl->ccTouchBegan(touch, event)) {
        m_rotationTouchID = touch->m_nId;
        return true;
    }

    if (m_scaleTouchID == -1 && m_scaleControl && m_scaleControl->isVisible() && m_scaleControl->ccTouchBegan(touch, event)) {
        m_scaleTouchID = touch->m_nId;
        return true;
    }

    if (m_transformTouchID == -1 && m_transformControl && m_transformControl->isVisible() && m_transformControl->ccTouchBegan(touch, event)) {
        m_transformTouchID = touch->m_nId;
        return true;
    }

    auto world = getTouchPoint_(touch, event);
    m_swipeStart = world;

    if (world.y <= m_toolbarHeight && world.y != m_toolbarHeight) {
        if (m_editorLayer->m_playbackMode != PlaybackMode::Playing || m_playbackBtn->isVisible()) return false;

        m_editorLayer->m_uiLayer->ccTouchBegan(touch, event);
        return true;
    }

    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        m_editorLayer->m_uiLayer->ccTouchBegan(touch, event);
        return true;
    }

    if (m_touchID != -1) return false;

    if (m_undoObject) {
        m_undoObject->release();
        m_undoObject = nullptr;
    }

    auto keyboardDispatcher = CCKeyboardDispatcher::get();

    bool shift = keyboardDispatcher->getShiftKeyPressed();
    bool ctrl = keyboardDispatcher->getControlKeyPressed();
    bool command = keyboardDispatcher->getCommandKeyPressed();
    bool alt = keyboardDispatcher->getAltKeyPressed();

    bool shouldRunSnapLogic = !m_spaceSwiping && !shift && (m_freeMoveEnabled || (!ctrl && command));

    if (shouldRunSnapLogic && m_selectedMode == 3) {
        auto nodeSpace = m_editorLayer->m_objectLayer->convertToNodeSpace(world);

        m_snapSelected = false;
        m_snapObject = nullptr;
        m_findSnap = false;

        if (alt && (m_selectedObject || m_selectedObjects->count() > 0)) {
            onDuplicate(nullptr);
            m_noSnapUndo = true;
        }

        if (m_selectedObjects->count() == 0) {
            if (!m_selectedObject) {
                m_editingObject = true;
                clickOnPosition(world);

                if (m_snapSelected) m_snapObject = m_selectedObject;
            }
            else {
                auto arr = m_editorLayer->objectsAtPosition(nodeSpace);

                for (auto obj : CCArrayExt<GameObject, false>(arr)) {
                    if (obj->m_uniqueID == m_selectedObject->m_uniqueID) {
                        m_snapObject = obj;
                        break;
                    }
                }

                if (!m_snapObject && (!m_selectedObject || !m_swipeEnabled)) {
                    m_editingObject = true;
                    clickOnPosition(world);

                    if (m_snapSelected) m_snapObject = m_selectedObject;
                }
            }
        }
        else {
            findSnapObject(nodeSpace, 0.f);
        }

        if (m_findSnap && !m_snapObject && m_selectedObjects->count() > 0) {
            findSnapObject(nodeSpace, 0.f);
        }

        if (m_snapObject) {
            m_snapObjectExists = true;
            m_snapPosition = m_snapObject->getPosition();
        }

        m_cameraTest = world;

        if (!m_noSnapUndo) {
            auto undoObject = createUndoObject(UndoCommand::Transform, true);

            if (m_undoObject != undoObject) {
                if (undoObject) undoObject->retain();
                if (m_undoObject) m_undoObject->release();

                m_undoObject = undoObject;
            }
        }
    }

    if (!m_spaceSwiping && (m_swipeEnabled || shift) && m_selectedMode == 3) {
        m_swipeActive = true;
        m_swipeEnd = world;
    }

    bool swipeAllowed = m_swipeEnabled || shift;

    m_swipeSelected = swipeAllowed;
    m_continuousSnap = false;
    m_touchID = touch->m_nId;

    stopActionByTag(123);

    if (!m_swipeSelected && GameManager::get()->getGameVariable(GameVar::HoldToSwipe)) {
        auto call = CCCallFunc::create(this, SEL_CallFunc(&EditorUI::triggerSwipeMode));
        auto delay = CCDelayTime::create(0.3f);
        auto seq = CCSequence::create(delay, call, nullptr);

        seq->setTag(123);
        runAction(seq);
    }

    double t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;

    m_keyTime = t;
    m_keyTime2 = t;
    m_swipeModeTriggered = false;

    return true;
}

void REEditorUI::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    int touchID = touch->m_nId;

    if (m_rotationTouchID == touchID) {
        stopActionByTag(123);
        m_rotationControl->ccTouchMoved(touch, event);
        return;
    }

    if (m_scaleTouchID == touchID) {
        stopActionByTag(123);
        m_scaleControl->ccTouchMoved(touch, event);
        return;
    }

    if (m_transformTouchID == touchID) {
        stopActionByTag(123);
        m_transformControl->ccTouchMoved(touch, event);
        return;
    }

    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        m_editorLayer->m_uiLayer->ccTouchMoved(touch, event);
        return;
    }

    if (m_touchID != touchID) {
        return;
    }

    auto world = getTouchPoint_(touch, event);
    auto nodePos = m_editorLayer->m_objectLayer->convertToNodeSpace(world);

    if (!m_snapObjectExists) {
        bool allowSwipe = false;

        if (CCKeyboardDispatcher::get()->getShiftKeyPressed() || m_swipeModeTriggered || m_swipeEnabled) {
            allowSwipe = true;
        }

        if (m_spaceSwiping) {
            allowSwipe = false;
        }

        if (allowSwipe) {
            if (m_selectedMode == 3) {
                m_swipeEnd = world;
                stopActionByTag(123);
                return;
            }

            m_createPosition = getGridSnappedPos_(world);

            clickOnPosition(world);

            stopActionByTag(123);
            return;
        }

        if (m_isDraggingCamera) {
            auto delta = world - m_cameraTest;
            auto camPos = m_editorLayer->m_objectLayer->getPosition();
            camPos += delta;

            m_editorLayer->m_objectLayer->setPosition(camPos);
            constrainGameLayerPosition_();

            m_cameraTest = world;
            updateSlider();
            return;
        }

        float dist = world.getDistance(m_swipeStart);

        if (std::abs(dist) < 20.f) {
            return;
        }

        m_cameraTest = world;
        m_isDraggingCamera = true;

        stopActionByTag(123);
        return;
    }

    if (!m_continueSwipe) {
        float dist = world.getDistance(m_swipeStart);

        if (std::abs(dist) >= 4.f) {
            m_continueSwipe = true;
        }

        if (!m_continueSwipe) {
            stopActionByTag(123);
            return;
        }
    }

    auto cameraNodeSpace = m_editorLayer->m_objectLayer->convertToNodeSpace(m_cameraTest);

    if (!nodePos.equals(cameraNodeSpace)) {
        if (!m_continuousSnap && m_snapObject) {
            if (GameManager::get()->getGameVariable(GameVar::EnableSnap)) {
                if (static_cast<int>(m_snapObject->getRotation()) % 90 == 0) {
                    m_snapObject->setColor({255, 155, 50});
                }
            }
        }

        m_continuousSnap = true;

        auto delta = nodePos - cameraNodeSpace;

        if (m_selectedObjects->count() == 0) {
            moveObject(m_selectedObject, delta);
        } 
        else {
            for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                moveObject(obj, delta);
            }
        }

        m_cameraTest = world;
        m_pivotPoint += delta;

        if (m_rotationControl->isVisible()) {
            auto pos = m_rotationControl->getPosition();
            pos += delta;
            m_rotationControl->setPosition(pos);
            stopActionByTag(123);
            return;
        }

        if (m_scaleControl->isVisible()) {
            auto pos = m_scaleControl->getPosition();
            pos += delta;
            m_scaleControl->setPosition(pos);
            stopActionByTag(123);
            return;
        }

        if (m_transformControl->isVisible()) {
            auto pos = m_transformControl->getPosition();
            pos += delta;
            m_transformControl->setPosition(pos);
            m_transformControl->refreshControl();
        }
    }

    stopActionByTag(123);
}

void REEditorUI::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_canActivateControls = false;
    stopActionByTag(123);

    int touchID = touch->m_nId;

    if (m_rotationTouchID == touchID) {
        m_rotationTouchID = -1;
        m_rotationControl->ccTouchEnded(touch, event);
        return;
    }

    if (m_scaleTouchID == touchID) {
        m_scaleTouchID = -1;
        m_scaleControl->ccTouchEnded(touch, event);
        return;
    }

    if (m_transformTouchID == touchID) {
        m_transformTouchID = -1;
        m_transformControl->ccTouchEnded(touch, event);
        return;
    }

    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        m_editorLayer->m_uiLayer->ccTouchEnded(touch, event);
        return;
    }

    if (m_touchID != touchID) return;

    bool smallThreshold = GameManager::get()->getGameVariable(GameVar::SwipeCycleMode);

    auto world = getTouchPoint_(touch, event);

    float distStart = m_swipeStart.getDistance(world);
    float distLast = m_lastTouchPoint.getDistance(world);

    bool tapCandidate = false;
    bool validTap = false;

    if (smallThreshold && distStart < 2.f) {
        tapCandidate = true;
        if (distLast < 2.f) {
            validTap = true;
        }
    } 
    else {
        if (!smallThreshold && distLast < 2.f) {
            validTap = false;
        }
    }

    m_touchID = -1;
    auto nodePos = m_editorLayer->m_objectLayer->convertToNodeSpace(world);

    if (m_snapObjectExists && m_continuousSnap && m_snapObject) {

        if (GameManager::get()->getGameVariable(GameVar::EnableSnap)) {
            float rot = m_snapObject->getRotation();

            if (static_cast<int>(rot) % 90 == 0) {
                if (!world.equals(m_swipeStart)) {
                    auto gridSnapped = getGridSnappedPos_(nodePos);
                    auto offset = getRelativeOffset_(m_snapObject);
                    auto finalPos = applySpecialOffset(gridSnapped, m_snapObject, offset);

                    auto delta = finalPos - offset;
                    auto selectOffset = finalPos - m_snapObject->getPosition();

                    m_snapObject->setPosition(finalPos);

                    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                        if (obj->m_uniqueID != m_snapObject->m_uniqueID) {
                            moveObject(obj, offset + selectOffset);
                        }
                    }

                    m_pivotPoint += delta;

                    if (m_rotationControl->isVisible()) {
                        auto pos = m_rotationControl->getPosition();
                        pos += delta;
                        m_rotationControl->setPosition(pos);
                    } 
                    else if (m_scaleControl->isVisible()) {
                        auto pos = m_scaleControl->getPosition();
                        pos += delta;
                        m_scaleControl->setPosition(pos);
                    } 
                    else if (m_transformControl->isVisible()) {
                        auto pos = m_transformControl->getPosition();
                        pos += delta;
                        m_transformControl->setPosition(pos);
                        m_transformControl->refreshControl();
                    }
                }
            }
        }

        if (m_snapObject->getPosition().equals(m_snapPosition)) {
            m_noSnapUndo = true;
        }

        if (!m_noSnapUndo && m_undoObject) {
            auto layer = m_editorLayer;
            layer->m_redoObjects->removeAllObjects();

            unsigned int limit = layer->m_increaseMaxUndoRedo ? 1000 : 200;

            if (layer->m_undoObjects->count() >= limit) {
                layer->m_undoObjects->removeObjectAtIndex(0, true);
            }

            layer->m_undoObjects->addObject(m_undoObject);
            m_undoObject->release();
            m_undoObject = nullptr;
        }
    } 
    else {
        if (!m_swipeActive || tapCandidate) {
            if (m_isDraggingCamera) {
                m_isDraggingCamera = false;
            } 
            else {
                float dist = m_swipeStart.getDistance(world);

                if (!(m_swipeSelected && dist >= 20.f)) {
                    if (m_selectedMode != 3) {
                        if (!m_swipeModeTriggered) {
                            clickOnPosition(m_swipeStart);
                        }
                    } 
                    else {
                        if (!(m_snapObject && m_editingObject)) {
                            auto objects = m_editorLayer->objectsAtPosition(nodePos);
                            auto obj = getCycledObject_(objects, false);

                            if (obj) {
                                createUndoSelectObject(false);

                                if (!m_swipeActive) {
                                    selectObject(obj, false);
                                } 
                                else if (validTap) {
                                    if (!m_selectedObject) {
                                        auto last = static_cast<GameObject*>(m_selectedObjects->lastObject());
                                        if (last) {
                                            last->m_isSelected = false;
                                            last->updateObjectEditorColor();
                                            m_selectedObjects->removeLastObject(true);
                                        }
                                    } 
                                    else {
                                        stopActionByTag(124);

                                        m_selectedObject->m_isSelected = false;
                                        m_selectedObject->updateObjectEditorColor();
                                        m_selectedObject = nullptr;

                                        toggleEditObjectButton();
                                        m_canActivateControls = true;
                                        m_lastTouchPoint = CCPoint{0, 0};
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } 
        else {
            auto start = m_editorLayer->m_objectLayer->convertToNodeSpace(m_swipeStart);

            float dx = std::abs(start.x - nodePos.x);
            float dy = std::abs(start.y - nodePos.y);

            auto origin = CCPoint{std::min(start.x, nodePos.x), std::min(start.y, nodePos.y)};
            auto rect = CCRect{origin.x, origin.y, dx, dy};

            float dist = m_swipeStart.getDistance(world);

            if (dist >= 2.f) {
                selectObjectsInRect_(rect);
            } 
            else {
                auto objects = m_editorLayer->objectsAtPosition(nodePos);

                auto obj = getCycledObject_(objects, false);

                if (obj) {
                    createUndoSelectObject(false);

                    auto arr = CCArray::createWithObject(obj);
                    selectObjects(arr, false);
                }
            }
        }
    }

    if (m_snapObject) {
        if (m_snapObjectExists && m_continuousSnap) {
            if (!m_snapObject->m_isSelected) {
                m_snapObject->updateObjectEditorColor();
            } 
            else {
                m_snapObject->selectObject({0, 255, 0});
            }
        }
        m_snapObject = nullptr;
    }

    m_swipeActive = false;
    m_swipeSelected = false;
    m_snapObjectExists = false;
    m_noSnapUndo = false;
    m_createPosition = CCPoint{0, 0};
    m_continuousSnap = false;
    m_editingObject = false;
    m_swipePosition = m_swipeStart;
    m_continueSwipe = false;

    updateButtons();

    m_cycledObjectIndex++;

    m_snapPositions->removeAllObjects();

    if (m_canActivateControls) {
        updateSpecialUIElements_();
        updateTransformControl();
        m_canActivateControls = false;
    }

    m_lastTouchPoint = world;
}

void REEditorUI::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    ccTouchEnded(touch, event);
}

void REEditorUI::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, 0, true);
}

void REEditorUI::keyBackClicked() {
    onPause(this);
}

void REEditorUI::keyDown(enumKeyCodes key, double timestamp) {
    m_keyTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;

    if (key == enumKeyCodes::KEY_Space) {
        if (m_pressedSpace) return;
        m_pressedSpace = true;
    }

    auto keyboardDispatcher = CCDirector::get()->getKeyboardDispatcher();

    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        m_editorLayer->m_uiLayer->keyDown(key, timestamp);
    }
    else {
        if (key == enumKeyCodes::KEY_Z) {
            if (keyboardDispatcher->getControlKeyPressed()) {
                if (keyboardDispatcher->getShiftKeyPressed()) {
                    redoLastAction(nullptr);
                    return;
                }
                undoLastAction(nullptr);
                return;
            }
        }
        if (key == enumKeyCodes::KEY_D) {
            if (!keyboardDispatcher->getAltKeyPressed()) {
                if (!keyboardDispatcher->getControlKeyPressed()) {
                    auto move = EditCommand::Right;
                    if (keyboardDispatcher->getShiftKeyPressed()) {
                        move = EditCommand::SmallRight;
                    }
                    moveObjectCall(move);
                }
                else {
                    onDuplicate(nullptr);
                }
            }
            else {
                if (m_selectedObject || m_selectedObjects->count() != 0) {
                    createUndoSelectObject(false);
                }
                deselectAll();
            }
            return;
        }
        if (key == enumKeyCodes::KEY_C) {
            if (keyboardDispatcher->getControlKeyPressed() && key == 0x43) {
                onCopy(nullptr);
                return;
            }
        }
        if (key == enumKeyCodes::KEY_V) {
            if (keyboardDispatcher->getControlKeyPressed()) {
                onPaste(nullptr);
                return;
            }
        }
        if (key == enumKeyCodes::KEY_Right) {
            onGroupUp(nullptr);
            return;
        }
        if (key == enumKeyCodes::KEY_Left) {
            onGroupDown(nullptr);
            return;
        }
        if (key >= enumKeyCodes::KEY_Zero && key <= enumKeyCodes::KEY_Nine) {
            auto num = key - 0x30;
            auto objectLayerScale = m_editorLayer->m_objectLayer->getScale();
            auto winSize = CCDirector::get()->getWinSize();
            auto posY = winSize.height * 0.5f + m_toolbarHeight * 0.5f;

            if (keyboardDispatcher->getAltKeyPressed()) {
                auto savedPos = m_editorLayer->getSavedEditorPosition(num);
                if (savedPos.x != 0.f || savedPos.y != 0.f) {
                    m_editorLayer->m_objectLayer->setPosition(winSize.width * 0.5 - savedPos.x * objectLayerScale, posY - savedPos.y * objectLayerScale);
                }
                return;
            }
            if (keyboardDispatcher->getControlKeyPressed()) {
                auto objectLayerPos = m_editorLayer->m_objectLayer->getPosition();
                auto adjusted = CCPoint{(winSize.width * 0.5f - objectLayerPos.x) / objectLayerScale, (posY - objectLayerPos.y) / objectLayerScale};

                m_editorLayer->saveEditorPosition(adjusted, num);

                auto str = CCString::createWithFormat("Position %i saved.", num);
                auto alert = TextAlertPopup::create(str->getCString(), 0.5f, 0.6f, 0x96, "bigFont.fnt");
                addChild(alert, 100, 0x96);
                return;
            }
        }
        if (key == enumKeyCodes::KEY_One) {
            toggleMode(m_buildModeBtn);
            return;
        }
        if (key == enumKeyCodes::KEY_Two) {
            toggleMode(m_editModeBtn);
            return;
        }
        if (key == enumKeyCodes::KEY_Three) {
            toggleMode(m_deleteModeBtn);
            return;
        }
        if (key == enumKeyCodes::KEY_OEMPlus) {
            if (!keyboardDispatcher->getShiftKeyPressed()) {
                moveGamelayer({0.f, -10.f});
            }
            else {
                zoomGameLayer_(true);
            }
            return;
        }
        if (key == enumKeyCodes::KEY_OEMMinus) {
            if (!keyboardDispatcher->getShiftKeyPressed()) {
                moveGamelayer({0.f, 10.f});
            }
            else {
                zoomGameLayer_(false);
            }
            return;
        }
        if (key == enumKeyCodes::KEY_Delete || key == enumKeyCodes::KEY_Backspace){
            onDeleteSelected(nullptr);
            return;
        }
        if (key == enumKeyCodes::KEY_T) {
            if (!keyboardDispatcher->getControlKeyPressed()) {
                toggleSwipe(nullptr);;
            }
            else {
                activateTransformControl(nullptr);
            }
            return;
        }
        if (key == enumKeyCodes::KEY_Q) {
            if (!keyboardDispatcher->getAltKeyPressed()) {
                transformObjectCall(EditCommand::RotateCCW);
            }
            else {
                transformObjectCall(EditCommand::FlipX);
            }
            return;
        }
        if (key == enumKeyCodes::KEY_E) {
            if (!keyboardDispatcher->getAltKeyPressed()) {
                transformObjectCall(EditCommand::RotateCW);
            }
            else {
                transformObjectCall(EditCommand::FlipY);
            }
            return;
        }
        if (key == enumKeyCodes::KEY_A) {
            auto move = EditCommand::Left;
            if (keyboardDispatcher->getShiftKeyPressed()) {
                move = EditCommand::SmallLeft;
            }
            moveObjectCall(move);
            return;
        }
        if (key == enumKeyCodes::KEY_W) {
            auto move = EditCommand::Up;
            if (keyboardDispatcher->getShiftKeyPressed()) {
                move = EditCommand::SmallUp;
            }
            moveObjectCall(move);
            return;
        }
        if (key == enumKeyCodes::KEY_S) {
            auto move = EditCommand::Down;
            if (keyboardDispatcher->getShiftKeyPressed()) {
                move = EditCommand::SmallDown;
            }
            moveObjectCall(move);
            return;
        }
        if (key == enumKeyCodes::KEY_R) {
            toggleFreeMove(nullptr);
            return;
        }
        if (key == enumKeyCodes::KEY_G) {
            toggleSnap(nullptr);
            return;
        }
        if (key == enumKeyCodes::KEY_F1) {
            auto objectLayerScale = m_editorLayer->m_objectLayer->getScale();
            auto winSize = CCDirector::get()->getWinSize();
            auto objectLayerPos = m_editorLayer->m_objectLayer->getPosition();
            auto posY = winSize.height * 0.5f + m_toolbarHeight * 0.5f;
            
            auto adjusted = CCPoint{(winSize.width * 0.5f - objectLayerPos.x) / objectLayerScale, (posY - objectLayerPos.y) / objectLayerScale};
            m_editorLayer->m_previewPosition = adjusted;

            auto alert = TextAlertPopup::create("Preview Locked", 0.5f, 0.6f, 150, "bigFont.fnt");
            addChild(alert, 100, 150);
            return;
        }
        if (key == enumKeyCodes::KEY_F2) {
            m_editorLayer->m_previewPosition = CCPoint{0, 0};

            auto alert = TextAlertPopup::create("Preview Unlocked", 0.5f, 0.6f, 150, "bigFont.fnt");
            addChild(alert, 100, 150);
            return;
        }
        auto gameManager = GameManager::get();
        if (key == enumKeyCodes::KEY_F6) {
            gameManager->toggleGameVariable("0121");
            auto hideInvisible = gameManager->getGameVariable("0121");

            std::string str = hideInvisible ? "On" : "Off";

            auto alert = TextAlertPopup::create("Hide Invisible: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            addChild(alert, 100, 100);
            return;
        }
        if (key == enumKeyCodes::KEY_F5) {
            gameManager->toggleGameVariable("0045");
            m_editorLayer->updateOptions();

            auto showHitboxes = gameManager->getGameVariable("0045");

            std::string str = showHitboxes ? "On" : "Off";

            auto alert = TextAlertPopup::create("Show Hitboxes: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            addChild(alert, 100, 100);
            return;
        }
        if (key == enumKeyCodes::KEY_F4) {
            gameManager->toggleGameVariable("0137");
            m_editorLayer->updatePreviewParticles();

            auto previewParticles = gameManager->getGameVariable("0137");

            std::string str = previewParticles ? "On" : "Off";

            auto alert = TextAlertPopup::create("Particle Icons: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            addChild(alert, 100, 100);
            return;
        }
        if (key == enumKeyCodes::KEY_F3) {
            gameManager->toggleGameVariable("0036");
            m_editorLayer->updateEditorMode();

            auto previewMode = gameManager->getGameVariable("0036");

            std::string str = previewMode ? "On" : "Off";

            auto alert = TextAlertPopup::create("Preview Mode: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            addChild(alert, 100, 100);
            return;
        }
        if (key == enumKeyCodes::KEY_F7) {
            auto linkControlsToggle = gameManager->getGameVariable("0097");
            if (!linkControlsToggle) return;

            gameManager->toggleGameVariable("0180");
            auto linkControlsOff = gameManager->getGameVariable("0180");
            
            m_linkControlsDisabled = linkControlsOff;

            updateButtons();

            std::string str = !linkControlsOff ? "On" : "Off";

            auto alert = TextAlertPopup::create("Link Mode: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            addChild(alert, 100, 100);
            return;
        }
        if (key == enumKeyCodes::KEY_F8) {
            if (m_selectedObjects->count() == 0) {
                auto obj = m_selectedObject;
                if (!obj || m_editorLayer->m_traceInObject == obj) {
                    obj = nullptr;
                }
                m_editorLayer->m_traceInObject = static_cast<EffectGameObject*>(obj);

                std::string str = m_editorLayer->m_traceInObject ? "On" : "Off";

                auto alert = TextAlertPopup::create("Trace In: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
                alert->setLabelColor({0, 255, 0});
                alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
                addChild(alert, 100, 100);

                m_editorLayer->m_unk3b22 = true;
            }
            return;
        }
        if (key == enumKeyCodes::KEY_F9) {
            if (m_selectedObjects->count() == 0) {
                auto obj = m_selectedObject;
                if (!obj || m_editorLayer->m_traceOutObject == obj || obj->m_classType != GameObjectClassType::Effect) {
                    obj = nullptr;
                }
                m_editorLayer->m_traceOutObject = static_cast<EffectGameObject*>(obj);

                std::string str = m_editorLayer->m_traceOutObject ? "On" : "Off";

                auto alert = TextAlertPopup::create("Trace Out: " + str, 0.6f, 0.6f, 100, "chatFont.fnt");
                alert->setLabelColor({0, 255, 0});
                alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
                addChild(alert, 100, 100);
            }
            return;
        }

        if (!keyboardDispatcher->getControlKeyPressed()) {
            if (keyboardDispatcher->getAltKeyPressed() && key == enumKeyCodes::KEY_Up) {
                onTargetIDChange(1);
                return;
            }
            if (keyboardDispatcher->getAltKeyPressed() && key == enumKeyCodes::KEY_Down) {
                onTargetIDChange(-1);
                return;
            }
            if (key == enumKeyCodes::KEY_F3) {
                onAssignNewGroupID();
                return;
            }
        }
        else {
            if (key == enumKeyCodes::KEY_Enter) {
                onPlayback(nullptr);
            }
        }
    }
    if (key == enumKeyCodes::CONTROLLER_Start || key == enumKeyCodes::KEY_Enter) {
        if (!m_playtestStopBtn->isVisible()) {
            if (m_touchID == -1) {
                onPlaytest(nullptr);
            }
        }
        else {
            onStopPlaytest(nullptr);
        }
        return;
    }
    if (key == enumKeyCodes::KEY_Escape || key == enumKeyCodes::CONTROLLER_Back) {
        onPause(nullptr);
        return;
    }
    if (key == enumKeyCodes::KEY_Space) {
        m_spaceSwiping = true;
        return;
    }
}

EditorUI* REEditorUI::getUI() {
    return this;
}

void REEditorUI::setIDPopupClosed(SetIDPopup* popup, int value) {
    //todo
    EditorUI::setIDPopupClosed(popup, value);
}

void REEditorUI::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    auto tag = layer->getTag();

    if (tag == 122 || tag == 125 || tag == 126) {
        m_alertShown = false;
    }

    switch (tag) {
        case 122: {
            return;
        }
        case 125: {
            if (btn2) {
                m_editorLayer->m_level->m_highObjectsEnabled = true;
            }
            return;
        }
        case 126: {
            if (btn2) {
                m_editorLayer->m_level->m_unlimitedObjectsEnabled = true;
            }
            return;
        }
        default: {
            if (btn2) {
                onDeleteAll(this);
            }
            return;
        }
    }
}

void REEditorUI::updateTransformControl() {
    //todo
    EditorUI::updateTransformControl();
}

void REEditorUI::transformChangeBegin() {
    createUndoObject(UndoCommand::Transform, false);
}

void REEditorUI::transformChangeEnded_() {}

CCNode* REEditorUI::getTransformNode() {
    return m_transformNode;
}

void REEditorUI::transformScaleXChanged(float scaleX) {
    m_transformState.m_scaleX = scaleX;
    transformObjectsActive();
}

void REEditorUI::transformScaleYChanged(float scaleY) {
    m_transformState.m_scaleY = scaleY;
    transformObjectsActive();
}

void REEditorUI::transformScaleXYChanged(float scaleX, float scaleY) {
    m_transformState.m_scaleX = scaleX;
    m_transformState.m_scaleY = scaleY;
    transformObjectsActive();
}

void REEditorUI::transformSkewXChanged(float skewX) {
    m_transformState.m_skewX = skewX;
    transformObjectsActive();
}

void REEditorUI::transformSkewYChanged(float skewY) {
    m_transformState.m_skewY = skewY;
    transformObjectsActive();
}

void REEditorUI::transformRotationXChanged(float rotationX) {
    m_transformState.m_angleX = rotationX;
    transformObjectsActive();
}

void REEditorUI::transformRotationYChanged(float rotationY) {
    m_transformState.m_angleY = rotationY;
    transformObjectsActive();
}

void REEditorUI::transformRotationChanged(float rotation) {
    m_transformState.m_angleX = rotation;
    m_transformState.m_angleY = rotation;
    transformObjectsActive();
}

void REEditorUI::transformResetRotation() {
    float angleX = m_transformState.m_angleX;
    m_transformState.m_transformRotation = angleX;

    if (angleX == 0) {
        return;
    }

    m_transformState.m_angleX = 0;
    m_transformState.m_angleY = 0;
    transformObjectsActive();
}

void REEditorUI::transformRestoreRotation() {
    float rotation = m_transformState.m_transformRotation;

    if (rotation == 0) {
        return;
    }

    m_transformState.m_angleX = rotation;
    m_transformState.m_angleY = rotation;
    transformObjectsActive();
}

void REEditorUI::songStateChanged() {
    updatePlaybackBtn();
}

void REEditorUI::colorSelectClosed(CCNode* popup) {
    bool closeButtonPressed;
    if (m_colorOverlay == popup) {
        closeButtonPressed = m_colorOverlay->m_closeButtonPressed;
        m_colorOverlay = nullptr;
    }
    else if (m_hsvOverlay == popup) {
        closeButtonPressed = m_hsvOverlay->m_unkBool3;
        m_hsvOverlay = nullptr;
    }

    resetSelectedObjectsColor();
    if (closeButtonPressed) {
        updateEditColorButton();
    }
}

void REEditorUI::keyUp(enumKeyCodes key, double timestamp) {
    if (key == enumKeyCodes::KEY_Space || key == enumKeyCodes::KEY_W) {
        m_pressedSpace = false;
        m_spaceSwiping = false;
    }
    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        m_editorLayer->m_editorUI->keyUp(key, timestamp);
    }
}

void REEditorUI::scrollWheel(float y, float x) {
    if (m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_keyTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;

    if (CCKeyboardDispatcher::get()->getControlKeyPressed()) {
        zoomGameLayer(y <= 0.f && x <= 0.f);
    }
    else {
        moveGamelayer({x, y});
        updateSlider();
    }
}

void REEditorUI::angleChangeBegin() {
    createUndoObject(UndoCommand::Transform, false);
}

void REEditorUI::angleChangeEnded_() {}

void REEditorUI::angleChanged(float rotation) {
    if (!m_selectedObject) {
        if (m_selectedObjects->count() != 0) {
            rotateObjects(m_selectedObjects, rotation, m_pivotPoint);
        }
    }
    else {
        auto rotX = m_selectedObject->getRotationX();
        auto rotY = m_selectedObject->getRotationY();

        if (rotX != rotY) {
            m_selectedObject->setRotationX(m_selectedObject->getRotationX() + rotX);
            m_selectedObject->setRotationY(m_selectedObject->getRotationY() + rotY);
        }
        else {
            m_selectedObject->setRotation(m_selectedObject->getRotation() + rotation);
        }
    }
}

void REEditorUI::updateScaleControl() {
    if (!m_scaleControl->isVisible()) return;

    bool isTrigger = false;
    if (!m_selectedObject) {
        if (m_selectedObjects->count() != 0) {
            isTrigger = true;
            for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                if (!obj->isTrigger()) {
                    isTrigger = false;
                    break;
                }
            }
        }
    }
    else {
        isTrigger = m_selectedObject->isTrigger();
    }

    if (!m_increaseScale) {
        m_scaleControl->m_lowerBound = 0.5f;
        if (!isTrigger && !m_selectedObject || m_selectedObject->m_objectID != 0x2eb) {
            m_scaleControl->m_upperBound = 2.f;
        }
        else {
            m_scaleControl->m_upperBound = 4.f;
        }
    }
    else {
        m_scaleControl->m_upperBound = 4.f;
        m_scaleControl->m_lowerBound = 0.25f;
    }

    m_scaleControl->loadValues(m_selectedObject, m_selectedObjects, m_objectEditorStates);
}

void REEditorUI::anchorPointMoved(CCPoint anchorPoint) {
    auto nodeSpace = m_editorLayer->m_objectLayer->convertToNodeSpace(anchorPoint);
    m_pivotPoint = nodeSpace;
    m_transformControl->setPosition(nodeSpace);
}

void REEditorUI::scaleChangeBegin() {
    createUndoObject(UndoCommand::Transform, false);
}

void REEditorUI::scaleChangeEnded_() {}

void REEditorUI::scaleXChanged(float scaleX, bool lock) {
    if (m_selectedObject) {
        m_selectedObject->updateCustomScaleX(scaleX * m_selectedObject->m_pixelScaleX);
    }
    else if (m_selectedObjects->count() != 0) {
        scaleObjects(m_selectedObjects, scaleX, 0, m_pivotPoint, ObjectScaleType::X, lock);
    }
}

void REEditorUI::scaleYChanged(float scaleY, bool lock) {
    if (m_selectedObject) {
        m_selectedObject->updateCustomScaleY(scaleY * m_selectedObject->m_pixelScaleY);
    }
    else if (m_selectedObjects->count() != 0) {
        scaleObjects(m_selectedObjects, 0, scaleY, m_pivotPoint, ObjectScaleType::Y, lock);
    }
}

void REEditorUI::scaleXYChanged(float scaleX, float scaleY, bool lock) {
    if (m_selectedObject) {
        scaleXChanged(scaleX, lock);
        scaleYChanged(scaleY, lock);
    }
    else if (m_selectedObjects->count() != 0) {
        scaleObjects(m_selectedObjects, scaleX, scaleY, m_pivotPoint, ObjectScaleType::XY, lock);
    }
}

void REEditorUI::activateRotationControl(CCObject* sender) {
    if ((m_selectedObjects->count() != 0 || m_selectedObject) && (!m_rotationControl->isVisible() || m_canActivateControls)) {
        deactivateTransformControl();
        m_canActivateControls = false;
        if (m_selectedObjects->count() == 0) {
            m_rotationControl->setPosition(m_selectedObject->getPosition());
        }
        else {
            auto pos = getGroupCenter(m_selectedObjects, false);
            m_pivotPoint = pos;
            m_rotationControl->setPosition(pos);
        }
        auto scale = m_editorLayer->m_objectLayer->getScale();
        m_rotationControl->setScale(1.f / scale);
        m_rotationControl->setVisible(true);
    }
}

void REEditorUI::activateScaleControl(CCObject* sender) {
    if (m_selectedObjects->count() != 0 || m_selectedObject) {
        deactivateTransformControl();
        if (m_scaleControl->isVisible()) {
            auto tag = m_scaleControl->m_senderTag;
            deactivateScaleControl();
            if (tag == sender->getTag()) {
                return;
            }
        }
        if (!m_scaleControl->isVisible() || m_canActivateControls) {
            updateEditButtonColor(sender->getTag(), {0xa6, 0xa6, 0xa6});
            m_canActivateControls = false;

            CCPoint position;

            if (m_selectedObjects->count() == 0) {
                position = m_selectedObject->getPosition();
            }
            else {
                position = getGroupCenter(m_selectedObjects, false);
            }
            m_pivotPoint = position;
            m_scaleControl->m_senderTag = sender->getTag();

            m_scaleControl->setPosition(m_pivotPoint + CCPoint{0.f, 40.f});

            auto scale = m_editorLayer->m_objectLayer->getScale();

            m_scaleControl->setScale(1.f / scale);
            m_scaleControl->setVisible(true);

            updateScaleControl();
        }
    }
}

void REEditorUI::activateTransformControl(CCObject* sender) {
    if (m_selectedObjects->count() != 0 || m_selectedObject) {
        deactivateScaleControl();
        deactivateRotationControl_();
        if (m_transformControl->isVisible()) {
            deactivateTransformControl();
            return;
        }

        if (!m_transformControl->isVisible() || m_canActivateControls) {
            m_transformState.m_transformReset = true;
            updateEditButtonColor(sender->getTag(), {0xa6, 0xa6, 0xa6});
            m_canActivateControls = false;

            CCPoint position;

            if (m_selectedObjects->count() == 0) {
                position = m_selectedObject->getPosition();
            }
            else {
                position = getGroupCenter(m_selectedObjects, false);
            }

            m_pivotPoint = position;
            m_transformControl->setPosition(position);

            auto scale = m_editorLayer->m_objectLayer->getScale();

            m_transformControl->scaleButtons(1.f / scale);
            updateTransformControl();
            m_transformControl->setVisible(true);
            //m_transformControl->updateAnchorSprite(m_pivotPoint - position) FIX BINDINGS, Tag is 1

            m_transformControl->spriteByTag(1)->setPosition(m_pivotPoint - m_transformControl->getPosition());
            if (m_transformControl->m_delegate) m_transformControl->m_delegate->updateTransformControl();
        }

    }
}

void REEditorUI::addObjectsToSmartTemplate(GJSmartTemplate* smartTemplate, CCArray* objects) {
    //todo
    EditorUI::addObjectsToSmartTemplate(smartTemplate, objects);
}

void REEditorUI::addSnapPosition_(CCPoint position) {
    auto node = CCNode::create();
    node->setPosition(position);
    m_snapPositions->addObject(node);
}

void REEditorUI::alignObjects(CCArray* objects, bool axisY) {
    //todo
    EditorUI::alignObjects(objects, axisY);
}

void REEditorUI::applyOffset(GameObject* object) {
    auto offset = offsetForKey(object->m_objectID);

    auto relativeOffset = GameToolbox::getRelativeOffset(object, offset);
    object->setPosition(object->getPosition() + relativeOffset);

    m_editorLayer->reorderObjectSection(object);
}

CCPoint REEditorUI::applySpecialOffset(CCPoint position, GameObject* object, CCPoint offset) {
    if (isSpecialSnapObject(object->m_objectID)) {
        auto objPos = object->getPosition();
        auto gridSnapped = getGridSnappedPos_(objPos);

        if (!offset.equals({0.f, 0.f})) {
            gridSnapped = offset;
        }

        float dx = std::abs(position.x);

        if (dx > 0.f) {
            if (std::abs((objPos.x + dx) - gridSnapped.x) < std::abs((objPos.x - dx) - gridSnapped.x)) {
                dx = -dx;
            }
            position.x = dx;
        }

        float dy = std::abs(position.y);

        if (dy > 0.f) {
            if (std::abs((objPos.y + dy) - gridSnapped.y) < std::abs((objPos.y - dy) - gridSnapped.y)) {
                dy = -dy;
            }
            position.y = dy;
        }
    }

    return position;
}

bool REEditorUI::arrayContainsClass_(CCArray* objects, int classType) {
    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if (static_cast<int>(obj->m_classType) == classType) {
            return true;
        }
    }

    return false;
}

void REEditorUI::assignNewGroups(bool groupY) {
    //todo
    return EditorUI::assignNewGroups(groupY);
}

bool REEditorUI::canAllowMultiActivate_(GameObject* object, CCArray* objects) {
    if (object) {
        return object->canAllowMultiActivate();
    }
    else {
        for (auto obj : CCArrayExt<GameObject, false>(objects)) {
            if (!obj->canAllowMultiActivate()) {
                return false;
            }
        }
        return true;
    }
}

bool REEditorUI::canSelectObject(GameObject* object) {
    auto gameManager = GameManager::get();

    if (gameManager->getGameVariable(GameVar::SelectFilter)) {
        if (gameManager->getIntGameVariable(GameVar::DeleteFilter) == 3) {
            int deleteFilter = gameManager->getIntGameVariable(GameVar::CustomDeleteFilter);

            if (deleteFilter != object->m_objectID) {
                return false;
            }
        }
    }

    int groupFilter = gameManager->getIntGameVariable(GameVar::GroupIDFilter);

    if (groupFilter != 0) {
        if (!object->belongsToGroup(groupFilter)) {
            return false;
        }
    }

    int colorFilter = gameManager->getIntGameVariable(GameVar::ColorIDFilter);

    if (colorFilter != 0) {
        int mainColor = object->getMainColorMode();
        int secondaryColor = object->getSecondaryColorMode();

        if (colorFilter != mainColor && colorFilter != secondaryColor) {
            return false;
        }
    }

    return true;
}

void REEditorUI::centerCameraOnObject_(GameObject* object) {
    auto winSize = CCDirector::get()->getWinSize();
    auto scale = m_editorLayer->m_objectLayer->getScale();

    m_editorLayer->m_objectLayer->setPosition(winSize * .5f - object->getPosition() * scale);
    constrainGameLayerPosition_();
}

void REEditorUI::changeSelectedObjects_(CCArray* objects, bool ignoreFilter) {
    deselectAll();
    if (objects->count() == 0) return;

    selectObjects(objects, ignoreFilter);
    m_canActivateControls = true;

    updateObjectInfoLabel();
    updateScaleControl();
}

CCPoint REEditorUI::checkDiffAfterTransformAnchor(CCPoint diff, CCArray* objects) {
    //todo
    return EditorUI::checkDiffAfterTransformAnchor(diff, objects);
}

void REEditorUI::checkLiveColorSelect() {
    //todo
    EditorUI::checkLiveColorSelect();
}

void REEditorUI::clickOnPosition(CCPoint position) {
    if (m_toolbarHeight <= position.y) {
        m_clickAtPosition = m_editorLayer->m_objectLayer->convertToNodeSpace(position);
        if (m_selectedMode == 1) {
            onDelete(nullptr);
        }
        if (m_selectedMode == 2) {
            bool created = onCreate();
            if (!created && !m_swipeEnabled) {
                auto object = m_editorLayer->objectAtPosition(m_clickAtPosition);
                if (object) {
                    m_selectedObjectIndex = object->m_objectID;
                    updateCreateMenu(true);
                    updateButtons();
                }
            }
        }
        if (m_selectedMode == 3) {
            auto object = m_editorLayer->objectAtPosition(m_clickAtPosition);
            if (object && (!m_selectedObject || m_selectedObject->m_uniqueID != object->m_uniqueID)) {
                createUndoSelectObject(false);
                selectObject(object, false);
            }
            updateEditMenu_();
            updateSpecialUIElements_();
        }
        updateButtons();
    }
}

void REEditorUI::closeLiveColorSelect() {
    if (m_colorOverlay) {
        m_colorOverlay->closeColorSelect(nullptr);
        m_colorOverlay = nullptr;
    }
}

void REEditorUI::closeLiveHSVSelect() {
    if (m_hsvOverlay) {
        m_hsvOverlay->closeColorSelect(nullptr);
        m_hsvOverlay = nullptr;
    }
}

void REEditorUI::colorSelectClosed_(ccColor3B color) {}

void REEditorUI::constrainGameLayerPosition_() {
    constrainGameLayerPosition(-100, -100);
}

void REEditorUI::constrainGameLayerPosition(float x, float y) {
    auto currentPos = m_editorLayer->m_objectLayer->getPosition();

    m_editorLayer->m_objectLayer->setPosition({0, 0});

    auto director = CCDirector::get();

    auto screenLeftBound = CCPoint{director->getScreenLeft(), m_constrainedHeight};
    auto targetGridPos = CCPoint{x * 30.f - 30.f, y * 30.f + 90.f};
    auto worldTarget = m_editorLayer->convertToNodeSpace(m_editorLayer->m_objectLayer->convertToWorldSpace(targetGridPos));
    auto screenTopRight = CCPoint{director->getScreenRight(), director->getScreenTop()};

    int heightIndex = 80;

    if (m_editorLayer->m_levelSettings->m_dynamicLevelHeight) {
        heightIndex = 1000;
    }

    auto bottomBoundWorld = CCPoint{240030.f, heightIndex * 30.f + 90.f + 30.f};
    auto bottomBound = m_editorLayer->m_objectLayer->convertToWorldSpace(bottomBoundWorld);
    auto bottomLocal = m_editorLayer->convertToNodeSpace(bottomBoundWorld);

    bottomBound = bottomLocal;

    float minX = screenLeftBound.x - worldTarget.x;
    if (currentPos.x <= minX) {
        minX = currentPos.x;
    }

    float maxX = screenTopRight.x - bottomBound.x;
    if (maxX <= minX) {
        maxX = minX;
    }

    currentPos.x = maxX;

    float minY = screenLeftBound.y - worldTarget.y;
    if (currentPos.y <= minY) {
        minY = currentPos.y;
    }

    float maxY = screenTopRight.y - bottomBound.y;
    if (maxY <= minY) {
        maxY = minY;
    }

    currentPos.y = maxY;

    m_editorLayer->m_objectLayer->setPosition(currentPos);
}

int REEditorUI::convertKeyBasedOnNeighbors(int smartKey, int objectID, CCPoint position, CCArray* objects) {
    int base = convertToBaseKey(smartKey);

    auto top = getNeighbor(objectID, position, GJSmartDirection::Top, objects);
    auto bottom = getNeighbor(objectID, position, GJSmartDirection::Bottom, objects);
    auto left = getNeighbor(objectID, position, GJSmartDirection::Left, objects);
    auto right = getNeighbor(objectID, position, GJSmartDirection::Right, objects);

    if (base >= 2838 && base <= 2841) {
        switch (base) {
            case 2838:
                if ((top && bottom) || left) {
                    return 2838;
                }
                if (top) {
                    return bottom ? 2838 : 2846;
                }
                return 2842;

            case 2839:
                if ((top && bottom && left) || right) {
                    return 2839;
                }
                if (top) {
                    return bottom ? 2839 : 2847;
                }
                return 2843;

            case 2840:
                if ((top && bottom) || left) {
                    return 2840;
                }
                if (top) {
                    return bottom ? 2840 : 2848;
                }
                return 2844;

            case 2841:
                if ((top && bottom && left) || right) {
                    return 2841;
                }
                if (top) {
                    return bottom ? 2841 : 2849;
                }
                return 2845;
        }
    }

    if (base == 2703) {
        if (top) {
            if (bottom) {
                if (left) {
                    return right ? 2703 : 2811;
                }
                else {
                    return right ? 2810 : 2812;
                }
            }
            else {
                if (!left) {
                    return right ? 2816 : 2819;
                }
                else {
                    return right ? 2809 : 2817;
                }
            }
        }
        else {
            if (!bottom) {
                if (!left) {
                    return right ? 2820 : 2822;
                }
                else {
                    return right ? 2813 : 2821;
                }
            }
            else {
                if (!left) {
                    return right ? 2814 : 2818;
                }
                else {
                    return right ? 2808 : 2815;
                }
            }
        }
    }

    if (base == 2704) {
        if (!top) {
            if (!bottom) {
                if (!left) {
                    return right ? 2835 : 2837;
                }
                else {
                    return right ? 2828 : 2836;
                }
            }
            else {
                if (!left) {
                    return right ? 2829 : 2833;
                }
                else {
                    return right ? 2823 : 2830;
                }
            }
        }
        else {
            if (!bottom) {
                if (!left) {
                    return right ? 2831 : 2834;
                }
                else {
                    return right ? 2824 : 2832;
                }
            }
            else {
                if (!left) {
                    return right ? 2825 : 2827;
                }
                else {
                    return right ? 2704 : 2826;
                }
            }
        }
    }

    if (base == 869) {
        s_rngSeed = s_rngSeed * 0x343FD + 0x269EC3;
        bool r = (((s_rngSeed >> 16) & 0x7FFF) / 32767.f) > 0.5f;

        if (!top) {
            if (bottom) {
                if (left) {
                    if (!right) {
                        return r ? 872 : 1267;
                    }
                    return r ? 870 : 869;
                }
                return r ? 871 : 1266;
            }
            else {
                if (!left) {
                    if (!right) {
                        return (!r) + 867;
                    }
                    return r ? 871 : 1266;
                }
                if (!right) {
                    return r ? 872 : 1267;
                }
                return (!r) + 869;
            }
        }
        else {
            if (bottom && left && right) {
                return 869;
            }

            if (!bottom) {
                if (left) {
                    if (!right) {
                        return r ? 872 : 1267;
                    }
                    return (!r) + 869;
                }
                return r ? 871 : 1266;
            }
        }
    }

    return base;
}

int REEditorUI::convertToBaseKey(int objectID) {
    if (objectID < 1267) {
        if (objectID == 1266 || (objectID >= 867 && objectID <= 872)) {
            return 869;
        }

        return objectID;
    }

    if (objectID == 1267) {
        return 869;
    }

    if (objectID == 2808 || (objectID >= 2809 && objectID <= 2822)) {
        return 2703;
    }

    if (objectID >= 2823 && objectID <= 2837) {
        return 2704;
    }

    switch (objectID) {
        case 2842:
        case 2846:
            return 2838;

        case 2843:
        case 2847:
            return 2839;

        case 2844:
        case 2848:
            return 2840;

        case 2845:
        case 2849:
            return 2841;
    }

    return objectID;
}

gd::string REEditorUI::copyObjects(CCArray* objects, bool copyColors, bool sort) {
    //todo
    return EditorUI::copyObjects(objects, copyColors, sort);
}

gd::string REEditorUI::copyObjectsDetailed_(CCArray* objects) {
    qsort(objects->data->arr, objects->data->num, sizeof(GameObject*), [](void const* a, void const* b) {
        auto objA = *static_cast<GameObject* const*>(a);
        auto objB = *static_cast<GameObject* const*>(b);
        return objA->m_uniqueID - objB->m_uniqueID;
    });

    auto groupCenter = getGroupCenter(objects, false);
    std::string result = "";

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if (obj->m_objectID == 749) continue;

        auto position = obj->getPosition();
        obj->setPosition(position - groupCenter);
        result += obj->getSaveString(m_editorLayer) + ";";
        obj->setPosition(position);
    }
    return result;
}

CCArray* REEditorUI::createCustomItems() {
    //todo
    return EditorUI::createCustomItems();
}

GameObject* REEditorUI::createEdgeForObject(GameObject* object, int type) {
    auto edge = edgeForObject_(object->m_objectID, type);

    if (edge < 1) {
        return nullptr;
    }

    auto offset = offsetForKey(edge);

    CCPoint pos;
    switch (type) {
        case 0: 
            pos = CCPoint{0.f, 30.f};
            break;
        case 1:
            pos = CCPoint{30.f, 0.f};
            break;
        case 2:
            pos = CCPoint{0.f, -30.f};
            break;
        case 3:
            pos = CCPoint{-30.f, 0.f};
            break;
        case 4:
            pos = CCPoint{30.f, 30.f};
            break;
        case 5:
            pos = CCPoint{30.f, -30.f};
            break;
        case 6:
            pos = CCPoint{-30.f, -30.f};
            break;
        case 7:
            pos = CCPoint{-30.f, 30.f};
            break;
    }

    auto edgePos = object->getPosition() + offset + pos;

    auto obj = createObject(edge, edgePos);

    if (obj) {
        obj->m_editorLayer2 = object->m_editorLayer + 1;
    }

    return obj;
}

void REEditorUI::createExtraObject(int id, CCPoint position, GameObject* object, CCArray* objects, int zOrder, int editorLayer) {
    auto pos = object->getPosition() + offsetForKey(id) + position;
    auto obj = createObject(id, pos);
    if (obj) {
        obj->m_editorLayer2 = object->m_editorLayer;
        auto oldZOrder = object->getObjectZOrder();
        if (oldZOrder == 0) {
            oldZOrder = object->m_defaultZOrder;
        }
        obj->m_zOrder = oldZOrder + zOrder;
        if (objects) {
            objects->addObject(obj);
        }
    }
}

CCArray* REEditorUI::createExtras_(CCArray* objects) {
    if (!objects || objects->count() == 0) return nullptr;

    auto extras = CCArray::create();
    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if (obj->m_objectID > 2807 && obj->m_objectID < 2838) {
            createExtrasForObject(obj->m_objectID, obj, extras);
        }
    }

    return extras;
}

void REEditorUI::createExtrasForObject(int id, GameObject* object, CCArray* objects) {
    switch(id) {
        case 2808:
        case 2823:
            createExtraObject(2713, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2716, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2712, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2715, {0.f, 30.f}, object, objects, 2, 3);
            break;
        case 2810:
            createExtraObject(2754, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2763, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2753, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2762, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2812:
            createExtrasForObject(2810, object, objects);
        case 2811:
            createExtraObject(2737, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2740, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2736, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2739, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2813:
        case 2828:
            createExtrasForObject(2808, object, objects);
        case 2809:
        case 2824:
            createExtraObject(2936, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2935, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            break;
        case 2814:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2716, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2754, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2760, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2715, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2753, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2759, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2815:
            createExtraObject(2713, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2734, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2740, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2712, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2733, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2739, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2816:
            createExtraObject(2936, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2757, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2763, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2935, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2756, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2762, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2817:
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2737, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2743, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2736, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2742, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2818:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2754, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2760, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2734, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2740, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2753, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2759, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2733, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2739, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2819:
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2757, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2763, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2737, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2743, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2756, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2762, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2736, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2742, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2820:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2716, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2936, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2757, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2760, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2715, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2935, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2756, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2759, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2821:
            createExtraObject(2713, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2734, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2743, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2712, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2733, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2742, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2822:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2778, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2787, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2801, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2804, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2777, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2786, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2800, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2803, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2825:
            createExtraObject(2801, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2804, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2800, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2803, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2827:
            createExtrasForObject(2825, object, objects);
        case 2826:
            createExtraObject(2778, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2787, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2777, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2786, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2829:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2716, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2801, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2807, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2715, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2800, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2806, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2830:
            createExtraObject(2713, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2781, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2787, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2712, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2780, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2786, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2831:
            createExtraObject(2936, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2798, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2804, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2935, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2797, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2803, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2832:
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2778, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2784, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2777, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2783, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2833:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2801, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2807, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2781, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2787, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2800, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2806, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2780, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2786, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2834:
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2801, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2807, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2781, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2787, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2800, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2806, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2780, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2786, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2835:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2716, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2936, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2798, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2807, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2715, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2935, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2797, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2806, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2836:
            createExtraObject(2713, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2942, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2781, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2784, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2712, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2941, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2780, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2783, {30.f, 0.f}, object, objects, 2, 3);
            break;
        case 2837:
            createExtraObject(2710, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2719, {0.f, 30.f}, object, objects, 1, 2);
            createExtraObject(2938, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2940, {0.f, -30.f}, object, objects, 1, 2);
            createExtraObject(2781, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2784, {30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2798, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2807, {-30.f, 0.f}, object, objects, 1, 2);
            createExtraObject(2709, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2718, {0.f, 30.f}, object, objects, 2, 3);
            createExtraObject(2937, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2939, {0.f, -30.f}, object, objects, 2, 3);
            createExtraObject(2780, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2783, {30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2797, {-30.f, 0.f}, object, objects, 2, 3);
            createExtraObject(2806, {-30.f, 0.f}, object, objects, 2, 3);
            break;
        default:
            break;
    }
}

void REEditorUI::createGlow_() {}

void REEditorUI::createLoop() {
    //todo
    EditorUI::createLoop();
}

void REEditorUI::createMoveMenu() {
    //todo
    EditorUI::createMoveMenu();
}

void REEditorUI::createNewKeyframeAnim() {
    //todo
    EditorUI::createNewKeyframeAnim();
}

GameObject* REEditorUI::createObject(int objectID, CCPoint position) {
    auto obj = m_editorLayer->createObject(objectID, position, false);
    if (obj) {
        float scaleX = obj->m_pixelScaleX;
        if (obj->m_pixelScaleX != 1.0 || obj->m_pixelScaleY != 1.0) {
            obj->updateCustomScaleX(obj->m_pixelScaleX);
            obj->updateCustomScaleY(obj->m_pixelScaleY);
        }
    }

    return obj;
}

CCArray* REEditorUI::createOutlines(CCArray* objects) {
    //todo
    return EditorUI::createOutlines(objects);
}

void REEditorUI::createPrefab(GJSmartTemplate* smartTemplate, gd::string key, int id) {
    auto winSize = CCDirector::get()->getWinSize();
    auto centerPos = m_editorLayer->m_objectLayer->convertToNodeSpace(winSize / 2.f + CCPoint{0.f, m_toolbarHeight / 2.f});
    
    centerPos.x += (smartTemplate->m_prefabIndex * 120.f);

    auto snappedPos = getGridSnappedPos_(centerPos);
    smartTemplate->m_prefabIndex++;

    CCPoint point;

    for (int i = 0; i < 9; i++) {
        char ch = key.at(i);
        if (ch == '0') continue;

        CCPoint offset = GJSmartTemplate::offsetForType((SmartBlockType)ch);

        if (i == 0) {
            point = offset;
        }
        else {
            if (i == 1 || i == 5 || i == 6) offset.y += 30.f;
            if (i == 2 || i == 7 || i == 8) offset.y -= 30.f;
            if (i == 3 || i == 5 || i == 7) offset.x -= 30.f;
            if (i == 4 || i == 6 || i == 8) offset.x += 30.f;
        }

        int objectKey = GJSmartTemplate::smartTypeToObjectKey(static_cast<SmartBlockType>(ch));
        auto smartObject = static_cast<SmartGameObject*>(createObject(objectKey, snappedPos + offset));
        
        GJSmartTemplate::applyTransformationsForType(static_cast<SmartBlockType>(ch), smartObject);

        if (i != 0) {
            smartObject->m_referenceOnly = true;
            smartObject->updateSmartFrame();
        }
    }

    if (id > 0) {
        if (auto prefab = smartTemplate->getPrefabWithID(key, id)) {
            auto objects = pasteObjects(prefab->m_prefabData, false, false);
            repositionObjectsToCenter(objects, snappedPos + point, false);
            deleteSmartBlocksFromObjects(objects);
        }
    }
}

CCArray* REEditorUI::createRockBases(CCArray* objects) {
    //todo
    return EditorUI::createRockBases(objects);
}

CCArray* REEditorUI::createRockEdges(CCArray* objects) {
    //todo
    return EditorUI::createRockEdges(objects);
}

void REEditorUI::createSmartObjectsFromTemplate(GJSmartTemplate* smartTemplate, CCArray* objects, bool useNearby, bool dontDelete, bool referenceOnly, bool dontPaste) {
    //todo
    EditorUI::createSmartObjectsFromTemplate(smartTemplate, objects, useNearby, dontDelete, referenceOnly, dontPaste);
}

void REEditorUI::createSmartObjectsFromType(int type, CCArray* objects, bool extras, bool dontDelete) {
    //todo
    EditorUI::createSmartObjectsFromType(type, objects, extras, dontDelete);
}

UndoObject* REEditorUI::createUndoObject(UndoCommand command, bool addToList) {
    //todo
    return EditorUI::createUndoObject(command, addToList);
}

void REEditorUI::createUndoSelectObject(bool redo) {
    //todo
    EditorUI::createUndoSelectObject(redo);
}

void REEditorUI::deactivateRotationControl_() {
    m_rotationTouchID = -1;
    if (m_rotationControl->isVisible()) {
        m_rotationControl->setVisible(false);
        m_rotationControl->finishTouch();
    }
}

void REEditorUI::deactivateScaleControl() {
    if (m_scaleControl->isVisible()) {
        m_scaleControl->setVisible(false);
        m_scaleControl->finishTouch();
        m_scaleControl->m_upperBound = 2.f;
        m_scaleControl->m_lowerBound = 0.5f;
    }

    updateEditButtonColor(m_scaleControl->m_senderTag, {255, 255, 255});
    m_scaleControl->m_senderTag = 0;
}

void REEditorUI::deactivateTransformControl() {
    if (m_transformControl->isVisible()) {
        m_transformControl->setVisible(false);
        m_transformControl->finishTouch();
    }

    updateEditButtonColor(31, {255, 255, 255});
}

void REEditorUI::deleteObject_(GameObject* object, bool noUndo) {
    if (!object) return;

    object->m_wasSelected = object->m_isSelected;
    m_editorLayer->removeObject(object, noUndo);

    deactivateRotationControl_();
    deactivateScaleControl();
    deactivateTransformControl();
}

void REEditorUI::deleteSmartBlocksFromObjects(CCArray* objects) {
    for (int i = 0; i < objects->count(); i++) {
        auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));

        if (obj->m_classType != GameObjectClassType::Smart) continue;

        if (obj->m_linkedGroup > 0) {
            auto sticky = m_editorLayer->getStickyGroup(obj->m_linkedGroup);
            if (sticky) {
                sticky->removeObject(obj);
            }
        }
        deleteObject_(obj, true);
        objects->removeObjectAtIndex(i, true);
    }
}

void REEditorUI::deleteTypeFromObjects_(int id, CCArray* objects) {
    for (int i = 0; i < objects->count(); i++) {
        auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));

        if (obj->m_objectID != id) continue;
        if (obj->m_linkedGroup > 0) {
            auto sticky = m_editorLayer->getStickyGroup(obj->m_linkedGroup);
            if (sticky) {
                sticky->removeObject(obj);
            }
        }
        deleteObject_(obj, true);
        objects->removeObjectAtIndex(i, true);
    }
}

void REEditorUI::deselectAll() {
    deselectObject_();

    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
        obj->m_isSelected = false;
        obj->updateObjectEditorColor();
    }

    m_selectedObjects->removeAllObjects();

    updateButtons();

    deactivateRotationControl_();
    deactivateScaleControl();
    deactivateTransformControl();
    updateObjectInfoLabel();
}

void REEditorUI::deselectObject_() {
    stopActionByTag(124);
    if (m_selectedObject) m_selectedObject->deselectObject();
    m_selectedObject = nullptr;

    toggleEditObjectButton();
    m_canActivateControls = true;
    m_lastTouchPoint = CCPoint{0.f, 0.f};
}

void REEditorUI::deselectObject(GameObject* object) {
    if (m_selectedObject && m_selectedObject->m_uniqueID == object->m_uniqueID) {
        stopActionByTag(124);
        m_selectedObject->deselectObject();
        m_selectedObject = nullptr;

        toggleEditObjectButton();
        m_canActivateControls = true;
        m_lastTouchPoint = CCPoint{0, 0};
        return;
    }

    object->deselectObject();

    if (m_selectedObjects->containsObject(object)) {
        m_canActivateControls = true;
        m_selectedObjects->removeObject(object, true);
    }
}

void REEditorUI::deselectObjectsColor() {
    if (m_selectedObject) {
        m_selectedObject->deselectObject();
        return;
    }

    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
        obj->deselectObject();
    }
}

void REEditorUI::deselectTargetPortals() {
    if (!m_selectedObjects || m_selectedObjects->count() == 0) {
        if (m_selectedObject && m_selectedObject->m_objectID == 749) {
            stopActionByTag(124);

            m_selectedObject->deselectObject();
            m_selectedObject = nullptr;

            toggleEditObjectButton();
            m_canActivateControls = true;

            m_lastTouchPoint = CCPoint{0, 0};
        }
        return;
    }

    auto portals = CCArray::create();

    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
        if (obj->m_objectID == 749) {
            portals->addObject(obj);
        }
    }

    for (auto obj : CCArrayExt<GameObject, false>(portals)) {
        deselectObject(obj);
    }
}

void REEditorUI::disableButton_(CreateMenuItem* button) {
    auto sprite = static_cast<ButtonSprite*>(button->getNormalImage());
    if (sprite->m_subBGSprite) {
        sprite->m_subBGSprite->setColor({127, 127, 127});
    }

    else if (sprite->m_BGSprite) {
        sprite->m_BGSprite->setColor({127, 127, 127});
    }
    if (button->m_objectID < 0) return;

    auto object = static_cast<EffectGameObject*>(sprite->m_subSprite);
    if (object->m_classType == GameObjectClassType::Effect && object->isColorObject() && object->m_shouldPreview) {
        object->setObjectColor({100, 100, 127});
    }
    else {
        object->setObjectColor({127, 127, 127});
    }
    object->setChildColor({100, 100, 127});

    // FIX BINDINGS, color values are wrong order
}

void REEditorUI::doCopyObjects(bool withColor) {
    //todo
    EditorUI::doCopyObjects(withColor);
}

void REEditorUI::doPasteInPlace_(bool withColor) {
    auto& clipboard = GameManager::get()->m_editorClipboard;
    if (clipboard.empty()) return;

    pasteObjects(clipboard, withColor, false);
    updateButtons();
    updateObjectInfoLabel();
}

void REEditorUI::doPasteObjects(bool withColor) {
    auto gameManager = GameManager::sharedState();
    if (gameManager->m_editorClipboard.empty()) return;

    auto level = m_editorLayer->m_level;
    int objectCount = level->m_objectCount.value() + gameManager->m_copiedObjectCount;
    if (objectCount > 80000 && !level->m_unlimitedObjectsEnabled) {
        showMaxError();
        return;
    }
    
    if (objectCount > 40000 && !level->m_highObjectsEnabled) {
        showMaxBasicError();
        return;
    }

    if (auto objects = pasteObjects(gameManager->m_editorClipboard, withColor, false)) {
        auto winSize = CCDirector::get()->getWinSize();
        auto centerPos = m_editorLayer->m_objectLayer->convertToNodeSpace(winSize / 2.f + CCPoint{0.f, m_toolbarHeight / 2.f});
        repositionObjectsToCenter(objects, getGridSnappedPos_(centerPos), false);
    }

    updateButtons();
    updateObjectInfoLabel();
}

void REEditorUI::dynamicGroupUpdate(bool reGroup) {
    //todo
    EditorUI::dynamicGroupUpdate(reGroup);
}

int REEditorUI::edgeForObject_(int id, int type) {
    if (id == 1348) {
        switch (type) {
            case 0: return 1359;
            case 3: return 1395;
            case 7: return 1363;
        }
    }
    else if (id == 1349) {
        switch (type) {
            case 0: return 1360;
            case 1: return 1357;
            case 4: return 1364;
        }
    }
    else if (id == 1350) {
        switch (type) {
            case 2: return 1361;
            case 3: return 1356;
            case 6: return 1365;
        }
    }
    else if (id == 1351) {
        switch (type) {
            case 1: return 1358;
            case 2: return 1362;
            case 5: return 1366;
        }
    }
    else if (id == 1431 || id == 1432 || id == 1433) {
        switch (type) {
            case 0: return 1435;
            case 1: return 1436;
            case 2: return 1437;
            case 3: return 1434;
            case 4: return 1439;
            case 5: return 1441;
            case 6: return 1440;
            case 7: return 1438;
        }
    }
    else if (id == 1461) {
        switch (type) {
            case 0: return 1629;
            case 1: return 1625;
            case 2: return 1633;
            case 3: return 1621;
            case 4: return 1638;
            case 5: return 1640;
            case 6: return 1639;
            case 7: return 1637;
        }
    }
    else if (id == 1462) {
        switch (type) {
            case 0: return 1630;
            case 1: return 1627;
            case 2: return 1634;
            case 3: return 1623;
            case 4: return 1642;
            case 5: return 1644;
            case 6: return 1643;
            case 7: return 1641;
        }
    }
    else if (id == 1463) {
        switch (type) {
            case 0: return 1631;
            case 1: return 1626;
            case 2: return 1635;
            case 3: return 1622;
            case 4: return 1646;
            case 5: return 1648;
            case 6: return 1647;
            case 7: return 1645;
        }
    }
    else if (id == 1464) {
        switch (type) {
            case 0: return 1632;
            case 1: return 1628;
            case 2: return 1636;
            case 3: return 1624;
            case 4: return 1650;
            case 5: return 1652;
            case 6: return 1651;
            case 7: return 1649;
        }
    }
    return 0;
}

bool REEditorUI::editButton2Usable() {
    //todo
    return EditorUI::editButton2Usable();
}

bool REEditorUI::editButtonUsable() {
    //todo
    return EditorUI::editButtonUsable();
}

void REEditorUI::editColor() {
    ColorAction* mainColor;
    ColorAction* detailColor;
    EffectGameObject* object;

    getEditColorTargets(mainColor, detailColor, object);

    if (mainColor || detailColor || object) {
        ColorSelectLiveOverlay* overlay;

        closeLiveColorSelect();
        closeLiveHSVSelect();

        if (!object) {
            overlay = ColorSelectLiveOverlay::createWithActions(mainColor, detailColor);
        }
        else {
            overlay = ColorSelectLiveOverlay::createWithObject(object);
        }
        m_colorOverlay = overlay;
        m_colorOverlay->m_delegate = this;
        m_colorOverlay->m_scene = this;
        m_colorOverlay->show();

        if (!object) {
            if (m_selectedObject) {
                m_selectedObject->deselectObject();
            }
            else {
                for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                    obj->deselectObject();
                }
            }
        }
    }
}

bool REEditorUI::editColorButtonUsable_() {
    return m_selectedObject || m_selectedObjects->count() != 0;
}

void REEditorUI::editGroup(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) return;

    auto layer = SetGroupIDLayer::create(m_selectedObject, m_selectedObjects);

    layer->m_nextFreeID = m_editorLayer->getNextFreeGroupID({});
    layer->show();
}

void REEditorUI::editHSV() {
    closeLiveHSVSelect();
    closeLiveColorSelect();
    
    auto objects = m_selectedObjects;

    if (m_selectedObject) {
        m_selectedObject->deselectObject();
        objects = nullptr;
    }
    else {
        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            obj->deselectObject();
        }
    }

    auto overlay = HSVLiveOverlay::create(m_selectedObject, objects);
    m_hsvOverlay = overlay;
    m_hsvOverlay->m_delegate = this;
    m_hsvOverlay->m_scene = this;
    m_hsvOverlay->show();
}

class REGameObject : public GameObject {
public:
    static bool isBasicEnterEffect(int id) {
        return (id > 21 && id < 29) || (id > 54 && id < 60) || id == 1915;
    }
};

void REEditorUI::editObject(CCObject* sender) {
    if (!editColorButtonUsable()) return;

    bool isMultiBasicTrigger = false;
    bool isMultiAreaTrigger = false;
    bool isMultiSpawnable = false;
    bool isEnterEffect = false;
    int objectID = -1;

    closeLiveColorSelect();

    if (m_selectedObject) {
        objectID = m_selectedObject->m_objectID;
        isEnterEffect = REGameObject::isBasicEnterEffect(objectID);
        // FIX BINDINGS, should be (id > 54 && id < 60)
    }
    
    if (m_selectedObjects && m_selectedObjects->count() != 0) {
        isEnterEffect = false;
        isMultiAreaTrigger = true;
        isMultiBasicTrigger = true;
        isMultiSpawnable = true;

        int currentObjectID;

        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            if ((isMultiBasicTrigger) && !obj->isBasicTrigger()) {
                isMultiBasicTrigger = false;
            }
            if (objectID != 0) {
                if (objectID == -1) {
                    objectID = obj->m_objectID;
                }
                else {
                    currentObjectID = obj->m_objectID;
                    if (objectID != currentObjectID) {
                        objectID = 0;
                    }
                }
            }
            if (isMultiSpawnable && !obj->isSpawnableTrigger()) {
                isMultiSpawnable = false;
            }
            if (isMultiAreaTrigger && !(obj->m_objectID == 3006 || obj->m_objectID == 3007 || obj->m_objectID == 3008 || obj->m_objectID == 3009 || obj->m_objectID == 3010)) {
                isMultiAreaTrigger = false;
            }
            if (isEnterEffect) {
                isEnterEffect = GameObject::isBasicEnterEffect(obj->m_objectID);
            }
        }
    }

    if (objectID == 142) {
        SetupCoinLayer::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 31) {
        auto startSettings = static_cast<StartPosObject*>(m_selectedObject)->m_startSettings;
        startSettings->m_platformerMode = m_editorLayer->m_levelSettings->m_platformerMode;
        
        LevelSettingsLayer::create(startSettings, nullptr)->show();
        return;
    }
    if (objectID == 2900) {
        SetupRotateGameplayPopup::create(static_cast<RotateGameplayGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2901) {
        SetupGameplayOffsetPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2899) {
        SetupOptionsTriggerPopup::create(static_cast<GameOptionsTrigger*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3642) {
        SetupAudioLineGuidePopup::create(static_cast<AudioLineGuideGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3022) {
        SetupTeleportPopup::create(static_cast<TeleportPortalObject*>(m_selectedObject), m_selectedObjects, objectID, m_editorLayer->m_levelSettings->m_platformerMode)->show();
        return;
    }
    if (objectID == 3006) {
        SetupAreaMoveTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3008) {
        SetupAreaTransformTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3007) {
        SetupAreaRotateTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3009) {
        SetupAreaFadeTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3010) {
        SetupAreaTintTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID >= 3011 && objectID <= 3015) {
        SetupAreaAnimTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects, objectID)->show();
        return;
    }
    if (objectID == 3024) {
        SetTargetIDLayer::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, "Stop Area Trigger", "EffectID:", 0, 9999, objectID)->show();
        return;
    }
    if (isMultiAreaTrigger) {
        SetupAreaTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if ((objectID >= 3017 && objectID <= 3021) || (objectID == 3023)) {
        SetupEnterEffectPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects, objectID)->show();
        return;
    }
    if (isEnterEffect) {
        SetupEnterTriggerPopup::create(static_cast<EnterEffectObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if ((objectID >= 2904 && objectID <= 2905) || objectID == 2907 || (objectID >= 2909 && objectID <= 2917) || (objectID >= 2919 && objectID <= 2924)) {
        SetupShaderEffectPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, objectID)->show();
        return;
    }
    if (objectID - 3029U < 3) {
        SetupArtSwitchPopup::create(static_cast<ArtTriggerGameObject*>(m_selectedObject), m_selectedObjects, objectID)->show();
        return;
    }
    if (objectID == 2925) {
        SetupCameraModePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1007) {
        SetupOpacityPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1268) {
        SetupSpawnPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1006) {
        SetupPulsePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 899) {
        ColorSelectPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1049) {
        SetupObjectTogglePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, m_editorLayer->m_levelSettings->m_platformerMode)->show();
        return;
    }
    if (objectID == 1520) {
        SetupShakePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1585) {
        SetupAnimationPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3033) {
        SetupKeyframeAnimPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3032) {
        SetupKeyframePopup::create(static_cast<KeyframeGameObject*>(m_selectedObject), m_selectedObjects, m_editorLayer)->show();
        return;
    }
    if (objectID == 3600) {
        SetupPlatformerEndPopup::create(static_cast<EndTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 901) {
        SetupMoveCommandPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1346) {
        SetupRotateCommandPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1347) {
        GJFollowCommandLayer::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1814) {
        GJPFollowCommandLayer::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3016) {
        SetupAdvFollowPopup::create(static_cast<AdvancedFollowTriggerObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3660) {
        SetupAdvFollowEditPhysicsPopup::create(static_cast<AdvancedFollowEditObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3661) {
        SetupAdvFollowRetargetPopup::create(static_cast<AdvancedFollowEditObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2067) {
        SetupTransformPopup::create(static_cast<TransformTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1934) {
        SetupSongTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3602) {
        SetupSFXPopup::create(static_cast<SFXTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3603) {
        SetupSFXEditPopup::create(static_cast<SFXTriggerGameObject*>(m_selectedObject), m_selectedObjects, false)->show();
        return;
    }
    if (objectID == 3605) {
        SetupSFXEditPopup::create(static_cast<SFXTriggerGameObject*>(m_selectedObject), m_selectedObjects, true)->show();
        return;
    }
    if (objectID == 3604) {
        SetupEventLinkPopup::create(static_cast<EventLinkTrigger*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1935) {
        SetupTimeWarpPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2015) {
        SetupCameraRotatePopup2::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2016) {
        SetupCameraGuidePopup::create(static_cast<CameraTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3645 || objectID == 2069) {
        SetupForceBlockPopup::create(static_cast<ForceBlockGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2066) {
        SetupGravityTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2068) {
        SetupRandAdvTriggerPopup::create(static_cast<RandTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3607) {
        SetupSequenceTriggerPopup::create(static_cast<SequenceTriggerGameObject*>(m_selectedObject))->show();
        return;
    }
    if (objectID == 1816) {
        CollisionBlockPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2999) {
        SetupMGTrigger::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3606 || objectID == 3612) {
        SetupBGSpeedTrigger::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, objectID)->show();
        return;
    }
    if (objectID == 3613) {
        UIObjectSettingsPopup::create(static_cast<UISettingsGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3662) {
        SetTargetIDLayer::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, "Visibility Link", "GroupID:", 0, 9999, objectID)->show();
    }
    if (objectID == 1932) {
        SetupPlayerControlPopup::create(static_cast<PlayerControlGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3655) {
        SetupObjectControlPopup::create(static_cast<ObjectControlGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if ((objectID >= 200 && objectID < 204) || objectID == 1334) {
        MultiTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1931) {
        SetupEndPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1812) {
        SetupObjectTogglePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, m_editorLayer->m_levelSettings->m_platformerMode)->show();
        return;
    }
    if (objectID == 1595) {
        SetupTouchTogglePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1611) {
        SetupCountTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1811) {
        SetupInstantCountPopup::create(static_cast<CountTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1817) {
        SetupPickupTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3614) {
        SetupTimerTriggerPopup::create(static_cast<TimerTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3615) {
        SetupTimerEventTriggerPopup::create(static_cast<TimerTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3617) {
        SetupTimerControlTriggerPopup::create(static_cast<TimerTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3618) {
        SetupResetTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3619) {
        SetupItemEditTriggerPopup::create(static_cast<ItemTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3620) {
        SetupItemCompareTriggerPopup::create(static_cast<ItemTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3641) {
        SetupPersistentItemTriggerPopup::create(static_cast<ItemTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1912) {
        SetupRandTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1913) {
        SetupZoomTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1914) {
        SetupStaticCameraPopup::create(static_cast<CameraTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 2062) {
        SetupCameraEdgePopup::create(static_cast<CameraTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1916) {
        SetupCameraOffsetTrigger::create(static_cast<CameraTriggerGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (m_selectedObject && m_selectedObject->isConfigurablePortal()) {
        SetupCameraModePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1616) {
        SetupStopTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3608) {
        SetupSpawnParticlePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 1815) {
        SetupCollisionTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3609) {
        SetupInstantCollisionTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3640) {
        SetupCollisionStateTriggerPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
        return;
    }
    if (objectID == 3643) {
        SetupObjectTogglePopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects, m_editorLayer->m_levelSettings->m_platformerMode)->show();
        return;
    }

    if (!isMultiBasicTrigger) {
        if (!m_selectedObject) {
            if ((!isMultiSpawnable && objectID != 1917) || objectID == 2903) {
                int count = m_selectedObjects->count();
                if (count != 0) {
                    CustomizeObjectLayer::create(m_selectedObject, m_selectedObjects)->show();
                }
                return;
            }
        }
        else {
            if ((!m_selectedObject->isBasicTrigger() && !isMultiSpawnable && objectID != 1917) || objectID == 2903) {
                bool canChangeCustomColor = m_selectedObject->canChangeCustomColor();
                if (!canChangeCustomColor) {
                    ColorSelectPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
                    return;
                }

                int count = m_selectedObjects->count();
                if (count != 0 || canChangeCustomColor) {
                    CustomizeObjectLayer::create(m_selectedObject, m_selectedObjects)->show();
                }
                return;
            }
        }
    }
    EditTriggersPopup::create(static_cast<EffectGameObject*>(m_selectedObject), m_selectedObjects)->show();
}

void REEditorUI::editObject2(CCObject* sender) {
    editObjectSpecial(0);
}

void REEditorUI::editObject3_(CCObject* sender) {
    editObjectSpecial(1);
}

void REEditorUI::editObjectSpecial(int type) {
    ColorAction* baseAction = nullptr;
    ColorAction* detailAction = nullptr;
    EffectGameObject* object = nullptr;

    getEditColorTargets(baseAction, detailAction, object);

    if (!baseAction && !detailAction && !object) return;

    closeLiveColorSelect();
    closeLiveHSVSelect();

    m_colorOverlay = nullptr;

    auto useBase = object ? nullptr : baseAction;
    auto useDetail = object ? nullptr : detailAction;
    auto useObject = object;

    auto overlay = ColorSelectLiveOverlay::create(useBase, useDetail, useObject);
    m_colorOverlay = overlay;

    overlay->m_delegate = this;
    overlay->m_scene = this;
    overlay->show();

    if (!object) {
        if (m_selectedObject) {
            m_selectedObject->m_isSelected = false;
            m_selectedObject->updateObjectEditorColor();
        } else {
            for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                obj->m_isSelected = false;
                obj->updateObjectEditorColor();
            }
        }
    }
}

int REEditorUI::editorLayerForArray_(CCArray* objects, bool layer2) {
    if (!objects) return 0;

    auto previousLayer = -1;
    auto currentLayer = -1;

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        currentLayer = layer2 ? obj->m_editorLayer2 : obj->m_editorLayer;
        if (previousLayer != -1 && previousLayer != currentLayer) return 0;
        previousLayer = currentLayer;
    }

    return currentLayer != -1 ? currentLayer : 0;
}

void REEditorUI::enableButton_(CreateMenuItem* button) {
    auto sprite = static_cast<ButtonSprite*>(button->getNormalImage());
    if (sprite->m_subBGSprite) {
        sprite->m_subBGSprite->setColor({255, 255, 255});
    }
    else if (sprite->m_BGSprite) {
        sprite->m_BGSprite->setColor({255, 255, 255});
    }

    if (button->m_objectID < 0) return;

    auto object = static_cast<EffectGameObject*>(sprite->m_subSprite);
    if (object->m_classType == GameObjectClassType::Effect && object->isColorObject() && object->m_shouldPreview) {
        object->setObjectColor({200, 200, 255});
    }
    else {
        auto objectID = object->m_objectID;
        if (objectID == 918 || objectID == 919 || (objectID != 1584 && objectID != 2012) && object->getMainColorMode() != 1010) {
            object->setObjectColor({255, 255, 255});
        }
        else {
            object->setObjectColor({0, 0, 0});
        }
    }
    object->setChildColor({200, 200, 255});
}

void REEditorUI::findAndSelectObject(int id, bool useUniqueID) {
    //todo
    EditorUI::findAndSelectObject(id, useUniqueID);
}

void REEditorUI::findSnapObject(CCArray* objects, float offset) {
    if (!objects || objects->count() == 0) {
        if (m_selectedObject && static_cast<int>(m_selectedObject->getRotation()) % 90 != 0) {
            auto position = positionWithoutOffset(m_selectedObject);
            auto gridSnapped = getGridSnappedPos_(position);
            auto offsetPos = applySpecialOffset(position, m_selectedObject, {0.f, 0.f});

            if (offset < 0.0f || (std::abs(gridSnapped.x - offsetPos.x) <= offset && std::abs(gridSnapped.y - offsetPos.y) <= offset)) {
                m_snapObject = m_selectedObject;
            }
        }
    }
    else {
        float delta = 999.f;
        for (auto obj : CCArrayExt<GameObject, false>(objects)) {
            obj->updateStartPos();
            auto position = positionWithoutOffset(obj);
            auto gridSnapped = getGridSnappedPos_(position);
            auto offsetPos = applySpecialOffset(position, obj, {0.f, 0.f});
            
            if (offset < 0.f || (std::abs(gridSnapped.x - offsetPos.x) <= offset && std::abs(gridSnapped.y - offsetPos.y) <= offset)) {
                float delta2 = std::abs(gridSnapped.y - offsetPos.y) + std::abs(gridSnapped.x - offsetPos.x);
                if (delta2 < delta) {
                    delta = delta2;
                    if (shouldSnap(obj)) {
                        m_snapObject = obj;
                    }
                }
            }
            if (delta == 0.f) break;
        }
    }
}

void REEditorUI::findSnapObject(CCPoint position, float offset) {
    //todo
    EditorUI::findSnapObject(position, offset);
}

void REEditorUI::findTriggerTest_() {
    // robtop what is the point of this?
    for (auto obj : CCArrayExt<GameObject, false>(m_editorLayer->getAllObjects())) {}
}

void REEditorUI::flipObjectsX(CCArray* objects) {
    //todo
    EditorUI::flipObjectsX(objects);
}

void REEditorUI::flipObjectsY(CCArray* objects) {
    //todo
    EditorUI::flipObjectsY(objects);
}

CCMenuItemSpriteExtra* REEditorUI::getButton_(char const* text, int width, SEL_MenuHandler selector, CCMenu* menu) {
    auto sprite = ButtonSprite::create(text, width, 0, 1.f, true, "goldFont.fnt", "GJ_button_01.png", 0.f);
    auto button = CCMenuItemSpriteExtra::create(sprite, this, selector);
    if (menu) {
        menu->addChild(button);
    }
    return button;
}

CreateMenuItem* REEditorUI::getCreateBtn(int id, int bg) {
    //todo
    return EditorUI::getCreateBtn(id, bg);
}

CreateMenuItem* REEditorUI::getCreateMenuItemButton_(CCSprite* sprite, SEL_MenuHandler selector, CCMenu* menu, float scale, int background, CCPoint offset) {
    const char* bgSprite = "GJ_button_01.png";
    switch (background) {
        case 2: bgSprite = "GJ_button_02.png"; break;
        case 3: bgSprite = "GJ_button_03.png"; break;
        case 4: bgSprite = "GJ_button_04.png"; break;
        case 5: bgSprite = "GJ_button_05.png"; break;
        case 6: bgSprite = "GJ_button_06.png"; break;
        default: break;
    }

    auto buttonSprite = ButtonSprite::create(sprite, 32, 0, 32.f, 1.f, true, bgSprite, true);
    auto button = CreateMenuItem::create(buttonSprite, nullptr, this, selector);
    button->setScale(scale);
    button->m_baseScale = scale;

    if (menu) {
        menu->addChild(button);
    }

    buttonSprite->updateSpriteOffset(offset);
    return button;
}

GameObject* REEditorUI::getCycledObject_(CCArray* objects, bool dontSkipSelected) {
    GameObject* cycledObject = nullptr;
    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if ((!dontSkipSelected && obj->m_isSelected) || (cycledObject && cycledObject->m_unk460 <= obj->m_unk460 )) continue;
        if (canSelectObject(obj)) {
            cycledObject = obj;
        }
    }
    if (cycledObject) {
        cycledObject->m_unk460 = m_cycledObjectIndex;
    }

    return cycledObject;
}

void REEditorUI::getEditColorTargets(ColorAction*& mainColor, ColorAction*& detailColor, EffectGameObject*& object) {
    //todo
    EditorUI::getEditColorTargets(mainColor, detailColor, object);
}

CCPoint REEditorUI::getGridSnappedPos_(CCPoint pos) {
    auto size = m_editorLayer->m_drawGridLayer->m_gridSize;
    auto xVal = std::floorf(pos.x / size);
    auto yVal = std::floorf(pos.y / size);
    return getLimitedPosition(CCPoint{(xVal + 0.5f) * size, (yVal + 0.5f) * size});
}

CCPoint REEditorUI::getGroupCenter(CCArray* objects, bool absolute) {
    if (objects->count() == 1) {
        auto obj = static_cast<GameObject*>(objects->objectAtIndex(0));
        obj->updateStartPos();
        return obj->getPosition();
    }

    float minX = 999999.f;
    float maxX = -999999.f;
    float minY = 999999.f;
    float maxY = -999999.f;

    GameObject* parentObject = nullptr;
    bool multipleParents = false;

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        obj->updateStartPos();

        auto rect = obj->getObjectRect(1.0f, 1.0f);

        maxX = std::max(maxX, rect.getMaxX());
        minX = std::min(minX, rect.getMinX());
        maxY = std::max(maxY, rect.getMaxY());
        minY = std::min(minY, rect.getMinY());

        if (obj->m_hasGroupParent) {
            if (parentObject != nullptr) {
                multipleParents = true;
            }
            parentObject = obj;
        }
    }

    if (parentObject && !multipleParents) {
        return parentObject->getRealPosition();
    }

    return {minX + (maxX - minX) * 0.5f, minY + (maxY - minY) * 0.5f};
}

void REEditorUI::getGroupInfo_(GameObject* selectedObject, CCArray* selectedObjects, int& objectID, int& classType, int& objectType) {
    objectID = classType = objectType = -1;
    if (selectedObject) {
        objectID = selectedObject->m_objectID;
        classType = static_cast<int>(selectedObject->m_classType);
        objectType = static_cast<int>(selectedObject->m_savedObjectType);
    } 
    else if (selectedObjects) {
        for (auto obj : CCArrayExt<GameObject, false>(selectedObjects)) {
            if (objectID != 0) {
                if (objectID == -1) objectID = obj->m_objectID;
                else if (objectID != obj->m_objectID) objectID = 0;
            }
            if (classType != 0) {
                if (classType == -1) classType = static_cast<int>(obj->m_classType);
                else if (classType != static_cast<int>(obj->m_classType)) classType = 0;
            }
            if (objectType != 0) {
                if (objectType == -1) objectType = static_cast<int>(obj->m_savedObjectType);
                else if (objectType != static_cast<int>(obj->m_savedObjectType)) objectType = 0;
            }
        }
    }
}

CCPoint REEditorUI::getLimitedPosition(CCPoint position) {
    auto result = position;

    if (result.y < -3000.f) result.y = -3000.f;
    if (result.x < -3000.f) result.x = -3000.f;
    if (result.x > 240000.f) result.x = 240000.f;

    int height = 80;
    if (m_editorLayer->m_levelSettings->m_dynamicLevelHeight) height = 1000;

    float maxY = height * 30.f + 90.f;
    if (result.y > maxY) result.y = maxY;

    return result;
}

CCMenuItemSpriteExtra* REEditorUI::getModeBtn_(char const* frameName, int tag) {
    auto button = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(frameName), this, menu_selector(EditorUI::toggleMode));
    button->setTag(tag);
    return button;
}

CCNode* REEditorUI::getNeighbor(int smartKey, CCPoint position, GJSmartDirection direction, CCArray* objects) {
    auto offset = GJSmartTemplate::offsetForDir(direction, smartKey);
    auto target = position + offset;

    int targetX = static_cast<int>(target.x);
    int targetY = static_cast<int>(target.y);

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        auto pos = obj->getPosition();

        if (static_cast<int>(pos.x) == targetX && static_cast<int>(pos.y) == targetY) {
            return obj;
        }
    }

    return nullptr;
}

CCPoint REEditorUI::getRelativeOffset_(GameObject* object) {
    return GameToolbox::getRelativeOffset(object, offsetForKey(object->m_objectID));
}

CCArray* REEditorUI::getSelectedObjects() {
    if (m_selectedObject && m_selectedObject->m_objectID != 749) {
        return CCArray::createWithObject(m_selectedObject);
    }

    auto result = CCArray::create();

    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
        if (obj->m_objectID != 749) {
            result->addObject(obj);
        }
    }

    return result;
}

CCMenuItemSpriteExtra* REEditorUI::getSimpleButton_(gd::string frameName, SEL_MenuHandler selector, CCMenu* menu) {
    auto sprite = CCSprite::createWithSpriteFrameName(frameName.c_str());
    sprite->setScale(std::min(18.f / sprite->getContentSize().width, 18.f / sprite->getContentSize().height));

    auto button = CCMenuItemSpriteExtra::create(sprite, this, selector);

    if (menu) {
        menu->addChild(button);
    }
    return button;
}

SmartGameObject* REEditorUI::getSmartNeighbor(SmartGameObject* object, CCPoint position, GJSmartDirection direction, CCArray* objects) {
    int uniqueID = 0;
    if (object) uniqueID = object->m_uniqueID;

    auto offset = GJSmartTemplate::offsetForDir(direction, 0);
    auto target = position + offset;
    auto queryRect = CCRect{target.x - 1.f, target.y - 1.f, 2.f, 2.f};

    for (auto obj : CCArrayExt<SmartGameObject, false>(objects)) {
        if (obj->m_uniqueID == uniqueID) {
            continue;
        }

        auto objRect = obj->boundingBox();

        if (queryRect.intersectsRect(objRect)) {
            if (obj->m_classType != GameObjectClassType::Smart) {
                return nullptr;
            }

            return obj;
        }
    }

    return nullptr;
}

float REEditorUI::getSnapAngle_(GameObject* object, CCArray* objects) {
    if (objects && objects->count() > 0) {
        for (auto obj : CCArrayExt<GameObject, false>(objects)) {
            auto rotation = m_editorLayer->rotationForSlopeNearObject(obj);
            if (rotation != -1.f) {
                return rotation;
            }
        }
    }
    else if (object) {
        return m_editorLayer->rotationForSlopeNearObject(object);
    }
    
    return -1.f;
}

CCMenuItemSpriteExtra* REEditorUI::getSpriteButton_(char const* spriteFrameName, SEL_MenuHandler selector, CCMenu* menu, float scale) {
    return getSpriteButton_(spriteFrameName, selector, menu, scale, 1, {0, 0});
}

CCMenuItemSpriteExtra* REEditorUI::getSpriteButton_(char const* spriteFrameName, SEL_MenuHandler selector, CCMenu* menu, float scale, int background, CCPoint offset) {
    return getSpriteButton_(CCSprite::createWithSpriteFrameName(spriteFrameName), selector, menu, scale, background, offset);
}

CCMenuItemSpriteExtra* REEditorUI::getSpriteButton_(CCSprite* sprite, SEL_MenuHandler selector, CCMenu* menu, float scale, int background, CCPoint offset) {
    const char* bgSprite = "GJ_button_01.png";
    switch (background) {
        case 2: bgSprite = "GJ_button_02.png"; break;
        case 3: bgSprite = "GJ_button_03.png"; break;
        case 4: bgSprite = "GJ_button_04.png"; break;
        case 5: bgSprite = "GJ_button_05.png"; break;
        case 6: bgSprite = "GJ_button_06.png"; break;
        default: break;
    }

    auto buttonSprite = ButtonSprite::create(sprite, 32.f, 0, 32.f, 1.f, false, bgSprite, true);
    auto button = CCMenuItemSpriteExtra::create(buttonSprite, nullptr, this, selector);
    button->setScale(scale);
    button->m_baseScale = scale;

    if (menu) {
        menu->addChild(button);
    }

    buttonSprite->updateSpriteOffset(offset);
    return button;
}

CCPoint REEditorUI::getTouchPoint_(CCTouch* touch, CCEvent* event) {
    return convertToWorldSpace(CCDirector::get()->convertToGL(touch->getLocationInView()));
}

GJTransformState& REEditorUI::getTransformState_() {
    m_transformControl->saveToState(m_transformState);
    return m_transformState;
}

float REEditorUI::getXMin(int offset) {
    auto objectLayer = m_editorLayer->m_objectLayer;
    auto objectLayerPos = objectLayer->getPosition();
    objectLayer->setPosition({0, 0});

    float screenLeft = CCDirector::get()->getScreenLeft();
    auto screenRef = CCPoint{screenLeft, m_constrainedHeight};
    auto localOffset = CCPoint{offset + -30.f, 60.f};

    auto worldPos = objectLayer->convertToWorldSpace(localOffset);
    auto nodePos = m_editorLayer->convertToNodeSpace(worldPos);

    objectLayer->setPosition(objectLayerPos);

    return screenRef.x - nodePos.x;
}

bool REEditorUI::init(LevelEditorLayer* editorLayer) {
    //todo
    return EditorUI::init(editorLayer);
}

bool REEditorUI::isLiveColorSelectTrigger_(GameObject* object) {
    return object && object->m_classType == GameObjectClassType::Effect && (object->m_isColorTrigger || object->m_objectID == 1006);
}

bool REEditorUI::isSpecialSnapObject(int id) {
    //todo
    return EditorUI::isSpecialSnapObject(id);
}

bool REEditorUI::liveEditColorUsable_() {
    return m_selectedObject || m_selectedObjects->count() != 0;
}

CreateMenuItem* REEditorUI::menuItemFromObjectString(gd::string str, int id) {
    auto sprite = spriteFromObjectString(str, 1, 0, 100, 0, 0, 0);

    auto size = sprite->getContentSize();
    sprite->setScale(40 / std::min(size.width, size.height));

    auto button = ButtonSprite::create(sprite, 32, 0, 32.f, 1.f, true, "GJ_button_04.png", false);

    auto pos = sprite->getPosition() + CCPoint{0.f, -1.f};

    sprite->setPosition(pos);

    auto menuItem = CreateMenuItem::create(button, nullptr, this, menu_selector(EditorUI::onCreateButton));
    menuItem->m_objectID = id;
    menuItem->setScale(0.9f);
    menuItem->m_baseScale = 0.9f;

    m_customObjectButtonArray->addObject(menuItem);

    return menuItem;
}

CCPoint REEditorUI::moveForCommand(EditCommand command) {
    //todo
    return EditorUI::moveForCommand(command);
}

void REEditorUI::moveGamelayer(CCPoint offset) {
    m_editorLayer->m_objectLayer->setPosition(m_editorLayer->m_objectLayer->getPosition() + offset);
    constrainGameLayerPosition_();
}

class RELevelEditorLayer : public LevelEditorLayer {
public:
    void objectMoved(GameObject* object) {
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
};

void REEditorUI::moveObject(GameObject* object, CCPoint offset) {
    if (!object) return;
    auto limitedPos = getLimitedPosition(object->getPosition() + offset);
    object->setPosition(limitedPos);
    object->updateStartValues();

    m_editorLayer->reorderObjectSection(object);

    if (object->m_objectID == 747) {
        TeleportPortalObject* teleportObject = static_cast<TeleportPortalObject*>(object);
        if (teleportObject->m_orangePortal) {
            m_editorLayer->reorderObjectSection(teleportObject->m_orangePortal);
        }
    }

    static_cast<RELevelEditorLayer*>(m_editorLayer)->objectMoved(object);
    
    if (object->isSpeedObject() || object->canReverse()) {
        m_editorLayer->m_drawGridLayer->m_updateSpeedObjects = true;
    }
}

void REEditorUI::moveObjectCall(EditCommand command) {
    //todo
    EditorUI::moveObjectCall(command);
}

void REEditorUI::moveObjectCall(CCObject* sender) {
    moveObjectCall(static_cast<EditCommand>(sender->getTag()));
}

CCPoint REEditorUI::offsetForKey(int id) {  
    switch(id) {
        case 9:
            return {0.f,-12.5f};
        case 15:
            return {0.f, 6.f};
        case 16:
        case 125:
            return {0.f, -1.f};
        case 17:
        case 21:
        case 178:
            return {0.f, -8.f};
        case 18:
        case 19:
        case 151:
            return {0.f, 4.f};
        case 20:
        case 49:
        case 114:
            return {0.f, -2.f};
        case 35:
        case 140:
            return {0.f, -13.f};
        case 39:
        case 198:
        case 205:
        case 217:
            return {0.f, -9.f};
        case 40:
        case 128:
        case 147:
        case 215:
            return {0.f, 8.f};
        case 41:
            return {0.f, 19.5f};
        case 48:
            return {0.f, 2.f};
        case 61:
        case 135:
        case 243:
        case 244:
            return {0.f, -11.f};
        case 62:
        case 65:
        case 66:
        case 68:
            return {0.f, 7.f};
        case 64:
            return {-8.f, 8.f};
        case 67:
            return {0.f, -12.f};
        case 103:
        case 145:
        case 153:
        case 179:
        case 199:
        case 218:
            return {0.f, -6.f};
        case 106:
            return {0.f, 17.5f};
        case 107:
            return {0.f, 3.f};
        case 110:
        case 113:
        case 134:
            return {0.f, 1.f};
        case 115:
            return {0.f, -5.f};
        case 124:
            return {0.f, 0.5f};
        case 126:
            return {0.f, -5.5f};
        case 127:
            return {0.f, -7.f};
        case 152:
            return {0.f, 0.f};
        case 157:
        case 158:
        case 159:
            return {0.f, -1.5f};
        case 170:
        case 171:
        case 172:
        case 174:
        case 176:
        case 192:
        case 194:
        case 197:
            return {0.f, 4.5f};
        case 190:
            return {0.f, 9.f};
        case 227:
            return {0.f, -4.f};
        case 228:
            return {-7.5f, -7.5f};
        case 234:
        case 245:
            return {7.5f, 0.f};
        case 277:
            return {7.5f, 0.f};
        case 291:
        case 295:
        case 301:
        case 307:
        case 311:
        case 317:
        case 323:
        case 327:
        case 329:
        case 333:
        case 339:
        case 345:
        case 351:
        case 355:
        case 364:
        case 367:
        case 372:
        case 484:
        case 493:
            return {15.f, 0.f};
        case 365:
            return {0.f, -14.f};
        case 368:
        case 407:
            return {0.f, -11.f};
        case 369:
        case 370:
        case 466:
            return {0.f, 8.f};
        case 373:
            return {-7.5f, 8.5f};
        case 374:
            return {0.5f, 8.5f};
        case 392:
        case 393:
        case 458:
        case 459:
            return {0.f, -2.25};
        case 406:
            return {0.f, -8.f};
        case 408:
            return {0.f,-12.5f};
        case 414:
        case 421:
        case 422:
            return {0.f, -9.f};
        case 419:
        case 420:
            return {0.f, -2.5f};
        case 446:
        case 447:
            return {0.f, -7.5f};
        case 448:
        case 449:
        case 452:
            return {0.f, -4.f};
        case 450:
            return {0.f, 15.f};
        case 451:
            return {0.f, 4.f};
        case 462:
            return {0.f, 0.5f};
        case 463:
            return {0.f, -1.f};
        case 464:
            return {0.f, -5.5f};
        case 465:
            return {0.f, -7.f};
        case 468:
            return {0.f, 14.25f};
        case 475:
            return {0.f, -14.25f};
        case 503:
            return {0.f, -5.f};
        case 763:
            return {15.f, 0.f};
        case 504:
            return {5.f, -5.f};
        case 506:
        case 507:
        case 515:
        case 516:
        case 524:
        case 525:
        case 533:
        case 534:
        case 542:
        case 543:
        case 551:
        case 552:
        case 560:
        case 561:
        case 569:
        case 570:
        case 578:
        case 579:
        case 587:
        case 588:
        case 596:
        case 597:
        case 605:
        case 606:
        case 614:
        case 615:
        case 623:
        case 624:
        case 632:
        case 633:
            return {0.f, -10.f};
        case 508:
        case 512:
        case 517:
        case 521:
        case 526:
        case 530:
        case 535:
        case 539:
        case 544:
        case 548:
        case 553:
        case 557:
        case 562:
        case 566:
        case 571:
        case 575:
        case 580:
        case 584:
        case 589:
        case 593:
        case 598:
        case 602:
        case 607:
        case 611:
        case 616:
        case 620:
        case 625:
        case 629:
        case 634:
        case 638:
            return {-7.5f, -10.f};
        case 509:
        case 510:
        case 511:
        case 518:
        case 519:
        case 520:
        case 527:
        case 528:
        case 529:
        case 536:
        case 537:
        case 538:
        case 545:
        case 546:
        case 547:
        case 554:
        case 555:
        case 556:
        case 563:
        case 564:
        case 565:
        case 572:
        case 573:
        case 574:
        case 581:
        case 582:
        case 583:
        case 590:
        case 591:
        case 592:
        case 599:
        case 600:
        case 601:
        case 608:
        case 609:
        case 610:
        case 617:
        case 618:
        case 619:
        case 626:
        case 627:
        case 628:
        case 635:
        case 636:
        case 637:
            return {-10.f, -10.f};
        case 513:
        case 522:
        case 531:
        case 540:
        case 549:
        case 558:
        case 567:
        case 576:
        case 585:
        case 594:
        case 603:
        case 612:
        case 621:
        case 630:
        case 639:
            return {10.f, 5.f};
        case 514:
        case 523:
        case 532:
        case 541:
        case 550:
        case 559:
        case 568:
        case 577:
        case 586:
        case 595:
        case 604:
        case 613:
        case 622:
        case 631:
        case 640:
            return {-5.f, 5.f};
        case 652:
        case 666:
        case 674:
        case 682:
        case 684:
        case 686:
        case 688:
        case 690:
        case 692:
        case 694:
        case 696:
        case 698:
        case 700:
        case 702:
        case 704:
        case 706:
        case 708:
        case 710:
        case 712:
        case 714:
        case 716:
        case 727:
        case 729:
        case 731:
        case 733:
            return {15.f, 0.f};
        case 655:
        case 656:
        case 662:
        case 663:
        case 664:
        case 723:
        case 735:
            return {7.5f, 0.f};
        case 667:
        case 668:
        case 719:
            return {0.f, -7.5f};
        case 670:
            return {11.25f, -11.25f};
        case 720:
        case 721:
            return {-11.5f, -11.5f};
        case 767:
            return {-7.5f, -2.5f};
        case 768:
            return {7.5f, -9.f};
        case 769:
        case 770:
        case 967:
        case 968:
            return {0.f, 3.f};
        case 828:
        case 829:
        case 878:
        case 887:
        case 889:
        case 896:
        case 961:
        case 1015:
            return {15.f, 0.f};
        case 873:
        case 891:
            return {7.5f, 0.f};
        case 902:
        case 950:
        case 987:
            return {10.f, 5.f};
        case 906:
            return {0.f, -5.5f};
        case 907:
            return {0.f, -4.5f};
        case 908:
        case 909:
        case 941:
            return {0.f, -7.5f};
        case 910:
            return {0.f, -9.5f};
        case 919:
        case 942:
        case 943:
        case 944:
        case 980:
        case 981:
            break;
        case 920:
        case 921:
            return {0.f, 16.5f};
        case 923:
            return {0.f, 11.5f};
        case 924:
            return {0.f, 5.5f};
        case 939:
            return {0.f, -6.f};
        case 945:
        case 949:
        case 982:
        case 986:
            return {-7.5f, -10.f};
        case 946:
        case 947:
        case 948:
        case 983:
        case 984:
        case 985:
            return {-10.f, -10.f};
        case 951:
        case 988:
            return {-5.f, 5.f};
        case 991:
        case 992:
            return {11.5f, -11.5f};
        case 1009:
            return {2.5f, -2.5f};
        case 1024:
            return {0.f, -10.f};
        case 1025:
        case 1063:
        case 1064:
            return {0.f, -10.f};
        case 1026:
        case 1030:
        case 1065:
        case 1069:
            return {-7.5f, -10.f};
        case 1027:
        case 1028:
        case 1029:
        case 1066:
        case 1067:
        case 1068:
            return {-10.f, -10.f};
        case 1031:
        case 1070:
            return {10.f, 5.f};
        case 1032:
        case 1071:
            return {-5.f, 5.f};
        case 1034:
        case 1038:
        case 1042:
        case 1044:
        case 1092:
        case 1108:
        case 1188:
        case 1199:
        case 1257:
        case 1259:
            return {15.f, 0.f};
        case 1046:
        case 1048:
            return {0.f,-12.5f};
        case 1050:
        case 1051:
        case 1052:
            return {0.f, -2.f};
        case 1054:
        case 1079:
        case 1080:
        case 1081:
        case 1096:
        case 1097:
        case 1098:
        case 1111:
            return {7.5f, 0.f};
        case 1154:
            return {0.f, 6.75f};
        case 1202:
            return {0.f, 13.5f};
        case 1220:
            return {0.f, 12.f};
        case 1260:
            return {0.f, 15.75f};
        case 1262:
            return {0.f, 16.5f};
        case 1264:
            return {0.f, 18.f};
        case 1271:
        case 1272:
            return {2.5f, -2.5f};
        case 1273:
        case 1274:
            return {5.f, -5.f};
        case 1291:
            return {-7.5f, -5.f};
        case 1292:
        case 1327:
            return {0.f, -2.5f};
        case 1293:
            return {-7.5f, 0.f};
        case 1306:
        case 1317:
        case 1339:
        case 1342:
        case 1345:
            return {15.f, 0.f};
        case 1328:
            return {0.f, 3.f};
        case 1332:
            return {0.f,-12.5f};
        case 1340:
            return {0.f, 14.25f};
        case 1343:
            return {0.f, 13.5f};
        case 1356:
        case 1367:
        case 1368:
        case 1395:
            return {9.f, 0.f};
        case 1357:
        case 1358:
        case 1369:
        case 1370:
            return {-7.f, 0.f};
        case 1359:
        case 1360:
        case 1371:
        case 1372:
        case 1473:
        case 1496:
            return {0.f, -8.f};
        case 1361:
        case 1362:
        case 1373:
        case 1374:
            return {0.f, 7.5f};
        case 1363:
        case 1375:
            return {9.f, -8.f};
        case 1364:
        case 1376:
            return {-7.f, -8.f};
        case 1365:
        case 1377:
            return {9.f, 8.f};
        case 1366:
        case 1378:
            return {-7.f, 8.f};
        case 1391:
            return {0.f, 10.f};
        case 1392:
            return {-14.f, 0.f};
        case 1393:
            return {0.f, 14.5f};
        case 1394:
            return {14.5f, 0.f};
        case 1471:
            return {0.f, 9.5f};
        case 1472:
            return {-13.6f, 9.5f};
        case 1507:
            return {-7.5f, 0.5f};
        case 1529:
            return {0.f, -10.f};
        case 1530:
        case 1538:
        case 1539:
        case 1552:
        case 1553:
        case 1591:
        case 1593:
            return {0.f, -10.f};
        case 1531:
        case 1535:
        case 1554:
        case 1558:
            return {-7.5f, -10.f};
        case 1532:
        case 1533:
        case 1534:
        case 1540:
        case 1555:
        case 1556:
        case 1557:
            return {-10.f, -10.f};
        case 1536:
        case 1559:
            return {10.f, 5.f};
        case 1537:
        case 1560:
            return {-5.f, 5.f};
        case 1592:
            return {3.75f, 0.f};
        case 1617:
            return {-11.f, 9.5f};
        case 1621:
            return {10.f, 0.f};
        case 2099:
            return {0.f, -11.5f};
        case 2100:
        case 2133:
            return {0.f, -5.75f};
        case 2101:
            return {0.f, -4.5f};
        case 2102:
        case 2210:
            return {0.f, -9.5f};
        case 2107:
            return {0.f, -11.25f};
        case 2113:
            return {0.f, 5.5f};
        case 2114:
            return {0.f, 4.5f};
        case 2117:
        case 2168:
            return {0.f, -4.75f};
        case 2118:
        case 2134:
        case 2244:
            return {0.f, -4.f};
        case 2119:
        case 2182:
            return {0.f, -7.5f};
        case 2122:
            return {0.f, -2.f};
        case 2128:
        case 2131:
            return {0.f, 11.25f};
        case 2130:
            return {0.f, 9.f};
        case 2135:
        case 2136:
        case 2137:
        case 2138:
            return {0.f, -6.f};
        case 2140:
        case 2165:
            return {0.f, 2.f};
        case 2143:
        case 2169:
        case 2173:
            return {0.f, 8.5f};
        case 2149:
        case 2151:
        case 2191:
            return {0.f, 10.5f};
        case 2150:
        case 2208:
            return {0.f, 3.f};
        case 2166:
            return {0.f, 11.5f};
        case 2167:
        case 2192:
            return {0.f, 4.75f};
        case 2172:
            return {0.f, -1.f};
        case 2174:
            return {0.f, 6.75f};
        case 2179:
        case 2249:
        case 2250:
            return {0.f, -3.f};
        case 2206:
        case 2243:
            return {0.f, -8.f};
        case 2213:
            return {0.f, 1.75f};
        case 2220:
        case 2255:
            return {0.f, 4.f};
        case 2221:
            return {0.f, 6.5f};
        case 2238:
            return {15.f, 0.f};
        case 2242:
            return {0.f, -9.f};
        case 2245:
            return {-1.f, -14.f};
        case 2278:
        case 2279:
        case 2280:
            return {0.f, 14.75f};
        case 2302:
            return {-8.f, 8.f};
        case 2567:
            return {15.f, 0.f};
        case 2622:
            return {0.f, 11.f};
        case 2623:
            return {0.f, 9.f};
        case 2624:
            return {0.f, 3.f};
        case 2625:
            return {0.f, 6.f};
        case 2629:
            return {0.f, 5.f};
        case 2693:
            return {0.f, -8.f};
        case 2708:
            return {0.f, -15.25f};
        case 2709:
        case 2710:
        case 2711:
        case 2712:
        case 2713:
        case 2714:
        case 2715:
        case 2716:
        case 2717:
        case 2718:
        case 2719:
        case 2720:
        case 2721:
        case 2722:
        case 2723:
        case 2724:
        case 2725:
        case 2726:
        case 2727:
        case 2728:
        case 2729:
        case 2730:
        case 2731:
        case 2764:
        case 2765:
        case 2766:
        case 2767:
        case 2768:
        case 2769:
        case 2770:
        case 2773:
            return {0.f, -15.25f};
        case 2732:
        case 2733:
        case 2734:
        case 2735:
        case 2736:
        case 2737:
        case 2738:
        case 2739:
        case 2740:
        case 2741:
        case 2742:
        case 2743:
        case 2776:
        case 2777:
        case 2778:
        case 2779:
        case 2780:
        case 2781:
        case 2782:
        case 2783:
        case 2784:
        case 2785:
        case 2786:
        case 2787:
            return {-15.25f, 0.f};
        case 2744:
        case 2745:
        case 2746:
        case 2747:
        case 2748:
        case 2749:
        case 2750:
        case 2751:
        case 2788:
        case 2789:
        case 2790:
        case 2791:
        case 2792:
        case 2793:
        case 2794:
        case 2795:
        case 2927:
        case 2928:
        case 2929:
        case 2930:
        case 2931:
        case 2932:
        case 2933:
        case 2934:
        case 2935:
        case 2936:
        case 2937:
        case 2938:
        case 2939:
        case 2940:
        case 2941:
        case 2942:
            return {0.f, 15.25f};
        case 2752:
        case 2753:
        case 2754:
        case 2755:
        case 2756:
        case 2757:
        case 2758:
        case 2759:
        case 2760:
        case 2761:
        case 2762:
        case 2763:
        case 2796:
        case 2797:
        case 2798:
        case 2799:
        case 2800:
        case 2801:
        case 2802:
        case 2803:
        case 2804:
        case 2805:
        case 2806:
        case 2807:
            return {15.25f, 0.f};
        case 2897:
            return {15.f, 0.f};
        case 3000:
        case 3001:
        case 3002:
            return {0.f, -10.f};
        case 3005:
            return {0.f, -12.f};
        case 3118:
            return {0.f, -8.f};
        case 3132:
        case 3133:
        case 3134:
            return {0.f, -10.5f};
        case 3135:
            return {0.f, 3.f};
        case 3137:
            return {0.f, 8.f};
        case 3138:
            return {0.f, -8.f};
        case 3139:
        case 3337:
        case 3378:
            return {0.f, -3.f};
        case 3201:
            return {0.f, 4.f};
        case 3202:
        case 3293:
        case 3294:
        case 3295:
        case 3336:
            return {0.f, -6.f};
        case 3203:
        case 3292:
            return {0.f, -4.f};
        case 3204:
        case 3313:
        case 3314:
            return {0.f, 6.f};
        case 3205:
            return {0.f, 7.5f};
        case 3296:
            return {0.f, -10.f};
        case 3334:
            return {0.f, 2.5f};
        case 3379:
            return {0.f, 9.f};
        case 3390:
            return {0.f, 6.f};
        case 3391:
        case 3392:
        case 3444:
        case 3484:
            return {0.f, 6.f};
        case 3393:
            return {0.f, 7.f};
        case 3394:
        case 3395:
        case 3396:
        case 3397:
        case 3398:
        case 3399:
        case 3400:
        case 3401:
        case 3424:
            return {0.f, 2.f};
        case 3402:
        case 3404:
            return {0.f, -9.25f};
        case 3403:
        case 3405:
        case 3406:
        case 3414:
        case 3415:
        case 3416:
        case 3420:
        case 3421:
        case 3460:
            return {0.f, -10.f};
        case 3407:
        case 3408:
            return {0.f, -7.f};
        case 3412:
        case 3413:
        case 3441:
            return {0.f, -9.5f};
        case 3417:
        case 3418:
        case 3419:
        case 3436:
            return {0.f, -9.f};
        case 3422:
        case 3458:
        case 3459:
            return {0.f, -11.f};
        case 3423:
            return {0.f, 3.f};
        case 3437:
        case 3463:
            return {0.f, -4.f};
        case 3438:
        case 3461:
        case 3479:
            return {0.f, -7.5f};
        case 3439:
        case 3440:
            return {0.f, -5.f};
        case 3442:
            return {0.f, -6.5f};
        case 3445:
        case 3454:
        case 3483:
            return {0.f, 10.f};
        case 3447:
            return {0.f, 11.f};
        case 3450:
            return {0.f, 2.5f};
        case 3452:
            return {0.f, 8.5f};
        case 3462:
        case 3477:
            return {0.f, -6.f};
        case 3476:
            return {0.f, -3.f};
        case 3478:
        case 3482:
            return {0.f, -2.f};
        case 3480:
            return {0.f, -4.5f};
        case 3481:
            return {0.f, -2.5f};
        case 3540:
            return {15.f, 0.f};
        case 3553:
        case 3554:
        case 3558:
        case 3559:
        case 3560:
        case 3561:
        case 3562:
            return {7.5f, 0.f};
        case 3794:
            return {0.f, -2.f};
        case 4071:
            return {0.f, -7.f};
        case 3795:
            return {0.f, -6.f};
        case 3796:
            return {0.f, -8.f};
        case 3797:
        case 3798:
        case 3799:
            return {0.f, 1.5f};
        case 4076:
            return {0.f, 6.5f};
        case 4085:
        case 4086:
            return {0.f, 1.f};
        case 4087:
            return {0.f, 7.f};
        case 4234:
            return {0.f, -6.5f};
        case 4235:
            return {0.f, -11.f};
        case 4236:
            return {0.f, -7.5f};
        case 4237:
            return {0.f, -4.5f};
        case 4288:
        case 4289:
            return {0.f, 3.f};
        case 4300:
            return {0.f, 9.f};
        case 4323:
        case 4324:
        case 4325:
            return {0.f, 5.f};
        case 4326:
            return {0.f, -6.f};
        case 4327:
        case 4328:
            return {0.f, -8.f};
        case 2303:
            return {8.f, 8.f};
        case 2304:
        case 2305:
        case 2306:
        case 2376:
            return {0.f, 8.f};
        case 2307:
        case 2308:
        case 2309:
        case 2343:
        case 2388:
        case 2459:
            return {0.f, -7.5f};
        case 2310:
        case 2332:
        case 2538:
            return {0.f, -2.f};
        case 2311:
            return {0.f, 4.f};
        case 2315:
            return {0.f, -5.25f};
        case 2316:
            return {0.f, -9.25f};
        case 2317:
        case 2321:
            return {0.f, 4.5f};
        case 2318:
        case 2319:
            return {0.f, 3.5f};
        case 2320:
        case 2371:
        case 2372:
            return {0.f, 1.f};
        case 2322:
            return {0.f, 9.75f};
        case 2323:
            return {0.f, 9.f};
        case 2324:
        case 2325:
        case 2365:
        case 2536:
            return {0.f, -8.5f};
        case 2326:
            return {0.f, -1.75};
        case 2327:
        case 2366:
            return {0.f, -4.f};
        case 2328:
        case 2524:
        case 2525:
            return {0.f, -6.5f};
        case 2329:
            return {0.f, -7.75f};
        case 2330:
        case 2335:
        case 2341:
        case 2408:
            return {0.f, -9.f};
        case 2333:
        case 2367:
            return {0.f, -2.5f};
        case 2334:
        case 2390:
            return {0.f, -5.f};
        case 2342:
            return {0.f, -3.f};
        case 2344:
        case 2345:
            return {0.f, -11.f};
        case 2355:
            return {3.f, -7.f};
        case 2356:
            return {0.f, 5.f};
        case 2357:
            return {0.f, 6.25f};
        case 2358:
            return {0.f, 7.f};
        case 2369:
        case 2370:
            return {0.f, -1.f};
        case 2375:
            return {0.f, 13.f};
        case 2377:
            return {0.f, -12.f};
        case 2386:
            return {0.f, -9.5f};
        case 2387:
            return {0.f, -11.25f};
        case 2409:
        case 2410:
            return {0.f, -10.f};
        case 2423:
            return {0.f, -10.25f};
        case 2424:
            return {0.f, -6.75f};
        case 2473:
            break;
        case 2508:
        case 2509:
        case 2510:
            return {0.f, 5.5f};
        case 2534:
        case 2535:
            return {0.f, 2.5f};
        case 2537:
            return {0.f, -4.75f};
        case 2539:
            return {0.f, 10.f};
        case 2566:
            return {15.f, 0.f};
        case 1622:
        case 1623:
        case 1624:
        case 1653:
        case 1654:
        case 1655:
        case 1656:
            return {10.f, 0.f};
        case 1625:
        case 1626:
        case 1657:
        case 1658:
            return {-11.f, 0.f};
        case 1627:
        case 1628:
        case 1659:
        case 1660:
            return {-11.5f, 0.f};
        case 1629:
        case 1630:
        case 1661:
        case 1662:
        case 1725:
        case 1726:
        case 1890:
            return {0.f, -9.f};
        case 1631:
        case 1632:
        case 1663:
        case 1664:
            return {0.f, -9.5f};
        case 1633:
        case 1634:
        case 1635:
        case 1636:
        case 1665:
        case 1666:
        case 1667:
        case 1668:
            return {0.f, 10.f};
        case 1637:
        case 1669:
            return {13.f, -12.5f};
        case 1638:
        case 1670:
            return {-12.5f, -9.5f};
        case 1639:
        case 1671:
            return {11.5f, 11.75f};
        case 1640:
        case 1672:
            return {-13.5f, 12.f};
        case 1641:
        case 1673:
            return {11.f, -9.5f};
        case 1642:
        case 1674:
            return {-13.5f, -12.5f};
        case 1643:
        case 1675:
            return {13.f, 12.f};
        case 1644:
        case 1676:
            return {-12.25f, 11.f};
        case 1645:
        case 1677:
            return {11.5f, -12.5f};
        case 1646:
        case 1678:
            return {-12.5f, -10.5f};
        case 1647:
        case 1679:
            return {13.5f, 13.5f};
        case 1648:
        case 1680:
            return {-13.25f, 11.f};
        case 1649:
        case 1681:
            return {13.f, -11.f};
        case 1650:
        case 1682:
            return {-12.5f, -12.25f};
        case 1651:
        case 1683:
            return {12.f, 11.f};
        case 1652:
        case 1684:
            return {-13.75f, 13.5f};
        case 1711:
        case 1712:
        case 1713:
        case 1714:
        case 1719:
        case 1720:
        case 1721:
        case 1722:
            return {0.f, -11.f};
        case 1715:
            return {0.f,-12.5f};
        case 1716:
            return {0.f, -14.f};
        case 1718:
        case 1724:
        case 1744:
        case 1746:
        case 1748:
        case 1750:
        case 1773:
        case 1774:
        case 1775:
        case 1776:
        case 1785:
        case 1786:
        case 1787:
        case 1788:
        case 1789:
        case 1790:
        case 1791:
        case 1792:
        case 1794:
        case 1796:
        case 1800:
        case 1802:
        case 1804:
        case 1806:
        case 1808:
        case 1810:
        case 1900:
        case 1907:
            return {15.f, 0.f};
        case 1727:
            return {7.5f, -9.f};
        case 1728:
        case 1729:
        case 1730:
        case 1737:
            return {0.f, -7.5f};
        case 1731:
            return {-11.5f, -11.5f};
        case 1733:
            return {11.5f, -11.5f};
        case 1739:
            return {11.25f, -11.25f};
        case 1757:
            return {-7.5f, 0.f};
        case 1758:
            return {-7.25f, 7.f};
        case 1759:
            return {10.5f, 9.f};
        case 1760:
            return {-3.5f, 3.5f};
        case 1761:
            return {13.f, 4.25};
        case 1762:
            return {-10.5f, 10.5f};
        case 1763:
            return {8.5f, 13.25f};
        case 1891:
            return {0.f, -6.f};
        case 1892:
            return {0.f, -2.25};
        case 1893:
        case 1896:
        case 1897:
        case 1898:
            return {0.f, 7.f};
        case 1895:
            return {-8.f, 8.f};
        case 1903:
        case 1904:
        case 1905:
            return {0.f, 8.f};
        case 1908:
            return {-7.5f, 8.5f};
        case 1909:
            return {0.5f, 8.5f};
        case 1919:
            return {0.f, -5.f};
        case 1920:
            return {0.f, -7.f};
        case 1921:
            return {0.f, -2.5f};
        case 1938:
            return {0.f, -2.f};
        case 1939:
            return {0.f, -1.f};
        case 2098:
            return {0.f, -11.5f};
    }
    return {0.f, 0.f};
}

void REEditorUI::onAssignNewGroupID() {
    auto selectedObjects = m_selectedObject ? CCArray::createWithObject(m_selectedObject) : m_selectedObjects;
    if (selectedObjects->count() == 0) return;

    auto groupID = m_editorLayer->getNextFreeGroupID({});

    for (auto obj : CCArrayExt<GameObject, false>(selectedObjects)) {
        if (obj->addToGroup(groupID) == 1) m_editorLayer->addToGroup(obj, groupID, false);
    }

    updateObjectInfoLabel();
}

void REEditorUI::onColorFilter(CCObject* sender) {
    int id = GameManager::get()->getIntGameVariable(GameVar::ColorIDFilter);

    auto popup = SetColorIDPopup::create(id);

    popup->setTag(2);
    popup->m_delegate = this;

    popup->show();
}

void REEditorUI::onCopy(CCObject* sender) {
    doCopyObjects(false);
}

void REEditorUI::onCopyState(CCObject* sender) {
    if (m_selectedObject) {
        GameManager::get()->m_levelEditorLayer->copyObjectState(m_selectedObject);
        updateButtons();
    }
}

bool REEditorUI::onCreate() {
    if (m_selectedObjectIndex == 0) return false;
    
    if (m_selectedObjectIndex != 31) {
        if (m_editorLayer->m_objectCount >= 80000) {
            if (!m_editorLayer->m_level->m_unlimitedObjectsEnabled) {
                showMaxError();
                return true;
            }

            if (!m_editorLayer->m_level->m_highObjectsEnabled) {
                showMaxBasicError();
                return true;
            }
        }
        else if (m_editorLayer->m_objectCount > 39999) {
            if (!m_editorLayer->m_level->m_highObjectsEnabled) {
                showMaxBasicError();
                return true;
            }
        }
    }

    if (m_selectedObjectIndex == 1329 && m_editorLayer->m_coinCount > 2) {
        if (m_alertShown) {
            return true;
        }

        m_alertShown = true;
        showMaxCoinError_();
        return true;
    }

    if (m_selectedObject || m_selectedObjects->count() != 0) {
        createUndoSelectObject(true);
    }

    onCreateObject(m_selectedObjectIndex);
    return true;
}

void REEditorUI::onCreateButton(CCObject* sender) {
    int id = static_cast<CreateMenuItem*>(sender)->m_objectID;

    int newID = 0;
    if (m_selectedObjectIndex != id) {
        newID = id;
    }

    m_selectedObjectIndex = newID;
    updateCreateMenu(false);
    updateGridNodeSize();
}

void REEditorUI::onCreateObject(int id) {
    auto basePos = getGridSnappedPos_(m_clickAtPosition);
    auto offset = offsetForKey(id);
    auto objectPos = basePos + offset;

    auto checkPos = objectPos;
    bool isFlipX = false;
    bool isFlipY = false;
    float rot = 0.0f;

    if (m_selectedObject && m_selectedObject->m_objectID == id) {
        isFlipX = m_selectedObject->isFlipX();
        isFlipY = m_selectedObject->isFlipY();
        rot = m_selectedObject->getRotation();
        checkPos = basePos + getRelativeOffset_(m_selectedObject);
    }

    bool exists = m_editorLayer->typeExistsAtPosition(id, checkPos, isFlipX, isFlipY, rot);

    if ((exists || (id < 0 && positionIsInSnapped_(checkPos)))) {
        return;
    }

    addSnapPosition_(checkPos);

    if (id < 1) {
        auto objArray = CCArray::create();
        m_selectedObject ? objArray->addObject(m_selectedObject) : objArray->addObjectsFromArray(m_selectedObjects);

        auto str = GameManager::get()->stringForCustomObject(id);

        auto pasted = pasteObjects(str, false, false);
        if (pasted->count() == 0) return;

        auto offset = objectPos;
        for (auto obj : CCArrayExt<GameObject, false>(pasted)) {
            if (obj && obj->m_hasGroupParent) {
                offset += obj->getPosition() - getGroupCenter(pasted, false);
                break;
            }
        }
        
        repositionObjectsToCenter(pasted, offset, false);

        if (pasted->count() == objArray->count()) {
            for (unsigned int i = 0; i < objArray->count(); ++i) {
                auto original   = static_cast<GameObject*>(objArray->objectAtIndex(i));
                auto pastedObj  = static_cast<GameObject*>(pasted->objectAtIndex(i));

                if (!original || !pastedObj) continue;
                if (!original->m_hasGroupParent || !pastedObj->m_hasGroupParent) continue;
                if (original->m_objectID != pastedObj->m_objectID) continue;

                float delta = original->getRotation() - pastedObj->getRotation();

                if (delta != 0.f && static_cast<int>(delta) % 90 == 0) {
                    rotateObjects(pasted, delta, {0.f, 0.f});
                }
                break;
            }
        }
        return;
    }

    auto object = createObject(id, objectPos);

    if (m_selectedObject && object->m_objectID == m_selectedObject->m_objectID) {
        removeOffset(object);
        object->duplicateValues(m_selectedObject);
        applyOffset(object);
    }

    if (id == 914) {
        static_cast<TextGameObject*>(object)->updateTextObject("A", false);
    } 
    else if (id == 3032) {
        auto keyFrameObject = static_cast<KeyframeGameObject*>(object);
        m_editorLayer->removeSpecial(object);

        auto arr = m_editorLayer->createNewKeyframeAnim();
        keyFrameObject->m_keyframeGroup = arr->getTag();
        keyFrameObject->m_keyframeIndex = 0;
        m_editorLayer->addSpecial(object);
    }

    auto effectObject = static_cast<EffectGameObject*>(object);

    if (object->m_classType == GameObjectClassType::Effect && object->isSpeedObject() && (effectObject->m_easingType != EasingType::None || !effectObject->m_isReverse)) {
        effectObject->m_shouldPreview = true;
        m_editorLayer->m_drawGridLayer->addToSpeedObjects(effectObject);
    }

    selectObject(object, false);
    updateSlider();
}

void REEditorUI::onDelete(CCObject* sender) {
    //todo
    EditorUI::onDelete(sender);
}

void REEditorUI::onDeleteAll_(CCObject* sender) {
    deselectAll();
    m_editorLayer->removeAllObjects();
}

void REEditorUI::onDeleteCustomItem(CCObject* sender) {
    if (m_selectedObjectIndex > -1) return;
    int id = static_cast<CreateMenuItem*>(sender)->m_objectID;

    GameManager::get()->removeCustomObject(id);
    m_selectedObjectIndex = 0;

    reloadCustomItems();
}

void REEditorUI::onDeleteInfo(CCObject* sender) {
    FLAlertLayer::create(nullptr, "Delete Filters", "<co>Only remove certain objects.</c>\n<cy>Static:</c> Blocks, Spikes...\n<cg>Details</c>: Details, portals, triggers...\n<cl>Custom</c>: Selected object\n<cr>None: </c> Disabled", "OK", nullptr, 300.f, false, 0.f, 1.f)->show();
}

void REEditorUI::onDeleteSelected(CCObject* sender) {
    //todo
    EditorUI::onDeleteSelected(sender);
}

void REEditorUI::onDeleteSelectedType(CCObject* sender) {
    if (m_selectedObject) {
        int id = m_selectedObject->m_objectID;
        deselectAll();
        m_editorLayer->removeAllObjectsOfType(id);
        updateButtons();
        tryUpdateTimeMarkers_();
    }
}

void REEditorUI::onDeleteStartPos(CCObject* sender) {
    deselectAll();
    m_editorLayer->removeAllObjectsOfType(31);
}

void REEditorUI::onDeselectAll(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) {
        deselectAll();
        return;
    }
    createUndoSelectObject(false);
    deselectAll();
}

void REEditorUI::onDuplicate(CCObject* sender) {
    //todo
    EditorUI::onDuplicate(sender);
}

void REEditorUI::onEditColor(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) {
        return;
    }

    if (m_editHSVBtn->getTag() == 1){
        editHSV();
        updateEditColorButton();
        return;
    }
    editColor();
    updateEditColorButton();
}

void REEditorUI::onFindObject(CCObject* sender) {
    FindObjectPopup::create()->show();
}

void REEditorUI::onGoToBaseLayer(CCObject* sender) {
    m_editorLayer->m_currentLayer = -1;
    updateGroupIDLabel();
}

void REEditorUI::onGoToLayer(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) return;

    short targetLayer = -1;
    short targetLayer2 = -1;

    if (m_selectedObject) {
        targetLayer  = m_selectedObject->m_editorLayer;
        targetLayer2 = m_selectedObject->m_editorLayer2;
    }
    else {
        bool firstInit = true;

        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            if (firstInit) {
                targetLayer = obj->m_editorLayer;
                firstInit = false;
            }
            else if (targetLayer != obj->m_editorLayer) {
                targetLayer = -1;
                break;
            }
        }

        firstInit = true;

        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            if (firstInit) {
                targetLayer2 = obj->m_editorLayer2;
                firstInit = false;
            }
            else if (targetLayer2 != obj->m_editorLayer2) {
                targetLayer2 = -1;
                break;
            }
        }
    }

    short resultLayer = targetLayer;

    if (targetLayer2 != 0 && targetLayer != 0 && targetLayer2 != m_editorLayer->m_currentLayer) {
        resultLayer = targetLayer2;
    }

    m_editorLayer->m_currentLayer = resultLayer;

    updateGroupIDLabel();
}

void REEditorUI::onGroupDown(CCObject* sender) {
    int newLayer = m_editorLayer->m_currentLayer - 1;
    if (newLayer < -1) {
        newLayer = 65535;
    }

    m_editorLayer->m_currentLayer = newLayer;
    updateGroupIDLabel();
}

void REEditorUI::onGroupIDFilter(CCObject* sender) {
    auto current = GameManager::get()->getIntGameVariable(GameVar::GroupIDFilter);
    auto popup = SetIDPopup::create(current, 0, 9999, "GroupID Filter", "OK", false, 0, 0.f, true, true);

    popup->setTag(1);
    popup->m_delegate = this;
    popup->show();
}

void REEditorUI::onGroupSticky(CCObject* sender) {
    if (m_selectedObjects->count() == 0) {
        return;
    }

    m_editorLayer->groupStickyObjects(m_selectedObjects);
    updateButtons();
}

void REEditorUI::onGroupUp(CCObject* sender) {
    m_editorLayer->m_currentLayer++;
    updateGroupIDLabel();
}

void REEditorUI::onLockLayer(CCObject* sender) {
    if (!GameManager::get()->getGameVariable(GameVar::LayerLocking)) {
        return;
    }
    m_editorLayer->toggleLockActiveLayer();
    updateGroupIDLabel();
}

void REEditorUI::onNewCustomItem(CCObject* sender) {
    //todo
    EditorUI::onNewCustomItem(sender);
}

void REEditorUI::onPaste(CCObject* sender) {
    doPasteObjects(false);
}

void REEditorUI::onPasteColor(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) {
        return;
    }
    auto editorLayer = GameManager::get()->m_levelEditorLayer;
    editorLayer->pasteColorState(m_selectedObject, m_selectedObjects);
    updateObjectInfoLabel();
}

void REEditorUI::onPasteInPlace_(CCObject* sender) {
    doPasteInPlace_(false);
}

void REEditorUI::onPasteState(CCObject* sender) {
    if (!m_selectedObject && m_selectedObjects->count() == 0) {
        return;
    }
    auto editorLayer = GameManager::get()->m_levelEditorLayer;
    editorLayer->pasteAttributeState(m_selectedObject, m_selectedObjects);
    updateObjectInfoLabel();
}

void REEditorUI::onPause(CCObject* sender) {
    //todo
    EditorUI::onPause(sender);
}

void REEditorUI::onPlayback(CCObject* sender) {
    //todo
    EditorUI::onPlayback(sender);
}

void REEditorUI::onPlaytest(CCObject* sender) {
    //todo
    EditorUI::onPlaytest(sender);
}

void REEditorUI::onResetSpecialFilter(CCObject* sender) {
    auto gameManager = GameManager::get();

    gameManager->setIntGameVariable(GameVar::GroupIDFilter, 0);
    m_deleteGroupSprite->setString("0");

    gameManager->setIntGameVariable(GameVar::ColorIDFilter, 0);
    m_deleteColorSprite->setString("0");
}

void REEditorUI::onSelectBuildTab(CCObject* sender) {
    selectBuildTab(sender->getTag());
}

void REEditorUI::onSettings(CCObject* sender) {
    if (m_isPaused) return;

    if (!m_editorLayer->m_level || m_editorLayer->m_level->m_levelType == GJLevelType::Editor) {
        auto settings = LevelSettingsLayer::create(m_editorLayer->m_levelSettings, m_editorLayer);
        settings->m_delegate = m_editorLayer;
        settings->show();
    }
}

void REEditorUI::onStickyToggle(CCObject* sender) {
    auto gameManager = GameManager::get();
    gameManager->toggleGameVariable(GameVar::LinkControlsQuickToggle);

    m_linkControlsDisabled = gameManager->getGameVariable(GameVar::LinkControlsQuickToggle);
    updateButtons();
}

void REEditorUI::onStopPlaytest(CCObject* sender) {
    showUI(true);
    if (m_isPlayingMusic) {
        onPlayback(nullptr);
    }
    m_editorLayer->onStopPlaytest();
    playtestStopped();
}

void REEditorUI::onTargetIDChange(int change) {
    CCArray* objects = nullptr;

    if (m_selectedObject) {
        objects = CCArray::createWithObject(m_selectedObject);
    }
    else {
        objects = m_selectedObjects;
    }

    if (!objects || objects->count() == 0) return;

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if (!obj->isTrigger()) return;
    }

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        auto effectGameObject = static_cast<EffectGameObject*>(obj);

        if (!obj->isTrigger() || effectGameObject->m_targetGroupID <= 0) continue;

        effectGameObject->setTargetID(effectGameObject->m_targetGroupID + change);

        auto label = static_cast<CCLabelBMFont*>(obj->getChildByTag(999));
        if (label) {
            auto str = CCString::createWithFormat("%i", effectGameObject->m_targetGroupID);
            label->setString(str->getCString());
        }
    }
}

void REEditorUI::onToggleGuide_(EffectGameObject* object) {
    object->m_shouldPreview = !object->m_shouldPreview;

    if (m_editorLayer->tryUpdateSpeedObject(object, false)) {
        m_speedObjectsUpdated = true;
    }
    tryUpdateTimeMarkers_();
}

void REEditorUI::onToggleSelectedOrder_(EffectGameObject* object) {
    if (m_editorLayer->m_playbackObject && m_editorLayer->m_playbackObject->m_uniqueID == object->m_uniqueID) {
        m_editorLayer->m_playbackObject = nullptr;
    }
    m_editorLayer->m_playbackObject = object;
}

void REEditorUI::onToggleTraceIn_(GameObject* object) {
    if (m_editorLayer->m_traceInObject && m_editorLayer->m_traceInObject->m_uniqueID == object->m_uniqueID) {
        m_editorLayer->m_traceInObject = nullptr;
    }
    m_editorLayer->m_traceInObject = reinterpret_cast<EffectGameObject*>(object);
    // FIX BINDINGS, m_traceInObject IS GameObject*, not EffectGameObject*
}

void REEditorUI::onToggleTraceOut_(EffectGameObject* object) {
    if (m_editorLayer->m_traceOutObject && m_editorLayer->m_traceOutObject->m_uniqueID == object->m_uniqueID) {
        m_editorLayer->m_traceOutObject = nullptr;
    }
    m_editorLayer->m_traceOutObject = object;
}

void REEditorUI::onUngroupSticky(CCObject* sender) {
    if (m_selectedObjects->count() == 0) {
        return;
    }

    m_editorLayer->ungroupStickyObjects(m_selectedObjects);
    updateButtons();
}

void REEditorUI::onUpdateDeleteFilter(CCObject* sender) {
    int tag = sender->getTag();

    auto gameManager = GameManager::sharedState();

    if (tag >= 0 && tag <= 2) {
        gameManager->setIntGameVariable("0005", tag);
    }
    else if (tag == 3) {
        auto obj = m_selectedObject;

        if (!obj) {
            gameManager->setIntGameVariable("0005", 0);
        } else {
            gameManager->setIntGameVariable("0005", 3);
            gameManager->setIntGameVariable("0006", obj->m_objectID);
        }
    }

    updateDeleteButtons();
}

void REEditorUI::orderDownCustomItem(CCObject* sender) {
    int i = GameManager::get()->reorderKey(m_selectedObjectIndex, false);

    if (i != m_selectedObjectIndex) {
        reloadCustomItems();
        m_selectedObjectIndex = i;
        updateCreateMenu(false);
    }
}

void REEditorUI::orderUpCustomItem(CCObject* sender) {
    int i = GameManager::get()->reorderKey(m_selectedObjectIndex, true);

    if (i != m_selectedObjectIndex) {
        reloadCustomItems();
        m_selectedObjectIndex = i;
        updateCreateMenu(false);
    }
}

CCArray* REEditorUI::pasteObjects(gd::string str, bool withColor, bool noUndo) {
    //todo
    return EditorUI::pasteObjects(str, withColor, noUndo);
}

void REEditorUI::playCircleAnim_(CCPoint position, float radius, float duration) {
    auto wave = CCCircleWave::create(1.f, radius, duration, false, false);
    wave->m_color = {255, 255, 255};
    wave->setPosition(position);

    addChild(wave, 0);
    wave->m_circleMode = CircleMode::Outline;
}

void REEditorUI::playerTouchBegan_(CCTouch* touch, CCEvent* event) {
    auto winSize = CCDirector::get()->getWinSize();
    auto touchLocation = convertToNodeSpace(touch->getLocation());

    if (m_editorLayer->m_gameState.m_isDualMode) {
        if (winSize.width * 0.5f < touchLocation.x) {
            if (m_playerTouchID2 == -1) {
                m_playerTouchID2 = touch->getID();
                m_editorLayer->queueButton(1, true, true, touch->getTimestamp());
            }
        }
    }
    else if (GameManager::get()->getGameVariable(GameVar::AlwaysLimitControls)) {
        if (m_playerTouchID2 == -1) {
            m_playerTouchID2 = touch->getID();
            m_editorLayer->queueButton(1, true, true, touch->getTimestamp());
        }
    }

    if (m_playerTouchID1 == -1) {
        m_playerTouchID1 = touch->getID();
        m_editorLayer->queueButton(1, true, false, touch->getTimestamp());
    }
}

void REEditorUI::playerTouchEnded_(CCTouch* touch, CCEvent* event) {
    if (touch->getID() == m_playerTouchID1) {
        m_playerTouchID1 = -1;
        m_editorLayer->queueButton(1, false, false, touch->getTimestamp());
    }
    else if (touch->getID() == m_playerTouchID2) {
        m_playerTouchID2 = -1;
        m_editorLayer->queueButton(1, false, true, touch->getTimestamp());
    }
}

void REEditorUI::playtestStopped() {
    showUI(true);
    m_playerTouchID1 = -1;
    m_playerTouchID2 = -1;

    auto spr = static_cast<CCSprite*>(m_playtestBtn->getNormalImage());

    auto scale = spr->getScale();
    auto width = spr->m_obRect.size.width;

    spr->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("GJ_playEditorBtn_001.png"));

    spr->setScale((scale * width) / spr->m_obRect.size.width);

    m_playtestStopBtn->setVisible(false);
    updatePlaybackBtn();
    updateZoom(m_editorZoom);
}

bool REEditorUI::positionIsInSnapped_(CCPoint position) {
    for (auto snapNode : CCArrayExt<CCNode, false>(m_snapPositions)) {
        auto& snapPos = snapNode->getPosition();
        if (snapPos.x == position.x && snapPos.y == position.y) return true;
    }
    return false;
}

CCPoint REEditorUI::positionWithoutOffset_(GameObject* object) {
    return object->getPosition() - getRelativeOffset_(object);
}

void REEditorUI::processSelectObjects(CCArray* objects) {
    //todo
    EditorUI::processSelectObjects(objects);
}

void REEditorUI::processSmartObjectsFromType(int type, CCArray* objects, CCArray* objects2, CCArray* objects3, CCArray* objects4) {
    //todo
    EditorUI::processSmartObjectsFromType(type, objects, objects2, objects3, objects4);
}

void REEditorUI::recreateButtonTabs_() {
    m_reloadItems = false;
    auto gameManager = GameManager::get();

    auto buttonsPerRow = gameManager->getIntGameVariable(GameVar::EditorButtonsPerRow);
    auto buttonRows = gameManager->getIntGameVariable(GameVar::EditorButtonRows);

    for (auto bar : CCArrayExt<EditButtonBar, false>(m_createButtonBars)) {
        bar->reloadItems(buttonsPerRow, buttonRows);
    }

    m_editButtonBar->reloadItems(buttonsPerRow, buttonRows);
}

void REEditorUI::redoLastAction(CCObject* sender) {
    //todo
    EditorUI::redoLastAction(sender);
}

void REEditorUI::reloadCustomItems() {
    //todo
    EditorUI::reloadCustomItems();
}

void REEditorUI::removeOffset(GameObject* object) {
    //todo
    EditorUI::removeOffset(object);
}

void REEditorUI::replaceGroupID(GameObject* object, int oldID, int newID) {
    //todo
    EditorUI::replaceGroupID(object, oldID, newID);
}

void REEditorUI::repositionObjectsToCenter(CCArray* objects, CCPoint position, bool absolute) {
    //todo
    EditorUI::repositionObjectsToCenter(objects, position, absolute);
}

void REEditorUI::resetObjectEditorValues_(CCArray* objects) {
    if (!objects) return;

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        obj->updateStartPos();
        m_objectEditorStates[obj->m_uniqueID].loadValues(obj);
    }
}

void REEditorUI::resetSelectedObjectsColor() {
    if (m_selectedObject) {
        m_selectedObject->selectObject({0, 255, 0});
        return;
    }

    for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
        obj->selectObject({0, 255, 0});
    }
}

void REEditorUI::resetUI() {
    //todo
    EditorUI::resetUI();
}

void REEditorUI::rotateObjects(CCArray* objects, float rotation, CCPoint pivotPoint) {
    //todo
    EditorUI::rotateObjects(objects, rotation, pivotPoint);
}

float REEditorUI::rotationforCommand_(EditCommand command) {
    switch (command) {
        case EditCommand::RotateCW: return 90.f;
        case EditCommand::RotateCCW: return -90.f;
        case EditCommand::RotateCW45: return 45.f;
        case EditCommand::RotateCCW45: return -45.f;
        default: return 0.f;
    }
}

void REEditorUI::scaleObjects(CCArray* objects, float scaleX, float scaleY, CCPoint pivotPoint, ObjectScaleType type, bool lockMove) {
    //todo
    EditorUI::scaleObjects(objects, scaleX, scaleY, pivotPoint, type, lockMove);
}

void REEditorUI::selectAll_() {
    auto selected = CCArray::create();
    for (auto obj : CCArrayExt<GameObject, false>(m_editorLayer->getAllObjects())) {
        if (m_editorLayer->validGroup(obj, false)) {
            selected->addObject(obj);
        }
    }

    processSelectObjects(selected);
    updateButtons();
    deactivateRotationControl_();
    deactivateScaleControl();
    deactivateTransformControl();
    updateObjectInfoLabel();
}

void REEditorUI::selectAllWithDirection(bool left) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto center = CCPoint{winSize.width * 0.5f, winSize.height * 0.5f + m_toolbarHeight * 0.5f};
    auto worldCenter = m_editorLayer->m_objectLayer->convertToNodeSpace(center);

    auto result = CCArray::create();

    for (auto obj : CCArrayExt<GameObject, false>(m_editorLayer->m_objects)) {
        if (!obj) continue;

        short objLayer2 = obj->m_editorLayer2;
        if (objLayer2 == 0) {
            objLayer2 = -1;
        }

        if (!(m_editorLayer->m_currentLayer == -1 || m_editorLayer->m_currentLayer == obj->m_editorLayer || m_editorLayer->m_currentLayer == objLayer2)) {
            continue;
        }

        float x = obj->getPosition().x;

        bool match = false;

        if (left) {
            match = !(worldCenter.x < x) && worldCenter.x != x;
        }
        else {
            match = !(x < worldCenter.x) && worldCenter.x != x;
        }

        if (match) {
            result->addObject(obj);
        }
    }

    processSelectObjects(result);
    updateButtons();

    deactivateRotationControl_();
    deactivateScaleControl();
    deactivateTransformControl();
    updateObjectInfoLabel();
}

void REEditorUI::selectBuildTab(int tabIndex) {
    if (m_selectedTab == tabIndex || (m_selectedMode != 2 && m_selectedTab != -1)) {
        return;
    }

    if (tabIndex < 0) {
        tabIndex = 0;
    }

    if (tabIndex >= m_createButtonBars->count() - 1) {
        if (m_createButtonBars->count() == 1) {
            tabIndex = 0;
        } else {
            tabIndex = m_createButtonBars->count() - 1;
        }
    }

    m_selectedTab = tabIndex;

    int i = 0;
    for (auto tab : CCArrayExt<CCNode, false>(m_createButtonBars)) {
        bool isSelected = (i == m_selectedTab);
        tab->setVisible(isSelected);

        if (isSelected) {
            m_createButtonBar = static_cast<EditButtonBar*>(tab);
        }
        i++;
    }

    i = 0;
    for (auto toggler : CCArrayExt<CCMenuItemToggler, false>(m_tabsArray)) {
        toggler->toggle(i == m_selectedTab);
        i++;
    }
}

void REEditorUI::selectObject(GameObject* object, bool ignoreFilter) {
    if (!canSelectObject(object)) return;

    if (m_stickyControlsEnabled && !m_linkControlsDisabled && object->m_linkedGroup > 0) {
        auto dict = m_editorLayer->m_linkedGroupDict;
        auto group = static_cast<CCArray*>(dict->objectForKey(object->m_linkedGroup));

        if (group && group->count() > 1) {
            deselectAll();

            auto single = CCArray::createWithObject(object);
            selectObjects(single, false);
            return;
        }
    }

    m_snapSelected = true;

    deselectAll();

    m_selectedObject = object;

    object->selectObject({0, 255, 0});
    object->m_unk460 = m_cycledObjectIndex;
    // (MAYBE) FIX BINDINGS, rename GameObject::m_unk460 to GameObject::m_cycledObjectIndex, potentially wrong decomp, please verify, (why is it float in GameObject)

    m_canActivateControls = true;

    checkLiveColorSelect();
}

void REEditorUI::selectObjects(CCArray* objects, bool ignoreFilter) {
    //todo
    EditorUI::selectObjects(objects, ignoreFilter);
}

void REEditorUI::selectObjectsInRect_(CCRect rect) {
    auto objects = m_editorLayer->objectsInRect(rect, false);
    if (objects->count() > 0) {
        for (auto obj : CCArrayExt<GameObject, false>(objects)) {
            if ((m_selectedObject && m_selectedObject->m_uniqueID != obj->m_uniqueID) || !m_selectedObjects->containsObject(obj)) {
                createUndoSelectObject(false);
                break;
            }
        }
        selectObjects(objects, false);
        updateSpecialUIElements_();
    }
}

void REEditorUI::setupCreateMenu() {
    m_selectedTab = -1;
    m_selectedObjectIndex = 0;

    auto gameManager = GameManager::get();

    auto winSize = CCDirector::get()->getWinSize();
    m_createButtonArray = CCArray::create();
    m_createButtonArray->retain();

    m_customObjectButtonArray = CCArray::create();
    m_customObjectButtonArray->retain();

    m_createButtonBars = CCArray::create();
    m_createButtonBars->retain();

    m_tabsArray = CCArray::create();
    m_tabsArray->retain();

    m_tabsMenu = CCMenu::create();
    addChild(m_tabsMenu, 100);

    int buttonsPerRow = gameManager->getIntGameVariable(GameVar::EditorButtonsPerRow);
    int buttonRows = gameManager->getIntGameVariable(GameVar::EditorButtonRows);

    if (buttonsPerRow < 6) {
        buttonsPerRow = 6;
        gameManager->setIntGameVariable(GameVar::EditorButtonsPerRow, 6);
    }

    if (buttonRows < 2) {
        buttonRows = 2;
        gameManager->setIntGameVariable(GameVar::EditorButtonRows, 2);
    }

    auto iconTextures = CCArray::create();
    auto objects = CCArray::create();

    auto screenBottom = CCDirector::get()->getScreenBottom();
    auto point = CCPoint{winSize.width / 2 - 5.f, screenBottom + m_toolbarHeight - 6.f};

    iconTextures->addObject(CCString::create("square_01_001.png"));

    objects->addObject(getCreateBtn(1, 4));
    objects->addObject(getCreateBtn(83, 4));
    objects->addObject(getCreateBtn(2, 4));
    objects->addObject(getCreateBtn(3, 4));
    objects->addObject(getCreateBtn(4, 4));
    objects->addObject(getCreateBtn(5, 4));
    objects->addObject(getCreateBtn(502, 4));
    objects->addObject(getCreateBtn(6, 4));
    objects->addObject(getCreateBtn(7, 4));
    objects->addObject(getCreateBtn(69, 4));
    objects->addObject(getCreateBtn(70, 4));
    objects->addObject(getCreateBtn(71, 4));
    objects->addObject(getCreateBtn(72, 4));
    objects->addObject(getCreateBtn(73, 4));
    objects->addObject(getCreateBtn(74, 4));
    objects->addObject(getCreateBtn(75, 4));
    objects->addObject(getCreateBtn(76, 4));
    objects->addObject(getCreateBtn(77, 4));
    objects->addObject(getCreateBtn(78, 4));
    objects->addObject(getCreateBtn(80, 4));
    objects->addObject(getCreateBtn(81, 4));
    objects->addObject(getCreateBtn(82, 4));
    objects->addObject(getCreateBtn(90, 4));
    objects->addObject(getCreateBtn(91, 4));
    objects->addObject(getCreateBtn(92, 4));
    objects->addObject(getCreateBtn(93, 4));
    objects->addObject(getCreateBtn(94, 4));
    objects->addObject(getCreateBtn(95, 4));
    objects->addObject(getCreateBtn(96, 4));
    objects->addObject(getCreateBtn(116, 4));
    objects->addObject(getCreateBtn(117, 4));
    objects->addObject(getCreateBtn(118, 4));
    objects->addObject(getCreateBtn(119, 4));
    objects->addObject(getCreateBtn(120, 4));
    objects->addObject(getCreateBtn(121, 4));
    objects->addObject(getCreateBtn(122, 4));
    objects->addObject(getCreateBtn(207, 4));
    objects->addObject(getCreateBtn(208, 4));
    objects->addObject(getCreateBtn(209, 4));
    objects->addObject(getCreateBtn(210, 4));
    objects->addObject(getCreateBtn(211, 4));
    objects->addObject(getCreateBtn(212, 4));
    objects->addObject(getCreateBtn(213, 4));
    objects->addObject(getCreateBtn(1820, 4));
    objects->addObject(getCreateBtn(1821, 4));
    objects->addObject(getCreateBtn(1823, 4));
    objects->addObject(getCreateBtn(1824, 4));
    objects->addObject(getCreateBtn(1825, 4));
    objects->addObject(getCreateBtn(1826, 4));
    objects->addObject(getCreateBtn(1827, 4));
    objects->addObject(getCreateBtn(1828, 4));
    objects->addObject(getCreateBtn(255, 4));
    objects->addObject(getCreateBtn(256, 4));
    objects->addObject(getCreateBtn(257, 4));
    objects->addObject(getCreateBtn(258, 4));
    objects->addObject(getCreateBtn(259, 4));
    objects->addObject(getCreateBtn(260, 4));
    objects->addObject(getCreateBtn(261, 4));
    objects->addObject(getCreateBtn(263, 4));
    objects->addObject(getCreateBtn(264, 4));
    objects->addObject(getCreateBtn(265, 4));
    objects->addObject(getCreateBtn(266, 4));
    objects->addObject(getCreateBtn(267, 4));
    objects->addObject(getCreateBtn(268, 4));
    objects->addObject(getCreateBtn(269, 4));
    objects->addObject(getCreateBtn(270, 4));
    objects->addObject(getCreateBtn(271, 4));
    objects->addObject(getCreateBtn(272, 4));
    objects->addObject(getCreateBtn(273, 4));
    objects->addObject(getCreateBtn(274, 4));
    objects->addObject(getCreateBtn(275, 4));
    objects->addObject(getCreateBtn(1142, 4));
    objects->addObject(getCreateBtn(1140, 4));
    objects->addObject(getCreateBtn(1141, 4));
    objects->addObject(getCreateBtn(1143, 4));
    objects->addObject(getCreateBtn(1144, 4));
    objects->addObject(getCreateBtn(1149, 4));
    objects->addObject(getCreateBtn(1145, 4));
    objects->addObject(getCreateBtn(1146, 4));
    objects->addObject(getCreateBtn(1147, 4));
    objects->addObject(getCreateBtn(1153, 4));
    objects->addObject(getCreateBtn(1148, 4));
    objects->addObject(getCreateBtn(1150, 4));
    objects->addObject(getCreateBtn(1151, 4));
    objects->addObject(getCreateBtn(1152, 4));
    objects->addObject(getCreateBtn(476, 4));
    objects->addObject(getCreateBtn(477, 4));
    objects->addObject(getCreateBtn(478, 4));
    objects->addObject(getCreateBtn(479, 4));
    objects->addObject(getCreateBtn(480, 4));
    objects->addObject(getCreateBtn(481, 4));
    objects->addObject(getCreateBtn(482, 4));
    objects->addObject(getCreateBtn(485, 4));
    objects->addObject(getCreateBtn(486, 4));
    objects->addObject(getCreateBtn(487, 4));
    objects->addObject(getCreateBtn(488, 4));
    objects->addObject(getCreateBtn(489, 4));
    objects->addObject(getCreateBtn(490, 4));
    objects->addObject(getCreateBtn(491, 4));
    objects->addObject(getCreateBtn(644, 4));
    objects->addObject(getCreateBtn(650, 4));
    objects->addObject(getCreateBtn(641, 4));
    objects->addObject(getCreateBtn(1075, 4));
    objects->addObject(getCreateBtn(646, 4));
    objects->addObject(getCreateBtn(642, 4));
    objects->addObject(getCreateBtn(739, 4));
    objects->addObject(getCreateBtn(643, 4));
    objects->addObject(getCreateBtn(645, 4));
    objects->addObject(getCreateBtn(647, 4));
    objects->addObject(getCreateBtn(648, 4));
    objects->addObject(getCreateBtn(649, 4));
    objects->addObject(getCreateBtn(1077, 4));
    objects->addObject(getCreateBtn(1298, 4));
    objects->addObject(getCreateBtn(1076, 4));
    objects->addObject(getCreateBtn(658, 4));
    objects->addObject(getCreateBtn(722, 4));
    objects->addObject(getCreateBtn(659, 4));
    objects->addObject(getCreateBtn(734, 4));
    objects->addObject(getCreateBtn(752, 4));
    objects->addObject(getCreateBtn(753, 4));
    objects->addObject(getCreateBtn(754, 4));
    objects->addObject(getCreateBtn(755, 4));
    objects->addObject(getCreateBtn(756, 4));
    objects->addObject(getCreateBtn(757, 4));
    objects->addObject(getCreateBtn(758, 4));
    objects->addObject(getCreateBtn(759, 4));
    objects->addObject(getCreateBtn(903, 4));
    objects->addObject(getCreateBtn(904, 4));
    objects->addObject(getCreateBtn(905, 4));
    objects->addObject(getCreateBtn(911, 4));
    objects->addObject(getCreateBtn(1045, 4));
    objects->addObject(getCreateBtn(1046, 4));
    objects->addObject(getCreateBtn(1294, 4));
    objects->addObject(getCreateBtn(1295, 4));
    objects->addObject(getCreateBtn(952, 4));
    objects->addObject(getCreateBtn(953, 4));
    objects->addObject(getCreateBtn(954, 4));
    objects->addObject(getCreateBtn(955, 4));
    objects->addObject(getCreateBtn(956, 4));
    objects->addObject(getCreateBtn(957, 4));
    objects->addObject(getCreateBtn(958, 4));
    objects->addObject(getCreateBtn(959, 4));
    objects->addObject(getCreateBtn(974, 4));
    objects->addObject(getCreateBtn(975, 4));
    objects->addObject(getCreateBtn(976, 4));
    objects->addObject(getCreateBtn(977, 4));
    objects->addObject(getCreateBtn(1047, 4));
    objects->addObject(getCreateBtn(1048, 4));
    objects->addObject(getCreateBtn(1296, 4));
    objects->addObject(getCreateBtn(1297, 4));
    objects->addObject(getCreateBtn(1299, 4));
    objects->addObject(getCreateBtn(1300, 4));
    objects->addObject(getCreateBtn(1301, 4));
    objects->addObject(getCreateBtn(1302, 4));
    objects->addObject(getCreateBtn(1303, 4));
    objects->addObject(getCreateBtn(1304, 4));
    objects->addObject(getCreateBtn(1162, 4));
    objects->addObject(getCreateBtn(1163, 4));
    objects->addObject(getCreateBtn(1164, 4));
    objects->addObject(getCreateBtn(1165, 4));
    objects->addObject(getCreateBtn(1166, 4));
    objects->addObject(getCreateBtn(1167, 4));
    objects->addObject(getCreateBtn(1168, 4));
    objects->addObject(getCreateBtn(1169, 4));
    objects->addObject(getCreateBtn(1170, 4));
    objects->addObject(getCreateBtn(1171, 4));
    objects->addObject(getCreateBtn(1172, 4));
    objects->addObject(getCreateBtn(1173, 4));
    objects->addObject(getCreateBtn(1174, 4));
    objects->addObject(getCreateBtn(1175, 4));
    objects->addObject(getCreateBtn(1176, 4));
    objects->addObject(getCreateBtn(1177, 4));
    objects->addObject(getCreateBtn(1178, 4));
    objects->addObject(getCreateBtn(1179, 4));
    objects->addObject(getCreateBtn(1180, 4));
    objects->addObject(getCreateBtn(1181, 4));
    objects->addObject(getCreateBtn(1182, 4));
    objects->addObject(getCreateBtn(1183, 4));
    objects->addObject(getCreateBtn(1184, 4));
    objects->addObject(getCreateBtn(1185, 4));
    objects->addObject(getCreateBtn(1186, 4));
    objects->addObject(getCreateBtn(1322, 4));
    objects->addObject(getCreateBtn(1191, 4));
    objects->addObject(getCreateBtn(1192, 4));
    objects->addObject(getCreateBtn(1193, 4));
    objects->addObject(getCreateBtn(1197, 4));
    objects->addObject(getCreateBtn(1196, 4));
    objects->addObject(getCreateBtn(1194, 4));
    objects->addObject(getCreateBtn(1195, 4));
    objects->addObject(getCreateBtn(807, 4));
    objects->addObject(getCreateBtn(808, 4));
    objects->addObject(getCreateBtn(809, 4));
    objects->addObject(getCreateBtn(810, 4));
    objects->addObject(getCreateBtn(811, 4));
    objects->addObject(getCreateBtn(812, 4));
    objects->addObject(getCreateBtn(813, 4));
    objects->addObject(getCreateBtn(814, 4));
    objects->addObject(getCreateBtn(815, 4));
    objects->addObject(getCreateBtn(816, 4));
    objects->addObject(getCreateBtn(817, 4));
    objects->addObject(getCreateBtn(818, 4));
    objects->addObject(getCreateBtn(819, 4));
    objects->addObject(getCreateBtn(820, 4));
    objects->addObject(getCreateBtn(821, 4));
    objects->addObject(getCreateBtn(822, 4));
    objects->addObject(getCreateBtn(823, 4));
    objects->addObject(getCreateBtn(824, 4));
    objects->addObject(getCreateBtn(825, 4));
    objects->addObject(getCreateBtn(1082, 4));
    objects->addObject(getCreateBtn(1100, 4));
    objects->addObject(getCreateBtn(1083, 4));
    objects->addObject(getCreateBtn(1101, 4));
    objects->addObject(getCreateBtn(1084, 4));
    objects->addObject(getCreateBtn(1102, 4));
    objects->addObject(getCreateBtn(1085, 4));
    objects->addObject(getCreateBtn(1103, 4));
    objects->addObject(getCreateBtn(1086, 4));
    objects->addObject(getCreateBtn(1104, 4));
    objects->addObject(getCreateBtn(1088, 4));
    objects->addObject(getCreateBtn(1105, 4));
    objects->addObject(getCreateBtn(1089, 4));
    objects->addObject(getCreateBtn(1106, 4));
    objects->addObject(getCreateBtn(1087, 4));
    objects->addObject(getCreateBtn(1090, 4));
    objects->addObject(getCreateBtn(1099, 4));
    objects->addObject(getCreateBtn(841, 4));
    objects->addObject(getCreateBtn(1112, 4));
    objects->addObject(getCreateBtn(842, 4));
    objects->addObject(getCreateBtn(1113, 4));
    objects->addObject(getCreateBtn(843, 4));
    objects->addObject(getCreateBtn(844, 4));
    objects->addObject(getCreateBtn(1114, 4));
    objects->addObject(getCreateBtn(845, 4));
    objects->addObject(getCreateBtn(1115, 4));
    objects->addObject(getCreateBtn(846, 4));
    objects->addObject(getCreateBtn(1116, 4));
    objects->addObject(getCreateBtn(847, 4));
    objects->addObject(getCreateBtn(1117, 4));
    objects->addObject(getCreateBtn(848, 4));
    objects->addObject(getCreateBtn(1118, 4));
    objects->addObject(getCreateBtn(867, 4));
    objects->addObject(getCreateBtn(868, 4));
    objects->addObject(getCreateBtn(869, 4));
    objects->addObject(getCreateBtn(870, 4));
    objects->addObject(getCreateBtn(871, 4));
    objects->addObject(getCreateBtn(872, 4));
    objects->addObject(getCreateBtn(1266, 4));
    objects->addObject(getCreateBtn(1267, 4));
    objects->addObject(getCreateBtn(873, 4));
    objects->addObject(getCreateBtn(874, 4));
    objects->addObject(getCreateBtn(880, 4));
    objects->addObject(getCreateBtn(881, 4));
    objects->addObject(getCreateBtn(882, 4));
    objects->addObject(getCreateBtn(883, 4));
    objects->addObject(getCreateBtn(884, 4));
    objects->addObject(getCreateBtn(885, 4));
    objects->addObject(getCreateBtn(1062, 4));
    objects->addObject(getCreateBtn(1159, 4));
    objects->addObject(getCreateBtn(1160, 4));
    objects->addObject(getCreateBtn(1161, 4));
    objects->addObject(getCreateBtn(890, 4));
    objects->addObject(getCreateBtn(1247, 4));
    objects->addObject(getCreateBtn(1277, 4));
    objects->addObject(getCreateBtn(1278, 4));
    objects->addObject(getCreateBtn(1248, 4));
    objects->addObject(getCreateBtn(1279, 4));
    objects->addObject(getCreateBtn(1249, 4));
    objects->addObject(getCreateBtn(1280, 4));
    objects->addObject(getCreateBtn(1250, 4));
    objects->addObject(getCreateBtn(1281, 4));
    objects->addObject(getCreateBtn(1251, 4));
    objects->addObject(getCreateBtn(1252, 4));
    objects->addObject(getCreateBtn(1253, 4));
    objects->addObject(getCreateBtn(1282, 4));
    objects->addObject(getCreateBtn(1254, 4));
    objects->addObject(getCreateBtn(1283, 4));
    objects->addObject(getCreateBtn(1255, 4));
    objects->addObject(getCreateBtn(1284, 4));
    objects->addObject(getCreateBtn(1285, 4));
    objects->addObject(getCreateBtn(1286, 4));
    objects->addObject(getCreateBtn(1287, 4));
    objects->addObject(getCreateBtn(1288, 4));
    objects->addObject(getCreateBtn(1289, 4));
    objects->addObject(getCreateBtn(1290, 4));
    objects->addObject(getCreateBtn(927, 4));
    objects->addObject(getCreateBtn(928, 4));
    objects->addObject(getCreateBtn(929, 4));
    objects->addObject(getCreateBtn(930, 4));
    objects->addObject(getCreateBtn(931, 4));
    objects->addObject(getCreateBtn(932, 4));
    objects->addObject(getCreateBtn(933, 4));
    objects->addObject(getCreateBtn(934, 4));
    objects->addObject(getCreateBtn(935, 4));
    objects->addObject(getCreateBtn(1229, 4));
    objects->addObject(getCreateBtn(1230, 4));
    objects->addObject(getCreateBtn(1238, 4));
    objects->addObject(getCreateBtn(1231, 4));
    objects->addObject(getCreateBtn(1232, 4));
    objects->addObject(getCreateBtn(1233, 4));
    objects->addObject(getCreateBtn(1234, 4));
    objects->addObject(getCreateBtn(1237, 4));
    objects->addObject(getCreateBtn(1235, 4));
    objects->addObject(getCreateBtn(1236, 4));
    objects->addObject(getCreateBtn(1239, 4));
    objects->addObject(getCreateBtn(1240, 4));
    objects->addObject(getCreateBtn(1348, 4));
    objects->addObject(getCreateBtn(1352, 4));
    objects->addObject(getCreateBtn(1349, 4));
    objects->addObject(getCreateBtn(1353, 4));
    objects->addObject(getCreateBtn(1350, 4));
    objects->addObject(getCreateBtn(1354, 4));
    objects->addObject(getCreateBtn(1351, 4));
    objects->addObject(getCreateBtn(1355, 4));
    objects->addObject(getCreateBtn(1395, 4));
    objects->addObject(getCreateBtn(1356, 4));
    objects->addObject(getCreateBtn(1357, 4));
    objects->addObject(getCreateBtn(1358, 4));
    objects->addObject(getCreateBtn(1359, 4));
    objects->addObject(getCreateBtn(1360, 4));
    objects->addObject(getCreateBtn(1361, 4));
    objects->addObject(getCreateBtn(1362, 4));
    objects->addObject(getCreateBtn(1363, 4));
    objects->addObject(getCreateBtn(1364, 4));
    objects->addObject(getCreateBtn(1365, 4));
    objects->addObject(getCreateBtn(1366, 4));
    objects->addObject(getCreateBtn(1387, 4));
    objects->addObject(getCreateBtn(1388, 4));
    objects->addObject(getCreateBtn(1389, 4));
    objects->addObject(getCreateBtn(1390, 4));
    objects->addObject(getCreateBtn(1391, 4));
    objects->addObject(getCreateBtn(1392, 4));
    objects->addObject(getCreateBtn(1393, 4));
    objects->addObject(getCreateBtn(1394, 4));
    objects->addObject(getCreateBtn(1367, 4));
    objects->addObject(getCreateBtn(1368, 4));
    objects->addObject(getCreateBtn(1369, 4));
    objects->addObject(getCreateBtn(1370, 4));
    objects->addObject(getCreateBtn(1371, 4));
    objects->addObject(getCreateBtn(1372, 4));
    objects->addObject(getCreateBtn(1373, 4));
    objects->addObject(getCreateBtn(1374, 4));
    objects->addObject(getCreateBtn(1375, 4));
    objects->addObject(getCreateBtn(1376, 4));
    objects->addObject(getCreateBtn(1377, 4));
    objects->addObject(getCreateBtn(1378, 4));
    objects->addObject(getCreateBtn(1379, 4));
    objects->addObject(getCreateBtn(1380, 4));
    objects->addObject(getCreateBtn(1381, 4));
    objects->addObject(getCreateBtn(1382, 4));
    objects->addObject(getCreateBtn(1383, 4));
    objects->addObject(getCreateBtn(1384, 4));
    objects->addObject(getCreateBtn(1385, 4));
    objects->addObject(getCreateBtn(1386, 4));
    objects->addObject(getCreateBtn(1431, 4));
    objects->addObject(getCreateBtn(1442, 4));
    objects->addObject(getCreateBtn(1432, 4));
    objects->addObject(getCreateBtn(1443, 4));
    objects->addObject(getCreateBtn(1433, 4));
    objects->addObject(getCreateBtn(1444, 4));
    objects->addObject(getCreateBtn(1434, 4));
    objects->addObject(getCreateBtn(1445, 4));
    objects->addObject(getCreateBtn(1435, 4));
    objects->addObject(getCreateBtn(1446, 4));
    objects->addObject(getCreateBtn(1436, 4));
    objects->addObject(getCreateBtn(1447, 4));
    objects->addObject(getCreateBtn(1437, 4));
    objects->addObject(getCreateBtn(1448, 4));
    objects->addObject(getCreateBtn(1438, 4));
    objects->addObject(getCreateBtn(1449, 4));
    objects->addObject(getCreateBtn(1439, 4));
    objects->addObject(getCreateBtn(1450, 4));
    objects->addObject(getCreateBtn(1440, 4));
    objects->addObject(getCreateBtn(1451, 4));
    objects->addObject(getCreateBtn(1441, 4));
    objects->addObject(getCreateBtn(1452, 4));
    objects->addObject(getCreateBtn(1510, 4));
    objects->addObject(getCreateBtn(1513, 4));
    objects->addObject(getCreateBtn(1511, 4));
    objects->addObject(getCreateBtn(1514, 4));
    objects->addObject(getCreateBtn(1512, 4));
    objects->addObject(getCreateBtn(1515, 4));
    objects->addObject(getCreateBtn(1453, 4));
    objects->addObject(getCreateBtn(1454, 4));
    objects->addObject(getCreateBtn(1455, 4));
    objects->addObject(getCreateBtn(1456, 4));
    objects->addObject(getCreateBtn(1457, 4));
    objects->addObject(getCreateBtn(1458, 4));
    objects->addObject(getCreateBtn(1459, 4));
    objects->addObject(getCreateBtn(1460, 4));
    objects->addObject(getCreateBtn(1461, 4));
    objects->addObject(getCreateBtn(1462, 4));
    objects->addObject(getCreateBtn(1463, 4));
    objects->addObject(getCreateBtn(1464, 4));
    objects->addObject(getCreateBtn(1621, 4));
    objects->addObject(getCreateBtn(1622, 4));
    objects->addObject(getCreateBtn(1623, 4));
    objects->addObject(getCreateBtn(1624, 4));
    objects->addObject(getCreateBtn(1625, 4));
    objects->addObject(getCreateBtn(1626, 4));
    objects->addObject(getCreateBtn(1627, 4));
    objects->addObject(getCreateBtn(1628, 4));
    objects->addObject(getCreateBtn(1629, 4));
    objects->addObject(getCreateBtn(1630, 4));
    objects->addObject(getCreateBtn(1631, 4));
    objects->addObject(getCreateBtn(1632, 4));
    objects->addObject(getCreateBtn(1633, 4));
    objects->addObject(getCreateBtn(1634, 4));
    objects->addObject(getCreateBtn(1635, 4));
    objects->addObject(getCreateBtn(1636, 4));
    objects->addObject(getCreateBtn(1637, 4));
    objects->addObject(getCreateBtn(1638, 4));
    objects->addObject(getCreateBtn(1639, 4));
    objects->addObject(getCreateBtn(1640, 4));
    objects->addObject(getCreateBtn(1641, 4));
    objects->addObject(getCreateBtn(1642, 4));
    objects->addObject(getCreateBtn(1643, 4));
    objects->addObject(getCreateBtn(1644, 4));
    objects->addObject(getCreateBtn(1645, 4));
    objects->addObject(getCreateBtn(1646, 4));
    objects->addObject(getCreateBtn(1647, 4));
    objects->addObject(getCreateBtn(1648, 4));
    objects->addObject(getCreateBtn(1649, 4));
    objects->addObject(getCreateBtn(1650, 4));
    objects->addObject(getCreateBtn(1651, 4));
    objects->addObject(getCreateBtn(1652, 4));
    objects->addObject(getCreateBtn(1471, 4));
    objects->addObject(getCreateBtn(1507, 4));
    objects->addObject(getCreateBtn(1617, 4));
    objects->addObject(getCreateBtn(1472, 4));
    objects->addObject(getCreateBtn(1473, 4));
    objects->addObject(getCreateBtn(1496, 4));
    objects->addObject(getCreateBtn(1653, 4));
    objects->addObject(getCreateBtn(1654, 4));
    objects->addObject(getCreateBtn(1655, 4));
    objects->addObject(getCreateBtn(1656, 4));
    objects->addObject(getCreateBtn(1657, 4));
    objects->addObject(getCreateBtn(1658, 4));
    objects->addObject(getCreateBtn(1659, 4));
    objects->addObject(getCreateBtn(1660, 4));
    objects->addObject(getCreateBtn(1661, 4));
    objects->addObject(getCreateBtn(1662, 4));
    objects->addObject(getCreateBtn(1663, 4));
    objects->addObject(getCreateBtn(1664, 4));
    objects->addObject(getCreateBtn(1665, 4));
    objects->addObject(getCreateBtn(1666, 4));
    objects->addObject(getCreateBtn(1667, 4));
    objects->addObject(getCreateBtn(1668, 4));
    objects->addObject(getCreateBtn(1669, 4));
    objects->addObject(getCreateBtn(1670, 4));
    objects->addObject(getCreateBtn(1671, 4));
    objects->addObject(getCreateBtn(1672, 4));
    objects->addObject(getCreateBtn(1673, 4));
    objects->addObject(getCreateBtn(1674, 4));
    objects->addObject(getCreateBtn(1675, 4));
    objects->addObject(getCreateBtn(1676, 4));
    objects->addObject(getCreateBtn(1677, 4));
    objects->addObject(getCreateBtn(1678, 4));
    objects->addObject(getCreateBtn(1679, 4));
    objects->addObject(getCreateBtn(1680, 4));
    objects->addObject(getCreateBtn(1681, 4));
    objects->addObject(getCreateBtn(1682, 4));
    objects->addObject(getCreateBtn(1683, 4));
    objects->addObject(getCreateBtn(1684, 4));
    objects->addObject(getCreateBtn(1685, 4));
    objects->addObject(getCreateBtn(1686, 4));
    objects->addObject(getCreateBtn(1687, 4));
    objects->addObject(getCreateBtn(1688, 4));
    objects->addObject(getCreateBtn(1689, 4));
    objects->addObject(getCreateBtn(1690, 4));
    objects->addObject(getCreateBtn(1691, 4));
    objects->addObject(getCreateBtn(1692, 4));
    objects->addObject(getCreateBtn(1693, 4));
    objects->addObject(getCreateBtn(1694, 4));
    objects->addObject(getCreateBtn(1695, 4));
    objects->addObject(getCreateBtn(1696, 4));
    objects->addObject(getCreateBtn(1769, 4));
    objects->addObject(getCreateBtn(1772, 4));
    objects->addObject(getCreateBtn(1770, 4));
    objects->addObject(getCreateBtn(1771, 4));
    objects->addObject(getCreateBtn(1773, 4));
    objects->addObject(getCreateBtn(1774, 4));
    objects->addObject(getCreateBtn(1775, 4));
    objects->addObject(getCreateBtn(1776, 4));
    objects->addObject(getCreateBtn(1777, 4));
    objects->addObject(getCreateBtn(1781, 4));
    objects->addObject(getCreateBtn(1780, 4));
    objects->addObject(getCreateBtn(1784, 4));
    objects->addObject(getCreateBtn(1778, 4));
    objects->addObject(getCreateBtn(1782, 4));
    objects->addObject(getCreateBtn(1779, 4));
    objects->addObject(getCreateBtn(1783, 4));
    objects->addObject(getCreateBtn(1785, 4));
    objects->addObject(getCreateBtn(1789, 4));
    objects->addObject(getCreateBtn(1786, 4));
    objects->addObject(getCreateBtn(1790, 4));
    objects->addObject(getCreateBtn(1787, 4));
    objects->addObject(getCreateBtn(1791, 4));
    objects->addObject(getCreateBtn(1788, 4));
    objects->addObject(getCreateBtn(1792, 4));
    objects->addObject(getCreateBtn(1793, 4));
    objects->addObject(getCreateBtn(1799, 4));
    objects->addObject(getCreateBtn(1803, 4));
    objects->addObject(getCreateBtn(1805, 4));
    objects->addObject(getCreateBtn(1794, 4));
    objects->addObject(getCreateBtn(1800, 4));
    objects->addObject(getCreateBtn(1804, 4));
    objects->addObject(getCreateBtn(1806, 4));
    objects->addObject(getCreateBtn(1795, 4));
    objects->addObject(getCreateBtn(1801, 4));
    objects->addObject(getCreateBtn(1807, 4));
    objects->addObject(getCreateBtn(1809, 4));
    objects->addObject(getCreateBtn(1796, 4));
    objects->addObject(getCreateBtn(1802, 4));
    objects->addObject(getCreateBtn(1808, 4));
    objects->addObject(getCreateBtn(1810, 4));
    objects->addObject(getCreateBtn(1797, 4));
    objects->addObject(getCreateBtn(1798, 4));
    objects->addObject(getCreateBtn(1861, 4));
    objects->addObject(getCreateBtn(1862, 4));
    objects->addObject(getCreateBtn(1863, 4));
    objects->addObject(getCreateBtn(1864, 4));
    objects->addObject(getCreateBtn(1865, 4));
    objects->addObject(getCreateBtn(1866, 4));
    objects->addObject(getCreateBtn(1867, 4));
    objects->addObject(getCreateBtn(1868, 4));
    objects->addObject(getCreateBtn(1869, 4));
    objects->addObject(getCreateBtn(1870, 4));
    objects->addObject(getCreateBtn(1871, 4));
    objects->addObject(getCreateBtn(1872, 4));
    objects->addObject(getCreateBtn(1873, 4));
    objects->addObject(getCreateBtn(1874, 4));
    objects->addObject(getCreateBtn(1875, 4));
    objects->addObject(getCreateBtn(1876, 4));
    objects->addObject(getCreateBtn(1877, 4));
    objects->addObject(getCreateBtn(1878, 4));
    objects->addObject(getCreateBtn(1879, 4));
    objects->addObject(getCreateBtn(1880, 4));
    objects->addObject(getCreateBtn(1881, 4));
    objects->addObject(getCreateBtn(1882, 4));
    objects->addObject(getCreateBtn(1883, 4));
    objects->addObject(getCreateBtn(1884, 4));
    objects->addObject(getCreateBtn(1885, 4));
    objects->addObject(getCreateBtn(2703, 4));
    objects->addObject(getCreateBtn(2704, 4));
    objects->addObject(getCreateBtn(2708, 4));
    objects->addObject(getCreateBtn(2709, 4));
    objects->addObject(getCreateBtn(2710, 4));
    objects->addObject(getCreateBtn(2711, 4));
    objects->addObject(getCreateBtn(2712, 4));
    objects->addObject(getCreateBtn(2713, 4));
    objects->addObject(getCreateBtn(2714, 4));
    objects->addObject(getCreateBtn(2715, 4));
    objects->addObject(getCreateBtn(2716, 4));
    objects->addObject(getCreateBtn(2717, 4));
    objects->addObject(getCreateBtn(2718, 4));
    objects->addObject(getCreateBtn(2719, 4));
    objects->addObject(getCreateBtn(2720, 4));
    objects->addObject(getCreateBtn(2721, 4));
    objects->addObject(getCreateBtn(2722, 4));
    objects->addObject(getCreateBtn(2723, 4));
    objects->addObject(getCreateBtn(2724, 4));
    objects->addObject(getCreateBtn(2725, 4));
    objects->addObject(getCreateBtn(2726, 4));
    objects->addObject(getCreateBtn(2727, 4));
    objects->addObject(getCreateBtn(2728, 4));
    objects->addObject(getCreateBtn(2729, 4));
    objects->addObject(getCreateBtn(2730, 4));
    objects->addObject(getCreateBtn(2731, 4));
    objects->addObject(getCreateBtn(2732, 4));
    objects->addObject(getCreateBtn(2733, 4));
    objects->addObject(getCreateBtn(2734, 4));
    objects->addObject(getCreateBtn(2735, 4));
    objects->addObject(getCreateBtn(2736, 4));
    objects->addObject(getCreateBtn(2737, 4));
    objects->addObject(getCreateBtn(2738, 4));
    objects->addObject(getCreateBtn(2739, 4));
    objects->addObject(getCreateBtn(2740, 4));
    objects->addObject(getCreateBtn(2741, 4));
    objects->addObject(getCreateBtn(2742, 4));
    objects->addObject(getCreateBtn(2743, 4));
    objects->addObject(getCreateBtn(2744, 4));
    objects->addObject(getCreateBtn(2745, 4));
    objects->addObject(getCreateBtn(2746, 4));
    objects->addObject(getCreateBtn(2747, 4));
    objects->addObject(getCreateBtn(2748, 4));
    objects->addObject(getCreateBtn(2749, 4));
    objects->addObject(getCreateBtn(2750, 4));
    objects->addObject(getCreateBtn(2751, 4));
    objects->addObject(getCreateBtn(2752, 4));
    objects->addObject(getCreateBtn(2753, 4));
    objects->addObject(getCreateBtn(2754, 4));
    objects->addObject(getCreateBtn(2755, 4));
    objects->addObject(getCreateBtn(2756, 4));
    objects->addObject(getCreateBtn(2757, 4));
    objects->addObject(getCreateBtn(2758, 4));
    objects->addObject(getCreateBtn(2759, 4));
    objects->addObject(getCreateBtn(2760, 4));
    objects->addObject(getCreateBtn(2761, 4));
    objects->addObject(getCreateBtn(2762, 4));
    objects->addObject(getCreateBtn(2763, 4));
    objects->addObject(getCreateBtn(2764, 4));
    objects->addObject(getCreateBtn(2765, 4));
    objects->addObject(getCreateBtn(2766, 4));
    objects->addObject(getCreateBtn(2767, 4));
    objects->addObject(getCreateBtn(2768, 4));
    objects->addObject(getCreateBtn(2769, 4));
    objects->addObject(getCreateBtn(2770, 4));
    objects->addObject(getCreateBtn(2773, 4));
    objects->addObject(getCreateBtn(2776, 4));
    objects->addObject(getCreateBtn(2777, 4));
    objects->addObject(getCreateBtn(2778, 4));
    objects->addObject(getCreateBtn(2779, 4));
    objects->addObject(getCreateBtn(2780, 4));
    objects->addObject(getCreateBtn(2781, 4));
    objects->addObject(getCreateBtn(2782, 4));
    objects->addObject(getCreateBtn(2783, 4));
    objects->addObject(getCreateBtn(2784, 4));
    objects->addObject(getCreateBtn(2785, 4));
    objects->addObject(getCreateBtn(2786, 4));
    objects->addObject(getCreateBtn(2787, 4));
    objects->addObject(getCreateBtn(2788, 4));
    objects->addObject(getCreateBtn(2927, 4));
    objects->addObject(getCreateBtn(2928, 4));
    objects->addObject(getCreateBtn(2789, 4));
    objects->addObject(getCreateBtn(2929, 4));
    objects->addObject(getCreateBtn(2930, 4));
    objects->addObject(getCreateBtn(2790, 4));
    objects->addObject(getCreateBtn(2931, 4));
    objects->addObject(getCreateBtn(2932, 4));
    objects->addObject(getCreateBtn(2791, 4));
    objects->addObject(getCreateBtn(2933, 4));
    objects->addObject(getCreateBtn(2934, 4));
    objects->addObject(getCreateBtn(2792, 4));
    objects->addObject(getCreateBtn(2935, 4));
    objects->addObject(getCreateBtn(2936, 4));
    objects->addObject(getCreateBtn(2793, 4));
    objects->addObject(getCreateBtn(2937, 4));
    objects->addObject(getCreateBtn(2938, 4));
    objects->addObject(getCreateBtn(2794, 4));
    objects->addObject(getCreateBtn(2939, 4));
    objects->addObject(getCreateBtn(2940, 4));
    objects->addObject(getCreateBtn(2795, 4));
    objects->addObject(getCreateBtn(2941, 4));
    objects->addObject(getCreateBtn(2942, 4));
    objects->addObject(getCreateBtn(2796, 4));
    objects->addObject(getCreateBtn(2797, 4));
    objects->addObject(getCreateBtn(2798, 4));
    objects->addObject(getCreateBtn(2799, 4));
    objects->addObject(getCreateBtn(2800, 4));
    objects->addObject(getCreateBtn(2801, 4));
    objects->addObject(getCreateBtn(2802, 4));
    objects->addObject(getCreateBtn(2803, 4));
    objects->addObject(getCreateBtn(2804, 4));
    objects->addObject(getCreateBtn(2805, 4));
    objects->addObject(getCreateBtn(2806, 4));
    objects->addObject(getCreateBtn(2807, 4));
    objects->addObject(getCreateBtn(2838, 4));
    objects->addObject(getCreateBtn(2839, 4));
    objects->addObject(getCreateBtn(2840, 4));
    objects->addObject(getCreateBtn(2841, 4));
    objects->addObject(getCreateBtn(2842, 4));
    objects->addObject(getCreateBtn(2843, 4));
    objects->addObject(getCreateBtn(2844, 4));
    objects->addObject(getCreateBtn(2845, 4));
    objects->addObject(getCreateBtn(2846, 4));
    objects->addObject(getCreateBtn(2847, 4));
    objects->addObject(getCreateBtn(2848, 4));
    objects->addObject(getCreateBtn(2849, 4));
    objects->addObject(getCreateBtn(2850, 4));
    objects->addObject(getCreateBtn(2851, 4));
    objects->addObject(getCreateBtn(2852, 4));
    objects->addObject(getCreateBtn(2853, 4));
    objects->addObject(getCreateBtn(2854, 4));
    objects->addObject(getCreateBtn(2855, 4));
    objects->addObject(getCreateBtn(2856, 4));
    objects->addObject(getCreateBtn(2857, 4));
    objects->addObject(getCreateBtn(2858, 4));
    objects->addObject(getCreateBtn(2859, 4));
    objects->addObject(getCreateBtn(2860, 4));
    objects->addObject(getCreateBtn(2861, 4));
    objects->addObject(getCreateBtn(2862, 4));
    objects->addObject(getCreateBtn(2863, 4));
    objects->addObject(getCreateBtn(2943, 4));
    objects->addObject(getCreateBtn(2944, 4));
    objects->addObject(getCreateBtn(2945, 4));
    objects->addObject(getCreateBtn(2946, 4));
    objects->addObject(getCreateBtn(2947, 4));
    objects->addObject(getCreateBtn(2948, 4));
    objects->addObject(getCreateBtn(2949, 4));
    objects->addObject(getCreateBtn(2950, 4));
    objects->addObject(getCreateBtn(2951, 4));
    objects->addObject(getCreateBtn(2952, 4));
    objects->addObject(getCreateBtn(2953, 4));
    objects->addObject(getCreateBtn(2954, 4));
    objects->addObject(getCreateBtn(2955, 4));
    objects->addObject(getCreateBtn(2956, 4));
    objects->addObject(getCreateBtn(2957, 4));
    objects->addObject(getCreateBtn(2958, 4));
    objects->addObject(getCreateBtn(2959, 4));
    objects->addObject(getCreateBtn(2960, 4));
    objects->addObject(getCreateBtn(2961, 4));
    objects->addObject(getCreateBtn(2962, 4));
    objects->addObject(getCreateBtn(2963, 4));
    objects->addObject(getCreateBtn(2964, 4));
    objects->addObject(getCreateBtn(2965, 4));
    objects->addObject(getCreateBtn(2966, 4));
    objects->addObject(getCreateBtn(2967, 4));
    objects->addObject(getCreateBtn(2968, 4));
    objects->addObject(getCreateBtn(2969, 4));
    objects->addObject(getCreateBtn(2970, 4));
    objects->addObject(getCreateBtn(2971, 4));
    objects->addObject(getCreateBtn(2972, 4));
    objects->addObject(getCreateBtn(2973, 4));
    objects->addObject(getCreateBtn(2974, 4));
    objects->addObject(getCreateBtn(2975, 4));
    objects->addObject(getCreateBtn(2976, 4));
    objects->addObject(getCreateBtn(2977, 4));
    objects->addObject(getCreateBtn(2978, 4));
    objects->addObject(getCreateBtn(2979, 4));
    objects->addObject(getCreateBtn(2980, 4));
    objects->addObject(getCreateBtn(2981, 4));
    objects->addObject(getCreateBtn(2982, 4));
    objects->addObject(getCreateBtn(2983, 4));
    objects->addObject(getCreateBtn(2984, 4));
    objects->addObject(getCreateBtn(2985, 4));
    objects->addObject(getCreateBtn(2986, 4));
    objects->addObject(getCreateBtn(2987, 4));
    objects->addObject(getCreateBtn(2988, 4));
    objects->addObject(getCreateBtn(2989, 4));
    objects->addObject(getCreateBtn(2990, 4));
    objects->addObject(getCreateBtn(2991, 4));
    objects->addObject(getCreateBtn(2992, 4));
    objects->addObject(getCreateBtn(2993, 4));
    objects->addObject(getCreateBtn(2994, 4));
    objects->addObject(getCreateBtn(2995, 4));
    objects->addObject(getCreateBtn(2996, 4));
    objects->addObject(getCreateBtn(2997, 4));
    objects->addObject(getCreateBtn(2998, 4));
    objects->addObject(getCreateBtn(3034, 4));
    objects->addObject(getCreateBtn(3038, 4));
    objects->addObject(getCreateBtn(3042, 4));
    objects->addObject(getCreateBtn(3046, 4));
    objects->addObject(getCreateBtn(3035, 4));
    objects->addObject(getCreateBtn(3039, 4));
    objects->addObject(getCreateBtn(3043, 4));
    objects->addObject(getCreateBtn(3047, 4));
    objects->addObject(getCreateBtn(3036, 4));
    objects->addObject(getCreateBtn(3040, 4));
    objects->addObject(getCreateBtn(3044, 4));
    objects->addObject(getCreateBtn(3048, 4));
    objects->addObject(getCreateBtn(3037, 4));
    objects->addObject(getCreateBtn(3041, 4));
    objects->addObject(getCreateBtn(3045, 4));
    objects->addObject(getCreateBtn(3049, 4));
    objects->addObject(getCreateBtn(3050, 4));
    objects->addObject(getCreateBtn(3051, 4));
    objects->addObject(getCreateBtn(3052, 4));
    objects->addObject(getCreateBtn(3053, 4));
    objects->addObject(getCreateBtn(3054, 4));
    objects->addObject(getCreateBtn(3067, 4));
    objects->addObject(getCreateBtn(3055, 4));
    objects->addObject(getCreateBtn(3068, 4));
    objects->addObject(getCreateBtn(3056, 4));
    objects->addObject(getCreateBtn(3069, 4));
    objects->addObject(getCreateBtn(3057, 4));
    objects->addObject(getCreateBtn(3070, 4));
    objects->addObject(getCreateBtn(3058, 4));
    objects->addObject(getCreateBtn(3071, 4));
    objects->addObject(getCreateBtn(3059, 4));
    objects->addObject(getCreateBtn(3072, 4));
    objects->addObject(getCreateBtn(3060, 4));
    objects->addObject(getCreateBtn(3073, 4));
    objects->addObject(getCreateBtn(3061, 4));
    objects->addObject(getCreateBtn(3074, 4));
    objects->addObject(getCreateBtn(3062, 4));
    objects->addObject(getCreateBtn(3075, 4));
    objects->addObject(getCreateBtn(3063, 4));
    objects->addObject(getCreateBtn(3076, 4));
    objects->addObject(getCreateBtn(3064, 4));
    objects->addObject(getCreateBtn(3077, 4));
    objects->addObject(getCreateBtn(3065, 4));
    objects->addObject(getCreateBtn(3078, 4));
    objects->addObject(getCreateBtn(3066, 4));
    objects->addObject(getCreateBtn(3079, 4));
    objects->addObject(getCreateBtn(3090, 4));
    objects->addObject(getCreateBtn(3091, 4));
    objects->addObject(getCreateBtn(3080, 4));
    objects->addObject(getCreateBtn(3081, 4));
    objects->addObject(getCreateBtn(3082, 4));
    objects->addObject(getCreateBtn(3083, 4));
    objects->addObject(getCreateBtn(3084, 4));
    objects->addObject(getCreateBtn(3085, 4));
    objects->addObject(getCreateBtn(3087, 4));
    objects->addObject(getCreateBtn(3086, 4));
    objects->addObject(getCreateBtn(3089, 4));
    objects->addObject(getCreateBtn(3088, 4));
    objects->addObject(getCreateBtn(1919, 4));
    objects->addObject(getCreateBtn(1920, 4));
    objects->addObject(getCreateBtn(1921, 4));
    objects->addObject(getCreateBtn(1922, 4));
    objects->addObject(getCreateBtn(1923, 4));
    objects->addObject(getCreateBtn(1924, 4));
    objects->addObject(getCreateBtn(1925, 4));
    objects->addObject(getCreateBtn(1926, 4));
    objects->addObject(getCreateBtn(1927, 4));
    objects->addObject(getCreateBtn(1928, 4));
    objects->addObject(getCreateBtn(1903, 4));
    objects->addObject(getCreateBtn(1904, 4));
    objects->addObject(getCreateBtn(1905, 4));
    objects->addObject(getCreateBtn(1910, 4));
    objects->addObject(getCreateBtn(1911, 4));
    objects->addObject(getCreateBtn(215, 4));
    objects->addObject(getCreateBtn(220, 4));
    objects->addObject(getCreateBtn(219, 4));
    objects->addObject(getCreateBtn(62, 4));
    objects->addObject(getCreateBtn(66, 4));
    objects->addObject(getCreateBtn(65, 4));
    objects->addObject(getCreateBtn(68, 4));
    objects->addObject(getCreateBtn(63, 4));
    objects->addObject(getCreateBtn(64, 4));
    objects->addObject(getCreateBtn(170, 4));
    objects->addObject(getCreateBtn(171, 4));
    objects->addObject(getCreateBtn(172, 4));
    objects->addObject(getCreateBtn(192, 4));
    objects->addObject(getCreateBtn(173, 4));
    objects->addObject(getCreateBtn(194, 4));
    objects->addObject(getCreateBtn(174, 4));
    objects->addObject(getCreateBtn(175, 4));
    objects->addObject(getCreateBtn(176, 4));
    objects->addObject(getCreateBtn(197, 4));
    objects->addObject(getCreateBtn(143, 4));
    objects->addObject(getCreateBtn(653, 4));
    objects->addObject(getCreateBtn(724, 4));
    objects->addObject(getCreateBtn(654, 4));
    objects->addObject(getCreateBtn(736, 4));
    objects->addObject(getCreateBtn(657, 4));
    objects->addObject(getCreateBtn(656, 4));
    objects->addObject(getCreateBtn(723, 4));
    objects->addObject(getCreateBtn(655, 4));
    objects->addObject(getCreateBtn(735, 4));
    objects->addObject(getCreateBtn(769, 4));
    objects->addObject(getCreateBtn(770, 4));
    objects->addObject(getCreateBtn(967, 4));
    objects->addObject(getCreateBtn(968, 4));
    objects->addObject(getCreateBtn(1078, 4));
    objects->addObject(getCreateBtn(1079, 4));
    objects->addObject(getCreateBtn(1080, 4));
    objects->addObject(getCreateBtn(1081, 4));
    objects->addObject(getCreateBtn(1095, 4));
    objects->addObject(getCreateBtn(1110, 4));
    objects->addObject(getCreateBtn(1096, 4));
    objects->addObject(getCreateBtn(1097, 4));
    objects->addObject(getCreateBtn(1098, 4));
    objects->addObject(getCreateBtn(1111, 4));
    objects->addObject(getCreateBtn(850, 4));
    objects->addObject(getCreateBtn(853, 4));
    objects->addObject(getCreateBtn(854, 4));
    objects->addObject(getCreateBtn(855, 4));
    objects->addObject(getCreateBtn(856, 4));
    objects->addObject(getCreateBtn(857, 4));
    objects->addObject(getCreateBtn(859, 4));
    objects->addObject(getCreateBtn(861, 4));
    objects->addObject(getCreateBtn(862, 4));
    objects->addObject(getCreateBtn(863, 4));
    objects->addObject(getCreateBtn(891, 4));
    objects->addObject(getCreateBtn(893, 4));
    objects->addObject(getCreateBtn(894, 4));
    objects->addObject(getCreateBtn(2895, 4));
    objects->addObject(getCreateBtn(2896, 4));
    objects->addObject(getCreateBtn(2897, 4));

    auto tab1 = EditButtonBar::create(objects, point, 0, true, buttonsPerRow, buttonRows);
    addChild(tab1, 10);

    m_createButtonBars->addObject(tab1);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("blockOutline_01_001.png"));

    objects->addObject(getCreateBtn(467, 4));
    objects->addObject(getCreateBtn(468, 4));
    objects->addObject(getCreateBtn(469, 4));
    objects->addObject(getCreateBtn(470, 4));
    objects->addObject(getCreateBtn(471, 4));
    objects->addObject(getCreateBtn(1338, 4));
    objects->addObject(getCreateBtn(1339, 4));
    objects->addObject(getCreateBtn(472, 4));
    objects->addObject(getCreateBtn(473, 4));
    objects->addObject(getCreateBtn(474, 4));
    objects->addObject(getCreateBtn(661, 4));
    objects->addObject(getCreateBtn(662, 4));
    objects->addObject(getCreateBtn(663, 4));
    objects->addObject(getCreateBtn(664, 4));
    objects->addObject(getCreateBtn(1154, 4));
    objects->addObject(getCreateBtn(1155, 4));
    objects->addObject(getCreateBtn(1156, 4));
    objects->addObject(getCreateBtn(1157, 4));
    objects->addObject(getCreateBtn(1158, 4));
    objects->addObject(getCreateBtn(475, 4));
    objects->addObject(getCreateBtn(1210, 4));
    objects->addObject(getCreateBtn(1202, 4));
    objects->addObject(getCreateBtn(1203, 4));
    objects->addObject(getCreateBtn(1204, 4));
    objects->addObject(getCreateBtn(1209, 4));
    objects->addObject(getCreateBtn(1205, 4));
    objects->addObject(getCreateBtn(1206, 4));
    objects->addObject(getCreateBtn(1207, 4));
    objects->addObject(getCreateBtn(1208, 4));
    objects->addObject(getCreateBtn(1227, 4));
    objects->addObject(getCreateBtn(1220, 4));
    objects->addObject(getCreateBtn(1221, 4));
    objects->addObject(getCreateBtn(1222, 4));
    objects->addObject(getCreateBtn(1226, 4));
    objects->addObject(getCreateBtn(1223, 4));
    objects->addObject(getCreateBtn(1224, 4));
    objects->addObject(getCreateBtn(1225, 4));
    objects->addObject(getCreateBtn(1260, 4));
    objects->addObject(getCreateBtn(1261, 4));
    objects->addObject(getCreateBtn(1262, 4));
    objects->addObject(getCreateBtn(1263, 4));
    objects->addObject(getCreateBtn(1264, 4));
    objects->addObject(getCreateBtn(1265, 4));
    objects->addObject(getCreateBtn(146, 5));
    objects->addObject(getCreateBtn(147, 5));
    objects->addObject(getCreateBtn(206, 5));
    objects->addObject(getCreateBtn(204, 5));
    objects->addObject(getCreateBtn(673, 5));
    objects->addObject(getCreateBtn(674, 5));
    objects->addObject(getCreateBtn(1340, 5));
    objects->addObject(getCreateBtn(1341, 5));
    objects->addObject(getCreateBtn(1342, 5));
    objects->addObject(getCreateBtn(1343, 5));
    objects->addObject(getCreateBtn(1344, 5));
    objects->addObject(getCreateBtn(1345, 5));

    auto tab2 = EditButtonBar::create(objects, point, 1, true, buttonsPerRow, buttonRows);
    addChild(tab2, 10);

    m_createButtonBars->addObject(tab2);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("triangle_a_02_001.png"));

    objects->addObject(getCreateBtn(1743, 4));
    objects->addObject(getCreateBtn(1744, 4));
    objects->addObject(getCreateBtn(1745, 4));
    objects->addObject(getCreateBtn(1746, 4));
    objects->addObject(getCreateBtn(305, 4));
    objects->addObject(getCreateBtn(307, 4));
    objects->addObject(getCreateBtn(1747, 4));
    objects->addObject(getCreateBtn(1748, 4));
    objects->addObject(getCreateBtn(1749, 4));
    objects->addObject(getCreateBtn(1750, 4));
    objects->addObject(getCreateBtn(371, 4));
    objects->addObject(getCreateBtn(372, 4));
    objects->addObject(getCreateBtn(373, 4));
    objects->addObject(getCreateBtn(374, 4));
    objects->addObject(getCreateBtn(1906, 4));
    objects->addObject(getCreateBtn(1907, 4));
    objects->addObject(getCreateBtn(1908, 4));
    objects->addObject(getCreateBtn(1909, 4));
    objects->addObject(getCreateBtn(294, 4));
    objects->addObject(getCreateBtn(295, 4));
    objects->addObject(getCreateBtn(296, 4));
    objects->addObject(getCreateBtn(297, 4));
    objects->addObject(getCreateBtn(326, 4));
    objects->addObject(getCreateBtn(327, 4));
    objects->addObject(getCreateBtn(328, 4));
    objects->addObject(getCreateBtn(329, 4));
    objects->addObject(getCreateBtn(349, 4));
    objects->addObject(getCreateBtn(351, 4));
    objects->addObject(getCreateBtn(762, 4));
    objects->addObject(getCreateBtn(763, 4));
    objects->addObject(getCreateBtn(764, 4));
    objects->addObject(getCreateBtn(765, 4));
    objects->addObject(getCreateBtn(766, 4));
    objects->addObject(getCreateBtn(1033, 4));
    objects->addObject(getCreateBtn(1034, 4));
    objects->addObject(getCreateBtn(1035, 4));
    objects->addObject(getCreateBtn(1036, 4));
    objects->addObject(getCreateBtn(1041, 4));
    objects->addObject(getCreateBtn(1042, 4));
    objects->addObject(getCreateBtn(771, 4));
    objects->addObject(getCreateBtn(772, 4));
    objects->addObject(getCreateBtn(773, 4));
    objects->addObject(getCreateBtn(774, 4));
    objects->addObject(getCreateBtn(775, 4));
    objects->addObject(getCreateBtn(960, 4));
    objects->addObject(getCreateBtn(961, 4));
    objects->addObject(getCreateBtn(964, 4));
    objects->addObject(getCreateBtn(965, 4));
    objects->addObject(getCreateBtn(966, 4));
    objects->addObject(getCreateBtn(1037, 4));
    objects->addObject(getCreateBtn(1038, 4));
    objects->addObject(getCreateBtn(1039, 4));
    objects->addObject(getCreateBtn(1040, 4));
    objects->addObject(getCreateBtn(1043, 4));
    objects->addObject(getCreateBtn(1044, 4));
    objects->addObject(getCreateBtn(969, 4));
    objects->addObject(getCreateBtn(970, 4));
    objects->addObject(getCreateBtn(971, 4));
    objects->addObject(getCreateBtn(972, 4));
    objects->addObject(getCreateBtn(973, 4));
    objects->addObject(getCreateBtn(1305, 4));
    objects->addObject(getCreateBtn(1306, 4));
    objects->addObject(getCreateBtn(1307, 4));
    objects->addObject(getCreateBtn(1308, 4));
    objects->addObject(getCreateBtn(1309, 4));
    objects->addObject(getCreateBtn(681, 4));
    objects->addObject(getCreateBtn(682, 4));
    objects->addObject(getCreateBtn(683, 4));
    objects->addObject(getCreateBtn(684, 4));
    objects->addObject(getCreateBtn(685, 4));
    objects->addObject(getCreateBtn(686, 4));
    objects->addObject(getCreateBtn(687, 4));
    objects->addObject(getCreateBtn(688, 4));
    objects->addObject(getCreateBtn(689, 4));
    objects->addObject(getCreateBtn(690, 4));
    objects->addObject(getCreateBtn(691, 4));
    objects->addObject(getCreateBtn(692, 4));
    objects->addObject(getCreateBtn(324, 4));
    objects->addObject(getCreateBtn(325, 4));
    objects->addObject(getCreateBtn(358, 4));
    objects->addObject(getCreateBtn(693, 4));
    objects->addObject(getCreateBtn(694, 4));
    objects->addObject(getCreateBtn(695, 4));
    objects->addObject(getCreateBtn(696, 4));
    objects->addObject(getCreateBtn(697, 4));
    objects->addObject(getCreateBtn(698, 4));
    objects->addObject(getCreateBtn(699, 4));
    objects->addObject(getCreateBtn(700, 4));
    objects->addObject(getCreateBtn(701, 4));
    objects->addObject(getCreateBtn(702, 4));
    objects->addObject(getCreateBtn(703, 4));
    objects->addObject(getCreateBtn(704, 4));
    objects->addObject(getCreateBtn(705, 4));
    objects->addObject(getCreateBtn(706, 4));
    objects->addObject(getCreateBtn(707, 4));
    objects->addObject(getCreateBtn(708, 4));
    objects->addObject(getCreateBtn(713, 4));
    objects->addObject(getCreateBtn(714, 4));
    objects->addObject(getCreateBtn(730, 4));
    objects->addObject(getCreateBtn(731, 4));
    objects->addObject(getCreateBtn(715, 4));
    objects->addObject(getCreateBtn(716, 4));
    objects->addObject(getCreateBtn(732, 4));
    objects->addObject(getCreateBtn(733, 4));
    objects->addObject(getCreateBtn(1187, 4));
    objects->addObject(getCreateBtn(1188, 4));
    objects->addObject(getCreateBtn(1189, 4));
    objects->addObject(getCreateBtn(1190, 4));
    objects->addObject(getCreateBtn(1325, 4));
    objects->addObject(getCreateBtn(1326, 4));
    objects->addObject(getCreateBtn(1198, 4));
    objects->addObject(getCreateBtn(1199, 4));
    objects->addObject(getCreateBtn(1200, 4));
    objects->addObject(getCreateBtn(1201, 4));
    objects->addObject(getCreateBtn(826, 4));
    objects->addObject(getCreateBtn(827, 4));
    objects->addObject(getCreateBtn(828, 4));
    objects->addObject(getCreateBtn(829, 4));
    objects->addObject(getCreateBtn(830, 4));
    objects->addObject(getCreateBtn(831, 4));
    objects->addObject(getCreateBtn(832, 4));
    objects->addObject(getCreateBtn(833, 4));
    objects->addObject(getCreateBtn(1091, 4));
    objects->addObject(getCreateBtn(1107, 4));
    objects->addObject(getCreateBtn(1092, 4));
    objects->addObject(getCreateBtn(1108, 4));
    objects->addObject(getCreateBtn(1093, 4));
    objects->addObject(getCreateBtn(1094, 4));
    objects->addObject(getCreateBtn(1109, 4));
    objects->addObject(getCreateBtn(877, 4));
    objects->addObject(getCreateBtn(878, 4));
    objects->addObject(getCreateBtn(888, 4));
    objects->addObject(getCreateBtn(889, 4));
    objects->addObject(getCreateBtn(895, 4));
    objects->addObject(getCreateBtn(896, 4));
    objects->addObject(getCreateBtn(1256, 4));
    objects->addObject(getCreateBtn(1257, 4));
    objects->addObject(getCreateBtn(1258, 4));
    objects->addObject(getCreateBtn(1259, 4));
    objects->addObject(getCreateBtn(1014, 4));
    objects->addObject(getCreateBtn(1015, 4));
    objects->addObject(getCreateBtn(1016, 4));
    objects->addObject(getCreateBtn(1017, 4));
    objects->addObject(getCreateBtn(1018, 4));

    auto tab3 = EditButtonBar::create(objects, point, 2, true, buttonsPerRow, buttonRows);
    addChild(tab3, 10);

    m_createButtonBars->addObject(tab3);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("spike_01_001.png"));

    objects->addObject(getCreateBtn(8, 4));
    objects->addObject(getCreateBtn(39, 4));
    objects->addObject(getCreateBtn(103, 4));
    objects->addObject(getCreateBtn(392, 4));
    objects->addObject(getCreateBtn(216, 4));
    objects->addObject(getCreateBtn(217, 4));
    objects->addObject(getCreateBtn(218, 4));
    objects->addObject(getCreateBtn(458, 4));
    objects->addObject(getCreateBtn(144, 5));
    objects->addObject(getCreateBtn(205, 5));
    objects->addObject(getCreateBtn(145, 5));
    objects->addObject(getCreateBtn(459, 5));
    objects->addObject(getCreateBtn(177, 4));
    objects->addObject(getCreateBtn(178, 4));
    objects->addObject(getCreateBtn(179, 4));
    objects->addObject(getCreateBtn(1889, 4));
    objects->addObject(getCreateBtn(1890, 4));
    objects->addObject(getCreateBtn(1891, 4));
    objects->addObject(getCreateBtn(1892, 4));
    objects->addObject(getCreateBtn(1715, 4));
    objects->addObject(getCreateBtn(1719, 4));
    objects->addObject(getCreateBtn(1720, 4));
    objects->addObject(getCreateBtn(1721, 4));
    objects->addObject(getCreateBtn(135, 4));
    objects->addObject(getCreateBtn(1711, 4));
    objects->addObject(getCreateBtn(1712, 4));
    objects->addObject(getCreateBtn(1713, 4));
    objects->addObject(getCreateBtn(1714, 4));
    objects->addObject(getCreateBtn(1717, 4));
    objects->addObject(getCreateBtn(1718, 4));
    objects->addObject(getCreateBtn(1716, 4));
    objects->addObject(getCreateBtn(1723, 4));
    objects->addObject(getCreateBtn(1724, 4));
    objects->addObject(getCreateBtn(1722, 4));
    objects->addObject(getCreateBtn(1725, 4));
    objects->addObject(getCreateBtn(1726, 4));
    objects->addObject(getCreateBtn(1727, 4));
    objects->addObject(getCreateBtn(1728, 4));
    objects->addObject(getCreateBtn(1729, 4));
    objects->addObject(getCreateBtn(1730, 4));
    objects->addObject(getCreateBtn(1731, 4));
    objects->addObject(getCreateBtn(1732, 4));
    objects->addObject(getCreateBtn(1733, 4));
    objects->addObject(getCreateBtn(3610, 4));
    objects->addObject(getCreateBtn(3611, 4));

    auto tab4 = EditButtonBar::create(objects, point, 3, true, buttonsPerRow, buttonRows);
    addChild(tab4, 10);

    m_createButtonBars->addObject(tab4);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("persp_outline_01_001.png"));

    objects->addObject(getCreateBtn(506, 4));
    objects->addObject(getCreateBtn(507, 4));
    objects->addObject(getCreateBtn(508, 4));
    objects->addObject(getCreateBtn(509, 4));
    objects->addObject(getCreateBtn(510, 4));
    objects->addObject(getCreateBtn(511, 4));
    objects->addObject(getCreateBtn(512, 4));
    objects->addObject(getCreateBtn(513, 4));
    objects->addObject(getCreateBtn(514, 4));
    objects->addObject(getCreateBtn(515, 4));
    objects->addObject(getCreateBtn(516, 4));
    objects->addObject(getCreateBtn(517, 4));
    objects->addObject(getCreateBtn(518, 4));
    objects->addObject(getCreateBtn(519, 4));
    objects->addObject(getCreateBtn(520, 4));
    objects->addObject(getCreateBtn(521, 4));
    objects->addObject(getCreateBtn(522, 4));
    objects->addObject(getCreateBtn(523, 4));
    objects->addObject(getCreateBtn(524, 4));
    objects->addObject(getCreateBtn(525, 4));
    objects->addObject(getCreateBtn(526, 4));
    objects->addObject(getCreateBtn(527, 4));
    objects->addObject(getCreateBtn(528, 4));
    objects->addObject(getCreateBtn(529, 4));
    objects->addObject(getCreateBtn(530, 4));
    objects->addObject(getCreateBtn(531, 4));
    objects->addObject(getCreateBtn(532, 4));
    objects->addObject(getCreateBtn(533, 4));
    objects->addObject(getCreateBtn(534, 4));
    objects->addObject(getCreateBtn(535, 4));
    objects->addObject(getCreateBtn(536, 4));
    objects->addObject(getCreateBtn(537, 4));
    objects->addObject(getCreateBtn(538, 4));
    objects->addObject(getCreateBtn(539, 4));
    objects->addObject(getCreateBtn(540, 4));
    objects->addObject(getCreateBtn(541, 4));
    objects->addObject(getCreateBtn(542, 4));
    objects->addObject(getCreateBtn(543, 4));
    objects->addObject(getCreateBtn(544, 4));
    objects->addObject(getCreateBtn(545, 4));
    objects->addObject(getCreateBtn(546, 4));
    objects->addObject(getCreateBtn(547, 4));
    objects->addObject(getCreateBtn(548, 4));
    objects->addObject(getCreateBtn(549, 4));
    objects->addObject(getCreateBtn(550, 4));
    objects->addObject(getCreateBtn(551, 4));
    objects->addObject(getCreateBtn(552, 4));
    objects->addObject(getCreateBtn(553, 4));
    objects->addObject(getCreateBtn(554, 4));
    objects->addObject(getCreateBtn(555, 4));
    objects->addObject(getCreateBtn(556, 4));
    objects->addObject(getCreateBtn(557, 4));
    objects->addObject(getCreateBtn(558, 4));
    objects->addObject(getCreateBtn(559, 4));
    objects->addObject(getCreateBtn(560, 4));
    objects->addObject(getCreateBtn(561, 4));
    objects->addObject(getCreateBtn(562, 4));
    objects->addObject(getCreateBtn(563, 4));
    objects->addObject(getCreateBtn(564, 4));
    objects->addObject(getCreateBtn(565, 4));
    objects->addObject(getCreateBtn(566, 4));
    objects->addObject(getCreateBtn(567, 4));
    objects->addObject(getCreateBtn(568, 4));
    objects->addObject(getCreateBtn(569, 4));
    objects->addObject(getCreateBtn(570, 4));
    objects->addObject(getCreateBtn(571, 4));
    objects->addObject(getCreateBtn(572, 4));
    objects->addObject(getCreateBtn(573, 4));
    objects->addObject(getCreateBtn(574, 4));
    objects->addObject(getCreateBtn(575, 4));
    objects->addObject(getCreateBtn(576, 4));
    objects->addObject(getCreateBtn(577, 4));
    objects->addObject(getCreateBtn(578, 4));
    objects->addObject(getCreateBtn(579, 4));
    objects->addObject(getCreateBtn(580, 4));
    objects->addObject(getCreateBtn(581, 4));
    objects->addObject(getCreateBtn(582, 4));
    objects->addObject(getCreateBtn(583, 4));
    objects->addObject(getCreateBtn(584, 4));
    objects->addObject(getCreateBtn(585, 4));
    objects->addObject(getCreateBtn(586, 4));
    objects->addObject(getCreateBtn(902, 4));
    objects->addObject(getCreateBtn(587, 4));
    objects->addObject(getCreateBtn(588, 4));
    objects->addObject(getCreateBtn(589, 4));
    objects->addObject(getCreateBtn(590, 4));
    objects->addObject(getCreateBtn(591, 4));
    objects->addObject(getCreateBtn(592, 4));
    objects->addObject(getCreateBtn(593, 4));
    objects->addObject(getCreateBtn(594, 4));
    objects->addObject(getCreateBtn(595, 4));
    objects->addObject(getCreateBtn(596, 4));
    objects->addObject(getCreateBtn(597, 4));
    objects->addObject(getCreateBtn(598, 4));
    objects->addObject(getCreateBtn(599, 4));
    objects->addObject(getCreateBtn(600, 4));
    objects->addObject(getCreateBtn(601, 4));
    objects->addObject(getCreateBtn(602, 4));
    objects->addObject(getCreateBtn(603, 4));
    objects->addObject(getCreateBtn(604, 4));
    objects->addObject(getCreateBtn(605, 4));
    objects->addObject(getCreateBtn(606, 4));
    objects->addObject(getCreateBtn(607, 4));
    objects->addObject(getCreateBtn(608, 4));
    objects->addObject(getCreateBtn(609, 4));
    objects->addObject(getCreateBtn(610, 4));
    objects->addObject(getCreateBtn(611, 4));
    objects->addObject(getCreateBtn(612, 4));
    objects->addObject(getCreateBtn(613, 4));
    objects->addObject(getCreateBtn(614, 4));
    objects->addObject(getCreateBtn(615, 4));
    objects->addObject(getCreateBtn(616, 4));
    objects->addObject(getCreateBtn(617, 4));
    objects->addObject(getCreateBtn(618, 4));
    objects->addObject(getCreateBtn(619, 4));
    objects->addObject(getCreateBtn(620, 4));
    objects->addObject(getCreateBtn(621, 4));
    objects->addObject(getCreateBtn(622, 4));
    objects->addObject(getCreateBtn(623, 4));
    objects->addObject(getCreateBtn(624, 4));
    objects->addObject(getCreateBtn(625, 4));
    objects->addObject(getCreateBtn(626, 4));
    objects->addObject(getCreateBtn(627, 4));
    objects->addObject(getCreateBtn(628, 4));
    objects->addObject(getCreateBtn(629, 4));
    objects->addObject(getCreateBtn(630, 4));
    objects->addObject(getCreateBtn(631, 4));
    objects->addObject(getCreateBtn(632, 4));
    objects->addObject(getCreateBtn(633, 4));
    objects->addObject(getCreateBtn(634, 4));
    objects->addObject(getCreateBtn(635, 4));
    objects->addObject(getCreateBtn(636, 4));
    objects->addObject(getCreateBtn(637, 4));
    objects->addObject(getCreateBtn(638, 4));
    objects->addObject(getCreateBtn(639, 4));
    objects->addObject(getCreateBtn(640, 4));
    objects->addObject(getCreateBtn(943, 4));
    objects->addObject(getCreateBtn(944, 4));
    objects->addObject(getCreateBtn(945, 4));
    objects->addObject(getCreateBtn(946, 4));
    objects->addObject(getCreateBtn(947, 4));
    objects->addObject(getCreateBtn(948, 4));
    objects->addObject(getCreateBtn(949, 4));
    objects->addObject(getCreateBtn(950, 4));
    objects->addObject(getCreateBtn(951, 4));
    objects->addObject(getCreateBtn(1024, 4));
    objects->addObject(getCreateBtn(1025, 4));
    objects->addObject(getCreateBtn(1026, 4));
    objects->addObject(getCreateBtn(1027, 4));
    objects->addObject(getCreateBtn(1028, 4));
    objects->addObject(getCreateBtn(1029, 4));
    objects->addObject(getCreateBtn(1030, 4));
    objects->addObject(getCreateBtn(1031, 4));
    objects->addObject(getCreateBtn(1032, 4));
    objects->addObject(getCreateBtn(980, 4));
    objects->addObject(getCreateBtn(981, 4));
    objects->addObject(getCreateBtn(982, 4));
    objects->addObject(getCreateBtn(983, 4));
    objects->addObject(getCreateBtn(984, 4));
    objects->addObject(getCreateBtn(985, 4));
    objects->addObject(getCreateBtn(986, 4));
    objects->addObject(getCreateBtn(987, 4));
    objects->addObject(getCreateBtn(988, 4));
    objects->addObject(getCreateBtn(1063, 4));
    objects->addObject(getCreateBtn(1064, 4));
    objects->addObject(getCreateBtn(1065, 4));
    objects->addObject(getCreateBtn(1066, 4));
    objects->addObject(getCreateBtn(1067, 4));
    objects->addObject(getCreateBtn(1068, 4));
    objects->addObject(getCreateBtn(1069, 4));
    objects->addObject(getCreateBtn(1070, 4));
    objects->addObject(getCreateBtn(1071, 4));
    objects->addObject(getCreateBtn(1552, 4));
    objects->addObject(getCreateBtn(1553, 4));
    objects->addObject(getCreateBtn(1554, 4));
    objects->addObject(getCreateBtn(1555, 4));
    objects->addObject(getCreateBtn(1556, 4));
    objects->addObject(getCreateBtn(1557, 4));
    objects->addObject(getCreateBtn(1558, 4));
    objects->addObject(getCreateBtn(1559, 4));
    objects->addObject(getCreateBtn(1560, 4));
    objects->addObject(getCreateBtn(1529, 4));
    objects->addObject(getCreateBtn(1530, 4));
    objects->addObject(getCreateBtn(1531, 4));
    objects->addObject(getCreateBtn(1532, 4));
    objects->addObject(getCreateBtn(1533, 4));
    objects->addObject(getCreateBtn(1534, 4));
    objects->addObject(getCreateBtn(1535, 4));
    objects->addObject(getCreateBtn(1536, 4));
    objects->addObject(getCreateBtn(1537, 4));
    objects->addObject(getCreateBtn(1538, 4));
    objects->addObject(getCreateBtn(1539, 4));
    objects->addObject(getCreateBtn(1540, 4));

    auto tab5 = EditButtonBar::create(objects, point, 4, true, buttonsPerRow, buttonRows);
    addChild(tab5, 10);

    m_createButtonBars->addObject(tab5);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("ring_01_001.png"));

    objects->addObject(getCreateBtn(35, 4));
    objects->addObject(getCreateBtn(140, 4));
    objects->addObject(getCreateBtn(1332, 4));
    objects->addObject(getCreateBtn(67, 4));
    objects->addObject(getCreateBtn(3005, 4));
    objects->addObject(getCreateBtn(36, 4));
    objects->addObject(getCreateBtn(141, 4));
    objects->addObject(getCreateBtn(1333, 4));
    objects->addObject(getCreateBtn(84, 4));
    objects->addObject(getCreateBtn(1022, 4));
    objects->addObject(getCreateBtn(1330, 4));
    objects->addObject(getCreateBtn(1704, 4));
    objects->addObject(getCreateBtn(1751, 4));
    objects->addObject(getCreateBtn(3004, 4));
    objects->addObject(getCreateBtn(3027, 4));
    objects->addObject(getCreateBtn(1594, 4));
    objects->addObject(getCreateBtn(10, 4));
    objects->addObject(getCreateBtn(11, 4));
    objects->addObject(getCreateBtn(2926, 4));
    objects->addObject(getCreateBtn(12, 4));
    objects->addObject(getCreateBtn(13, 4));
    objects->addObject(getCreateBtn(47, 4));
    objects->addObject(getCreateBtn(111, 4));
    objects->addObject(getCreateBtn(660, 4));
    objects->addObject(getCreateBtn(745, 4));
    objects->addObject(getCreateBtn(45, 4));
    objects->addObject(getCreateBtn(46, 4));
    objects->addObject(getCreateBtn(99, 4));
    objects->addObject(getCreateBtn(101, 4));
    objects->addObject(getCreateBtn(286, 4));
    objects->addObject(getCreateBtn(287, 4));
    objects->addObject(getCreateBtn(747, 4));
    objects->addObject(getCreateBtn(2902, 4));
    objects->addObject(getCreateBtn(2064, 4));
    objects->addObject(getCreateBtn(1331, 4));
    objects->addObject(getCreateBtn(1933, 4));
    objects->addObject(getCreateBtn(200, 4));
    objects->addObject(getCreateBtn(201, 4));
    objects->addObject(getCreateBtn(202, 4));
    objects->addObject(getCreateBtn(203, 4));
    objects->addObject(getCreateBtn(1334, 4));
    objects->addObject(getCreateBtn(2063, 4));
    objects->addObject(getCreateBtn(914, 4));
    objects->addObject(getCreateBtn(1755, 4));
    objects->addObject(getCreateBtn(1813, 4));
    objects->addObject(getCreateBtn(1829, 4));
    objects->addObject(getCreateBtn(1859, 4));
    objects->addObject(getCreateBtn(2069, 4));
    objects->addObject(getCreateBtn(3645, 4));
    objects->addObject(getCreateBtn(2866, 4));

    auto tab6 = EditButtonBar::create(objects, point, 5, true, buttonsPerRow, buttonRows);
    addChild(tab6, 10);

    m_createButtonBars->addObject(tab6);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("GJBeast01_01_001.png"));

    objects->addObject(getCreateBtn(2065, 4));
    objects->addObject(getCreateBtn(1586, 4));
    objects->addObject(getCreateBtn(1700, 4));
    objects->addObject(getCreateBtn(918, 4));
    objects->addObject(getCreateBtn(1327, 4));
    objects->addObject(getCreateBtn(1328, 4));
    objects->addObject(getCreateBtn(1584, 4));
    objects->addObject(getCreateBtn(2012, 4));
    objects->addObject(getCreateBtn(919, 4));
    objects->addObject(getCreateBtn(1050, 4));
    objects->addObject(getCreateBtn(1051, 4));
    objects->addObject(getCreateBtn(1052, 4));
    objects->addObject(getCreateBtn(3000, 4));
    objects->addObject(getCreateBtn(3001, 4));
    objects->addObject(getCreateBtn(3002, 4));
    objects->addObject(getCreateBtn(1228, 4));
    objects->addObject(getCreateBtn(1053, 4));
    objects->addObject(getCreateBtn(1054, 4));
    objects->addObject(getCreateBtn(1592, 4));
    objects->addObject(getCreateBtn(920, 4));
    objects->addObject(getCreateBtn(923, 4));
    objects->addObject(getCreateBtn(924, 4));
    objects->addObject(getCreateBtn(921, 4));
    objects->addObject(getCreateBtn(1582, 4));
    objects->addObject(getCreateBtn(1583, 4));
    objects->addObject(getCreateBtn(1591, 4));
    objects->addObject(getCreateBtn(1593, 4));
    objects->addObject(getCreateBtn(1849, 4));
    objects->addObject(getCreateBtn(1850, 4));
    objects->addObject(getCreateBtn(1851, 4));
    objects->addObject(getCreateBtn(1852, 4));
    objects->addObject(getCreateBtn(1853, 4));
    objects->addObject(getCreateBtn(1854, 4));
    objects->addObject(getCreateBtn(1855, 4));
    objects->addObject(getCreateBtn(1858, 4));
    objects->addObject(getCreateBtn(1856, 4));
    objects->addObject(getCreateBtn(1697, 4));
    objects->addObject(getCreateBtn(1698, 4));
    objects->addObject(getCreateBtn(1699, 4));
    objects->addObject(getCreateBtn(1857, 4));
    objects->addObject(getCreateBtn(1860, 4));
    objects->addObject(getCreateBtn(1516, 4));
    objects->addObject(getCreateBtn(1518, 4));
    objects->addObject(getCreateBtn(1517, 4));
    objects->addObject(getCreateBtn(1519, 4));
    objects->addObject(getCreateBtn(1618, 4));
    objects->addObject(getCreateBtn(1839, 4));
    objects->addObject(getCreateBtn(1840, 4));
    objects->addObject(getCreateBtn(1841, 4));
    objects->addObject(getCreateBtn(1842, 4));
    objects->addObject(getCreateBtn(1936, 4));
    objects->addObject(getCreateBtn(1937, 4));
    objects->addObject(getCreateBtn(1938, 4));
    objects->addObject(getCreateBtn(1939, 4));
    objects->addObject(getCreateBtn(2020, 4));
    objects->addObject(getCreateBtn(2021, 4));
    objects->addObject(getCreateBtn(2022, 4));
    objects->addObject(getCreateBtn(2023, 4));
    objects->addObject(getCreateBtn(2024, 4));
    objects->addObject(getCreateBtn(2025, 4));
    objects->addObject(getCreateBtn(2026, 4));
    objects->addObject(getCreateBtn(2027, 4));
    objects->addObject(getCreateBtn(2028, 4));
    objects->addObject(getCreateBtn(2029, 4));
    objects->addObject(getCreateBtn(2030, 4));
    objects->addObject(getCreateBtn(2031, 4));
    objects->addObject(getCreateBtn(2032, 4));
    objects->addObject(getCreateBtn(2033, 4));
    objects->addObject(getCreateBtn(2034, 4));
    objects->addObject(getCreateBtn(2035, 4));
    objects->addObject(getCreateBtn(2036, 4));
    objects->addObject(getCreateBtn(2037, 4));
    objects->addObject(getCreateBtn(2038, 4));
    objects->addObject(getCreateBtn(2039, 4));
    objects->addObject(getCreateBtn(2040, 4));
    objects->addObject(getCreateBtn(2041, 4));
    objects->addObject(getCreateBtn(2042, 4));
    objects->addObject(getCreateBtn(2043, 4));
    objects->addObject(getCreateBtn(2044, 4));
    objects->addObject(getCreateBtn(2045, 4));
    objects->addObject(getCreateBtn(2046, 4));
    objects->addObject(getCreateBtn(2047, 4));
    objects->addObject(getCreateBtn(2048, 4));
    objects->addObject(getCreateBtn(2049, 4));
    objects->addObject(getCreateBtn(2050, 4));
    objects->addObject(getCreateBtn(2051, 4));
    objects->addObject(getCreateBtn(2052, 4));
    objects->addObject(getCreateBtn(2053, 4));
    objects->addObject(getCreateBtn(2054, 4));
    objects->addObject(getCreateBtn(2055, 4));
    objects->addObject(getCreateBtn(2864, 4));
    objects->addObject(getCreateBtn(2865, 4));
    objects->addObject(getCreateBtn(2867, 4));
    objects->addObject(getCreateBtn(2868, 4));
    objects->addObject(getCreateBtn(2869, 4));
    objects->addObject(getCreateBtn(2870, 4));
    objects->addObject(getCreateBtn(2871, 4));
    objects->addObject(getCreateBtn(2872, 4));
    objects->addObject(getCreateBtn(2873, 4));
    objects->addObject(getCreateBtn(2874, 4));
    objects->addObject(getCreateBtn(2875, 4));
    objects->addObject(getCreateBtn(2876, 4));
    objects->addObject(getCreateBtn(2877, 4));
    objects->addObject(getCreateBtn(2878, 4));
    objects->addObject(getCreateBtn(2879, 4));
    objects->addObject(getCreateBtn(2880, 4));
    objects->addObject(getCreateBtn(2881, 4));
    objects->addObject(getCreateBtn(2882, 4));
    objects->addObject(getCreateBtn(2883, 4));
    objects->addObject(getCreateBtn(2884, 4));
    objects->addObject(getCreateBtn(2885, 4));
    objects->addObject(getCreateBtn(2886, 4));
    objects->addObject(getCreateBtn(2887, 4));
    objects->addObject(getCreateBtn(2888, 4));
    objects->addObject(getCreateBtn(2889, 4));
    objects->addObject(getCreateBtn(2890, 4));
    objects->addObject(getCreateBtn(2891, 4));
    objects->addObject(getCreateBtn(2892, 4));
    objects->addObject(getCreateBtn(2893, 4));
    objects->addObject(getCreateBtn(2894, 4));
    objects->addObject(getCreateBtn(2222, 4));
    objects->addObject(getCreateBtn(2223, 4));
    objects->addObject(getCreateBtn(2245, 4));
    objects->addObject(getCreateBtn(2246, 4));
    objects->addObject(getCreateBtn(2605, 4));
    objects->addObject(getCreateBtn(2606, 4));
    objects->addObject(getCreateBtn(2629, 4));
    objects->addObject(getCreateBtn(2630, 4));
    objects->addObject(getCreateBtn(2694, 4));
    objects->addObject(getCreateBtn(2695, 4));
    objects->addObject(getCreateBtn(3119, 4));
    objects->addObject(getCreateBtn(3120, 4));
    objects->addObject(getCreateBtn(3121, 4));
    objects->addObject(getCreateBtn(3219, 4));
    objects->addObject(getCreateBtn(3302, 4));
    objects->addObject(getCreateBtn(3303, 4));
    objects->addObject(getCreateBtn(3304, 4));
    objects->addObject(getCreateBtn(3305, 4));
    objects->addObject(getCreateBtn(3482, 4));
    objects->addObject(getCreateBtn(3483, 4));
    objects->addObject(getCreateBtn(3484, 4));
    objects->addObject(getCreateBtn(3485, 4));
    objects->addObject(getCreateBtn(3486, 4));
    objects->addObject(getCreateBtn(3487, 4));
    objects->addObject(getCreateBtn(3488, 4));
    objects->addObject(getCreateBtn(3489, 4));
    objects->addObject(getCreateBtn(3490, 4));
    objects->addObject(getCreateBtn(3491, 4));
    objects->addObject(getCreateBtn(3492, 4));
    objects->addObject(getCreateBtn(3493, 4));
    objects->addObject(getCreateBtn(4211, 4));
    objects->addObject(getCreateBtn(4300, 4));

    auto tab7 = EditButtonBar::create(objects, point, 6, true, buttonsPerRow, buttonRows);
    addChild(tab7, 10);

    m_createButtonBars->addObject(tab7);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("pixelb_03_01_001.png"));

    objects->addObject(getCreateBtn(2083, 4));
    objects->addObject(getCreateBtn(2084, 4));
    objects->addObject(getCreateBtn(2085, 4));
    objects->addObject(getCreateBtn(2086, 4));
    objects->addObject(getCreateBtn(2087, 4));
    objects->addObject(getCreateBtn(2088, 4));
    objects->addObject(getCreateBtn(2089, 4));
    objects->addObject(getCreateBtn(2078, 4));
    objects->addObject(getCreateBtn(2079, 4));
    objects->addObject(getCreateBtn(2080, 4));
    objects->addObject(getCreateBtn(2081, 4));
    objects->addObject(getCreateBtn(2082, 4));
    objects->addObject(getCreateBtn(2093, 4));
    objects->addObject(getCreateBtn(2094, 4));
    objects->addObject(getCreateBtn(2095, 4));
    objects->addObject(getCreateBtn(2096, 4));
    objects->addObject(getCreateBtn(2120, 4));
    objects->addObject(getCreateBtn(2121, 4));
    objects->addObject(getCreateBtn(2097, 4));
    objects->addObject(getCreateBtn(2070, 4));
    objects->addObject(getCreateBtn(2071, 4));
    objects->addObject(getCreateBtn(2072, 4));
    objects->addObject(getCreateBtn(2073, 4));
    objects->addObject(getCreateBtn(2074, 4));
    objects->addObject(getCreateBtn(2075, 4));
    objects->addObject(getCreateBtn(2076, 4));
    objects->addObject(getCreateBtn(2092, 4));
    objects->addObject(getCreateBtn(2077, 4));
    objects->addObject(getCreateBtn(2090, 4));
    objects->addObject(getCreateBtn(2091, 4));
    objects->addObject(getCreateBtn(2098, 4));
    objects->addObject(getCreateBtn(2099, 4));
    objects->addObject(getCreateBtn(2100, 4));
    objects->addObject(getCreateBtn(2101, 4));
    objects->addObject(getCreateBtn(2102, 4));
    objects->addObject(getCreateBtn(2103, 4));
    objects->addObject(getCreateBtn(2104, 4));
    objects->addObject(getCreateBtn(2105, 4));
    objects->addObject(getCreateBtn(2106, 4));
    objects->addObject(getCreateBtn(2107, 4));
    objects->addObject(getCreateBtn(2457, 4));
    objects->addObject(getCreateBtn(2458, 4));
    objects->addObject(getCreateBtn(2108, 4));
    objects->addObject(getCreateBtn(2109, 4));
    objects->addObject(getCreateBtn(2110, 4));
    objects->addObject(getCreateBtn(2111, 4));
    objects->addObject(getCreateBtn(2456, 4));
    objects->addObject(getCreateBtn(2112, 4));
    objects->addObject(getCreateBtn(2113, 4));
    objects->addObject(getCreateBtn(2114, 4));
    objects->addObject(getCreateBtn(2115, 4));
    objects->addObject(getCreateBtn(2116, 4));
    objects->addObject(getCreateBtn(2117, 4));
    objects->addObject(getCreateBtn(2118, 4));
    objects->addObject(getCreateBtn(2119, 4));
    objects->addObject(getCreateBtn(2122, 4));
    objects->addObject(getCreateBtn(2123, 4));
    objects->addObject(getCreateBtn(2125, 4));
    objects->addObject(getCreateBtn(2124, 4));
    objects->addObject(getCreateBtn(2126, 4));
    objects->addObject(getCreateBtn(2127, 4));
    objects->addObject(getCreateBtn(2128, 4));
    objects->addObject(getCreateBtn(2129, 4));
    objects->addObject(getCreateBtn(2130, 4));
    objects->addObject(getCreateBtn(2131, 4));
    objects->addObject(getCreateBtn(2132, 4));
    objects->addObject(getCreateBtn(2133, 4));
    objects->addObject(getCreateBtn(2134, 4));
    objects->addObject(getCreateBtn(2135, 4));
    objects->addObject(getCreateBtn(2136, 4));
    objects->addObject(getCreateBtn(2137, 4));
    objects->addObject(getCreateBtn(2138, 4));
    objects->addObject(getCreateBtn(2139, 4));
    objects->addObject(getCreateBtn(2140, 4));
    objects->addObject(getCreateBtn(2141, 4));
    objects->addObject(getCreateBtn(2142, 4));
    objects->addObject(getCreateBtn(2143, 4));
    objects->addObject(getCreateBtn(2144, 4));
    objects->addObject(getCreateBtn(2145, 4));
    objects->addObject(getCreateBtn(2146, 4));
    objects->addObject(getCreateBtn(2147, 4));
    objects->addObject(getCreateBtn(2148, 4));
    objects->addObject(getCreateBtn(2149, 4));
    objects->addObject(getCreateBtn(2150, 4));
    objects->addObject(getCreateBtn(2151, 4));
    objects->addObject(getCreateBtn(2152, 4));
    objects->addObject(getCreateBtn(2153, 4));
    objects->addObject(getCreateBtn(2626, 4));
    objects->addObject(getCreateBtn(2154, 4));
    objects->addObject(getCreateBtn(2155, 4));
    objects->addObject(getCreateBtn(2156, 4));
    objects->addObject(getCreateBtn(2157, 4));
    objects->addObject(getCreateBtn(2158, 4));
    objects->addObject(getCreateBtn(2159, 4));
    objects->addObject(getCreateBtn(2160, 4));
    objects->addObject(getCreateBtn(2161, 4));
    objects->addObject(getCreateBtn(2162, 4));
    objects->addObject(getCreateBtn(2163, 4));
    objects->addObject(getCreateBtn(2164, 4));
    objects->addObject(getCreateBtn(2165, 4));
    objects->addObject(getCreateBtn(2166, 4));
    objects->addObject(getCreateBtn(2167, 4));
    objects->addObject(getCreateBtn(2168, 4));
    objects->addObject(getCreateBtn(2169, 4));
    objects->addObject(getCreateBtn(2170, 4));
    objects->addObject(getCreateBtn(2171, 4));
    objects->addObject(getCreateBtn(2172, 4));
    objects->addObject(getCreateBtn(2173, 4));
    objects->addObject(getCreateBtn(2174, 4));
    objects->addObject(getCreateBtn(2175, 4));
    objects->addObject(getCreateBtn(2176, 4));
    objects->addObject(getCreateBtn(2177, 4));
    objects->addObject(getCreateBtn(2178, 4));
    objects->addObject(getCreateBtn(2179, 4));
    objects->addObject(getCreateBtn(2180, 4));
    objects->addObject(getCreateBtn(2181, 4));
    objects->addObject(getCreateBtn(2182, 4));
    objects->addObject(getCreateBtn(2183, 4));
    objects->addObject(getCreateBtn(2184, 4));
    objects->addObject(getCreateBtn(2459, 4));
    objects->addObject(getCreateBtn(2460, 4));
    objects->addObject(getCreateBtn(2461, 4));
    objects->addObject(getCreateBtn(2185, 4));
    objects->addObject(getCreateBtn(2186, 4));
    objects->addObject(getCreateBtn(2187, 4));
    objects->addObject(getCreateBtn(2188, 4));
    objects->addObject(getCreateBtn(2189, 4));
    objects->addObject(getCreateBtn(2190, 4));
    objects->addObject(getCreateBtn(2191, 4));
    objects->addObject(getCreateBtn(2192, 4));
    objects->addObject(getCreateBtn(2193, 4));
    objects->addObject(getCreateBtn(2194, 4));
    objects->addObject(getCreateBtn(2195, 4));
    objects->addObject(getCreateBtn(2196, 4));
    objects->addObject(getCreateBtn(2197, 4));
    objects->addObject(getCreateBtn(2198, 4));
    objects->addObject(getCreateBtn(2199, 4));
    objects->addObject(getCreateBtn(2200, 4));
    objects->addObject(getCreateBtn(2201, 4));
    objects->addObject(getCreateBtn(2202, 4));
    objects->addObject(getCreateBtn(2203, 4));
    objects->addObject(getCreateBtn(2204, 4));
    objects->addObject(getCreateBtn(2205, 4));
    objects->addObject(getCreateBtn(2206, 4));
    objects->addObject(getCreateBtn(2207, 4));
    objects->addObject(getCreateBtn(2208, 4));
    objects->addObject(getCreateBtn(2209, 4));
    objects->addObject(getCreateBtn(2210, 4));
    objects->addObject(getCreateBtn(2211, 4));
    objects->addObject(getCreateBtn(2212, 4));
    objects->addObject(getCreateBtn(2213, 4));
    objects->addObject(getCreateBtn(2214, 4));
    objects->addObject(getCreateBtn(2215, 4));
    objects->addObject(getCreateBtn(2216, 4));
    objects->addObject(getCreateBtn(2217, 4));
    objects->addObject(getCreateBtn(2218, 4));
    objects->addObject(getCreateBtn(2219, 4));
    objects->addObject(getCreateBtn(2220, 4));
    objects->addObject(getCreateBtn(2221, 4));
    objects->addObject(getCreateBtn(2224, 4));
    objects->addObject(getCreateBtn(2225, 4));
    objects->addObject(getCreateBtn(2226, 4));
    objects->addObject(getCreateBtn(2227, 4));
    objects->addObject(getCreateBtn(2228, 4));
    objects->addObject(getCreateBtn(2229, 4));
    objects->addObject(getCreateBtn(2230, 4));
    objects->addObject(getCreateBtn(2231, 4));
    objects->addObject(getCreateBtn(2232, 4));
    objects->addObject(getCreateBtn(2233, 4));
    objects->addObject(getCreateBtn(2234, 4));
    objects->addObject(getCreateBtn(2235, 4));
    objects->addObject(getCreateBtn(2236, 4));
    objects->addObject(getCreateBtn(2237, 4));
    objects->addObject(getCreateBtn(2238, 4));
    objects->addObject(getCreateBtn(3540, 4));
    objects->addObject(getCreateBtn(3541, 4));
    objects->addObject(getCreateBtn(3542, 4));
    objects->addObject(getCreateBtn(3543, 4));
    objects->addObject(getCreateBtn(2239, 4));
    objects->addObject(getCreateBtn(2240, 4));
    objects->addObject(getCreateBtn(2241, 4));
    objects->addObject(getCreateBtn(2242, 4));
    objects->addObject(getCreateBtn(2243, 4));
    objects->addObject(getCreateBtn(2244, 4));
    objects->addObject(getCreateBtn(2249, 4));
    objects->addObject(getCreateBtn(2250, 4));
    objects->addObject(getCreateBtn(2251, 4));
    objects->addObject(getCreateBtn(2252, 4));
    objects->addObject(getCreateBtn(2253, 4));
    objects->addObject(getCreateBtn(2254, 4));
    objects->addObject(getCreateBtn(2255, 4));
    objects->addObject(getCreateBtn(2256, 4));
    objects->addObject(getCreateBtn(2257, 4));
    objects->addObject(getCreateBtn(2258, 4));
    objects->addObject(getCreateBtn(2259, 4));
    objects->addObject(getCreateBtn(2260, 4));
    objects->addObject(getCreateBtn(2261, 4));
    objects->addObject(getCreateBtn(2262, 4));
    objects->addObject(getCreateBtn(2263, 4));
    objects->addObject(getCreateBtn(2264, 4));
    objects->addObject(getCreateBtn(2265, 4));
    objects->addObject(getCreateBtn(2266, 4));
    objects->addObject(getCreateBtn(2267, 4));
    objects->addObject(getCreateBtn(2268, 4));
    objects->addObject(getCreateBtn(2269, 4));
    objects->addObject(getCreateBtn(2270, 4));
    objects->addObject(getCreateBtn(2271, 4));
    objects->addObject(getCreateBtn(2272, 4));
    objects->addObject(getCreateBtn(2273, 4));
    objects->addObject(getCreateBtn(2274, 4));
    objects->addObject(getCreateBtn(2275, 4));
    objects->addObject(getCreateBtn(2276, 4));
    objects->addObject(getCreateBtn(2277, 4));
    objects->addObject(getCreateBtn(2278, 4));
    objects->addObject(getCreateBtn(2279, 4));
    objects->addObject(getCreateBtn(2280, 4));
    objects->addObject(getCreateBtn(2281, 4));
    objects->addObject(getCreateBtn(2282, 4));
    objects->addObject(getCreateBtn(2283, 4));
    objects->addObject(getCreateBtn(2284, 4));
    objects->addObject(getCreateBtn(2285, 4));
    objects->addObject(getCreateBtn(2286, 4));
    objects->addObject(getCreateBtn(2287, 4));
    objects->addObject(getCreateBtn(2288, 4));
    objects->addObject(getCreateBtn(2289, 4));
    objects->addObject(getCreateBtn(2290, 4));
    objects->addObject(getCreateBtn(2291, 4));
    objects->addObject(getCreateBtn(2292, 4));
    objects->addObject(getCreateBtn(2293, 4));
    objects->addObject(getCreateBtn(2294, 4));
    objects->addObject(getCreateBtn(2295, 4));
    objects->addObject(getCreateBtn(2296, 4));
    objects->addObject(getCreateBtn(2297, 4));
    objects->addObject(getCreateBtn(2298, 4));
    objects->addObject(getCreateBtn(2299, 4));
    objects->addObject(getCreateBtn(2300, 4));
    objects->addObject(getCreateBtn(2301, 4));
    objects->addObject(getCreateBtn(2302, 4));
    objects->addObject(getCreateBtn(2303, 4));
    objects->addObject(getCreateBtn(2304, 4));
    objects->addObject(getCreateBtn(2305, 4));
    objects->addObject(getCreateBtn(2306, 4));
    objects->addObject(getCreateBtn(2307, 4));
    objects->addObject(getCreateBtn(2308, 4));
    objects->addObject(getCreateBtn(2309, 4));
    objects->addObject(getCreateBtn(2310, 4));
    objects->addObject(getCreateBtn(2311, 4));
    objects->addObject(getCreateBtn(2312, 4));
    objects->addObject(getCreateBtn(2313, 4));
    objects->addObject(getCreateBtn(2314, 4));
    objects->addObject(getCreateBtn(2315, 4));
    objects->addObject(getCreateBtn(2316, 4));
    objects->addObject(getCreateBtn(2317, 4));
    objects->addObject(getCreateBtn(2318, 4));
    objects->addObject(getCreateBtn(2319, 4));
    objects->addObject(getCreateBtn(2320, 4));
    objects->addObject(getCreateBtn(2321, 4));
    objects->addObject(getCreateBtn(2322, 4));
    objects->addObject(getCreateBtn(2323, 4));
    objects->addObject(getCreateBtn(2324, 4));
    objects->addObject(getCreateBtn(2325, 4));
    objects->addObject(getCreateBtn(2326, 4));
    objects->addObject(getCreateBtn(2327, 4));
    objects->addObject(getCreateBtn(2328, 4));
    objects->addObject(getCreateBtn(2329, 4));
    objects->addObject(getCreateBtn(2330, 4));
    objects->addObject(getCreateBtn(2331, 4));
    objects->addObject(getCreateBtn(2332, 4));
    objects->addObject(getCreateBtn(2333, 4));
    objects->addObject(getCreateBtn(2334, 4));
    objects->addObject(getCreateBtn(2335, 4));
    objects->addObject(getCreateBtn(2336, 4));
    objects->addObject(getCreateBtn(2337, 4));
    objects->addObject(getCreateBtn(2338, 4));
    objects->addObject(getCreateBtn(2339, 4));
    objects->addObject(getCreateBtn(2340, 4));
    objects->addObject(getCreateBtn(2341, 4));
    objects->addObject(getCreateBtn(2342, 4));
    objects->addObject(getCreateBtn(2343, 4));
    objects->addObject(getCreateBtn(2344, 4));
    objects->addObject(getCreateBtn(2345, 4));
    objects->addObject(getCreateBtn(2346, 4));
    objects->addObject(getCreateBtn(2347, 4));
    objects->addObject(getCreateBtn(2348, 4));
    objects->addObject(getCreateBtn(2349, 4));
    objects->addObject(getCreateBtn(2350, 4));
    objects->addObject(getCreateBtn(2351, 4));
    objects->addObject(getCreateBtn(2352, 4));
    objects->addObject(getCreateBtn(2353, 4));
    objects->addObject(getCreateBtn(2354, 4));
    objects->addObject(getCreateBtn(2355, 4));
    objects->addObject(getCreateBtn(2356, 4));
    objects->addObject(getCreateBtn(2357, 4));
    objects->addObject(getCreateBtn(2358, 4));
    objects->addObject(getCreateBtn(2359, 4));
    objects->addObject(getCreateBtn(2360, 4));
    objects->addObject(getCreateBtn(2361, 4));
    objects->addObject(getCreateBtn(2362, 4));
    objects->addObject(getCreateBtn(2363, 4));
    objects->addObject(getCreateBtn(2364, 4));
    objects->addObject(getCreateBtn(2365, 4));
    objects->addObject(getCreateBtn(2366, 4));
    objects->addObject(getCreateBtn(2367, 4));
    objects->addObject(getCreateBtn(2368, 4));
    objects->addObject(getCreateBtn(2369, 4));
    objects->addObject(getCreateBtn(2370, 4));
    objects->addObject(getCreateBtn(2371, 4));
    objects->addObject(getCreateBtn(2372, 4));
    objects->addObject(getCreateBtn(2373, 4));
    objects->addObject(getCreateBtn(2633, 4));
    objects->addObject(getCreateBtn(2374, 4));
    objects->addObject(getCreateBtn(2375, 4));
    objects->addObject(getCreateBtn(2376, 4));
    objects->addObject(getCreateBtn(2377, 4));
    objects->addObject(getCreateBtn(2378, 4));
    objects->addObject(getCreateBtn(2379, 4));
    objects->addObject(getCreateBtn(2380, 4));
    objects->addObject(getCreateBtn(2384, 4));
    objects->addObject(getCreateBtn(2381, 4));
    objects->addObject(getCreateBtn(2382, 4));
    objects->addObject(getCreateBtn(2383, 4));
    objects->addObject(getCreateBtn(2385, 4));
    objects->addObject(getCreateBtn(2386, 4));
    objects->addObject(getCreateBtn(2387, 4));
    objects->addObject(getCreateBtn(2388, 4));
    objects->addObject(getCreateBtn(2389, 4));
    objects->addObject(getCreateBtn(2390, 4));
    objects->addObject(getCreateBtn(2391, 4));
    objects->addObject(getCreateBtn(2392, 4));
    objects->addObject(getCreateBtn(2634, 4));
    objects->addObject(getCreateBtn(2635, 4));
    objects->addObject(getCreateBtn(2636, 4));
    objects->addObject(getCreateBtn(2393, 4));
    objects->addObject(getCreateBtn(2394, 4));
    objects->addObject(getCreateBtn(2395, 4));
    objects->addObject(getCreateBtn(2396, 4));
    objects->addObject(getCreateBtn(2397, 4));
    objects->addObject(getCreateBtn(2398, 4));
    objects->addObject(getCreateBtn(2399, 4));
    objects->addObject(getCreateBtn(2400, 4));
    objects->addObject(getCreateBtn(2401, 4));
    objects->addObject(getCreateBtn(2402, 4));
    objects->addObject(getCreateBtn(2403, 4));
    objects->addObject(getCreateBtn(2404, 4));
    objects->addObject(getCreateBtn(2405, 4));
    objects->addObject(getCreateBtn(2406, 4));
    objects->addObject(getCreateBtn(2407, 4));
    objects->addObject(getCreateBtn(2408, 4));
    objects->addObject(getCreateBtn(2409, 4));
    objects->addObject(getCreateBtn(2410, 4));
    objects->addObject(getCreateBtn(2411, 4));
    objects->addObject(getCreateBtn(2412, 4));
    objects->addObject(getCreateBtn(2413, 4));
    objects->addObject(getCreateBtn(2414, 4));
    objects->addObject(getCreateBtn(2627, 4));
    objects->addObject(getCreateBtn(2696, 4));
    objects->addObject(getCreateBtn(2697, 4));
    objects->addObject(getCreateBtn(2698, 4));
    objects->addObject(getCreateBtn(2699, 4));
    objects->addObject(getCreateBtn(2700, 4));
    objects->addObject(getCreateBtn(2415, 4));
    objects->addObject(getCreateBtn(2628, 4));
    objects->addObject(getCreateBtn(2416, 4));
    objects->addObject(getCreateBtn(2417, 4));
    objects->addObject(getCreateBtn(2418, 4));
    objects->addObject(getCreateBtn(2419, 4));
    objects->addObject(getCreateBtn(2420, 4));
    objects->addObject(getCreateBtn(2421, 4));
    objects->addObject(getCreateBtn(2422, 4));
    objects->addObject(getCreateBtn(2423, 4));
    objects->addObject(getCreateBtn(2424, 4));
    objects->addObject(getCreateBtn(2425, 4));
    objects->addObject(getCreateBtn(2426, 4));
    objects->addObject(getCreateBtn(2427, 4));
    objects->addObject(getCreateBtn(2428, 4));
    objects->addObject(getCreateBtn(2429, 4));
    objects->addObject(getCreateBtn(2430, 4));
    objects->addObject(getCreateBtn(2462, 4));
    objects->addObject(getCreateBtn(4368, 4));
    objects->addObject(getCreateBtn(2463, 4));
    objects->addObject(getCreateBtn(4369, 4));
    objects->addObject(getCreateBtn(2464, 4));
    objects->addObject(getCreateBtn(4370, 4));
    objects->addObject(getCreateBtn(2465, 4));
    objects->addObject(getCreateBtn(4371, 4));
    objects->addObject(getCreateBtn(2466, 4));
    objects->addObject(getCreateBtn(2467, 4));
    objects->addObject(getCreateBtn(4372, 4));
    objects->addObject(getCreateBtn(2468, 4));
    objects->addObject(getCreateBtn(4373, 4));
    objects->addObject(getCreateBtn(2469, 4));
    objects->addObject(getCreateBtn(4374, 4));
    objects->addObject(getCreateBtn(2470, 4));
    objects->addObject(getCreateBtn(4375, 4));
    objects->addObject(getCreateBtn(2471, 4));
    objects->addObject(getCreateBtn(4376, 4));
    objects->addObject(getCreateBtn(2472, 4));
    objects->addObject(getCreateBtn(4377, 4));
    objects->addObject(getCreateBtn(2473, 4));
    objects->addObject(getCreateBtn(4378, 4));
    objects->addObject(getCreateBtn(2474, 4));
    objects->addObject(getCreateBtn(4379, 4));
    objects->addObject(getCreateBtn(2475, 4));
    objects->addObject(getCreateBtn(2476, 4));
    objects->addObject(getCreateBtn(2477, 4));
    objects->addObject(getCreateBtn(2478, 4));
    objects->addObject(getCreateBtn(2479, 4));
    objects->addObject(getCreateBtn(2480, 4));
    objects->addObject(getCreateBtn(2481, 4));
    objects->addObject(getCreateBtn(2482, 4));
    objects->addObject(getCreateBtn(2483, 4));
    objects->addObject(getCreateBtn(2484, 4));
    objects->addObject(getCreateBtn(2485, 4));
    objects->addObject(getCreateBtn(2486, 4));
    objects->addObject(getCreateBtn(2487, 4));
    objects->addObject(getCreateBtn(2488, 4));
    objects->addObject(getCreateBtn(4380, 4));
    objects->addObject(getCreateBtn(2489, 4));
    objects->addObject(getCreateBtn(4381, 4));
    objects->addObject(getCreateBtn(2490, 4));
    objects->addObject(getCreateBtn(4382, 4));
    objects->addObject(getCreateBtn(2491, 4));
    objects->addObject(getCreateBtn(4383, 4));
    objects->addObject(getCreateBtn(2492, 4));
    objects->addObject(getCreateBtn(2493, 4));
    objects->addObject(getCreateBtn(2494, 4));
    objects->addObject(getCreateBtn(2495, 4));
    objects->addObject(getCreateBtn(2496, 4));
    objects->addObject(getCreateBtn(2497, 4));
    objects->addObject(getCreateBtn(2498, 4));
    objects->addObject(getCreateBtn(2499, 4));
    objects->addObject(getCreateBtn(2500, 4));
    objects->addObject(getCreateBtn(2501, 4));
    objects->addObject(getCreateBtn(2502, 4));
    objects->addObject(getCreateBtn(2503, 4));
    objects->addObject(getCreateBtn(2504, 4));
    objects->addObject(getCreateBtn(2505, 4));
    objects->addObject(getCreateBtn(2506, 4));
    objects->addObject(getCreateBtn(2507, 4));
    objects->addObject(getCreateBtn(2508, 4));
    objects->addObject(getCreateBtn(2509, 4));
    objects->addObject(getCreateBtn(2510, 4));
    objects->addObject(getCreateBtn(2511, 4));
    objects->addObject(getCreateBtn(2512, 4));
    objects->addObject(getCreateBtn(2513, 4));
    objects->addObject(getCreateBtn(2531, 4));
    objects->addObject(getCreateBtn(2514, 4));
    objects->addObject(getCreateBtn(2515, 4));
    objects->addObject(getCreateBtn(2516, 4));
    objects->addObject(getCreateBtn(2517, 4));
    objects->addObject(getCreateBtn(2518, 4));
    objects->addObject(getCreateBtn(2519, 4));
    objects->addObject(getCreateBtn(2520, 4));
    objects->addObject(getCreateBtn(2521, 4));
    objects->addObject(getCreateBtn(2522, 4));
    objects->addObject(getCreateBtn(2523, 4));
    objects->addObject(getCreateBtn(2524, 4));
    objects->addObject(getCreateBtn(2525, 4));
    objects->addObject(getCreateBtn(2526, 4));
    objects->addObject(getCreateBtn(2527, 4));
    objects->addObject(getCreateBtn(2528, 4));
    objects->addObject(getCreateBtn(2529, 4));
    objects->addObject(getCreateBtn(2530, 4));
    objects->addObject(getCreateBtn(2534, 4));
    objects->addObject(getCreateBtn(2535, 4));
    objects->addObject(getCreateBtn(2536, 4));
    objects->addObject(getCreateBtn(2537, 4));
    objects->addObject(getCreateBtn(2538, 4));
    objects->addObject(getCreateBtn(2539, 4));
    objects->addObject(getCreateBtn(2540, 4));
    objects->addObject(getCreateBtn(2541, 4));
    objects->addObject(getCreateBtn(2542, 4));
    objects->addObject(getCreateBtn(2543, 4));
    objects->addObject(getCreateBtn(2544, 4));
    objects->addObject(getCreateBtn(2545, 4));
    objects->addObject(getCreateBtn(2546, 4));
    objects->addObject(getCreateBtn(2547, 4));
    objects->addObject(getCreateBtn(2548, 4));
    objects->addObject(getCreateBtn(2549, 4));
    objects->addObject(getCreateBtn(2550, 4));
    objects->addObject(getCreateBtn(2551, 4));
    objects->addObject(getCreateBtn(2552, 4));
    objects->addObject(getCreateBtn(2553, 4));
    objects->addObject(getCreateBtn(2554, 4));
    objects->addObject(getCreateBtn(2631, 4));
    objects->addObject(getCreateBtn(2555, 4));
    objects->addObject(getCreateBtn(2556, 4));
    objects->addObject(getCreateBtn(2557, 4));
    objects->addObject(getCreateBtn(2558, 4));
    objects->addObject(getCreateBtn(2559, 4));
    objects->addObject(getCreateBtn(2560, 4));
    objects->addObject(getCreateBtn(2632, 4));
    objects->addObject(getCreateBtn(2561, 4));
    objects->addObject(getCreateBtn(2562, 4));
    objects->addObject(getCreateBtn(2563, 4));
    objects->addObject(getCreateBtn(2564, 4));
    objects->addObject(getCreateBtn(2565, 4));
    objects->addObject(getCreateBtn(2566, 4));
    objects->addObject(getCreateBtn(2567, 4));
    objects->addObject(getCreateBtn(2568, 4));
    objects->addObject(getCreateBtn(2569, 4));
    objects->addObject(getCreateBtn(2570, 4));
    objects->addObject(getCreateBtn(2571, 4));
    objects->addObject(getCreateBtn(2572, 4));
    objects->addObject(getCreateBtn(2573, 4));
    objects->addObject(getCreateBtn(2574, 4));
    objects->addObject(getCreateBtn(2575, 4));
    objects->addObject(getCreateBtn(2576, 4));
    objects->addObject(getCreateBtn(2577, 4));
    objects->addObject(getCreateBtn(2578, 4));
    objects->addObject(getCreateBtn(2579, 4));
    objects->addObject(getCreateBtn(2580, 4));
    objects->addObject(getCreateBtn(2581, 4));
    objects->addObject(getCreateBtn(2582, 4));
    objects->addObject(getCreateBtn(2583, 4));
    objects->addObject(getCreateBtn(2584, 4));
    objects->addObject(getCreateBtn(2585, 4));
    objects->addObject(getCreateBtn(2586, 4));
    objects->addObject(getCreateBtn(2587, 4));
    objects->addObject(getCreateBtn(2588, 4));
    objects->addObject(getCreateBtn(2589, 4));
    objects->addObject(getCreateBtn(2590, 4));
    objects->addObject(getCreateBtn(2591, 4));
    objects->addObject(getCreateBtn(2592, 4));
    objects->addObject(getCreateBtn(2593, 4));
    objects->addObject(getCreateBtn(2594, 4));
    objects->addObject(getCreateBtn(2595, 4));
    objects->addObject(getCreateBtn(2596, 4));
    objects->addObject(getCreateBtn(2597, 4));
    objects->addObject(getCreateBtn(2598, 4));
    objects->addObject(getCreateBtn(2599, 4));
    objects->addObject(getCreateBtn(2600, 4));
    objects->addObject(getCreateBtn(2601, 4));
    objects->addObject(getCreateBtn(2602, 4));
    objects->addObject(getCreateBtn(2603, 4));
    objects->addObject(getCreateBtn(2604, 4));
    objects->addObject(getCreateBtn(2693, 4));
    objects->addObject(getCreateBtn(2607, 4));
    objects->addObject(getCreateBtn(2608, 4));
    objects->addObject(getCreateBtn(2609, 4));
    objects->addObject(getCreateBtn(2610, 4));
    objects->addObject(getCreateBtn(2611, 4));
    objects->addObject(getCreateBtn(2612, 4));
    objects->addObject(getCreateBtn(2613, 4));
    objects->addObject(getCreateBtn(2614, 4));
    objects->addObject(getCreateBtn(2615, 4));
    objects->addObject(getCreateBtn(2616, 4));
    objects->addObject(getCreateBtn(2617, 4));
    objects->addObject(getCreateBtn(2618, 4));
    objects->addObject(getCreateBtn(2619, 4));
    objects->addObject(getCreateBtn(2620, 4));
    objects->addObject(getCreateBtn(2621, 4));
    objects->addObject(getCreateBtn(2622, 4));
    objects->addObject(getCreateBtn(2623, 4));
    objects->addObject(getCreateBtn(2624, 4));
    objects->addObject(getCreateBtn(2625, 4));
    objects->addObject(getCreateBtn(2637, 4));
    objects->addObject(getCreateBtn(2638, 4));
    objects->addObject(getCreateBtn(2639, 4));
    objects->addObject(getCreateBtn(2640, 4));
    objects->addObject(getCreateBtn(2641, 4));
    objects->addObject(getCreateBtn(2642, 4));
    objects->addObject(getCreateBtn(2643, 4));
    objects->addObject(getCreateBtn(2644, 4));
    objects->addObject(getCreateBtn(2645, 4));
    objects->addObject(getCreateBtn(2646, 4));
    objects->addObject(getCreateBtn(2647, 4));
    objects->addObject(getCreateBtn(2648, 4));
    objects->addObject(getCreateBtn(2649, 4));
    objects->addObject(getCreateBtn(2650, 4));
    objects->addObject(getCreateBtn(2651, 4));
    objects->addObject(getCreateBtn(2652, 4));
    objects->addObject(getCreateBtn(2653, 4));
    objects->addObject(getCreateBtn(2654, 4));
    objects->addObject(getCreateBtn(2655, 4));
    objects->addObject(getCreateBtn(2656, 4));
    objects->addObject(getCreateBtn(2657, 4));
    objects->addObject(getCreateBtn(2658, 4));
    objects->addObject(getCreateBtn(2659, 4));
    objects->addObject(getCreateBtn(2660, 4));
    objects->addObject(getCreateBtn(2661, 4));
    objects->addObject(getCreateBtn(2662, 4));
    objects->addObject(getCreateBtn(2663, 4));
    objects->addObject(getCreateBtn(2664, 4));
    objects->addObject(getCreateBtn(2665, 4));
    objects->addObject(getCreateBtn(2666, 4));
    objects->addObject(getCreateBtn(2667, 4));
    objects->addObject(getCreateBtn(2668, 4));
    objects->addObject(getCreateBtn(2669, 4));
    objects->addObject(getCreateBtn(2670, 4));
    objects->addObject(getCreateBtn(2671, 4));
    objects->addObject(getCreateBtn(2672, 4));
    objects->addObject(getCreateBtn(2673, 4));
    objects->addObject(getCreateBtn(2674, 4));
    objects->addObject(getCreateBtn(2675, 4));
    objects->addObject(getCreateBtn(2676, 4));
    objects->addObject(getCreateBtn(2677, 4));
    objects->addObject(getCreateBtn(2678, 4));
    objects->addObject(getCreateBtn(2679, 4));
    objects->addObject(getCreateBtn(2680, 4));
    objects->addObject(getCreateBtn(2681, 4));
    objects->addObject(getCreateBtn(2682, 4));
    objects->addObject(getCreateBtn(2683, 4));
    objects->addObject(getCreateBtn(2684, 4));
    objects->addObject(getCreateBtn(2685, 4));
    objects->addObject(getCreateBtn(2686, 4));
    objects->addObject(getCreateBtn(2687, 4));
    objects->addObject(getCreateBtn(2688, 4));
    objects->addObject(getCreateBtn(2689, 4));
    objects->addObject(getCreateBtn(2690, 4));
    objects->addObject(getCreateBtn(2691, 4));
    objects->addObject(getCreateBtn(2692, 4));
    objects->addObject(getCreateBtn(3101, 4));
    objects->addObject(getCreateBtn(3102, 4));
    objects->addObject(getCreateBtn(3103, 4));
    objects->addObject(getCreateBtn(3104, 4));
    objects->addObject(getCreateBtn(3105, 4));
    objects->addObject(getCreateBtn(3106, 4));
    objects->addObject(getCreateBtn(3107, 4));
    objects->addObject(getCreateBtn(3108, 4));
    objects->addObject(getCreateBtn(3109, 4));
    objects->addObject(getCreateBtn(3110, 4));
    objects->addObject(getCreateBtn(3111, 4));
    objects->addObject(getCreateBtn(3112, 4));
    objects->addObject(getCreateBtn(3113, 4));
    objects->addObject(getCreateBtn(3114, 4));
    objects->addObject(getCreateBtn(3115, 4));
    objects->addObject(getCreateBtn(3116, 4));
    objects->addObject(getCreateBtn(3117, 4));
    objects->addObject(getCreateBtn(3118, 4));
    objects->addObject(getCreateBtn(3122, 4));
    objects->addObject(getCreateBtn(3123, 4));
    objects->addObject(getCreateBtn(3124, 4));
    objects->addObject(getCreateBtn(3125, 4));
    objects->addObject(getCreateBtn(3126, 4));
    objects->addObject(getCreateBtn(3127, 4));
    objects->addObject(getCreateBtn(3128, 4));
    objects->addObject(getCreateBtn(3129, 4));
    objects->addObject(getCreateBtn(3130, 4));
    objects->addObject(getCreateBtn(3131, 4));
    objects->addObject(getCreateBtn(3132, 4));
    objects->addObject(getCreateBtn(3133, 4));
    objects->addObject(getCreateBtn(3134, 4));
    objects->addObject(getCreateBtn(3135, 4));
    objects->addObject(getCreateBtn(3136, 4));
    objects->addObject(getCreateBtn(3137, 4));
    objects->addObject(getCreateBtn(3138, 4));
    objects->addObject(getCreateBtn(3139, 4));
    objects->addObject(getCreateBtn(3140, 4));
    objects->addObject(getCreateBtn(3141, 4));
    objects->addObject(getCreateBtn(3142, 4));
    objects->addObject(getCreateBtn(3143, 4));
    objects->addObject(getCreateBtn(3144, 4));
    objects->addObject(getCreateBtn(3145, 4));
    objects->addObject(getCreateBtn(3146, 4));
    objects->addObject(getCreateBtn(3147, 4));
    objects->addObject(getCreateBtn(3148, 4));
    objects->addObject(getCreateBtn(3149, 4));
    objects->addObject(getCreateBtn(3150, 4));
    objects->addObject(getCreateBtn(3151, 4));
    objects->addObject(getCreateBtn(3152, 4));
    objects->addObject(getCreateBtn(3153, 4));
    objects->addObject(getCreateBtn(3154, 4));
    objects->addObject(getCreateBtn(3155, 4));
    objects->addObject(getCreateBtn(3156, 4));
    objects->addObject(getCreateBtn(3157, 4));
    objects->addObject(getCreateBtn(3158, 4));
    objects->addObject(getCreateBtn(3159, 4));
    objects->addObject(getCreateBtn(3160, 4));
    objects->addObject(getCreateBtn(3161, 4));
    objects->addObject(getCreateBtn(3162, 4));
    objects->addObject(getCreateBtn(3163, 4));
    objects->addObject(getCreateBtn(3164, 4));
    objects->addObject(getCreateBtn(3165, 4));
    objects->addObject(getCreateBtn(3166, 4));
    objects->addObject(getCreateBtn(3167, 4));
    objects->addObject(getCreateBtn(3168, 4));
    objects->addObject(getCreateBtn(3169, 4));
    objects->addObject(getCreateBtn(3170, 4));
    objects->addObject(getCreateBtn(3171, 4));
    objects->addObject(getCreateBtn(3172, 4));
    objects->addObject(getCreateBtn(3173, 4));
    objects->addObject(getCreateBtn(3174, 4));
    objects->addObject(getCreateBtn(3175, 4));
    objects->addObject(getCreateBtn(3176, 4));
    objects->addObject(getCreateBtn(3177, 4));
    objects->addObject(getCreateBtn(3178, 4));
    objects->addObject(getCreateBtn(3179, 4));
    objects->addObject(getCreateBtn(3180, 4));
    objects->addObject(getCreateBtn(3181, 4));
    objects->addObject(getCreateBtn(3182, 4));
    objects->addObject(getCreateBtn(3183, 4));
    objects->addObject(getCreateBtn(3184, 4));
    objects->addObject(getCreateBtn(3185, 4));
    objects->addObject(getCreateBtn(3186, 4));
    objects->addObject(getCreateBtn(3187, 4));
    objects->addObject(getCreateBtn(3188, 4));
    objects->addObject(getCreateBtn(3189, 4));
    objects->addObject(getCreateBtn(3190, 4));
    objects->addObject(getCreateBtn(3191, 4));
    objects->addObject(getCreateBtn(3192, 4));
    objects->addObject(getCreateBtn(3193, 4));
    objects->addObject(getCreateBtn(3194, 4));
    objects->addObject(getCreateBtn(3195, 4));
    objects->addObject(getCreateBtn(3196, 4));
    objects->addObject(getCreateBtn(3197, 4));
    objects->addObject(getCreateBtn(3198, 4));
    objects->addObject(getCreateBtn(3199, 4));
    objects->addObject(getCreateBtn(3200, 4));
    objects->addObject(getCreateBtn(3201, 4));
    objects->addObject(getCreateBtn(3202, 4));
    objects->addObject(getCreateBtn(3203, 4));
    objects->addObject(getCreateBtn(3204, 4));
    objects->addObject(getCreateBtn(3205, 4));
    objects->addObject(getCreateBtn(3206, 4));
    objects->addObject(getCreateBtn(3207, 4));
    objects->addObject(getCreateBtn(3208, 4));
    objects->addObject(getCreateBtn(3209, 4));
    objects->addObject(getCreateBtn(3210, 4));
    objects->addObject(getCreateBtn(3211, 4));
    objects->addObject(getCreateBtn(3212, 4));
    objects->addObject(getCreateBtn(3213, 4));
    objects->addObject(getCreateBtn(3214, 4));
    objects->addObject(getCreateBtn(3215, 4));
    objects->addObject(getCreateBtn(3216, 4));
    objects->addObject(getCreateBtn(3217, 4));
    objects->addObject(getCreateBtn(3218, 4));
    objects->addObject(getCreateBtn(3220, 4));
    objects->addObject(getCreateBtn(3221, 4));
    objects->addObject(getCreateBtn(3222, 4));
    objects->addObject(getCreateBtn(3223, 4));
    objects->addObject(getCreateBtn(3224, 4));
    objects->addObject(getCreateBtn(3225, 4));
    objects->addObject(getCreateBtn(3226, 4));
    objects->addObject(getCreateBtn(3227, 4));
    objects->addObject(getCreateBtn(3228, 4));
    objects->addObject(getCreateBtn(3229, 4));
    objects->addObject(getCreateBtn(3230, 4));
    objects->addObject(getCreateBtn(3231, 4));
    objects->addObject(getCreateBtn(3232, 4));
    objects->addObject(getCreateBtn(3233, 4));
    objects->addObject(getCreateBtn(3234, 4));
    objects->addObject(getCreateBtn(3235, 4));
    objects->addObject(getCreateBtn(3236, 4));
    objects->addObject(getCreateBtn(3237, 4));
    objects->addObject(getCreateBtn(3238, 4));
    objects->addObject(getCreateBtn(3239, 4));
    objects->addObject(getCreateBtn(3240, 4));
    objects->addObject(getCreateBtn(3241, 4));
    objects->addObject(getCreateBtn(3242, 4));
    objects->addObject(getCreateBtn(3243, 4));
    objects->addObject(getCreateBtn(3244, 4));
    objects->addObject(getCreateBtn(3245, 4));
    objects->addObject(getCreateBtn(3246, 4));
    objects->addObject(getCreateBtn(3247, 4));
    objects->addObject(getCreateBtn(3248, 4));
    objects->addObject(getCreateBtn(3249, 4));
    objects->addObject(getCreateBtn(3250, 4));
    objects->addObject(getCreateBtn(3251, 4));
    objects->addObject(getCreateBtn(3252, 4));
    objects->addObject(getCreateBtn(3253, 4));
    objects->addObject(getCreateBtn(3254, 4));
    objects->addObject(getCreateBtn(3255, 4));
    objects->addObject(getCreateBtn(3256, 4));
    objects->addObject(getCreateBtn(3257, 4));
    objects->addObject(getCreateBtn(3258, 4));
    objects->addObject(getCreateBtn(3259, 4));
    objects->addObject(getCreateBtn(3260, 4));
    objects->addObject(getCreateBtn(3261, 4));
    objects->addObject(getCreateBtn(3262, 4));
    objects->addObject(getCreateBtn(3263, 4));
    objects->addObject(getCreateBtn(3264, 4));
    objects->addObject(getCreateBtn(3265, 4));
    objects->addObject(getCreateBtn(3266, 4));
    objects->addObject(getCreateBtn(3267, 4));
    objects->addObject(getCreateBtn(3268, 4));
    objects->addObject(getCreateBtn(3269, 4));
    objects->addObject(getCreateBtn(3270, 4));
    objects->addObject(getCreateBtn(3271, 4));
    objects->addObject(getCreateBtn(3272, 4));
    objects->addObject(getCreateBtn(3273, 4));
    objects->addObject(getCreateBtn(3274, 4));
    objects->addObject(getCreateBtn(3275, 4));
    objects->addObject(getCreateBtn(3276, 4));
    objects->addObject(getCreateBtn(3277, 4));
    objects->addObject(getCreateBtn(3278, 4));
    objects->addObject(getCreateBtn(3279, 4));
    objects->addObject(getCreateBtn(3280, 4));
    objects->addObject(getCreateBtn(3281, 4));
    objects->addObject(getCreateBtn(3282, 4));
    objects->addObject(getCreateBtn(3283, 4));
    objects->addObject(getCreateBtn(3284, 4));
    objects->addObject(getCreateBtn(3285, 4));
    objects->addObject(getCreateBtn(3286, 4));
    objects->addObject(getCreateBtn(3287, 4));
    objects->addObject(getCreateBtn(3288, 4));
    objects->addObject(getCreateBtn(3289, 4));
    objects->addObject(getCreateBtn(3290, 4));
    objects->addObject(getCreateBtn(3291, 4));
    objects->addObject(getCreateBtn(3292, 4));
    objects->addObject(getCreateBtn(3293, 4));
    objects->addObject(getCreateBtn(3294, 4));
    objects->addObject(getCreateBtn(3295, 4));
    objects->addObject(getCreateBtn(3296, 4));
    objects->addObject(getCreateBtn(3297, 4));
    objects->addObject(getCreateBtn(3298, 4));
    objects->addObject(getCreateBtn(3299, 4));
    objects->addObject(getCreateBtn(3300, 4));
    objects->addObject(getCreateBtn(3301, 4));
    objects->addObject(getCreateBtn(3306, 4));
    objects->addObject(getCreateBtn(3307, 4));
    objects->addObject(getCreateBtn(3308, 4));
    objects->addObject(getCreateBtn(3309, 4));
    objects->addObject(getCreateBtn(3310, 4));
    objects->addObject(getCreateBtn(3311, 4));
    objects->addObject(getCreateBtn(3312, 4));
    objects->addObject(getCreateBtn(3318, 4));
    objects->addObject(getCreateBtn(3319, 4));
    objects->addObject(getCreateBtn(3320, 4));
    objects->addObject(getCreateBtn(3321, 4));
    objects->addObject(getCreateBtn(3322, 4));
    objects->addObject(getCreateBtn(3323, 4));
    objects->addObject(getCreateBtn(3324, 4));
    objects->addObject(getCreateBtn(3325, 4));
    objects->addObject(getCreateBtn(3313, 4));
    objects->addObject(getCreateBtn(3314, 4));
    objects->addObject(getCreateBtn(3315, 4));
    objects->addObject(getCreateBtn(3316, 4));
    objects->addObject(getCreateBtn(3317, 4));
    objects->addObject(getCreateBtn(3326, 4));
    objects->addObject(getCreateBtn(3327, 4));
    objects->addObject(getCreateBtn(3328, 4));
    objects->addObject(getCreateBtn(3329, 4));
    objects->addObject(getCreateBtn(3330, 4));
    objects->addObject(getCreateBtn(3331, 4));
    objects->addObject(getCreateBtn(3332, 4));
    objects->addObject(getCreateBtn(3333, 4));
    objects->addObject(getCreateBtn(3335, 4));
    objects->addObject(getCreateBtn(3336, 4));
    objects->addObject(getCreateBtn(3337, 4));
    objects->addObject(getCreateBtn(3338, 4));
    objects->addObject(getCreateBtn(3339, 4));
    objects->addObject(getCreateBtn(3340, 4));
    objects->addObject(getCreateBtn(3341, 4));
    objects->addObject(getCreateBtn(3342, 4));
    objects->addObject(getCreateBtn(3343, 4));
    objects->addObject(getCreateBtn(3344, 4));
    objects->addObject(getCreateBtn(3345, 4));
    objects->addObject(getCreateBtn(3346, 4));
    objects->addObject(getCreateBtn(3347, 4));
    objects->addObject(getCreateBtn(3348, 4));
    objects->addObject(getCreateBtn(3349, 4));
    objects->addObject(getCreateBtn(3350, 4));
    objects->addObject(getCreateBtn(3351, 4));
    objects->addObject(getCreateBtn(3352, 4));
    objects->addObject(getCreateBtn(3353, 4));
    objects->addObject(getCreateBtn(3354, 4));
    objects->addObject(getCreateBtn(3355, 4));
    objects->addObject(getCreateBtn(3356, 4));
    objects->addObject(getCreateBtn(3357, 4));
    objects->addObject(getCreateBtn(3358, 4));
    objects->addObject(getCreateBtn(3359, 4));
    objects->addObject(getCreateBtn(3360, 4));
    objects->addObject(getCreateBtn(3361, 4));
    objects->addObject(getCreateBtn(3362, 4));
    objects->addObject(getCreateBtn(3363, 4));
    objects->addObject(getCreateBtn(3364, 4));
    objects->addObject(getCreateBtn(3365, 4));
    objects->addObject(getCreateBtn(3366, 4));
    objects->addObject(getCreateBtn(3367, 4));
    objects->addObject(getCreateBtn(3368, 4));
    objects->addObject(getCreateBtn(3369, 4));
    objects->addObject(getCreateBtn(3370, 4));
    objects->addObject(getCreateBtn(3371, 4));
    objects->addObject(getCreateBtn(3372, 4));
    objects->addObject(getCreateBtn(3373, 4));
    objects->addObject(getCreateBtn(3374, 4));
    objects->addObject(getCreateBtn(3375, 4));
    objects->addObject(getCreateBtn(3376, 4));
    objects->addObject(getCreateBtn(3377, 4));
    objects->addObject(getCreateBtn(3378, 4));
    objects->addObject(getCreateBtn(3379, 4));
    objects->addObject(getCreateBtn(3380, 4));
    objects->addObject(getCreateBtn(3381, 4));
    objects->addObject(getCreateBtn(3382, 4));
    objects->addObject(getCreateBtn(3383, 4));
    objects->addObject(getCreateBtn(3384, 4));
    objects->addObject(getCreateBtn(3385, 4));
    objects->addObject(getCreateBtn(3386, 4));
    objects->addObject(getCreateBtn(3387, 4));
    objects->addObject(getCreateBtn(3388, 4));
    objects->addObject(getCreateBtn(3389, 4));
    objects->addObject(getCreateBtn(3390, 4));
    objects->addObject(getCreateBtn(3391, 4));
    objects->addObject(getCreateBtn(3392, 4));
    objects->addObject(getCreateBtn(3393, 4));
    objects->addObject(getCreateBtn(3394, 4));
    objects->addObject(getCreateBtn(3395, 4));
    objects->addObject(getCreateBtn(3396, 4));
    objects->addObject(getCreateBtn(3397, 4));
    objects->addObject(getCreateBtn(3398, 4));
    objects->addObject(getCreateBtn(3399, 4));
    objects->addObject(getCreateBtn(3400, 4));
    objects->addObject(getCreateBtn(3401, 4));
    objects->addObject(getCreateBtn(3402, 4));
    objects->addObject(getCreateBtn(3403, 4));
    objects->addObject(getCreateBtn(3404, 4));
    objects->addObject(getCreateBtn(3405, 4));
    objects->addObject(getCreateBtn(3406, 4));
    objects->addObject(getCreateBtn(3407, 4));
    objects->addObject(getCreateBtn(3408, 4));
    objects->addObject(getCreateBtn(3409, 4));
    objects->addObject(getCreateBtn(3410, 4));
    objects->addObject(getCreateBtn(3411, 4));
    objects->addObject(getCreateBtn(3412, 4));
    objects->addObject(getCreateBtn(3413, 4));
    objects->addObject(getCreateBtn(3414, 4));
    objects->addObject(getCreateBtn(3415, 4));
    objects->addObject(getCreateBtn(3416, 4));
    objects->addObject(getCreateBtn(3417, 4));
    objects->addObject(getCreateBtn(3418, 4));
    objects->addObject(getCreateBtn(3419, 4));
    objects->addObject(getCreateBtn(3420, 4));
    objects->addObject(getCreateBtn(3421, 4));
    objects->addObject(getCreateBtn(3422, 4));
    objects->addObject(getCreateBtn(3423, 4));
    objects->addObject(getCreateBtn(3424, 4));
    objects->addObject(getCreateBtn(3425, 4));
    objects->addObject(getCreateBtn(3426, 4));
    objects->addObject(getCreateBtn(3427, 4));
    objects->addObject(getCreateBtn(3428, 4));
    objects->addObject(getCreateBtn(3429, 4));
    objects->addObject(getCreateBtn(3430, 4));
    objects->addObject(getCreateBtn(3431, 4));
    objects->addObject(getCreateBtn(3432, 4));
    objects->addObject(getCreateBtn(3433, 4));
    objects->addObject(getCreateBtn(3434, 4));
    objects->addObject(getCreateBtn(3435, 4));
    objects->addObject(getCreateBtn(3436, 4));
    objects->addObject(getCreateBtn(3437, 4));
    objects->addObject(getCreateBtn(3438, 4));
    objects->addObject(getCreateBtn(3441, 4));
    objects->addObject(getCreateBtn(3442, 4));
    objects->addObject(getCreateBtn(3439, 4));
    objects->addObject(getCreateBtn(3440, 4));
    objects->addObject(getCreateBtn(3443, 4));
    objects->addObject(getCreateBtn(3444, 4));
    objects->addObject(getCreateBtn(3445, 4));
    objects->addObject(getCreateBtn(3446, 4));
    objects->addObject(getCreateBtn(3447, 4));
    objects->addObject(getCreateBtn(3448, 4));
    objects->addObject(getCreateBtn(3449, 4));
    objects->addObject(getCreateBtn(3450, 4));
    objects->addObject(getCreateBtn(3334, 4));
    objects->addObject(getCreateBtn(3451, 4));
    objects->addObject(getCreateBtn(3538, 4));
    objects->addObject(getCreateBtn(3452, 4));
    objects->addObject(getCreateBtn(3453, 4));
    objects->addObject(getCreateBtn(3539, 4));
    objects->addObject(getCreateBtn(3454, 4));
    objects->addObject(getCreateBtn(3455, 4));
    objects->addObject(getCreateBtn(3456, 4));
    objects->addObject(getCreateBtn(3457, 4));
    objects->addObject(getCreateBtn(3458, 4));
    objects->addObject(getCreateBtn(3459, 4));
    objects->addObject(getCreateBtn(3460, 4));
    objects->addObject(getCreateBtn(3461, 4));
    objects->addObject(getCreateBtn(3462, 4));
    objects->addObject(getCreateBtn(3463, 4));
    objects->addObject(getCreateBtn(3464, 4));
    objects->addObject(getCreateBtn(3465, 4));
    objects->addObject(getCreateBtn(3466, 4));
    objects->addObject(getCreateBtn(3467, 4));
    objects->addObject(getCreateBtn(3468, 4));
    objects->addObject(getCreateBtn(3469, 4));
    objects->addObject(getCreateBtn(3470, 4));
    objects->addObject(getCreateBtn(3471, 4));
    objects->addObject(getCreateBtn(3472, 4));
    objects->addObject(getCreateBtn(3473, 4));
    objects->addObject(getCreateBtn(3474, 4));
    objects->addObject(getCreateBtn(3475, 4));
    objects->addObject(getCreateBtn(3476, 4));
    objects->addObject(getCreateBtn(3477, 4));
    objects->addObject(getCreateBtn(3478, 4));
    objects->addObject(getCreateBtn(3479, 4));
    objects->addObject(getCreateBtn(3480, 4));
    objects->addObject(getCreateBtn(3481, 4));
    objects->addObject(getCreateBtn(3494, 4));
    objects->addObject(getCreateBtn(3495, 4));
    objects->addObject(getCreateBtn(3496, 4));
    objects->addObject(getCreateBtn(3497, 4));
    objects->addObject(getCreateBtn(3498, 4));
    objects->addObject(getCreateBtn(3499, 4));
    objects->addObject(getCreateBtn(3500, 4));
    objects->addObject(getCreateBtn(3501, 4));
    objects->addObject(getCreateBtn(3502, 4));
    objects->addObject(getCreateBtn(3503, 4));
    objects->addObject(getCreateBtn(3504, 4));
    objects->addObject(getCreateBtn(3505, 4));
    objects->addObject(getCreateBtn(3506, 4));
    objects->addObject(getCreateBtn(3507, 4));
    objects->addObject(getCreateBtn(3508, 4));
    objects->addObject(getCreateBtn(3509, 4));
    objects->addObject(getCreateBtn(3510, 4));
    objects->addObject(getCreateBtn(3511, 4));
    objects->addObject(getCreateBtn(3512, 4));
    objects->addObject(getCreateBtn(3513, 4));
    objects->addObject(getCreateBtn(3514, 4));
    objects->addObject(getCreateBtn(3515, 4));
    objects->addObject(getCreateBtn(3516, 4));
    objects->addObject(getCreateBtn(3517, 4));
    objects->addObject(getCreateBtn(3518, 4));
    objects->addObject(getCreateBtn(3519, 4));
    objects->addObject(getCreateBtn(3520, 4));
    objects->addObject(getCreateBtn(3521, 4));
    objects->addObject(getCreateBtn(3522, 4));
    objects->addObject(getCreateBtn(3523, 4));
    objects->addObject(getCreateBtn(3524, 4));
    objects->addObject(getCreateBtn(3525, 4));
    objects->addObject(getCreateBtn(3526, 4));
    objects->addObject(getCreateBtn(3527, 4));
    objects->addObject(getCreateBtn(3528, 4));
    objects->addObject(getCreateBtn(3529, 4));
    objects->addObject(getCreateBtn(3530, 4));
    objects->addObject(getCreateBtn(3531, 4));
    objects->addObject(getCreateBtn(3532, 4));
    objects->addObject(getCreateBtn(3533, 4));
    objects->addObject(getCreateBtn(3534, 4));
    objects->addObject(getCreateBtn(3535, 4));
    objects->addObject(getCreateBtn(3536, 4));
    objects->addObject(getCreateBtn(3537, 4));
    objects->addObject(getCreateBtn(3544, 4));
    objects->addObject(getCreateBtn(3545, 4));
    objects->addObject(getCreateBtn(3546, 4));
    objects->addObject(getCreateBtn(3547, 4));
    objects->addObject(getCreateBtn(3548, 4));
    objects->addObject(getCreateBtn(3549, 4));
    objects->addObject(getCreateBtn(3550, 4));
    objects->addObject(getCreateBtn(3551, 4));
    objects->addObject(getCreateBtn(3552, 4));
    objects->addObject(getCreateBtn(3553, 4));
    objects->addObject(getCreateBtn(3554, 4));
    objects->addObject(getCreateBtn(3555, 4));
    objects->addObject(getCreateBtn(3556, 4));
    objects->addObject(getCreateBtn(3557, 4));
    objects->addObject(getCreateBtn(3558, 4));
    objects->addObject(getCreateBtn(3559, 4));
    objects->addObject(getCreateBtn(3560, 4));
    objects->addObject(getCreateBtn(3561, 4));
    objects->addObject(getCreateBtn(3562, 4));
    objects->addObject(getCreateBtn(3563, 4));
    objects->addObject(getCreateBtn(3564, 4));
    objects->addObject(getCreateBtn(3565, 4));
    objects->addObject(getCreateBtn(3566, 4));
    objects->addObject(getCreateBtn(3567, 4));
    objects->addObject(getCreateBtn(3568, 4));
    objects->addObject(getCreateBtn(3569, 4));
    objects->addObject(getCreateBtn(3570, 4));
    objects->addObject(getCreateBtn(3571, 4));
    objects->addObject(getCreateBtn(3572, 4));
    objects->addObject(getCreateBtn(3573, 4));
    objects->addObject(getCreateBtn(3574, 4));
    objects->addObject(getCreateBtn(3575, 4));
    objects->addObject(getCreateBtn(3576, 4));
    objects->addObject(getCreateBtn(3577, 4));
    objects->addObject(getCreateBtn(3578, 4));
    objects->addObject(getCreateBtn(3579, 4));
    objects->addObject(getCreateBtn(3580, 4));
    objects->addObject(getCreateBtn(3581, 4));
    objects->addObject(getCreateBtn(3582, 4));
    objects->addObject(getCreateBtn(3583, 4));
    objects->addObject(getCreateBtn(3584, 4));
    objects->addObject(getCreateBtn(3585, 4));
    objects->addObject(getCreateBtn(3586, 4));
    objects->addObject(getCreateBtn(3587, 4));
    objects->addObject(getCreateBtn(3588, 4));
    objects->addObject(getCreateBtn(3589, 4));
    objects->addObject(getCreateBtn(3590, 4));
    objects->addObject(getCreateBtn(3591, 4));
    objects->addObject(getCreateBtn(3592, 4));
    objects->addObject(getCreateBtn(3593, 4));
    objects->addObject(getCreateBtn(3594, 4));
    objects->addObject(getCreateBtn(3595, 4));
    objects->addObject(getCreateBtn(3596, 4));
    objects->addObject(getCreateBtn(3597, 4));
    objects->addObject(getCreateBtn(3598, 4));
    objects->addObject(getCreateBtn(3599, 4));
    objects->addObject(getCreateBtn(3700, 4));
    objects->addObject(getCreateBtn(3701, 4));
    objects->addObject(getCreateBtn(3702, 4));
    objects->addObject(getCreateBtn(3703, 4));
    objects->addObject(getCreateBtn(3704, 4));
    objects->addObject(getCreateBtn(3705, 4));
    objects->addObject(getCreateBtn(3706, 4));
    objects->addObject(getCreateBtn(3707, 4));
    objects->addObject(getCreateBtn(3708, 4));
    objects->addObject(getCreateBtn(3709, 4));
    objects->addObject(getCreateBtn(3710, 4));
    objects->addObject(getCreateBtn(3711, 4));
    objects->addObject(getCreateBtn(3712, 4));
    objects->addObject(getCreateBtn(3713, 4));
    objects->addObject(getCreateBtn(3714, 4));
    objects->addObject(getCreateBtn(3715, 4));
    objects->addObject(getCreateBtn(3716, 4));
    objects->addObject(getCreateBtn(3717, 4));
    objects->addObject(getCreateBtn(3718, 4));
    objects->addObject(getCreateBtn(3719, 4));
    objects->addObject(getCreateBtn(3720, 4));
    objects->addObject(getCreateBtn(3721, 4));
    objects->addObject(getCreateBtn(3722, 4));
    objects->addObject(getCreateBtn(3723, 4));
    objects->addObject(getCreateBtn(3724, 4));
    objects->addObject(getCreateBtn(3725, 4));
    objects->addObject(getCreateBtn(3726, 4));
    objects->addObject(getCreateBtn(3727, 4));
    objects->addObject(getCreateBtn(3728, 4));
    objects->addObject(getCreateBtn(3729, 4));
    objects->addObject(getCreateBtn(3730, 4));
    objects->addObject(getCreateBtn(3731, 4));
    objects->addObject(getCreateBtn(3732, 4));
    objects->addObject(getCreateBtn(3733, 4));
    objects->addObject(getCreateBtn(3734, 4));
    objects->addObject(getCreateBtn(3735, 4));
    objects->addObject(getCreateBtn(3736, 4));
    objects->addObject(getCreateBtn(3737, 4));
    objects->addObject(getCreateBtn(3738, 4));
    objects->addObject(getCreateBtn(3739, 4));
    objects->addObject(getCreateBtn(3740, 4));
    objects->addObject(getCreateBtn(3741, 4));
    objects->addObject(getCreateBtn(3742, 4));
    objects->addObject(getCreateBtn(3743, 4));
    objects->addObject(getCreateBtn(3744, 4));
    objects->addObject(getCreateBtn(3745, 4));
    objects->addObject(getCreateBtn(3746, 4));
    objects->addObject(getCreateBtn(3747, 4));
    objects->addObject(getCreateBtn(3748, 4));
    objects->addObject(getCreateBtn(3749, 4));
    objects->addObject(getCreateBtn(3750, 4));
    objects->addObject(getCreateBtn(3751, 4));
    objects->addObject(getCreateBtn(3752, 4));
    objects->addObject(getCreateBtn(3753, 4));
    objects->addObject(getCreateBtn(3754, 4));
    objects->addObject(getCreateBtn(3755, 4));
    objects->addObject(getCreateBtn(3756, 4));
    objects->addObject(getCreateBtn(3757, 4));
    objects->addObject(getCreateBtn(3758, 4));
    objects->addObject(getCreateBtn(3759, 4));
    objects->addObject(getCreateBtn(3760, 4));
    objects->addObject(getCreateBtn(3761, 4));
    objects->addObject(getCreateBtn(3762, 4));
    objects->addObject(getCreateBtn(3763, 4));
    objects->addObject(getCreateBtn(3764, 4));
    objects->addObject(getCreateBtn(3765, 4));
    objects->addObject(getCreateBtn(3766, 4));
    objects->addObject(getCreateBtn(3767, 4));
    objects->addObject(getCreateBtn(3768, 4));
    objects->addObject(getCreateBtn(3769, 4));
    objects->addObject(getCreateBtn(3770, 4));
    objects->addObject(getCreateBtn(3771, 4));
    objects->addObject(getCreateBtn(3772, 4));
    objects->addObject(getCreateBtn(3773, 4));
    objects->addObject(getCreateBtn(3774, 4));
    objects->addObject(getCreateBtn(3775, 4));
    objects->addObject(getCreateBtn(3776, 4));
    objects->addObject(getCreateBtn(3777, 4));
    objects->addObject(getCreateBtn(3778, 4));
    objects->addObject(getCreateBtn(3779, 4));
    objects->addObject(getCreateBtn(3780, 4));
    objects->addObject(getCreateBtn(3781, 4));
    objects->addObject(getCreateBtn(3782, 4));
    objects->addObject(getCreateBtn(3783, 4));
    objects->addObject(getCreateBtn(3784, 4));
    objects->addObject(getCreateBtn(3785, 4));
    objects->addObject(getCreateBtn(3786, 4));
    objects->addObject(getCreateBtn(3787, 4));
    objects->addObject(getCreateBtn(3788, 4));
    objects->addObject(getCreateBtn(3789, 4));
    objects->addObject(getCreateBtn(3790, 4));
    objects->addObject(getCreateBtn(3791, 4));
    objects->addObject(getCreateBtn(3792, 4));
    objects->addObject(getCreateBtn(4000, 4));
    objects->addObject(getCreateBtn(4004, 4));
    objects->addObject(getCreateBtn(4001, 4));
    objects->addObject(getCreateBtn(4005, 4));
    objects->addObject(getCreateBtn(4002, 4));
    objects->addObject(getCreateBtn(4006, 4));
    objects->addObject(getCreateBtn(4003, 4));
    objects->addObject(getCreateBtn(4007, 4));
    objects->addObject(getCreateBtn(2452, 4));
    objects->addObject(getCreateBtn(2453, 4));
    objects->addObject(getCreateBtn(4384, 4));
    objects->addObject(getCreateBtn(4385, 4));
    objects->addObject(getCreateBtn(4008, 4));
    objects->addObject(getCreateBtn(4010, 4));
    objects->addObject(getCreateBtn(4009, 4));
    objects->addObject(getCreateBtn(4011, 4));
    objects->addObject(getCreateBtn(2454, 4));
    objects->addObject(getCreateBtn(2455, 4));
    objects->addObject(getCreateBtn(4012, 4));
    objects->addObject(getCreateBtn(4013, 4));
    objects->addObject(getCreateBtn(4014, 4));
    objects->addObject(getCreateBtn(4015, 4));
    objects->addObject(getCreateBtn(4016, 4));
    objects->addObject(getCreateBtn(4017, 4));
    objects->addObject(getCreateBtn(4018, 4));
    objects->addObject(getCreateBtn(4019, 4));
    objects->addObject(getCreateBtn(4020, 4));
    objects->addObject(getCreateBtn(4021, 4));
    objects->addObject(getCreateBtn(4022, 4));
    objects->addObject(getCreateBtn(4023, 4));
    objects->addObject(getCreateBtn(4024, 4));
    objects->addObject(getCreateBtn(4030, 4));
    objects->addObject(getCreateBtn(4031, 4));
    objects->addObject(getCreateBtn(4025, 4));
    objects->addObject(getCreateBtn(4032, 4));
    objects->addObject(getCreateBtn(4033, 4));
    objects->addObject(getCreateBtn(4026, 4));
    objects->addObject(getCreateBtn(2434, 4));
    objects->addObject(getCreateBtn(2435, 4));
    objects->addObject(getCreateBtn(4027, 4));
    objects->addObject(getCreateBtn(2436, 4));
    objects->addObject(getCreateBtn(2437, 4));
    objects->addObject(getCreateBtn(4028, 4));
    objects->addObject(getCreateBtn(2438, 4));
    objects->addObject(getCreateBtn(2439, 4));
    objects->addObject(getCreateBtn(4029, 4));
    objects->addObject(getCreateBtn(4214, 4));
    objects->addObject(getCreateBtn(2440, 4));
    objects->addObject(getCreateBtn(4215, 4));
    objects->addObject(getCreateBtn(2441, 4));
    objects->addObject(getCreateBtn(4216, 4));
    objects->addObject(getCreateBtn(2442, 4));
    objects->addObject(getCreateBtn(4217, 4));
    objects->addObject(getCreateBtn(2443, 4));
    objects->addObject(getCreateBtn(4218, 4));
    objects->addObject(getCreateBtn(2444, 4));
    objects->addObject(getCreateBtn(2451, 4));
    objects->addObject(getCreateBtn(4219, 4));
    objects->addObject(getCreateBtn(2445, 4));
    objects->addObject(getCreateBtn(4220, 4));
    objects->addObject(getCreateBtn(2446, 4));
    objects->addObject(getCreateBtn(4221, 4));
    objects->addObject(getCreateBtn(4222, 4));
    objects->addObject(getCreateBtn(2447, 4));
    objects->addObject(getCreateBtn(4223, 4));
    objects->addObject(getCreateBtn(2448, 4));
    objects->addObject(getCreateBtn(4224, 4));
    objects->addObject(getCreateBtn(2449, 4));
    objects->addObject(getCreateBtn(4225, 4));
    objects->addObject(getCreateBtn(2450, 4));
    objects->addObject(getCreateBtn(4034, 4));
    objects->addObject(getCreateBtn(4035, 4));
    objects->addObject(getCreateBtn(4036, 4));
    objects->addObject(getCreateBtn(4037, 4));
    objects->addObject(getCreateBtn(4038, 4));
    objects->addObject(getCreateBtn(4039, 4));
    objects->addObject(getCreateBtn(4040, 4));
    objects->addObject(getCreateBtn(4041, 4));
    objects->addObject(getCreateBtn(4042, 4));
    objects->addObject(getCreateBtn(4043, 4));
    objects->addObject(getCreateBtn(4044, 4));
    objects->addObject(getCreateBtn(4045, 4));
    objects->addObject(getCreateBtn(4046, 4));
    objects->addObject(getCreateBtn(4047, 4));
    objects->addObject(getCreateBtn(4048, 4));
    objects->addObject(getCreateBtn(4049, 4));
    objects->addObject(getCreateBtn(4050, 4));
    objects->addObject(getCreateBtn(4051, 4));
    objects->addObject(getCreateBtn(4052, 4));
    objects->addObject(getCreateBtn(4053, 4));
    objects->addObject(getCreateBtn(4054, 4));
    objects->addObject(getCreateBtn(4055, 4));
    objects->addObject(getCreateBtn(4056, 4));
    objects->addObject(getCreateBtn(4057, 4));
    objects->addObject(getCreateBtn(4058, 4));
    objects->addObject(getCreateBtn(4059, 4));
    objects->addObject(getCreateBtn(4062, 4));
    objects->addObject(getCreateBtn(4063, 4));
    objects->addObject(getCreateBtn(4064, 4));
    objects->addObject(getCreateBtn(4065, 4));
    objects->addObject(getCreateBtn(4066, 4));
    objects->addObject(getCreateBtn(4067, 4));
    objects->addObject(getCreateBtn(4068, 4));
    objects->addObject(getCreateBtn(4069, 4));
    objects->addObject(getCreateBtn(4070, 4));
    objects->addObject(getCreateBtn(4071, 4));
    objects->addObject(getCreateBtn(4072, 4));
    objects->addObject(getCreateBtn(4073, 4));
    objects->addObject(getCreateBtn(4074, 4));
    objects->addObject(getCreateBtn(4075, 4));
    objects->addObject(getCreateBtn(4076, 4));
    objects->addObject(getCreateBtn(4234, 4));
    objects->addObject(getCreateBtn(4235, 4));
    objects->addObject(getCreateBtn(4236, 4));
    objects->addObject(getCreateBtn(4237, 4));
    objects->addObject(getCreateBtn(4077, 4));
    objects->addObject(getCreateBtn(4078, 4));
    objects->addObject(getCreateBtn(4079, 4));
    objects->addObject(getCreateBtn(4080, 4));
    objects->addObject(getCreateBtn(4081, 4));
    objects->addObject(getCreateBtn(4082, 4));
    objects->addObject(getCreateBtn(4083, 4));
    objects->addObject(getCreateBtn(4084, 4));
    objects->addObject(getCreateBtn(4085, 4));
    objects->addObject(getCreateBtn(4086, 4));
    objects->addObject(getCreateBtn(4087, 4));
    objects->addObject(getCreateBtn(4088, 4));
    objects->addObject(getCreateBtn(4089, 4));
    objects->addObject(getCreateBtn(4090, 4));
    objects->addObject(getCreateBtn(4091, 4));
    objects->addObject(getCreateBtn(4092, 4));
    objects->addObject(getCreateBtn(4093, 4));
    objects->addObject(getCreateBtn(4094, 4));
    objects->addObject(getCreateBtn(4095, 4));
    objects->addObject(getCreateBtn(4096, 4));
    objects->addObject(getCreateBtn(4097, 4));
    objects->addObject(getCreateBtn(4098, 4));
    objects->addObject(getCreateBtn(4099, 4));
    objects->addObject(getCreateBtn(4100, 4));
    objects->addObject(getCreateBtn(4101, 4));
    objects->addObject(getCreateBtn(4102, 4));
    objects->addObject(getCreateBtn(4103, 4));
    objects->addObject(getCreateBtn(4104, 4));
    objects->addObject(getCreateBtn(4105, 4));
    objects->addObject(getCreateBtn(4106, 4));
    objects->addObject(getCreateBtn(4107, 4));
    objects->addObject(getCreateBtn(4108, 4));
    objects->addObject(getCreateBtn(4109, 4));
    objects->addObject(getCreateBtn(4110, 4));
    objects->addObject(getCreateBtn(4111, 4));
    objects->addObject(getCreateBtn(4112, 4));
    objects->addObject(getCreateBtn(4113, 4));
    objects->addObject(getCreateBtn(4114, 4));
    objects->addObject(getCreateBtn(4115, 4));
    objects->addObject(getCreateBtn(4116, 4));
    objects->addObject(getCreateBtn(4117, 4));
    objects->addObject(getCreateBtn(4118, 4));
    objects->addObject(getCreateBtn(4119, 4));
    objects->addObject(getCreateBtn(4120, 4));
    objects->addObject(getCreateBtn(4121, 4));
    objects->addObject(getCreateBtn(4123, 4));
    objects->addObject(getCreateBtn(4124, 4));
    objects->addObject(getCreateBtn(4125, 4));
    objects->addObject(getCreateBtn(4126, 4));
    objects->addObject(getCreateBtn(4127, 4));
    objects->addObject(getCreateBtn(4128, 4));
    objects->addObject(getCreateBtn(4129, 4));
    objects->addObject(getCreateBtn(4130, 4));
    objects->addObject(getCreateBtn(4131, 4));
    objects->addObject(getCreateBtn(4132, 4));
    objects->addObject(getCreateBtn(4133, 4));
    objects->addObject(getCreateBtn(4134, 4));
    objects->addObject(getCreateBtn(4135, 4));
    objects->addObject(getCreateBtn(4136, 4));
    objects->addObject(getCreateBtn(4137, 4));
    objects->addObject(getCreateBtn(4138, 4));
    objects->addObject(getCreateBtn(4139, 4));
    objects->addObject(getCreateBtn(4140, 4));
    objects->addObject(getCreateBtn(4141, 4));
    objects->addObject(getCreateBtn(4142, 4));
    objects->addObject(getCreateBtn(4143, 4));
    objects->addObject(getCreateBtn(4144, 4));
    objects->addObject(getCreateBtn(4145, 4));
    objects->addObject(getCreateBtn(4146, 4));
    objects->addObject(getCreateBtn(4147, 4));
    objects->addObject(getCreateBtn(4148, 4));
    objects->addObject(getCreateBtn(4149, 4));
    objects->addObject(getCreateBtn(4150, 4));
    objects->addObject(getCreateBtn(4151, 4));
    objects->addObject(getCreateBtn(4152, 4));
    objects->addObject(getCreateBtn(4153, 4));
    objects->addObject(getCreateBtn(4154, 4));
    objects->addObject(getCreateBtn(4155, 4));
    objects->addObject(getCreateBtn(4156, 4));
    objects->addObject(getCreateBtn(4157, 4));
    objects->addObject(getCreateBtn(4158, 4));
    objects->addObject(getCreateBtn(4159, 4));
    objects->addObject(getCreateBtn(4160, 4));
    objects->addObject(getCreateBtn(4161, 4));
    objects->addObject(getCreateBtn(4162, 4));
    objects->addObject(getCreateBtn(4163, 4));
    objects->addObject(getCreateBtn(4164, 4));
    objects->addObject(getCreateBtn(4165, 4));
    objects->addObject(getCreateBtn(4166, 4));
    objects->addObject(getCreateBtn(4167, 4));
    objects->addObject(getCreateBtn(4168, 4));
    objects->addObject(getCreateBtn(4169, 4));
    objects->addObject(getCreateBtn(4170, 4));
    objects->addObject(getCreateBtn(4171, 4));
    objects->addObject(getCreateBtn(4172, 4));
    objects->addObject(getCreateBtn(4173, 4));
    objects->addObject(getCreateBtn(4174, 4));
    objects->addObject(getCreateBtn(4175, 4));
    objects->addObject(getCreateBtn(4176, 4));
    objects->addObject(getCreateBtn(4177, 4));
    objects->addObject(getCreateBtn(4178, 4));
    objects->addObject(getCreateBtn(4179, 4));
    objects->addObject(getCreateBtn(4180, 4));
    objects->addObject(getCreateBtn(4181, 4));
    objects->addObject(getCreateBtn(4182, 4));
    objects->addObject(getCreateBtn(4183, 4));
    objects->addObject(getCreateBtn(4184, 4));
    objects->addObject(getCreateBtn(4185, 4));
    objects->addObject(getCreateBtn(4186, 4));
    objects->addObject(getCreateBtn(4187, 4));
    objects->addObject(getCreateBtn(4188, 4));
    objects->addObject(getCreateBtn(4189, 4));
    objects->addObject(getCreateBtn(4190, 4));
    objects->addObject(getCreateBtn(4191, 4));
    objects->addObject(getCreateBtn(4192, 4));
    objects->addObject(getCreateBtn(4193, 4));
    objects->addObject(getCreateBtn(4194, 4));
    objects->addObject(getCreateBtn(4195, 4));
    objects->addObject(getCreateBtn(4196, 4));
    objects->addObject(getCreateBtn(4197, 4));
    objects->addObject(getCreateBtn(4198, 4));
    objects->addObject(getCreateBtn(4199, 4));
    objects->addObject(getCreateBtn(4200, 4));
    objects->addObject(getCreateBtn(4201, 4));
    objects->addObject(getCreateBtn(4202, 4));
    objects->addObject(getCreateBtn(4203, 4));
    objects->addObject(getCreateBtn(4204, 4));
    objects->addObject(getCreateBtn(4205, 4));
    objects->addObject(getCreateBtn(4206, 4));
    objects->addObject(getCreateBtn(4207, 4));
    objects->addObject(getCreateBtn(4208, 4));
    objects->addObject(getCreateBtn(4209, 4));
    objects->addObject(getCreateBtn(4210, 4));
    objects->addObject(getCreateBtn(4212, 4));
    objects->addObject(getCreateBtn(4213, 4));
    objects->addObject(getCreateBtn(4226, 4));
    objects->addObject(getCreateBtn(4227, 4));
    objects->addObject(getCreateBtn(4228, 4));
    objects->addObject(getCreateBtn(4229, 4));
    objects->addObject(getCreateBtn(4230, 4));
    objects->addObject(getCreateBtn(4231, 4));
    objects->addObject(getCreateBtn(4232, 4));
    objects->addObject(getCreateBtn(4233, 4));
    objects->addObject(getCreateBtn(2532, 4));
    objects->addObject(getCreateBtn(2533, 4));
    objects->addObject(getCreateBtn(4299, 4));
    objects->addObject(getCreateBtn(4122, 4));
    objects->addObject(getCreateBtn(3793, 4));
    objects->addObject(getCreateBtn(3794, 4));
    objects->addObject(getCreateBtn(3795, 4));
    objects->addObject(getCreateBtn(3796, 4));
    objects->addObject(getCreateBtn(3797, 4));
    objects->addObject(getCreateBtn(3798, 4));
    objects->addObject(getCreateBtn(3799, 4));
    objects->addObject(getCreateBtn(4238, 4));
    objects->addObject(getCreateBtn(4239, 4));
    objects->addObject(getCreateBtn(4240, 4));
    objects->addObject(getCreateBtn(4241, 4));
    objects->addObject(getCreateBtn(4242, 4));
    objects->addObject(getCreateBtn(4243, 4));
    objects->addObject(getCreateBtn(4244, 4));
    objects->addObject(getCreateBtn(4245, 4));
    objects->addObject(getCreateBtn(4246, 4));
    objects->addObject(getCreateBtn(4247, 4));
    objects->addObject(getCreateBtn(4248, 4));
    objects->addObject(getCreateBtn(4249, 4));
    objects->addObject(getCreateBtn(4250, 4));
    objects->addObject(getCreateBtn(4251, 4));
    objects->addObject(getCreateBtn(4252, 4));
    objects->addObject(getCreateBtn(4253, 4));
    objects->addObject(getCreateBtn(4254, 4));
    objects->addObject(getCreateBtn(4256, 4));
    objects->addObject(getCreateBtn(4257, 4));
    objects->addObject(getCreateBtn(4258, 4));
    objects->addObject(getCreateBtn(4255, 4));
    objects->addObject(getCreateBtn(4264, 4));
    objects->addObject(getCreateBtn(4259, 4));
    objects->addObject(getCreateBtn(4260, 4));
    objects->addObject(getCreateBtn(4261, 4));
    objects->addObject(getCreateBtn(4060, 4));
    objects->addObject(getCreateBtn(4061, 4));
    objects->addObject(getCreateBtn(4262, 4));
    objects->addObject(getCreateBtn(4263, 4));
    objects->addObject(getCreateBtn(4265, 4));
    objects->addObject(getCreateBtn(4266, 4));
    objects->addObject(getCreateBtn(4267, 4));
    objects->addObject(getCreateBtn(4268, 4));
    objects->addObject(getCreateBtn(4269, 4));
    objects->addObject(getCreateBtn(4270, 4));
    objects->addObject(getCreateBtn(4271, 4));
    objects->addObject(getCreateBtn(2247, 4));
    objects->addObject(getCreateBtn(2248, 4));
    objects->addObject(getCreateBtn(4272, 4));
    objects->addObject(getCreateBtn(4273, 4));
    objects->addObject(getCreateBtn(2433, 4));
    objects->addObject(getCreateBtn(4274, 4));
    objects->addObject(getCreateBtn(4275, 4));
    objects->addObject(getCreateBtn(4276, 4));
    objects->addObject(getCreateBtn(4277, 4));
    objects->addObject(getCreateBtn(4278, 4));
    objects->addObject(getCreateBtn(4279, 4));
    objects->addObject(getCreateBtn(4280, 4));
    objects->addObject(getCreateBtn(4281, 4));
    objects->addObject(getCreateBtn(4282, 4));
    objects->addObject(getCreateBtn(4283, 4));
    objects->addObject(getCreateBtn(4284, 4));
    objects->addObject(getCreateBtn(4285, 4));
    objects->addObject(getCreateBtn(2431, 4));
    objects->addObject(getCreateBtn(2432, 4));
    objects->addObject(getCreateBtn(4286, 4));
    objects->addObject(getCreateBtn(4287, 4));
    objects->addObject(getCreateBtn(4288, 4));
    objects->addObject(getCreateBtn(4289, 4));
    objects->addObject(getCreateBtn(4290, 4));
    objects->addObject(getCreateBtn(4291, 4));
    objects->addObject(getCreateBtn(4292, 4));
    objects->addObject(getCreateBtn(4293, 4));
    objects->addObject(getCreateBtn(4294, 4));
    objects->addObject(getCreateBtn(4295, 4));
    objects->addObject(getCreateBtn(4296, 4));
    objects->addObject(getCreateBtn(4297, 4));
    objects->addObject(getCreateBtn(4298, 4));
    objects->addObject(getCreateBtn(4301, 4));
    objects->addObject(getCreateBtn(4302, 4));
    objects->addObject(getCreateBtn(4303, 4));
    objects->addObject(getCreateBtn(4304, 4));
    objects->addObject(getCreateBtn(4305, 4));
    objects->addObject(getCreateBtn(4306, 4));
    objects->addObject(getCreateBtn(4307, 4));
    objects->addObject(getCreateBtn(4308, 4));
    objects->addObject(getCreateBtn(4309, 4));
    objects->addObject(getCreateBtn(4310, 4));
    objects->addObject(getCreateBtn(4311, 4));
    objects->addObject(getCreateBtn(4312, 4));
    objects->addObject(getCreateBtn(4313, 4));
    objects->addObject(getCreateBtn(4314, 4));
    objects->addObject(getCreateBtn(4315, 4));
    objects->addObject(getCreateBtn(4316, 4));
    objects->addObject(getCreateBtn(4317, 4));
    objects->addObject(getCreateBtn(4318, 4));
    objects->addObject(getCreateBtn(4319, 4));
    objects->addObject(getCreateBtn(4320, 4));
    objects->addObject(getCreateBtn(4321, 4));
    objects->addObject(getCreateBtn(4322, 4));
    objects->addObject(getCreateBtn(4323, 4));
    objects->addObject(getCreateBtn(4324, 4));
    objects->addObject(getCreateBtn(4325, 4));
    objects->addObject(getCreateBtn(4326, 4));
    objects->addObject(getCreateBtn(4327, 4));
    objects->addObject(getCreateBtn(4328, 4));
    objects->addObject(getCreateBtn(4329, 4));
    objects->addObject(getCreateBtn(4330, 4));
    objects->addObject(getCreateBtn(4331, 4));
    objects->addObject(getCreateBtn(4332, 4));
    objects->addObject(getCreateBtn(4333, 4));
    objects->addObject(getCreateBtn(4334, 4));
    objects->addObject(getCreateBtn(4335, 4));
    objects->addObject(getCreateBtn(4336, 4));
    objects->addObject(getCreateBtn(4337, 4));
    objects->addObject(getCreateBtn(4338, 4));
    objects->addObject(getCreateBtn(4339, 4));
    objects->addObject(getCreateBtn(4340, 4));
    objects->addObject(getCreateBtn(4341, 4));
    objects->addObject(getCreateBtn(4342, 4));
    objects->addObject(getCreateBtn(4343, 4));
    objects->addObject(getCreateBtn(4344, 4));
    objects->addObject(getCreateBtn(4345, 4));
    objects->addObject(getCreateBtn(4346, 4));
    objects->addObject(getCreateBtn(4347, 4));
    objects->addObject(getCreateBtn(4348, 4));
    objects->addObject(getCreateBtn(4349, 4));
    objects->addObject(getCreateBtn(4350, 4));
    objects->addObject(getCreateBtn(4351, 4));
    objects->addObject(getCreateBtn(4352, 4));
    objects->addObject(getCreateBtn(4353, 4));
    objects->addObject(getCreateBtn(4354, 4));
    objects->addObject(getCreateBtn(4355, 4));
    objects->addObject(getCreateBtn(4356, 4));
    objects->addObject(getCreateBtn(4357, 4));
    objects->addObject(getCreateBtn(4358, 4));
    objects->addObject(getCreateBtn(4359, 4));
    objects->addObject(getCreateBtn(4360, 4));
    objects->addObject(getCreateBtn(4361, 4));
    objects->addObject(getCreateBtn(4362, 4));
    objects->addObject(getCreateBtn(4363, 4));
    objects->addObject(getCreateBtn(4364, 4));
    objects->addObject(getCreateBtn(4365, 4));
    objects->addObject(getCreateBtn(4366, 4));
    objects->addObject(getCreateBtn(4367, 4));
    objects->addObject(getCreateBtn(3092, 4));
    objects->addObject(getCreateBtn(3093, 4));
    objects->addObject(getCreateBtn(3094, 4));
    objects->addObject(getCreateBtn(3095, 4));
    objects->addObject(getCreateBtn(3096, 4));
    objects->addObject(getCreateBtn(3097, 4));

    auto tab8 = EditButtonBar::create(objects, point, 7, true, buttonsPerRow, buttonRows);
    addChild(tab8, 10);

    m_createButtonBars->addObject(tab8);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("pixelitem_001_001.png"));

    objects->addObject(getCreateBtn(1329, 4));
    objects->addObject(getCreateBtn(1275, 4));
    objects->addObject(getCreateBtn(1276, 4));
    objects->addObject(getCreateBtn(1587, 4));
    objects->addObject(getCreateBtn(1588, 4));
    objects->addObject(getCreateBtn(1589, 4));
    objects->addObject(getCreateBtn(1590, 4));
    objects->addObject(getCreateBtn(1598, 4));
    objects->addObject(getCreateBtn(1599, 4));
    objects->addObject(getCreateBtn(1614, 4));
    objects->addObject(getCreateBtn(3601, 4));
    objects->addObject(getCreateBtn(4401, 4));
    objects->addObject(getCreateBtn(4402, 4));
    objects->addObject(getCreateBtn(4403, 4));
    objects->addObject(getCreateBtn(4404, 4));
    objects->addObject(getCreateBtn(4405, 4));
    objects->addObject(getCreateBtn(4406, 4));
    objects->addObject(getCreateBtn(4407, 4));
    objects->addObject(getCreateBtn(4408, 4));
    objects->addObject(getCreateBtn(4409, 4));
    objects->addObject(getCreateBtn(4410, 4));
    objects->addObject(getCreateBtn(4411, 4));
    objects->addObject(getCreateBtn(4412, 4));
    objects->addObject(getCreateBtn(4413, 4));
    objects->addObject(getCreateBtn(4414, 4));
    objects->addObject(getCreateBtn(4415, 4));
    objects->addObject(getCreateBtn(4416, 4));
    objects->addObject(getCreateBtn(4417, 4));
    objects->addObject(getCreateBtn(4418, 4));
    objects->addObject(getCreateBtn(4419, 4));
    objects->addObject(getCreateBtn(4420, 4));
    objects->addObject(getCreateBtn(4421, 4));
    objects->addObject(getCreateBtn(4422, 4));
    objects->addObject(getCreateBtn(4423, 4));
    objects->addObject(getCreateBtn(4424, 4));
    objects->addObject(getCreateBtn(4425, 4));
    objects->addObject(getCreateBtn(4426, 4));
    objects->addObject(getCreateBtn(4427, 4));
    objects->addObject(getCreateBtn(4428, 4));
    objects->addObject(getCreateBtn(4429, 4));
    objects->addObject(getCreateBtn(4430, 4));
    objects->addObject(getCreateBtn(4431, 4));
    objects->addObject(getCreateBtn(4432, 4));
    objects->addObject(getCreateBtn(4433, 4));
    objects->addObject(getCreateBtn(4434, 4));
    objects->addObject(getCreateBtn(4435, 4));
    objects->addObject(getCreateBtn(4436, 4));
    objects->addObject(getCreateBtn(4437, 4));
    objects->addObject(getCreateBtn(4438, 4));
    objects->addObject(getCreateBtn(4439, 4));
    objects->addObject(getCreateBtn(4440, 4));
    objects->addObject(getCreateBtn(4441, 4));
    objects->addObject(getCreateBtn(4442, 4));
    objects->addObject(getCreateBtn(4443, 4));
    objects->addObject(getCreateBtn(4444, 4));
    objects->addObject(getCreateBtn(4445, 4));
    objects->addObject(getCreateBtn(4446, 4));
    objects->addObject(getCreateBtn(4447, 4));
    objects->addObject(getCreateBtn(4448, 4));
    objects->addObject(getCreateBtn(4449, 4));
    objects->addObject(getCreateBtn(4450, 4));
    objects->addObject(getCreateBtn(4451, 4));
    objects->addObject(getCreateBtn(4452, 4));
    objects->addObject(getCreateBtn(4453, 4));
    objects->addObject(getCreateBtn(4454, 4));
    objects->addObject(getCreateBtn(4455, 4));
    objects->addObject(getCreateBtn(4456, 4));
    objects->addObject(getCreateBtn(4457, 4));
    objects->addObject(getCreateBtn(4458, 4));
    objects->addObject(getCreateBtn(4459, 4));
    objects->addObject(getCreateBtn(4460, 4));
    objects->addObject(getCreateBtn(4461, 4));
    objects->addObject(getCreateBtn(4462, 4));
    objects->addObject(getCreateBtn(4463, 4));
    objects->addObject(getCreateBtn(4464, 4));
    objects->addObject(getCreateBtn(4465, 4));
    objects->addObject(getCreateBtn(4466, 4));
    objects->addObject(getCreateBtn(4467, 4));
    objects->addObject(getCreateBtn(4468, 4));
    objects->addObject(getCreateBtn(4469, 4));
    objects->addObject(getCreateBtn(4470, 4));
    objects->addObject(getCreateBtn(4471, 4));
    objects->addObject(getCreateBtn(4472, 4));
    objects->addObject(getCreateBtn(4473, 4));
    objects->addObject(getCreateBtn(4474, 4));
    objects->addObject(getCreateBtn(4475, 4));
    objects->addObject(getCreateBtn(4476, 4));
    objects->addObject(getCreateBtn(4477, 4));
    objects->addObject(getCreateBtn(4478, 4));
    objects->addObject(getCreateBtn(4479, 4));
    objects->addObject(getCreateBtn(4480, 4));
    objects->addObject(getCreateBtn(4481, 4));
    objects->addObject(getCreateBtn(4482, 4));
    objects->addObject(getCreateBtn(4483, 4));
    objects->addObject(getCreateBtn(4484, 4));
    objects->addObject(getCreateBtn(4485, 4));
    objects->addObject(getCreateBtn(4486, 4));
    objects->addObject(getCreateBtn(4487, 4));
    objects->addObject(getCreateBtn(4488, 4));
    objects->addObject(getCreateBtn(4538, 4));
    objects->addObject(getCreateBtn(4489, 4));
    objects->addObject(getCreateBtn(4490, 4));
    objects->addObject(getCreateBtn(4491, 4));
    objects->addObject(getCreateBtn(4492, 4));
    objects->addObject(getCreateBtn(4493, 4));
    objects->addObject(getCreateBtn(4494, 4));
    objects->addObject(getCreateBtn(4495, 4));
    objects->addObject(getCreateBtn(4496, 4));
    objects->addObject(getCreateBtn(4497, 4));
    objects->addObject(getCreateBtn(4537, 4));
    objects->addObject(getCreateBtn(4498, 4));
    objects->addObject(getCreateBtn(4499, 4));
    objects->addObject(getCreateBtn(4500, 4));
    objects->addObject(getCreateBtn(4501, 4));
    objects->addObject(getCreateBtn(4502, 4));
    objects->addObject(getCreateBtn(4503, 4));
    objects->addObject(getCreateBtn(4504, 4));
    objects->addObject(getCreateBtn(4505, 4));
    objects->addObject(getCreateBtn(4506, 4));
    objects->addObject(getCreateBtn(4507, 4));
    objects->addObject(getCreateBtn(4508, 4));
    objects->addObject(getCreateBtn(4509, 4));
    objects->addObject(getCreateBtn(4510, 4));
    objects->addObject(getCreateBtn(4511, 4));
    objects->addObject(getCreateBtn(4512, 4));
    objects->addObject(getCreateBtn(4513, 4));
    objects->addObject(getCreateBtn(4514, 4));
    objects->addObject(getCreateBtn(4515, 4));
    objects->addObject(getCreateBtn(4516, 4));
    objects->addObject(getCreateBtn(4517, 4));
    objects->addObject(getCreateBtn(4518, 4));
    objects->addObject(getCreateBtn(4519, 4));
    objects->addObject(getCreateBtn(4520, 4));
    objects->addObject(getCreateBtn(4521, 4));
    objects->addObject(getCreateBtn(4522, 4));
    objects->addObject(getCreateBtn(4523, 4));
    objects->addObject(getCreateBtn(4524, 4));
    objects->addObject(getCreateBtn(4525, 4));
    objects->addObject(getCreateBtn(4526, 4));
    objects->addObject(getCreateBtn(4527, 4));
    objects->addObject(getCreateBtn(4528, 4));
    objects->addObject(getCreateBtn(4529, 4));
    objects->addObject(getCreateBtn(4530, 4));
    objects->addObject(getCreateBtn(4531, 4));
    objects->addObject(getCreateBtn(4532, 4));
    objects->addObject(getCreateBtn(4533, 4));
    objects->addObject(getCreateBtn(4534, 4));
    objects->addObject(getCreateBtn(4535, 4));
    objects->addObject(getCreateBtn(4536, 4));
    objects->addObject(getCreateBtn(4539, 4));

    auto tab9 = EditButtonBar::create(objects, point, 8, true, buttonsPerRow, buttonRows);
    addChild(tab9, 10);

    m_createButtonBars->addObject(tab9);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("particle_01_001.png"));

    objects->addObject(getCreateBtn(3801, 4));
    objects->addObject(getCreateBtn(3802, 4));
    objects->addObject(getCreateBtn(3803, 4));
    objects->addObject(getCreateBtn(3804, 4));
    objects->addObject(getCreateBtn(3805, 4));
    objects->addObject(getCreateBtn(3806, 4));
    objects->addObject(getCreateBtn(3807, 4));
    objects->addObject(getCreateBtn(3808, 4));
    objects->addObject(getCreateBtn(3809, 4));
    objects->addObject(getCreateBtn(3810, 4));
    objects->addObject(getCreateBtn(3811, 4));
    objects->addObject(getCreateBtn(3812, 4));
    objects->addObject(getCreateBtn(3813, 4));
    objects->addObject(getCreateBtn(3814, 4));
    objects->addObject(getCreateBtn(3815, 4));
    objects->addObject(getCreateBtn(3816, 4));
    objects->addObject(getCreateBtn(3817, 4));
    objects->addObject(getCreateBtn(3818, 4));
    objects->addObject(getCreateBtn(3819, 4));
    objects->addObject(getCreateBtn(3820, 4));
    objects->addObject(getCreateBtn(3821, 4));
    objects->addObject(getCreateBtn(3822, 4));
    objects->addObject(getCreateBtn(3823, 4));
    objects->addObject(getCreateBtn(3824, 4));
    objects->addObject(getCreateBtn(3825, 4));
    objects->addObject(getCreateBtn(3826, 4));
    objects->addObject(getCreateBtn(3827, 4));
    objects->addObject(getCreateBtn(3828, 4));
    objects->addObject(getCreateBtn(3829, 4));
    objects->addObject(getCreateBtn(3830, 4));
    objects->addObject(getCreateBtn(3831, 4));
    objects->addObject(getCreateBtn(3832, 4));
    objects->addObject(getCreateBtn(3833, 4));
    objects->addObject(getCreateBtn(3834, 4));
    objects->addObject(getCreateBtn(3835, 4));
    objects->addObject(getCreateBtn(3836, 4));
    objects->addObject(getCreateBtn(3837, 4));
    objects->addObject(getCreateBtn(3838, 4));
    objects->addObject(getCreateBtn(3839, 4));
    objects->addObject(getCreateBtn(3840, 4));
    objects->addObject(getCreateBtn(3841, 4));
    objects->addObject(getCreateBtn(3842, 4));
    objects->addObject(getCreateBtn(3843, 4));
    objects->addObject(getCreateBtn(3844, 4));
    objects->addObject(getCreateBtn(3845, 4));
    objects->addObject(getCreateBtn(3846, 4));
    objects->addObject(getCreateBtn(3847, 4));
    objects->addObject(getCreateBtn(3848, 4));
    objects->addObject(getCreateBtn(3849, 4));
    objects->addObject(getCreateBtn(3850, 4));
    objects->addObject(getCreateBtn(3851, 4));
    objects->addObject(getCreateBtn(3852, 4));
    objects->addObject(getCreateBtn(3853, 4));
    objects->addObject(getCreateBtn(3854, 4));
    objects->addObject(getCreateBtn(3855, 4));
    objects->addObject(getCreateBtn(3856, 4));
    objects->addObject(getCreateBtn(3857, 4));
    objects->addObject(getCreateBtn(3858, 4));
    objects->addObject(getCreateBtn(3859, 4));
    objects->addObject(getCreateBtn(3860, 4));
    objects->addObject(getCreateBtn(3861, 4));
    objects->addObject(getCreateBtn(3862, 4));
    objects->addObject(getCreateBtn(3863, 4));
    objects->addObject(getCreateBtn(3864, 4));
    objects->addObject(getCreateBtn(3865, 4));
    objects->addObject(getCreateBtn(3866, 4));
    objects->addObject(getCreateBtn(3867, 4));
    objects->addObject(getCreateBtn(3868, 4));
    objects->addObject(getCreateBtn(3869, 4));
    objects->addObject(getCreateBtn(3870, 4));
    objects->addObject(getCreateBtn(3871, 4));
    objects->addObject(getCreateBtn(3872, 4));
    objects->addObject(getCreateBtn(3873, 4));
    objects->addObject(getCreateBtn(3874, 4));
    objects->addObject(getCreateBtn(3875, 4));
    objects->addObject(getCreateBtn(3876, 4));
    objects->addObject(getCreateBtn(3877, 4));
    objects->addObject(getCreateBtn(3878, 4));
    objects->addObject(getCreateBtn(3879, 4));
    objects->addObject(getCreateBtn(3880, 4));
    objects->addObject(getCreateBtn(3881, 4));
    objects->addObject(getCreateBtn(3882, 4));
    objects->addObject(getCreateBtn(3883, 4));
    objects->addObject(getCreateBtn(3884, 4));
    objects->addObject(getCreateBtn(3885, 4));
    objects->addObject(getCreateBtn(3886, 4));
    objects->addObject(getCreateBtn(3887, 4));
    objects->addObject(getCreateBtn(3888, 4));
    objects->addObject(getCreateBtn(3889, 4));
    objects->addObject(getCreateBtn(3890, 4));
    objects->addObject(getCreateBtn(3891, 4));
    objects->addObject(getCreateBtn(3892, 4));
    objects->addObject(getCreateBtn(3893, 4));
    objects->addObject(getCreateBtn(3894, 4));
    objects->addObject(getCreateBtn(3895, 4));
    objects->addObject(getCreateBtn(3896, 4));
    objects->addObject(getCreateBtn(3897, 4));
    objects->addObject(getCreateBtn(3898, 4));
    objects->addObject(getCreateBtn(3899, 4));
    objects->addObject(getCreateBtn(3900, 4));
    objects->addObject(getCreateBtn(3901, 4));
    objects->addObject(getCreateBtn(3902, 4));
    objects->addObject(getCreateBtn(3903, 4));
    objects->addObject(getCreateBtn(3904, 4));
    objects->addObject(getCreateBtn(3905, 4));
    objects->addObject(getCreateBtn(3906, 4));
    objects->addObject(getCreateBtn(3907, 4));
    objects->addObject(getCreateBtn(3908, 4));
    objects->addObject(getCreateBtn(3909, 4));
    objects->addObject(getCreateBtn(3910, 4));
    objects->addObject(getCreateBtn(3911, 4));
    objects->addObject(getCreateBtn(3912, 4));
    objects->addObject(getCreateBtn(3913, 4));
    objects->addObject(getCreateBtn(3914, 4));
    objects->addObject(getCreateBtn(3915, 4));
    objects->addObject(getCreateBtn(3916, 4));
    objects->addObject(getCreateBtn(3917, 4));
    objects->addObject(getCreateBtn(3918, 4));
    objects->addObject(getCreateBtn(3919, 4));
    objects->addObject(getCreateBtn(3920, 4));
    objects->addObject(getCreateBtn(3921, 4));
    objects->addObject(getCreateBtn(3922, 4));
    objects->addObject(getCreateBtn(3923, 4));
    objects->addObject(getCreateBtn(3924, 4));
    objects->addObject(getCreateBtn(3925, 4));
    objects->addObject(getCreateBtn(3926, 4));
    objects->addObject(getCreateBtn(3927, 4));
    objects->addObject(getCreateBtn(3928, 4));
    objects->addObject(getCreateBtn(3929, 4));
    objects->addObject(getCreateBtn(3930, 4));
    objects->addObject(getCreateBtn(3931, 4));
    objects->addObject(getCreateBtn(3932, 4));
    objects->addObject(getCreateBtn(3933, 4));
    objects->addObject(getCreateBtn(3934, 4));
    objects->addObject(getCreateBtn(3935, 4));
    objects->addObject(getCreateBtn(3936, 4));
    objects->addObject(getCreateBtn(3937, 4));
    objects->addObject(getCreateBtn(3938, 4));
    objects->addObject(getCreateBtn(3939, 4));
    objects->addObject(getCreateBtn(3940, 4));
    objects->addObject(getCreateBtn(3941, 4));
    objects->addObject(getCreateBtn(3942, 4));
    objects->addObject(getCreateBtn(3943, 4));
    objects->addObject(getCreateBtn(3944, 4));
    objects->addObject(getCreateBtn(3945, 4));
    objects->addObject(getCreateBtn(3946, 4));
    objects->addObject(getCreateBtn(3947, 4));
    objects->addObject(getCreateBtn(3948, 4));
    objects->addObject(getCreateBtn(3949, 4));
    objects->addObject(getCreateBtn(3950, 4));
    objects->addObject(getCreateBtn(3951, 4));
    objects->addObject(getCreateBtn(3952, 4));
    objects->addObject(getCreateBtn(3953, 4));
    objects->addObject(getCreateBtn(3954, 4));
    objects->addObject(getCreateBtn(3955, 4));
    objects->addObject(getCreateBtn(3956, 4));
    objects->addObject(getCreateBtn(3957, 4));
    objects->addObject(getCreateBtn(3958, 4));
    objects->addObject(getCreateBtn(3959, 4));
    objects->addObject(getCreateBtn(3960, 4));
    objects->addObject(getCreateBtn(3961, 4));
    objects->addObject(getCreateBtn(3962, 4));
    objects->addObject(getCreateBtn(3963, 4));
    objects->addObject(getCreateBtn(3964, 4));
    objects->addObject(getCreateBtn(3965, 4));
    objects->addObject(getCreateBtn(3966, 4));
    objects->addObject(getCreateBtn(3967, 4));
    objects->addObject(getCreateBtn(3968, 4));
    objects->addObject(getCreateBtn(3969, 4));
    objects->addObject(getCreateBtn(3970, 4));
    objects->addObject(getCreateBtn(3971, 4));
    objects->addObject(getCreateBtn(3972, 4));
    objects->addObject(getCreateBtn(3973, 4));
    objects->addObject(getCreateBtn(3974, 4));
    objects->addObject(getCreateBtn(3975, 4));
    objects->addObject(getCreateBtn(3976, 4));
    objects->addObject(getCreateBtn(3977, 4));
    objects->addObject(getCreateBtn(3978, 4));
    objects->addObject(getCreateBtn(3979, 4));
    objects->addObject(getCreateBtn(3980, 4));
    objects->addObject(getCreateBtn(3981, 4));
    objects->addObject(getCreateBtn(3982, 4));
    objects->addObject(getCreateBtn(3983, 4));
    objects->addObject(getCreateBtn(3984, 4));
    objects->addObject(getCreateBtn(3985, 4));
    objects->addObject(getCreateBtn(3986, 4));
    objects->addObject(getCreateBtn(3987, 4));
    objects->addObject(getCreateBtn(3988, 4));
    objects->addObject(getCreateBtn(3989, 4));
    objects->addObject(getCreateBtn(3990, 4));
    objects->addObject(getCreateBtn(3991, 4));
    objects->addObject(getCreateBtn(3992, 4));
    objects->addObject(getCreateBtn(3993, 4));
    objects->addObject(getCreateBtn(3994, 4));
    objects->addObject(getCreateBtn(3995, 4));
    objects->addObject(getCreateBtn(3996, 4));
    objects->addObject(getCreateBtn(3997, 4));
    objects->addObject(getCreateBtn(3998, 4));
    objects->addObject(getCreateBtn(3999, 4));
    objects->addObject(getCreateBtn(3646, 4));
    objects->addObject(getCreateBtn(3647, 4));
    objects->addObject(getCreateBtn(3648, 4));
    objects->addObject(getCreateBtn(3649, 4));
    objects->addObject(getCreateBtn(3650, 4));
    objects->addObject(getCreateBtn(3651, 4));
    objects->addObject(getCreateBtn(3652, 4));
    objects->addObject(getCreateBtn(3653, 4));
    objects->addObject(getCreateBtn(3654, 4));
    objects->addObject(getCreateBtn(3656, 4));
    objects->addObject(getCreateBtn(3657, 4));
    objects->addObject(getCreateBtn(3658, 4));
    objects->addObject(getCreateBtn(3659, 4));

    auto tab10 = EditButtonBar::create(objects, point, 9, true, buttonsPerRow, buttonRows);
    addChild(tab10, 10);

    m_createButtonBars->addObject(tab10);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("d_spikes_01_001.png"));

    objects->addObject(getCreateBtn(41, 4));
    objects->addObject(getCreateBtn(110, 4));
    objects->addObject(getCreateBtn(106, 4));
    objects->addObject(getCreateBtn(107, 4));
    objects->addObject(getCreateBtn(498, 5));
    objects->addObject(getCreateBtn(499, 5));
    objects->addObject(getCreateBtn(129, 4));
    objects->addObject(getCreateBtn(130, 4));
    objects->addObject(getCreateBtn(131, 4));
    objects->addObject(getCreateBtn(128, 4));
    objects->addObject(getCreateBtn(123, 4));
    objects->addObject(getCreateBtn(124, 4));
    objects->addObject(getCreateBtn(125, 4));
    objects->addObject(getCreateBtn(126, 4));
    objects->addObject(getCreateBtn(127, 4));
    objects->addObject(getCreateBtn(134, 4));
    objects->addObject(getCreateBtn(190, 4));
    objects->addObject(getCreateBtn(151, 4));
    objects->addObject(getCreateBtn(152, 4));
    objects->addObject(getCreateBtn(153, 4));
    objects->addObject(getCreateBtn(225, 4));
    objects->addObject(getCreateBtn(226, 4));
    objects->addObject(getCreateBtn(500, 5));
    objects->addObject(getCreateBtn(501, 5));
    objects->addObject(getCreateBtn(229, 4));
    objects->addObject(getCreateBtn(230, 4));
    objects->addObject(getCreateBtn(237, 4));
    objects->addObject(getCreateBtn(238, 4));
    objects->addObject(getCreateBtn(239, 4));
    objects->addObject(getCreateBtn(240, 4));
    objects->addObject(getCreateBtn(241, 4));
    objects->addObject(getCreateBtn(231, 4));
    objects->addObject(getCreateBtn(232, 4));
    objects->addObject(getCreateBtn(283, 4));
    objects->addObject(getCreateBtn(284, 4));
    objects->addObject(getCreateBtn(285, 4));
    objects->addObject(getCreateBtn(279, 4));
    objects->addObject(getCreateBtn(280, 4));
    objects->addObject(getCreateBtn(233, 4));
    objects->addObject(getCreateBtn(281, 4));
    objects->addObject(getCreateBtn(282, 4));
    objects->addObject(getCreateBtn(234, 4));
    objects->addObject(getCreateBtn(235, 4));
    objects->addObject(getCreateBtn(277, 5));
    objects->addObject(getCreateBtn(278, 5));
    objects->addObject(getCreateBtn(245, 4));
    objects->addObject(getCreateBtn(246, 4));
    objects->addObject(getCreateBtn(906, 4));
    objects->addObject(getCreateBtn(414, 4));
    objects->addObject(getCreateBtn(406, 4));
    objects->addObject(getCreateBtn(407, 4));
    objects->addObject(getCreateBtn(408, 4));
    objects->addObject(getCreateBtn(907, 4));
    objects->addObject(getCreateBtn(908, 4));
    objects->addObject(getCreateBtn(909, 4));
    objects->addObject(getCreateBtn(910, 4));
    objects->addObject(getCreateBtn(940, 4));
    objects->addObject(getCreateBtn(941, 4));
    objects->addObject(getCreateBtn(942, 4));
    objects->addObject(getCreateBtn(409, 4));
    objects->addObject(getCreateBtn(410, 4));
    objects->addObject(getCreateBtn(411, 4));
    objects->addObject(getCreateBtn(412, 4));
    objects->addObject(getCreateBtn(413, 4));
    objects->addObject(getCreateBtn(1756, 4));
    objects->addObject(getCreateBtn(453, 4));
    objects->addObject(getCreateBtn(454, 4));
    objects->addObject(getCreateBtn(455, 4));
    objects->addObject(getCreateBtn(456, 4));
    objects->addObject(getCreateBtn(457, 4));
    objects->addObject(getCreateBtn(1001, 4));
    objects->addObject(getCreateBtn(1002, 4));
    objects->addObject(getCreateBtn(1003, 4));
    objects->addObject(getCreateBtn(1004, 4));
    objects->addObject(getCreateBtn(1005, 4));
    objects->addObject(getCreateBtn(668, 4));
    objects->addObject(getCreateBtn(669, 4));
    objects->addObject(getCreateBtn(670, 4));
    objects->addObject(getCreateBtn(671, 4));
    objects->addObject(getCreateBtn(672, 4));
    objects->addObject(getCreateBtn(738, 4));
    objects->addObject(getCreateBtn(1737, 4));
    objects->addObject(getCreateBtn(1738, 4));
    objects->addObject(getCreateBtn(1739, 4));
    objects->addObject(getCreateBtn(1740, 4));
    objects->addObject(getCreateBtn(1741, 4));
    objects->addObject(getCreateBtn(1742, 4));
    objects->addObject(getCreateBtn(719, 5));
    objects->addObject(getCreateBtn(721, 5));
    objects->addObject(getCreateBtn(990, 5));
    objects->addObject(getCreateBtn(992, 5));
    objects->addObject(getCreateBtn(916, 4));
    objects->addObject(getCreateBtn(917, 4));
    objects->addObject(getCreateBtn(925, 4));
    objects->addObject(getCreateBtn(926, 4));
    objects->addObject(getCreateBtn(936, 4));
    objects->addObject(getCreateBtn(937, 4));
    objects->addObject(getCreateBtn(938, 4));
    objects->addObject(getCreateBtn(939, 4));
    objects->addObject(getCreateBtn(1120, 5));
    objects->addObject(getCreateBtn(1122, 5));
    objects->addObject(getCreateBtn(1123, 5));
    objects->addObject(getCreateBtn(1124, 5));
    objects->addObject(getCreateBtn(1125, 5));
    objects->addObject(getCreateBtn(1126, 5));
    objects->addObject(getCreateBtn(1127, 5));
    objects->addObject(getCreateBtn(1132, 5));
    objects->addObject(getCreateBtn(1133, 5));
    objects->addObject(getCreateBtn(1134, 5));
    objects->addObject(getCreateBtn(1135, 5));
    objects->addObject(getCreateBtn(1136, 5));
    objects->addObject(getCreateBtn(1137, 5));
    objects->addObject(getCreateBtn(1241, 5));
    objects->addObject(getCreateBtn(1242, 5));
    objects->addObject(getCreateBtn(1243, 5));
    objects->addObject(getCreateBtn(1244, 5));
    objects->addObject(getCreateBtn(1245, 5));
    objects->addObject(getCreateBtn(1246, 5));
    objects->addObject(getCreateBtn(1138, 5));
    objects->addObject(getCreateBtn(1139, 5));
    objects->addObject(getCreateBtn(1310, 4));
    objects->addObject(getCreateBtn(1311, 4));
    objects->addObject(getCreateBtn(1312, 4));
    objects->addObject(getCreateBtn(1313, 4));
    objects->addObject(getCreateBtn(1314, 4));
    objects->addObject(getCreateBtn(1315, 4));
    objects->addObject(getCreateBtn(1316, 4));
    objects->addObject(getCreateBtn(1317, 4));
    objects->addObject(getCreateBtn(1318, 4));
    objects->addObject(getCreateBtn(1319, 4));
    objects->addObject(getCreateBtn(1320, 4));
    objects->addObject(getCreateBtn(1596, 4));
    objects->addObject(getCreateBtn(1597, 4));
    objects->addObject(getCreateBtn(1600, 4));
    objects->addObject(getCreateBtn(1601, 4));
    objects->addObject(getCreateBtn(1843, 4));
    objects->addObject(getCreateBtn(1602, 4));
    objects->addObject(getCreateBtn(1603, 4));
    objects->addObject(getCreateBtn(1604, 4));
    objects->addObject(getCreateBtn(1605, 4));
    objects->addObject(getCreateBtn(1606, 4));
    objects->addObject(getCreateBtn(1607, 4));
    objects->addObject(getCreateBtn(1608, 4));
    objects->addObject(getCreateBtn(1609, 4));
    objects->addObject(getCreateBtn(1610, 4));
    objects->addObject(getCreateBtn(1753, 4));
    objects->addObject(getCreateBtn(1754, 4));
    objects->addObject(getCreateBtn(1757, 4));
    objects->addObject(getCreateBtn(1830, 4));
    objects->addObject(getCreateBtn(1764, 4));
    objects->addObject(getCreateBtn(1765, 4));
    objects->addObject(getCreateBtn(1766, 4));
    objects->addObject(getCreateBtn(1767, 4));
    objects->addObject(getCreateBtn(1768, 4));
    objects->addObject(getCreateBtn(1835, 4));
    objects->addObject(getCreateBtn(1836, 4));
    objects->addObject(getCreateBtn(1837, 4));
    objects->addObject(getCreateBtn(1838, 4));
    objects->addObject(getCreateBtn(1844, 4));
    objects->addObject(getCreateBtn(1845, 4));
    objects->addObject(getCreateBtn(1846, 4));
    objects->addObject(getCreateBtn(1847, 4));
    objects->addObject(getCreateBtn(1848, 4));
    objects->addObject(getCreateBtn(1893, 4));
    objects->addObject(getCreateBtn(1897, 4));
    objects->addObject(getCreateBtn(1896, 4));
    objects->addObject(getCreateBtn(1898, 4));
    objects->addObject(getCreateBtn(1894, 4));
    objects->addObject(getCreateBtn(1895, 4));
    objects->addObject(getCreateBtn(1899, 4));
    objects->addObject(getCreateBtn(1900, 4));
    objects->addObject(getCreateBtn(1901, 4));
    objects->addObject(getCreateBtn(1902, 4));
    objects->addObject(getCreateBtn(3621, 4));
    objects->addObject(getCreateBtn(3622, 4));
    objects->addObject(getCreateBtn(3623, 4));
    objects->addObject(getCreateBtn(3624, 4));
    objects->addObject(getCreateBtn(3625, 4));
    objects->addObject(getCreateBtn(3626, 4));
    objects->addObject(getCreateBtn(3627, 4));
    objects->addObject(getCreateBtn(3628, 4));
    objects->addObject(getCreateBtn(3629, 4));
    objects->addObject(getCreateBtn(3630, 4));
    objects->addObject(getCreateBtn(3631, 4));
    objects->addObject(getCreateBtn(3632, 4));
    objects->addObject(getCreateBtn(3633, 4));
    objects->addObject(getCreateBtn(3634, 4));
    objects->addObject(getCreateBtn(3635, 4));
    objects->addObject(getCreateBtn(3636, 4));
    objects->addObject(getCreateBtn(3637, 4));
    objects->addObject(getCreateBtn(3638, 4));
    objects->addObject(getCreateBtn(3639, 4));
    objects->addObject(getCreateBtn(18, 4));
    objects->addObject(getCreateBtn(19, 4));
    objects->addObject(getCreateBtn(20, 4));
    objects->addObject(getCreateBtn(21, 4));
    objects->addObject(getCreateBtn(48, 4));
    objects->addObject(getCreateBtn(49, 4));
    objects->addObject(getCreateBtn(113, 4));
    objects->addObject(getCreateBtn(114, 4));
    objects->addObject(getCreateBtn(115, 4));
    objects->addObject(getCreateBtn(157, 4));
    objects->addObject(getCreateBtn(158, 4));
    objects->addObject(getCreateBtn(159, 4));
    objects->addObject(getCreateBtn(227, 4));
    objects->addObject(getCreateBtn(228, 4));
    objects->addObject(getCreateBtn(242, 4));
    objects->addObject(getCreateBtn(419, 4));
    objects->addObject(getCreateBtn(420, 4));
    objects->addObject(getCreateBtn(767, 4));
    objects->addObject(getCreateBtn(448, 4));
    objects->addObject(getCreateBtn(449, 4));
    objects->addObject(getCreateBtn(450, 4));
    objects->addObject(getCreateBtn(451, 4));
    objects->addObject(getCreateBtn(452, 4));
    objects->addObject(getCreateBtn(1292, 5));
    objects->addObject(getCreateBtn(1010, 5));
    objects->addObject(getCreateBtn(1009, 5));
    objects->addObject(getCreateBtn(1271, 5));
    objects->addObject(getCreateBtn(1272, 5));
    objects->addObject(getCreateBtn(1760, 5));
    objects->addObject(getCreateBtn(1761, 5));
    objects->addObject(getCreateBtn(1887, 5));
    objects->addObject(getCreateBtn(503, 4));
    objects->addObject(getCreateBtn(505, 4));
    objects->addObject(getCreateBtn(504, 4));
    objects->addObject(getCreateBtn(1273, 4));
    objects->addObject(getCreateBtn(1274, 4));
    objects->addObject(getCreateBtn(1291, 4));
    objects->addObject(getCreateBtn(1758, 4));
    objects->addObject(getCreateBtn(1759, 4));
    objects->addObject(getCreateBtn(1886, 4));
    objects->addObject(getCreateBtn(1011, 5));
    objects->addObject(getCreateBtn(1013, 5));
    objects->addObject(getCreateBtn(1012, 5));
    objects->addObject(getCreateBtn(1269, 5));
    objects->addObject(getCreateBtn(1270, 5));
    objects->addObject(getCreateBtn(1293, 5));
    objects->addObject(getCreateBtn(1762, 5));
    objects->addObject(getCreateBtn(1763, 5));
    objects->addObject(getCreateBtn(1888, 5));

    auto tab11 = EditButtonBar::create(objects, point, 10, true, buttonsPerRow, buttonRows);
    addChild(tab11, 10);

    m_createButtonBars->addObject(tab11);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("sawblade_02_001.png"));

    objects->addObject(getCreateBtn(1705, 4));
    objects->addObject(getCreateBtn(1706, 4));
    objects->addObject(getCreateBtn(1707, 4));
    objects->addObject(getCreateBtn(1708, 4));
    objects->addObject(getCreateBtn(1709, 4));
    objects->addObject(getCreateBtn(1710, 4));
    objects->addObject(getCreateBtn(1734, 4));
    objects->addObject(getCreateBtn(1735, 4));
    objects->addObject(getCreateBtn(1736, 4));
    objects->addObject(getCreateBtn(186, 4));
    objects->addObject(getCreateBtn(187, 4));
    objects->addObject(getCreateBtn(188, 4));
    objects->addObject(getCreateBtn(740, 5));
    objects->addObject(getCreateBtn(741, 5));
    objects->addObject(getCreateBtn(742, 5));
    objects->addObject(getCreateBtn(678, 4));
    objects->addObject(getCreateBtn(679, 4));
    objects->addObject(getCreateBtn(680, 4));
    objects->addObject(getCreateBtn(1619, 4));
    objects->addObject(getCreateBtn(1620, 4));
    objects->addObject(getCreateBtn(1701, 4));
    objects->addObject(getCreateBtn(1702, 4));
    objects->addObject(getCreateBtn(1703, 4));
    objects->addObject(getCreateBtn(183, 4));
    objects->addObject(getCreateBtn(184, 4));
    objects->addObject(getCreateBtn(185, 4));
    objects->addObject(getCreateBtn(85, 4));
    objects->addObject(getCreateBtn(86, 4));
    objects->addObject(getCreateBtn(87, 4));
    objects->addObject(getCreateBtn(97, 4));
    objects->addObject(getCreateBtn(137, 4));
    objects->addObject(getCreateBtn(138, 4));
    objects->addObject(getCreateBtn(139, 4));
    objects->addObject(getCreateBtn(154, 4));
    objects->addObject(getCreateBtn(155, 4));
    objects->addObject(getCreateBtn(156, 4));
    objects->addObject(getCreateBtn(180, 4));
    objects->addObject(getCreateBtn(181, 4));
    objects->addObject(getCreateBtn(182, 4));
    objects->addObject(getCreateBtn(222, 4));
    objects->addObject(getCreateBtn(223, 4));
    objects->addObject(getCreateBtn(224, 4));
    objects->addObject(getCreateBtn(375, 4));
    objects->addObject(getCreateBtn(376, 4));
    objects->addObject(getCreateBtn(377, 4));
    objects->addObject(getCreateBtn(378, 4));
    objects->addObject(getCreateBtn(1521, 4));
    objects->addObject(getCreateBtn(1522, 4));
    objects->addObject(getCreateBtn(1523, 4));
    objects->addObject(getCreateBtn(1524, 4));
    objects->addObject(getCreateBtn(1525, 4));
    objects->addObject(getCreateBtn(1526, 4));
    objects->addObject(getCreateBtn(1527, 4));
    objects->addObject(getCreateBtn(1528, 4));
    objects->addObject(getCreateBtn(394, 4));
    objects->addObject(getCreateBtn(395, 4));
    objects->addObject(getCreateBtn(396, 4));
    objects->addObject(getCreateBtn(997, 4));
    objects->addObject(getCreateBtn(998, 4));
    objects->addObject(getCreateBtn(999, 4));
    objects->addObject(getCreateBtn(1000, 4));
    objects->addObject(getCreateBtn(1019, 4));
    objects->addObject(getCreateBtn(1020, 4));
    objects->addObject(getCreateBtn(1021, 4));
    objects->addObject(getCreateBtn(1055, 4));
    objects->addObject(getCreateBtn(1056, 4));
    objects->addObject(getCreateBtn(1057, 4));
    objects->addObject(getCreateBtn(1058, 4));
    objects->addObject(getCreateBtn(1059, 4));
    objects->addObject(getCreateBtn(1060, 4));
    objects->addObject(getCreateBtn(1061, 4));
    objects->addObject(getCreateBtn(1752, 4));
    objects->addObject(getCreateBtn(1831, 4));
    objects->addObject(getCreateBtn(1832, 4));
    objects->addObject(getCreateBtn(1833, 4));
    objects->addObject(getCreateBtn(1834, 4));
    objects->addObject(getCreateBtn(15, 4));
    objects->addObject(getCreateBtn(16, 4));
    objects->addObject(getCreateBtn(17, 4));
    objects->addObject(getCreateBtn(50, 4));
    objects->addObject(getCreateBtn(51, 4));
    objects->addObject(getCreateBtn(52, 4));
    objects->addObject(getCreateBtn(53, 4));
    objects->addObject(getCreateBtn(54, 4));
    objects->addObject(getCreateBtn(60, 4));
    objects->addObject(getCreateBtn(148, 4));
    objects->addObject(getCreateBtn(149, 4));
    objects->addObject(getCreateBtn(405, 4));
    objects->addObject(getCreateBtn(132, 4));
    objects->addObject(getCreateBtn(460, 4));
    objects->addObject(getCreateBtn(494, 4));
    objects->addObject(getCreateBtn(133, 4));
    objects->addObject(getCreateBtn(136, 4));
    objects->addObject(getCreateBtn(150, 4));
    objects->addObject(getCreateBtn(236, 4));
    objects->addObject(getCreateBtn(497, 4));
    objects->addObject(getCreateBtn(495, 4));
    objects->addObject(getCreateBtn(496, 4));

    auto tab12 = EditButtonBar::create(objects, point, 11, true, buttonsPerRow, buttonRows);
    addChild(tab12, 10);

    m_createButtonBars->addObject(tab12);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("edit_eTintCol01Btn_001.png"));

    objects->addObject(getCreateBtn(31, 4));
    objects->addObject(getCreateBtn(899, 4));
    objects->addObject(getCreateBtn(901, 4));
    objects->addObject(getCreateBtn(1616, 4));
    objects->addObject(getCreateBtn(1006, 4));
    objects->addObject(getCreateBtn(1007, 4));
    objects->addObject(getCreateBtn(1049, 4));
    objects->addObject(getCreateBtn(1268, 4));
    objects->addObject(getCreateBtn(1346, 4));
    objects->addObject(getCreateBtn(2067, 4));
    objects->addObject(getCreateBtn(1347, 4));
    objects->addObject(getCreateBtn(1520, 4));
    objects->addObject(getCreateBtn(1585, 4));
    objects->addObject(getCreateBtn(3033, 4));
    objects->addObject(getCreateBtn(1814, 4));
    objects->addObject(getCreateBtn(3016, 4));
    objects->addObject(getCreateBtn(3660, 4));
    objects->addObject(getCreateBtn(3661, 4));
    objects->addObject(getCreateBtn(3032, 4));
    objects->addObject(getCreateBtn(3006, 4));
    objects->addObject(getCreateBtn(3007, 4));
    objects->addObject(getCreateBtn(3008, 4));
    objects->addObject(getCreateBtn(3009, 4));
    objects->addObject(getCreateBtn(3010, 4));
    objects->addObject(getCreateBtn(3011, 4));
    objects->addObject(getCreateBtn(3012, 4));
    objects->addObject(getCreateBtn(3013, 4));
    objects->addObject(getCreateBtn(3014, 4));
    objects->addObject(getCreateBtn(3015, 4));
    objects->addObject(getCreateBtn(3024, 4));
    objects->addObject(getCreateBtn(3029, 4));
    objects->addObject(getCreateBtn(3030, 4));
    objects->addObject(getCreateBtn(3031, 4));
    objects->addObject(getCreateBtn(1595, 4));
    objects->addObject(getCreateBtn(1611, 4));
    objects->addObject(getCreateBtn(1811, 4));
    objects->addObject(getCreateBtn(1817, 4));
    objects->addObject(getCreateBtn(3614, 4));
    objects->addObject(getCreateBtn(3615, 4));
    objects->addObject(getCreateBtn(3617, 4));
    objects->addObject(getCreateBtn(3619, 4));
    objects->addObject(getCreateBtn(3620, 4));
    objects->addObject(getCreateBtn(3641, 4));
    objects->addObject(getCreateBtn(1912, 4));
    objects->addObject(getCreateBtn(2068, 4));
    objects->addObject(getCreateBtn(3607, 4));
    objects->addObject(getCreateBtn(3608, 4));
    objects->addObject(getCreateBtn(3618, 4));
    objects->addObject(getCreateBtn(1913, 4));
    objects->addObject(getCreateBtn(1914, 4));
    objects->addObject(getCreateBtn(1916, 4));
    objects->addObject(getCreateBtn(2901, 4));
    objects->addObject(getCreateBtn(2015, 4));
    objects->addObject(getCreateBtn(2062, 4));
    objects->addObject(getCreateBtn(2925, 4));
    objects->addObject(getCreateBtn(2016, 4));
    objects->addObject(getCreateBtn(1917, 4));
    objects->addObject(getCreateBtn(2900, 4));
    objects->addObject(getCreateBtn(1934, 4));
    objects->addObject(getCreateBtn(3605, 4));
    objects->addObject(getCreateBtn(3602, 4));
    objects->addObject(getCreateBtn(3603, 4));
    objects->addObject(getCreateBtn(3604, 4));
    objects->addObject(getCreateBtn(1935, 4));
    objects->addObject(getCreateBtn(2999, 4));
    objects->addObject(getCreateBtn(3606, 4));
    objects->addObject(getCreateBtn(3612, 4));
    objects->addObject(getCreateBtn(1615, 4));
    objects->addObject(getCreateBtn(3613, 4));
    objects->addObject(getCreateBtn(3662, 4));
    objects->addObject(getCreateBtn(1815, 4));
    objects->addObject(getCreateBtn(3609, 4));
    objects->addObject(getCreateBtn(3640, 4));
    objects->addObject(getCreateBtn(1816, 4));
    objects->addObject(getCreateBtn(3643, 4));
    objects->addObject(getCreateBtn(1812, 4));
    objects->addObject(getCreateBtn(33, 4));
    objects->addObject(getCreateBtn(32, 4));
    objects->addObject(getCreateBtn(1613, 4));
    objects->addObject(getCreateBtn(1612, 4));
    objects->addObject(getCreateBtn(1818, 4));
    objects->addObject(getCreateBtn(1819, 4));
    objects->addObject(getCreateBtn(3600, 4));
    objects->addObject(getCreateBtn(1932, 4));
    objects->addObject(getCreateBtn(2899, 4));
    objects->addObject(getCreateBtn(3642, 4));
    objects->addObject(getCreateBtn(2903, 4));
    objects->addObject(getCreateBtn(2066, 4));
    objects->addObject(getCreateBtn(3022, 4));
    objects->addObject(getCreateBtn(2904, 4));
    objects->addObject(getCreateBtn(2905, 4));
    objects->addObject(getCreateBtn(2907, 4));
    objects->addObject(getCreateBtn(2909, 4));
    objects->addObject(getCreateBtn(2910, 4));
    objects->addObject(getCreateBtn(2911, 4));
    objects->addObject(getCreateBtn(2912, 4));
    objects->addObject(getCreateBtn(2913, 4));
    objects->addObject(getCreateBtn(2914, 4));
    objects->addObject(getCreateBtn(2915, 4));
    objects->addObject(getCreateBtn(2916, 4));
    objects->addObject(getCreateBtn(2917, 4));
    objects->addObject(getCreateBtn(2919, 4));
    objects->addObject(getCreateBtn(2920, 4));
    objects->addObject(getCreateBtn(2921, 4));
    objects->addObject(getCreateBtn(2922, 4));
    objects->addObject(getCreateBtn(2923, 4));
    objects->addObject(getCreateBtn(2924, 4));
    objects->addObject(getCreateBtn(22, 4));
    objects->addObject(getCreateBtn(24, 4));
    objects->addObject(getCreateBtn(23, 4));
    objects->addObject(getCreateBtn(25, 4));
    objects->addObject(getCreateBtn(26, 4));
    objects->addObject(getCreateBtn(27, 4));
    objects->addObject(getCreateBtn(28, 4));
    objects->addObject(getCreateBtn(55, 4));
    objects->addObject(getCreateBtn(56, 4));
    objects->addObject(getCreateBtn(57, 4));
    objects->addObject(getCreateBtn(58, 4));
    objects->addObject(getCreateBtn(59, 4));
    objects->addObject(getCreateBtn(1915, 4));
    objects->addObject(getCreateBtn(3017, 4));
    objects->addObject(getCreateBtn(3018, 4));
    objects->addObject(getCreateBtn(3019, 4));
    objects->addObject(getCreateBtn(3020, 4));
    objects->addObject(getCreateBtn(3021, 4));
    objects->addObject(getCreateBtn(3023, 4));

    auto tab13 = EditButtonBar::create(objects, point, 12, true, buttonsPerRow, buttonRows);
    addChild(tab13, 10);

    m_createButtonBars->addObject(tab13);
    objects->removeAllObjects();

    iconTextures->addObject(CCString::create("custom"));

    auto custom = createCustomItems();

    auto tab14 = EditButtonBar::create(custom, point, 13, true, buttonsPerRow, buttonRows);
    addChild(tab14, 10);

    m_createButtonBars->addObject(tab14);
    objects->removeAllObjects();

    int i = 0;
    for (auto str : CCArrayExt<CCString, false>(iconTextures)) {
        auto tabOff = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
        auto tabOn = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");

        auto texture = str->getCString();

        CCSprite* spriteOff;
        CCSprite* spriteOn;

        if (strcmp(texture, "custom") == 0) {
            auto label = CCLabelBMFont::create("C", "bigFont.fnt");
            auto child = label->getChildrenExt<CCSprite>()[0];

            spriteOff = CCSprite::createWithTexture(child->getTexture(), child->getTextureRect());
            spriteOn = CCSprite::createWithTexture(child->getTexture(), child->getTextureRect());
        }
        else {
            spriteOff = CCSprite::createWithSpriteFrameName(texture);
            spriteOn = CCSprite::createWithSpriteFrameName(texture);
        }

        auto center = tabOff->convertToNodeSpace({0, 0});
        auto tabSize = tabOff->getContentSize();

        auto pos = center + CCPoint{0, -1};

        spriteOff->setPosition(pos);
        spriteOn->setPosition(pos);

        auto iconSize = spriteOff->getContentSize();

        float scaleX = (tabSize.width  - 8.f) / iconSize.width;
        float scaleY = (tabSize.height - 4.f) / iconSize.height;

        float scale = std::min(scaleX, scaleY);

        spriteOff->setScale(scale);
        spriteOn->setScale(scale);

        tabOff->addChild(spriteOff);
        tabOn->addChild(spriteOn);

        tabOff->setOpacity(150);
        spriteOff->setOpacity(150);

        auto toggler = CCMenuItemToggler::create(tabOff, tabOn, this, menu_selector(EditorUI::onSelectBuildTab));
        
        toggler->m_notClickable = true;
        toggler->setTag(i);

        m_tabsMenu->addChild(toggler);
        m_tabsArray->addObject(toggler);

        i++;
    }

    m_tabsMenu->alignItemsHorizontallyWithPadding(2.f);
    for (auto item : CCArrayExt<CCMenuItemToggler, false>(m_tabsArray)) {
        item->setSizeMult(1.2f);
    }

    m_tabsMenu->setPosition(point + CCPoint{0, 14});

    for (auto bar : CCArrayExt<EditButtonBar, false>(m_createButtonBars)) {
        auto lastBuildPage = m_editorLayer->m_level->getLastBuildPageForTab(bar->m_tabIndex);
        bar->goToPage(lastBuildPage);
    }

    selectBuildTab(m_editorLayer->m_level->m_lastBuildTab);

    m_createButtonBar->goToPage(m_editorLayer->m_level->m_lastBuildPage);

    m_editorLayer->m_currentLayer = m_editorLayer->m_level->m_lastBuildGroupID;

    updateCreateMenu(false);
}

void REEditorUI::setupDeleteMenu() {
    //todo
    EditorUI::setupDeleteMenu();
}

void REEditorUI::setupEditMenu_() {
    createMoveMenu();
    updateEditMenu_();
}

void REEditorUI::setupTransformControl_() {}

bool REEditorUI::shouldDeleteObject(GameObject* object) {
    auto gameManager = GameManager::get();

    auto filter = gameManager->getIntGameVariable(GameVar::DeleteFilter);
    if (filter != 0) {
        if (filter == 1) {
            return object->getType() == GameObjectType::Decoration;
        }
        if (filter == 2) {
            auto type = object->getType();
            return type == GameObjectType::Solid || type == GameObjectType::Slope;
        }
        if (filter == 3) {
            auto objFilter = gameManager->getIntGameVariable(GameVar::CustomDeleteFilter);
            return object->m_objectID == objFilter;
        }
    }
    return true;
}

bool REEditorUI::shouldSnap_(GameObject* object) {
    return static_cast<int>(object->getRotation()) % 90 == 0;
}

void REEditorUI::showDeleteConfirmation_() {
    FLAlertLayer::create(this, "Delete All", "Are you sure you want to <cr>Delete</c> all objects? (<cy>This action cannot be undone</c>)", "Cancel", "Yes", 300.f)->show();
}

void REEditorUI::showLiveColorSelectForMode(int colorID) {
    closeLiveColorSelect();
    closeLiveHSVSelect();

    auto base = GameManager::get()->m_levelEditorLayer->m_levelSettings->m_effectManager->getColorAction(colorID);
    m_colorOverlay = ColorSelectLiveOverlay::create(base, nullptr, nullptr);
    m_colorOverlay->m_delegate = this;
    m_colorOverlay->m_scene = this;
    m_colorOverlay->show();
}

void REEditorUI::showLiveColorSelectForModeSpecial_(int colorID) {
    if (m_selectedObject) {
        m_selectedObject->deselectObject();
    }
    else {
        for (auto object : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            object->deselectObject();
        }
    }
    showLiveColorSelectForMode(colorID);
}

void REEditorUI::showMaxBasicError() {
    auto message = CCString::createWithFormat("You have reached the max limit of <cg>%i</c> objects. You can increase it to <cy>%i</c>, but your level may become unstable on some devices, so it is recommended to stay below this limit. Levels with object count above this li mit will display a <co>warning</c> when played online.", 40000, 80000);
    auto alert = FLAlertLayer::create(this, "Max Objects", message->getCString(), "Cancel", "Increase", 380.f, false, 0.f, 1.f);
    alert->setTag(125);
    alert->show();
}

void REEditorUI::showMaxCoinError_() {
    auto alert = FLAlertLayer::create(this, "Max Coins", "You cannot add more than <cy>3</c> <cg>coins</c> in a single level.", "OK", nullptr, 300.f, false, 0.f, 1.f);
    alert->setTag(122);
    alert->show();
}

void REEditorUI::showMaxError() {
    auto message = CCString::createWithFormat("You have reached the upper max limit of <cg>%i</c> objects. This limit can  be removed completely, but it is <cr>not</c> recommended. Levels with objec t count above this limit will display a <co>warning</c> when played online.\n<cy>Unoptimized levels that use too many objects have a lower chance of bei ng rated and featured.</c>", 80000);
    auto alert = FLAlertLayer::create(this, "Max Objects", message->getCString(), "Cancel", "Unlimited", 380.f, false, 0.f, 1.f);
    alert->setTag(126);
    alert->show();
}

void REEditorUI::showUI(bool show) {
    if (!GameManager::get()->getGameVariable(GameVar::PlaytestNoUI)) {
        show = true;
    }
    for (auto item : CCArrayExt<CreateMenuItem, false>(m_uiItems)) {
        item->setVisible(show);
    }

    if (!m_stickyControlsEnabled) {
        m_unlinkBtn->setVisible(false);
        m_linkBtn->setVisible(false);
        m_enableLinkBtn->setVisible(false);
    }
    if (show) {
        if (m_selectedMode == 1) {
            m_deleteMenu->setVisible(true);
        }
        if (m_selectedMode == 2) {
            m_createButtonBar->setVisible(true);
        }
        if (m_selectedMode == 3) {
            m_editButtonBar->setVisible(true);
        }
        updateGroupIDLabel();
    }
    else {
        m_deleteMenu->setVisible(false);
        m_createButtonBar->setVisible(false);
        m_editButtonBar->setVisible(false);
    }
}

void REEditorUI::sliderChanged(CCObject* sender) {
    auto thumb = static_cast<SliderThumb*>(sender);
    auto xPos = xPosFromValue_(thumb->getValue());
    m_editorLayer->m_objectLayer->setPosition(-xPos, m_editorLayer->m_objectLayer->getPosition().y);
    constrainGameLayerPosition_();
}

CCSprite* REEditorUI::spriteFromObjectString(gd::string str, bool absoluteCenter, bool useGroup, int objLimit, CCArray* objects, CCArray* group, GameObject* groupParent) {
    //todo
    return EditorUI::spriteFromObjectString(str, absoluteCenter, useGroup, objLimit, objects, group, groupParent);
}

void REEditorUI::toggleDuplicateButton_() {
    if (m_selectedObject || m_selectedObjects->count() != 0) {
        m_copyPasteBtn->m_animationEnabled = true;
        m_copyPasteBtn->setColor({ 255, 255, 255 });
        m_copyPasteBtn->setOpacity(255);
        m_copyBtn->m_animationEnabled = true;
        m_copyBtn->setColor({ 255, 255, 255 });
        m_copyBtn->setOpacity(255);
    }
    else {
        m_copyPasteBtn->m_animationEnabled = false;
        m_copyPasteBtn->setColor({ 166, 166, 166 });
        m_copyPasteBtn->setOpacity(175);
        m_copyBtn->m_animationEnabled = false;
        m_copyBtn->setColor({ 166, 166, 166 });
        m_copyBtn->setOpacity(175);
    }

    auto hasClipboard = !GameManager::get()->m_editorClipboard.empty();
    m_pasteBtn->m_animationEnabled = hasClipboard;

    if (hasClipboard) {
        m_pasteBtn->setColor({ 255, 255, 255 });
        m_pasteBtn->setOpacity(255);
    }
    else {
        m_pasteBtn->setColor({ 166, 166, 166 });
        m_pasteBtn->setOpacity(175);
    }
}

void REEditorUI::toggleEditObjectButton() {
    bool usable = editButtonUsable();

    m_editObjectBtn->m_animationEnabled = usable;
    m_editObjectBtn->setColor(usable ? ccColor3B{255, 255, 255} : ccColor3B{166, 166, 166});
    m_editObjectBtn->setOpacity(usable ? 255 : 175);

    bool specialUsable = editButton2Usable();

    m_editSpecialBtn->m_animationEnabled = specialUsable;
    m_editSpecialBtn->setColor(specialUsable ? ccColor3B{255, 255, 255} : ccColor3B{166, 166, 166});
    m_editSpecialBtn->setOpacity(specialUsable ? 255 : 175);

    updateEditColorButton();
}

void REEditorUI::toggleEnableRotate(CCObject* sender) {
    auto gameManager = GameManager::get();

    bool enableRotate = gameManager->getGameVariable(GameVar::EnableRotate);
    gameManager->setGameVariable(GameVar::EnableRotate, !enableRotate);

    auto buttonSprite = static_cast<ButtonSprite*>(m_rotateBtn->getNormalImage());

    if (enableRotate) {
        buttonSprite->updateBGImage("GJ_button_01.png");
        deactivateRotationControl_();
    }
    else {
        buttonSprite->updateBGImage("GJ_button_02.png");
        toggleSpecialEditButtons();
    }
}

void REEditorUI::toggleFreeMove(CCObject* sender) {
    m_freeMoveEnabled = !m_freeMoveEnabled;

    auto buttonSprite = static_cast<ButtonSprite*>(m_freeMoveBtn->getNormalImage());
    if (!m_freeMoveEnabled) {
        buttonSprite->updateBGImage("GJ_button_01.png");
    }
    else {
        buttonSprite->updateBGImage("GJ_button_02.png");
    }

    GameManager::get()->setGameVariable(GameVar::FreeMove, m_freeMoveEnabled);
}

void REEditorUI::toggleLockUI_(bool lockUI) {
    m_isPaused = lockUI;
}

void REEditorUI::toggleMode(CCObject* sender) {
    int mode = sender->getTag();
    if (m_selectedMode != mode) {
        m_selectedMode = mode;
        resetUI();

        if (m_selectedMode == 1) {
            auto sprite = static_cast<CCSprite*>(m_deleteModeBtn->getNormalImage());
            sprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("edit_deleteSBtn_001.png"));
        }
        if (m_selectedMode == 2) {
            auto sprite = static_cast<CCSprite*>(m_buildModeBtn->getNormalImage());
            sprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("edit_buildSBtn_001.png"));
        }
        if (m_selectedMode == 3) {
            auto sprite = static_cast<CCSprite*>(m_editModeBtn->getNormalImage());
            sprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("edit_editSBtn_001.png"));
        }
    }
}

void REEditorUI::toggleObjectInfoLabel() {
    bool enableObjectInfo = GameManager::get()->getGameVariable(GameVar::ShowObjectInfo);
    m_objectInfoLabel->setVisible(enableObjectInfo);
    if (enableObjectInfo) {
        updateObjectInfoLabel();
    }
}

void REEditorUI::toggleSnap(CCObject* sender) {
    auto gameManager = GameManager::get();

    bool enableSnap = gameManager->getGameVariable(GameVar::EnableSnap);
    gameManager->setGameVariable(GameVar::EnableSnap, !enableSnap);

    auto buttonSprite = static_cast<ButtonSprite*>(m_snapBtn->getNormalImage());

    if (enableSnap) {
        buttonSprite->updateBGImage("GJ_button_01.png");
    }
    else {
        buttonSprite->updateBGImage("GJ_button_02.png");
    }
}

void REEditorUI::toggleSpecialEditButtons() {
    //todo
    EditorUI::toggleSpecialEditButtons();
}

void REEditorUI::toggleSwipe(CCObject* sender) {
    m_swipeEnabled = !m_swipeEnabled;

    auto buttonSprite = static_cast<ButtonSprite*>(m_swipeBtn->getNormalImage());
    if (!m_swipeEnabled) {
        buttonSprite->updateBGImage("GJ_button_01.png");
    }
    else {
        buttonSprite->updateBGImage("GJ_button_02.png");
    }

    GameManager::get()->setGameVariable(GameVar::Swipe, m_swipeEnabled);
}

void REEditorUI::transformObject(GameObject* object, EditCommand command, bool noOffset) {
    //todo
    EditorUI::transformObject(object, command, noOffset);
}

void REEditorUI::transformObjectCall(EditCommand command) {
    //todo
    EditorUI::transformObjectCall(command);
}

void REEditorUI::transformObjectCall(CCObject* sender) {
    transformObjectCall(static_cast<EditCommand>(sender->getTag()));
}

void REEditorUI::transformObjects(CCArray* objs, CCPoint anchor, float scaleX, float scaleY, float rotateX, float rotateY, float warpX, float warpY) {
    //todo
    EditorUI::transformObjects(objs, anchor, scaleX, scaleY, rotateX, rotateY, warpX, warpY);
}

void REEditorUI::transformObjectsActive() {
    auto selectedObjects = m_selectedObjects;

    if (!selectedObjects || selectedObjects->count() == 0) {
        if (!m_selectedObject) return;
        selectedObjects = CCArray::createWithObject(m_selectedObject);
    }

    if (selectedObjects) {
        transformObjects(selectedObjects, m_pivotPoint, m_transformState.m_scaleX, m_transformState.m_scaleY, m_transformState.m_angleX, m_transformState.m_angleY, m_transformState.m_skewX, m_transformState.m_skewY);
    }
}

void REEditorUI::transformObjectsReset_() {
    auto selectedObjects = m_selectedObjects;

    if (!selectedObjects || selectedObjects->count() == 0) {
        if (!m_selectedObject) return;
        selectedObjects = CCArray::createWithObject(m_selectedObject);
    }
    if (selectedObjects) {
        transformObjects(selectedObjects, m_pivotPoint, 1.f, 1.f, 0.f, 0.f, m_transformState.m_skewX, m_transformState.m_skewY);
    }
}

void REEditorUI::triggerSwipeMode() {
    m_swipeModeTriggered = true;

    if (m_selectedMode == 3) {
        m_swipeActive = true;

        m_swipeEnd = m_swipeStart;

        if (!m_continuousSnap) {
            m_snapObject = nullptr;
            m_snapObjectExists = false;
        }
    }

    playCircleAnim_(m_swipeStart, 30.f, 0.2f);
}

void REEditorUI::tryUpdateTimeMarkers_() {}

void REEditorUI::undoLastAction(CCObject* object) {
    m_editorLayer->undoLastAction();
    updateButtons();

    if (m_rotationControl->isVisible()) {
        m_canActivateControls = true;
        activateRotationControl(nullptr);
    }

    tryUpdateTimeMarkers_();
    updateSpecialUIElements_();
}

void REEditorUI::updateButtons() {
    toggleEditObjectButton();
    auto gameManager = GameManager::get();

    bool hasSelection = m_selectedObject || (m_selectedObjects && m_selectedObjects->count() > 0);

    auto applyState = [](CCMenuItemSpriteExtra* btn, bool enabled) {
        btn->m_animationEnabled = enabled;

        btn->setColor(enabled ? ccColor3B{255, 255, 255} : ccColor3B{175, 175, 175});
        btn->setOpacity(enabled ? 255 : 175);
    };

    applyState(m_copyPasteBtn, hasSelection);
    applyState(m_copyBtn, hasSelection);

    bool hasClipboard = !gameManager->m_editorClipboard.empty();

    applyState(m_pasteBtn, hasClipboard);

    m_deleteMenu->setVisible(m_selectedMode == 1);
    updateDeleteButtons();
    toggleSpecialEditButtons();

    applyState(m_deselectBtn, hasSelection);

    applyState(m_undoBtn, m_editorLayer->m_undoObjects->count() > 0);
    applyState(m_redoBtn, m_editorLayer->m_redoObjects->count() > 0);

    m_deleteButton->setEnabled(hasSelection);
    applyState(m_trashBtn, hasSelection);


    applyState(m_editGroupBtn, hasSelection);
    applyState(m_copyValuesBtn, m_selectedObject != nullptr);

    bool canGoToLayer = false;

    if (m_selectedObject) {
        canGoToLayer = true;
    }
    else if (m_selectedObjects && m_selectedObjects->count() > 0) {
        int sharedLayer = -1;
        bool different = false;

        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            int layer = obj->m_editorLayer;

            if (sharedLayer != -1 && layer != sharedLayer) {
                different = true;
                break;
            }

            sharedLayer = layer;
        }

        canGoToLayer = (!different && sharedLayer > 0);
    }

    applyState(m_goToLayerBtn, canGoToLayer);

    bool canPasteState = gameManager->m_levelEditorLayer->m_copyStateObject && hasSelection;

    applyState(m_pasteStateBtn, canPasteState);
    applyState(m_pasteColorBtn, canPasteState);

    if (m_stickyControlsEnabled) {
        const char* frame = m_linkControlsDisabled ? "gj_linkDisableBtn_001.png" : "gj_linkEnableBtn_001.png";

        static_cast<CCSprite*>(m_enableLinkBtn->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frame));

        m_enableLinkBtn->setOpacity(m_linkControlsDisabled ? 200 : 255);

        bool canLink = !m_linkControlsDisabled && m_selectedObjects && m_selectedObjects->count() > 0;

        applyState(m_linkBtn, canLink);

        bool canUnlink = false;

        if (!m_linkControlsDisabled) {
            if (m_selectedObject) {
                canUnlink = m_selectedObject->m_linkedGroup > 0;
            }
            else if (m_selectedObjects) {
                for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
                    if (obj->m_linkedGroup > 0) {
                        canUnlink = true;
                        break;
                    }
                }
            }
        }

        applyState(m_unlinkBtn, canUnlink);
    }
}

void REEditorUI::updateCreateMenu(bool selectTab) {
    if (m_selectedMode != 2) {
        m_createButtonBar->setVisible(false);
        m_tabsMenu->setVisible(false);
        return;
    }

    m_createButtonBar->setVisible(true);
    m_tabsMenu->setVisible(true);

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_createButtonArray)) {
        enableButton_(item);
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_customObjectButtonArray)) {
        enableButton_(item);
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_createButtonArray)) {
        if (item->m_objectID == m_selectedObjectIndex) {
            disableButton_(item);
            if (!selectTab) {
                return;
            }
            selectBuildTab(item->m_tabIndex);
            m_createButtonBar->goToPage(item->m_pageIndex);
            return;
        }
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_customObjectButtonArray)) {
        if (item->m_objectID == m_selectedObjectIndex) {
            disableButton_(item);
            if (!selectTab) {
                return;
            }
            selectBuildTab(item->m_tabIndex);
            m_createButtonBar->goToPage(item->m_pageIndex);
            return;
        }
    }
}

void REEditorUI::updateDeleteButtons() {
    auto gameManager = GameManager::get();
    auto objectToolbox = ObjectToolbox::sharedState();

    int deleteFilter = gameManager->getIntGameVariable(GameVar::DeleteFilter);

    static_cast<ButtonSprite*>(m_deleteFilterNone->getNormalImage())->updateBGImage(deleteFilter == 0 ? "GJ_button_02.png" : "GJ_button_04.png");
    static_cast<ButtonSprite*>(m_deleteFilterDetails->getNormalImage())->updateBGImage(deleteFilter == 1 ? "GJ_button_02.png" : "GJ_button_04.png");
    static_cast<ButtonSprite*>(m_deleteFilterStatic->getNormalImage())->updateBGImage(deleteFilter == 2 ? "GJ_button_02.png" : "GJ_button_04.png");
    static_cast<ButtonSprite*>(m_deleteFilterCustom->getNormalImage())->updateBGImage(deleteFilter == 3 ? "GJ_button_02.png" : "GJ_button_04.png");

    if (deleteFilter == 3) {
        int deleteID = gameManager->getIntGameVariable(GameVar::CustomDeleteFilter);

        auto frameName = objectToolbox->intKeyToFrame(deleteID);
        m_customDeleteSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
        m_customDeleteSprite->setVisible(true);
    }
    else if (m_selectedObject && !m_selectedObject->isSpecialObject()) {
        auto frameName = objectToolbox->intKeyToFrame(m_selectedObject->m_objectID);
        m_customDeleteSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
        m_customDeleteSprite->setVisible(true);
    }
    else {
        m_customDeleteSprite->setVisible(false);
    }

    float size = std::max(
        m_customDeleteSprite->getContentSize().width,
        m_customDeleteSprite->getContentSize().height
    );

    m_customDeleteSprite->setScale(20.f / size);

    if (m_customDeleteSprite->getScale() > 1.f) {
        m_customDeleteSprite->setScale(1.f);
    }

    if (!m_selectedObject || (m_selectedObject && m_selectedObject->isSpecialObject())) {
        m_deleteAllSprite->setVisible(false);
    }
    else if (m_selectedObject) {
        auto frameName = objectToolbox->intKeyToFrame(m_selectedObject->m_objectID);
        m_deleteAllSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
        m_deleteAllSprite->setVisible(true);
    }

    float size2 = std::max(
        m_deleteAllSprite->getContentSize().width,
        m_deleteAllSprite->getContentSize().height
    );

    m_deleteAllSprite->setScale(15.f / size2);

    if (m_deleteAllSprite->getScale() > 1.f) {
        m_deleteAllSprite->setScale(1.f);
    }

    bool hasSelection = m_selectedObject || (m_selectedObjects->count() > 0);

    m_deleteButton->setEnabled(hasSelection);
    auto deleteBtnSprite = static_cast<ButtonSprite*>(m_deleteButton->getNormalImage());
    auto deleteBtnColor = hasSelection ? ccColor3B{255, 255, 255} : ccColor3B{175, 175, 175};
    deleteBtnSprite->setColor(deleteBtnColor);

    m_deleteAllButton->setEnabled(m_selectedObject);
    auto deleteAllBtnSprite = static_cast<ButtonSprite*>(m_deleteAllButton->getNormalImage());
    auto deleteAllBtnColor = m_selectedObject ? ccColor3B{255, 255, 255} : ccColor3B{175, 175, 175};
    deleteAllBtnSprite->setColor(deleteAllBtnColor);
    
    bool deleteFilterEnabled = deleteFilter == 3 || m_selectedObject;

    m_deleteFilterCustom->setEnabled(deleteFilterEnabled);
    auto deleteFilterSprite = static_cast<ButtonSprite*>(m_deleteFilterCustom->getNormalImage());
    auto deleteFilterColor = deleteFilterEnabled ? ccColor3B{255, 255, 255} : ccColor3B{175, 175, 175};
    deleteFilterSprite->setColor(deleteFilterColor);
}

void REEditorUI::updateDeleteMenu_() {
    m_deleteMenu->setVisible(m_selectedMode == 1);
    updateDeleteButtons();
}

void REEditorUI::updateEditButtonColor(int tag, ccColor3B color) {
    if (!m_editButtonDict) return;

    auto btnSprite = static_cast<CCSprite*>(m_editButtonDict->objectForKey(CCString::createWithFormat("%i", tag)->getCString()));
    if (btnSprite) {
        btnSprite->setColor(color);
    }
}

void REEditorUI::updateEditColorButton() {
    bool hasSelection = m_selectedObject || (m_selectedObjects && m_selectedObjects->count() > 0);

    m_editHSVBtn->m_animationEnabled = hasSelection;

    if (hasSelection) {
        m_editHSVBtn->setColor({255, 255, 255});
        m_editHSVBtn->setOpacity(255);
    }
    else {
        m_editHSVBtn->setColor({175, 175, 175});
        m_editHSVBtn->setOpacity(175);
        return;
    }

    if (!m_colorOverlay) {
        ColorAction* colorAction1 = nullptr;
        ColorAction* colorAction2 = nullptr;
        EffectGameObject* effectObj = nullptr;

        getEditColorTargets(colorAction1, colorAction2, effectObj);

        if (colorAction1 || colorAction2 || effectObj) {
            auto normalSprite = static_cast<CCSprite*>(m_editHSVBtn->getNormalImage());

            auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_editHSVBtn2_001.png");

            normalSprite->setDisplayFrame(frame);
            m_editHSVBtn->setTag(0);
            return;
        }
    }

    auto normalSprite = static_cast<CCSprite*>(m_editHSVBtn->getNormalImage());

    auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_hsvBtn_001.png");

    normalSprite->setDisplayFrame(frame);

    m_editHSVBtn->setTag(1);
}

void REEditorUI::updateEditMenu_() {
    m_editButtonBar->setVisible(m_selectedMode == 3);
}

void REEditorUI::updateGridNodeSize() {
    int sharedID = -1;

    if (m_selectedMode == 2) {
        sharedID = m_selectedObjectIndex;
    }
    else if (m_selectedObject) {
        sharedID = m_selectedObject->m_objectID;
    }
    else {
        float lastSize = -1.f;

        for (auto obj : CCArrayExt<GameObject, false>(m_selectedObjects)) {
            float size = ObjectToolbox::sharedState()->gridNodeSizeForKey(obj->m_objectID);

            if (lastSize != -1.f && size != lastSize) {
                sharedID = -1;
                break;
            }

            lastSize = size;
            sharedID = obj->m_objectID;
        }
    }
    
    updateGridNodeSize_(sharedID);
}

void REEditorUI::updateGridNodeSize_(int objectID) {
    auto size = ObjectToolbox::sharedState()->gridNodeSizeForKey(objectID);
    m_gridSize = size;
    m_editorLayer->m_drawGridLayer->m_gridSize = size;
}

void REEditorUI::updateGroupIDBtn2_() {
    m_goToBaseBtn->setVisible(m_editorLayer->m_currentLayer >= 0);
}

void REEditorUI::updateGroupIDLabel() {
    if (m_editorLayer->m_currentLayer >= 0) {
        m_currentLayerLabel->setString(CCString::createWithFormat("%i",m_editorLayer->m_currentLayer)->getCString());
    }
    else {
        m_currentLayerLabel->setString("All");
    }

    m_currentLayerLabel->limitLabelWidth(38.f, 0.6f, 0.f);

    bool locked = m_editorLayer->m_currentLayer >= 0 && m_editorLayer->isLayerLocked(m_editorLayer->m_currentLayer);

    ccColor3B color = {255, 255, 255};
    if (locked) color = {255, 150, 0};

    m_currentLayerLabel->setColor(color);
    m_layerLockSprite->setVisible(locked);

    updateGroupIDBtn2_();
}

void REEditorUI::updateObjectInfoLabel() {
    auto gameManager = GameManager::get();

    if (!gameManager->getGameVariable(GameVar::ShowObjectInfo))
        return;

    if ((!m_selectedObjects || m_selectedObjects->count() == 0) && !m_selectedObject) {
        m_objectInfoLabel->setString(" ");
        return;
    }

    const int UNSET = 0xfffffffe;
    const int MIXED = 0xffffd8ef;

    auto mergeVal = [&](int& a, int b) {
        if (a == UNSET) a = b;
        else if (a != b) a = MIXED;
    };

    CCArray* objects = (m_selectedObjects && m_selectedObjects->count() > 0) ? m_selectedObjects : CCArray::createWithObject(m_selectedObject);

    int baseColor = UNSET;
    int detailColor = UNSET;

    int zOrder = UNSET;
    int editorLayer1 = UNSET;
    int editorLayer2 = UNSET;

    ZLayer zLayer = ZLayer::B3;
    bool zLayerInit = false;

    std::map<int,int> groups;
    std::map<int,int> parentGroups;
    std::map<int,int> controlIDs;

    int totalGroupsProcessed = 0;

    for (auto obj : CCArrayExt<GameObject, false>(objects)) {
        if (!obj) continue;

        int bc = UNSET;
        if (obj->m_baseColor) {
            bc = obj->m_baseColor->m_colorID;
            if (bc == 0 || bc == obj->m_baseColor->m_defaultColorID) {
                bc = obj->m_baseColor->m_defaultColorID;
            }
        }
        mergeVal(baseColor, bc);

        int dc = UNSET;
        if (obj->m_detailColor) {
            dc = obj->m_detailColor->m_colorID;
            if (dc == 0 || dc == obj->m_detailColor->m_defaultColorID) {
                dc = obj->m_detailColor->m_defaultColorID;
            }
        }
        mergeVal(detailColor, dc);

        ZLayer zl = (obj->m_zLayer == ZLayer::Default) ? obj->m_defaultZLayer : obj->m_zLayer;

        if (!zLayerInit) {
            zLayer = zl;
            zLayerInit = true;
        }
        else if (zLayer != zl) {
            zLayer = static_cast<ZLayer>(MIXED);
        }

        int zo = (obj->m_zOrder == 0) ? obj->m_defaultZOrder : obj->m_zOrder;
        mergeVal(zOrder, zo);

        mergeVal(editorLayer1, obj->m_editorLayer);
        mergeVal(editorLayer2, obj->m_editorLayer2);

        if (obj->m_groups) {
            for (int g : *obj->m_groups) {
                if (g <= 0) continue;

                groups[g]++;
                totalGroupsProcessed++;

                auto parent = gameManager->m_levelEditorLayer->m_parentGroupsDict->objectForKey(g);
                if (parent == obj) {
                    parentGroups[g] = 1;
                }
            }
        }

        if (obj->m_classType == GameObjectClassType::Effect) {
            auto e = static_cast<EffectGameObject*>(obj);
            if (e->m_controlID > 0) {
                controlIDs[e->m_controlID]++;
            }
        }
    }

    std::string out;

    std::string colorStr;
    if (baseColor == MIXED) {
        colorStr = "?";
    }
    else if (baseColor < 1000) {
        colorStr = fmt::format("{}", baseColor);
    }
    else {
        colorStr = GJSpecialColorSelect::textForColorIdx(baseColor);
    }

    std::string detailColorStr;
    if (detailColor == MIXED) {
        detailColorStr = "?";
    }
    else if (detailColor < 0) {
        detailColorStr = "-";
    }
    else if (detailColor < 1000) {
        detailColorStr = fmt::format("{}", detailColor);
    }
    else {
        detailColorStr = GJSpecialColorSelect::textForColorIdx(detailColor);
    }

    std::string baseColorStr = "-";
    if (baseColor != UNSET) {
        baseColorStr = colorStr;
    }

    colorStr = "-";
    if (detailColor != UNSET) {
        colorStr = detailColorStr;
    }

    if (detailColor == UNSET || baseColor == UNSET) {
        if (detailColor == UNSET) {
            colorStr = baseColorStr;
        }
        out += fmt::format("C: {}\n", colorStr);
    }
    else {
        out += fmt::format("C1: {}\nC2: {}\n", baseColorStr, detailColorStr);
    }

    if (totalGroupsProcessed > 0) {
        out += "G: ";

        int shown = 0;
        for (auto& [g, cnt] : groups) {
            if (shown++) out += ",";

            bool full = (cnt >= objects->count());
            std::string parent = parentGroups.count(g) ? "*" : "";

            if (full) {
                out += fmt::format("{}{}", g, parent);
            }
            else {
                out += fmt::format("({}{})", g, parent);
            }

            if (shown >= 5) break;
        }

        if (groups.size() > 5) {
            out += "...";
        }

        out += "\n";
    }

    const char* layer = "?";
    switch (zLayer) {
        case ZLayer::B1: layer = "B1"; break;
        case ZLayer::B2: layer = "B2"; break;
        case ZLayer::B3: layer = "B3"; break;
        case ZLayer::T1: layer = "T1"; break;
        case ZLayer::T2: layer = "T2"; break;
        case ZLayer::T3: layer = "T3"; break;
        case ZLayer::T4: layer = "T4"; break;
        default: break;
    }

    std::string zOrderStr;
    if (zOrder != MIXED && zOrder != UNSET) {
        zOrderStr = fmt::format("{}", zOrder);
    }

    out += fmt::format("Layer: {}\nZ: {}\n", layer, zOrderStr);

    if (editorLayer1 > 0 || editorLayer2 > 0) {
        std::string el;
        std::string format;
        bool first = true;

        if (editorLayer1 != MIXED) {
            el += fmt::format("{}", editorLayer1);
            first = false;
        }

        if (editorLayer2 != MIXED && editorLayer1 > 0 && editorLayer2 > 0) {
            if (first) {
                el += fmt::format("{}", editorLayer2);
            }
            else {
                el += fmt::format(", {}", editorLayer2);
            }
        }

        out += fmt::format("EL: {}\n", el);
    }

    if (!controlIDs.empty()) {
        out += "CID: ";

        int shown = 0;
        for (auto& [cid, cnt] : controlIDs) {
            if (shown++) out += ",";

            bool full = (cnt >= objects->count());

            if (full) {
                out += fmt::format("{}", cid);
            }
            else {
                out += fmt::format("({})", cid);
            }

            if (shown >= 5) break;
        }

        if (controlIDs.size() > 5)
            out += "...";

        out += "\n";
    }

    if (objects->count() == 1) {
        if (m_selectedObject->m_classType == GameObjectClassType::Effect) {
            auto e = static_cast<EffectGameObject*>(m_selectedObject);

            if (e->m_channelValue != 0.f || e->m_ordValue != 0.f) {
                out += fmt::format("Order: {},{}\n", e->m_channelValue, e->m_ordValue);
            }

            auto pos = m_selectedObject->getPosition();
            out += fmt::format("{:.1f} {:.1f}, {:.1f} {:.1f}\n", pos.x, pos.y, e->m_speedStart.x, e->m_speedStart.y);
        }
    }

    if (objects->count() > 1) {
        out += fmt::format("Objects: {}", objects->count());
    }

    m_objectInfoLabel->setString(out.c_str());
}

void REEditorUI::updatePlaybackBtn() {
    bool isNotPlaying = m_editorLayer->m_playbackMode == PlaybackMode::Not;

    auto sprite = static_cast<CCSprite*>(m_playbackBtn->getNormalImage());

    if (isNotPlaying && m_isPlayingMusic) {
        sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_stopMusicBtn_001.png"));
        m_playbackBtn->m_animationEnabled = true;
    } 
    else {
        sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_playMusicBtn_001.png"));
        m_playbackBtn->m_animationEnabled = isNotPlaying;
    }

    if (!isNotPlaying) {
        sprite->setColor({175, 175, 175});
        m_playbackBtn->setOpacity(175);
    } 
    else {
        sprite->setColor({255, 255, 255});
        m_playbackBtn->setOpacity(255);
    }

    m_playbackActive = m_isPlayingMusic && isNotPlaying;
}

void REEditorUI::updateSlider() {
    float x = m_editorLayer->m_objectLayer->getPositionX();
    float value = valueFromXPos_(x);
    m_positionSlider->setValue(value);
}

void REEditorUI::updateSpecialUIElements_() {
    updateObjectInfoLabel();
    updateScaleControl();
}

void REEditorUI::updateStickyControls_() {
    auto gameManager = GameManager::get();

    m_stickyControlsEnabled = gameManager->getGameVariable(GameVar::LinkControls);
    m_linkControlsDisabled = gameManager->getGameVariable(GameVar::LinkControlsQuickToggle);

    m_enableLinkBtn->setEnabled(m_stickyControlsEnabled);
    m_enableLinkBtn->setVisible(m_stickyControlsEnabled);

    m_linkBtn->setEnabled(m_stickyControlsEnabled);
    m_linkBtn->setVisible(m_stickyControlsEnabled);

    m_unlinkBtn->setEnabled(m_stickyControlsEnabled);
    m_unlinkBtn->setVisible(m_stickyControlsEnabled);
}

void REEditorUI::updateZoom(float zoom) {
    auto winSize = CCDirector::get()->getWinSize();

    auto oldScale = m_editorLayer->getScale();
    m_editorLayer->setScale(1.f);

    auto center = winSize / 2.f;

    auto nodeCenter = m_editorLayer->m_objectLayer->convertToNodeSpace(center);

    m_editorLayer->m_objectLayer->setScale(zoom);

    auto worldCenter = m_editorLayer->m_objectLayer->convertToWorldSpace(nodeCenter);

    m_editorLayer->m_objectLayer->setPosition(m_editorLayer->m_objectLayer->getPosition() + (center - worldCenter));

    constrainGameLayerPosition_();
    m_editorLayer->setScale(oldScale);
    
    updateSlider();
    m_rotationControl->setScale(1.f / zoom);
    m_transformControl->scaleButtons(1.f / zoom);
}

float REEditorUI::valueFromXPos_(float xPos) {
    auto xMin = getXMin(0);
    auto sectionCount = std::max(m_editorLayer->getSectionCount(), 100);
    auto scale = m_editorLayer->m_objectLayer->getScale();
    return std::clamp(-(xPos - xMin) / (sectionCount * 100.f * scale), 0.f, 1.f);
}

float REEditorUI::xPosFromValue_(float value) {
    auto xMin = getXMin(0);
    auto sectionCount = std::max(m_editorLayer->getSectionCount(), 100);
    auto scale = m_editorLayer->m_objectLayer->getScale();
    return sectionCount * 100.f * scale * value - xMin;
}

void REEditorUI::zoomGameLayer_(bool zoomingIn) {
    auto scale = m_editorLayer->m_objectLayer->getScale();
    updateZoom(std::clamp(zoomingIn ? scale + 0.1f : scale - 0.1f, 0.1f, 4.f));
}

void REEditorUI::zoomIn(CCObject* sender) {
    zoomGameLayer_(true);
}

void REEditorUI::zoomOut(CCObject* sender) {
    zoomGameLayer_(false);
}