#include "PlayerObject.hpp"

PlayerObject* REPlayerObject::create(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
    auto ret = new PlayerObject();
    if (ret->init(player, ship, gameLayer, layer, playLayer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void REPlayerObject::update(float dt) {
    auto dtModified = dt * 0.9f;

    if (m_flashTime >= 0) {
        double elapsed = (m_totalTime - m_flashTime) - m_flashDelay;
        double duration = m_flashDuration;

        if (elapsed < duration) {
            float t = elapsed / duration;

            auto mainColor = GameToolbox::multipliedColorValue(m_flashMainColor, m_originalMainColor, t);
            auto secondColor = GameToolbox::multipliedColorValue(m_flashSecondColor, m_originalSecondColor, t);

            setColor(mainColor);
            setSecondColor(secondColor);
        }
        else {
            m_flashTime = -1;

            setColor(m_originalMainColor);
            setSecondColor(m_originalSecondColor);
        }
    }

    m_yVelocity = std::clamp(m_yVelocity, -1000.0, 1000.0);

    if (m_isPlatformer) {
        m_platformerXVelocity = std::clamp(m_platformerXVelocity, -1000.0, 1000.0);
    }

    if (m_isDead) return;

    auto pos = getPosition();

    float movedDist = m_lastPosition.getDistance(pos);
    m_lastPosition = pos;

    m_yVelocityRelated3 = 0.f;

    if (!m_isLocked) {
        updateJump(dtModified);

        if (m_stateForce > 0) {
            float modeForceScale = 1.f;

            if (m_isShip) {
                modeForceScale = 0.47f;
            }
            else if (m_isBird) {
                modeForceScale = 0.58f;
            }
            else if (m_isSwing) {
                modeForceScale = 0.4f;
            }
            else if (m_isBall || m_isSpider) {
                modeForceScale = 0.6f;
            }
            else if (m_isRobot) {
                modeForceScale = 0.9f;
            }

            if (m_vehicleSize != 1.f) {
                if (m_isShip) {
                    modeForceScale = 0.5875f;
                }
                else if (m_isBird) {
                    modeForceScale = 0.725f;
                }
                else if (m_isSwing) {
                    modeForceScale = 0.61538464f;
                }
            }

            auto stateForceYDelta = static_cast<double>(dt * m_stateForceVector.y * modeForceScale);
            addToYVelocity_(stateForceYDelta, 0);

            if (stateForceYDelta != 0) {
                m_isAccelerating = true;
            }

            bool shouldAffectAcceleration = (!m_isUpsideDown && stateForceYDelta > 0) || (m_isUpsideDown && stateForceYDelta < 0);

            if (shouldAffectAcceleration) {
                m_accelerationOrSpeed = (stateForceYDelta / 12.94) * 1.5 + m_accelerationOrSpeed;
            }

            if (m_isPlatformer) {
                auto stateForceX = m_stateForceVector.x;
                m_platformerXVelocity = static_cast<double>(modeForceScale * dtModified) + m_platformerXVelocity;

                if (modeForceScale != 0) {
                    m_affectedByForces = true;
                }

                bool canAutoRotate = isInNormalMode_() && !m_isRotating && std::abs(modeForceScale) > 0.1;

                if (canAutoRotate) {
                    runNormalRotation(true, 1);
                }
            }
        }

        bool isDashing = m_isDashing;
        if (isDashing) {
            setYVelocity(0, 0);
        }

        double scaledDt = dtModified;
        double yOffset = scaledDt * m_yVelocity;

        if (m_isPlatformer) {
            updateMove(dtModified);
            isDashing = m_isDashing;
        }

        double xOffset = getCurrentXVelocity_();
        xOffset = xOffset * dt;

        double reverseDecayStep = xOffset * 0.02;

        if (isDashing) {
            double dashY = m_dashY;

            if (!m_isPlatformer) {
                yOffset = dashY * xOffset;
            }
            else {
                double dashX = m_dashX;
                xOffset = scaledDt * dashX;
                yOffset = dashY * scaledDt;
                m_platformerXVelocity = dashX;
                m_yVelocity = dashY;
            }
        }
        else if (m_isDart) {
            int bufferSign = m_jumpBuffered ? 1 : -1;

            yOffset = std::abs(xOffset) * flipMod_() * bufferSign;
            if (m_vehicleSize != 1) {
                yOffset = yOffset + yOffset;
            }
        }

        m_yVelocityRelated3 = yOffset;

        if (m_isGoingLeft && !m_isPlatformer) {
            xOffset = xOffset * -1;
        }

        xOffset = xOffset + m_maybeReverseAcceleration;

        double finalYOffset = yOffset;
        if (m_isSideways) {
            finalYOffset = xOffset;
            xOffset = yOffset;
        }

        auto frameOffset = CCPoint{static_cast<float>(xOffset), static_cast<float>(finalYOffset)};
        auto position = getPosition();
        auto offsetPosition = position + frameOffset;
        setPosition(offsetPosition);

        m_maybeReverseAcceleration = 0;
        double reverseSpeed = m_maybeReverseSpeed;

        if (reverseSpeed != 0) {
            double reverseStep = std::abs(reverseSpeed);

            if (reverseDecayStep <= std::abs(reverseSpeed)) {
                reverseStep = reverseDecayStep;
            }
            if (reverseSpeed <= 0) {
                reverseStep = -reverseStep;
            }

            m_maybeReverseAcceleration = reverseStep;
            m_maybeReverseSpeed = reverseSpeed - reverseStep;
        }
    }

    if (m_isShip) {
        if (!m_jumpBuffered || levelFlipping() || m_isHidden) {
            if (m_hasShipParticles) {
                m_shipClickParticles->stopSystem();
                m_hasShipParticles = false;
            }
        }
        else {
            if (!m_hasShipParticles) {
                m_shipClickParticles->resumeSystem();
                m_hasShipParticles = true;
            }
        }
    }
    else if (!m_isBird && !m_isSwing && !m_isDart) {
        if ((!m_isOnGround2 || levelFlipping() || m_isLocked || m_isHidden) || (m_isPlatformer && std::abs(m_platformerXVelocity) <= 2.5)) {
            if (m_hasGroundParticles) {
                auto action = getActionByTag(3);
                if (!action) {
                    auto call = CCCallFunc::create(this, callfunc_selector(REPlayerObject::deactivateParticle_));
                    auto delay = CCDelayTime::create(0.06);
                    action = CCSequence::create(delay, call, nullptr);
                    runAction(action);
                }
            }
        }
        else {
            if (!m_hasGroundParticles) {
                m_playerGroundParticles->resumeSystem();
            }
            m_hasGroundParticles = true;
            stopActionByTag(3);
        }
    }

    bool active = !m_isLocked && !m_isHidden;
    bool moving = !m_isPlatformer || m_holdingLeft || m_holdingRight || m_platformerMovingLeft || m_platformerMovingRight;

    if (!m_isDart && (isFlying_()) && m_isOnGround2 && m_yVelocity * flipMod_() > -1 && active && moving) {
        m_vehicleGroundParticles->resumeSystem();
    }
    else {
        m_vehicleGroundParticles->stopSystem();
    }
    
    m_waveTrail->m_pulseSize = (m_audioScale - 0.1) * 2.1 + 0.4;

    if (m_playEffects) {
        if (!m_isGoingLeft) {
            auto gameManager = GameManager::get();
            m_waveTrail->clearBehindXPos(gameManager->m_playLayer->m_gameState.m_cameraPosition2.x);
        }
        else {
            auto winSize = CCDirector::get()->getWinSize();
            auto gameManager = GameManager::get();

            float cameraX = gameManager->m_playLayer->m_gameState.m_cameraPosition2.x;
            float cameraZoom = gameManager->m_playLayer->m_gameState.m_cameraZoom;
            float thresholdX = cameraX + (winSize.width / cameraZoom);

            m_waveTrail->clearAboveXPos(thresholdX);
        }
    }

    if (m_robotFire) {
        bool shouldDisable = !m_isRobot || !m_jumpBuffered || m_touchedPad || !m_maybeIsBoosted || m_accelerationOrSpeed <= 0.27f || m_accelerationOrSpeed >= 1.5f;
        bool isVisible = m_robotFire->isVisible();

        if (shouldDisable) {
            if (isVisible && !m_robotFire->getActionByTag(9)) {
                m_robotFire->stopAllActions();

                auto scaleDown = CCScaleTo::create(0.15f, 0.05f, 0.1f);
                auto hide = CCHide::create();

                auto seq = CCSequence::create(scaleDown, hide, nullptr);
                seq->setTag(9);

                m_robotFire->runAction(seq);
                m_robotBurstParticles->stopSystem();
            }
        }
        else {
            if (!isVisible) {
                m_robotFire->stopAllActions();
                m_robotFire->update(0.1f);
                m_robotFire->setVisible(true);

                auto a1 = CCScaleTo::create(0.05f, 1.44f, 0.9f);
                auto a2 = CCScaleTo::create(0.05f, 0.72f, 1.35f);
                auto a3 = CCScaleTo::create(0.05f, 1.8f, 0.9f);
                auto a4 = CCScaleTo::create(0.05f, 0.9f, 1.35f);

                auto seq = CCSequence::create(a1, a2, a3, a4, nullptr);

                m_robotFire->runAction(seq);
                m_robotBurstParticles->resumeSystem();
            }
        }
    }

    bool shouldDisableBurst = !m_isRobot || !m_jumpBuffered || m_touchedPad || m_isHidden || (!m_maybeIsBoosted && m_accelerationOrSpeed <= 0.1f) || (m_accelerationOrSpeed >= 1.5f);
    bool isRunning = m_robotBurstParticles->isActive();

    if (shouldDisableBurst) {
        if (isRunning) {
            m_robotBurstParticles->stopSystem();
        }
    }
    else {
        if (!isRunning) {
            m_robotBurstParticles->resumeSystem();
        }
    }

    m_maybeSpriteRelated = false;

    if (m_ghostTrail) {
        CCPoint ghostPos;

        if (m_isRobot) {
            auto head = m_robotSprite->m_headSprite;
            auto headPos = head->getPosition();
            float yOffset = headPos.y * 0.4f + 7.f;

            ghostPos = CCPoint{0.f, yOffset};
        }
        else if (m_isSpider) {
            auto head = m_spiderSprite->m_headSprite;
            auto headPos = head->getPosition();
            float yOffset = headPos.y * 0.4f + 2.f;

            ghostPos = CCPoint{0.f, yOffset};
        }
        else {
            ghostPos = CCPoint{0.f, 0.f};
        }

        m_ghostTrail->m_position = ghostPos;
    }

    if (m_isDashing) {
        updateDashAnimation();
    }

    if (m_isSwing) {
        float rotation = getRotation();

        float angle1 = (45.f - rotation) - 180.f;
        m_swingBurstParticles1->setAngle(angle1 + (m_isGoingLeft ? 90 : 0));

        float angle2 = (-rotation - 45.f) - 180.f;
        m_swingBurstParticles2->setAngle(angle2 + (m_isGoingLeft ? -90 : 0));
    }

    updateJumpVariables_();
    updateStateVariables_();

    if (m_shipStreak) {
        auto textureName = getFrameForStreak__(m_shipStreakType, m_totalTime);
        auto texture = CCTextureCache::get()->addImage(textureName.c_str(), false);

        m_shipStreak->setTexture(texture);
    }

    if (m_isPlatformer && m_isDashing && m_dashRing && m_dashRing->m_maxDuration > 0 && m_dashRing->m_maxDuration < m_totalTime - m_dashStartTime) {
        stopDashing();
        m_jumpBuffered = false;
    }
}

std::string getFrameForStreak__(ShipStreak type, float time) { // FIX BINDINGS 0x386060 windows 2.2084 add to bindings, static method
    float frameRate;
    int frameCount;

    switch (type) {
        case ShipStreak::ShipFire2: 
            frameRate = 0.03125f; 
            frameCount = 9;  
            break;
        case ShipStreak::ShipFire3: 
            frameRate = 0.03125f; 
            frameCount = 10; 
            break;
        case ShipStreak::ShipFire4: 
            frameRate = 0.0416667f; 
            frameCount = 6;  
            break;
        case ShipStreak::ShipFire5: 
            frameRate = 0.05f;      
            frameCount = 16; 
            break;
        case ShipStreak::ShipFire6: 
            frameRate = 0.0416667f; 
            frameCount = 5;  
            break;
        default: 
            frameRate = 0.03125f;  
            frameCount = 1;  
            break;
    }

    int frameIndex = (static_cast<int>(std::floor(time / frameRate)) % frameCount) + 1;

    std::string result = "shipfire";

    if (static_cast<int>(type) < 10) result += "0";

    result += std::to_string(static_cast<int>(type));
    result += "_";

    if (frameIndex < 10) {
        result += "00";
    }
    else if (frameIndex < 100) {
        result += "0";
    }

    result += std::to_string(frameIndex);
    result += ".png";

    return result;
}

void REPlayerObject::setScaleX(float scale) {
    m_isOrientedBoxDirty = true;

    CCSprite::setScaleX(scale);

    if (m_hasFollower && m_propagateScaleChanges) {
        if (m_followers->count() != 0) {
            for (auto follower : CCArrayExt<CCSprite, false>(m_followers)) {
                follower->setScaleX(scale);
            }
        }
    }

    if (m_glowSprite) {
        m_glowSprite->setScaleX(scale);
    }

    if (m_colorSprite) {
        m_colorSprite->setScaleX(scale);
    }

    if (m_particle && !m_isParticleSpriteLocked) {
        m_particle->setScaleX(scale);
    }
}

void REPlayerObject::setScaleY(float scale) {
    m_isOrientedBoxDirty = true;

    CCSprite::setScaleY(scale);

    if (m_hasFollower && m_propagateScaleChanges) {
        if (m_followers->count() != 0) {
            for (auto follower : CCArrayExt<CCSprite, false>(m_followers)) {
                follower->setScaleY(scale);
            }
        }
    }

    if (m_glowSprite) {
        m_glowSprite->setScaleY(scale);
    }

    if (m_colorSprite) {
        m_colorSprite->setScaleY(scale);
    }

    if (m_particle && !m_isParticleSpriteLocked) {
        m_particle->setScaleY(scale);
    }
}

void REPlayerObject::setScale(float scale) {
    m_isOrientedBoxDirty = true;

    CCSprite::setScale(scale);

    if (m_hasFollower && m_propagateScaleChanges) {
        if (m_followers->count() != 0) {
            for (auto follower : CCArrayExt<CCSprite, false>(m_followers)) {
                follower->setScale(scale);
            }
        }
    }

    if (m_glowSprite) {
        m_glowSprite->setScale(scale);
    }

    if (m_colorSprite) {
        m_colorSprite->setScale(scale);
    }

    if (m_particle && !m_isParticleSpriteLocked) {
        m_particle->setScale(scale);
    }
}

void REPlayerObject::setPosition(cocos2d::CCPoint const& position) {
    m_isObjectRectDirty = true;
    m_isOrientedBoxDirty = true;

    GameObject::setPosition(position);

    auto vehiclePos = m_vehicleSprite->getPosition();

    if (m_isSwing) {
        vehiclePos = CCPoint{m_vehicleSize * -10.f, 0.f};
    }
    else {
        CCPoint offset;

        if (m_isPlatformer && m_isShip) {
            offset = CCPoint{m_vehicleSize * -10.f, -18.f};
        }
        else if (m_isBird) {
            offset = CCPoint{0.f, -3.f};
        }
        else {
            offset = CCPoint{m_vehicleSize * -12.f, -4.f};
        }

        vehiclePos += offset;
    }

    auto worldPos = m_mainLayer->convertToWorldSpace(vehiclePos);
    auto trailingParticlesParent = m_trailingParticles->getParent();

    if (trailingParticlesParent) {
        auto localPos = trailingParticlesParent->convertToNodeSpace(worldPos);
        m_trailingParticles->setPosition(localPos);
    }

    auto trailPos = m_trailingParticles->getPosition();

    m_shipClickParticles->setPosition(trailPos);
    m_ufoClickParticles->setPosition(trailPos);
    m_dashParticles->setPosition(trailPos);

    auto footSize = m_robotSprite->m_footSprite->getContentSize();
    auto worldPosF = m_robotSprite->m_footSprite->convertToWorldSpace({footSize.width * 0.5f, 2.5f});
    auto localPos = m_robotFire->getParent()->convertToNodeSpace(worldPosF);

    m_robotFire->setPosition(localPos);
    m_robotFire->setScaleX(m_robotSprite->m_paSprite->getScaleX());
    m_robotFire->setRotation(m_robotSprite->m_footSprite->getRotation());

    auto burstParent = m_robotBurstParticles->getParent();

    if (m_robotBurstParticles && burstParent) {
        auto worldPos = m_robotFire->getParent()->convertToWorldSpace(m_robotFire->getPosition());
        auto localPos = burstParent->convertToNodeSpace(worldPos);

        m_robotBurstParticles->setPosition(localPos);
    }

    auto swingBurstParent1 = m_swingBurstParticles1->getParent();

    if (m_swingBurstParticles1 && swingBurstParent1) {
        auto worldPos = m_swingFireBottom->getParent()->convertToWorldSpace(m_swingFireBottom->getPosition());
        auto localPos = swingBurstParent1->convertToNodeSpace(worldPos);

        m_swingBurstParticles1->setPosition(localPos);
    }

    auto swingBurstParent2 = m_swingBurstParticles2->getParent();

    if (m_swingBurstParticles2 && swingBurstParent2) {
        auto worldPos = m_swingFireTop->getParent()->convertToWorldSpace(m_swingFireTop->getPosition());
        auto localPos = swingBurstParent2->convertToNodeSpace(worldPos);

        m_swingBurstParticles2->setPosition(localPos);
    }

    int direction = -1;

    if (!m_isUpsideDown || m_isSwing) {
        direction = 1;
    }

    int verticalDir = m_isOnGround3 ? direction : -direction;

    auto particleOffset = CCPoint{0.f, 0.f};

    if (m_isRobot) {
        particleOffset = CCPoint{0.f, -2.f};
    }

    float offsetY = (particleOffset.y - 13.f) * verticalDir * m_vehicleSize;
    float offsetX = reverseMod_() * (particleOffset.x - 10.f) * m_vehicleSize;

    auto finalParticleOffset = CCPoint{offsetX, offsetY};

    if (!m_isOnSlope) {
        if (m_isOnGround2) {
            if (m_isSideways) std::swap(finalParticleOffset.x, finalParticleOffset.y);
            
            m_playerGroundParticles->setPosition(position + finalParticleOffset);
        }
    }
    else {
        float height = m_vehicleSize * m_height;
        float width = m_vehicleSize * 20.f;

        int rotatedSlidingSign = m_slopeSlidingMaybeRotated ? 1 : -1;

        float length = std::sqrt(width * width + height * height);

        float angle = m_slopeAngleRadians;
        if (rotatedSlidingSign == -1) {
            angle = -angle;
        }

        float baseAngle = std::atan(height / width);

        bool flip = (m_isGoingLeft != m_slopeSlidingMaybeRotated);

        if (m_isUpsideDown) {
            flip = (m_isGoingLeft == m_slopeSlidingMaybeRotated);
        }

        if (flip) {
            angle += rotatedSlidingSign * baseAngle;
        }
        else {
            angle -= rotatedSlidingSign * baseAngle;
        }

        int horizontalDir = reverseMod_();

        float x = std::cos(angle) * length * 0.5f * horizontalDir;
        float y = std::sin(angle) * length * 0.5f * horizontalDir;

        auto slopeOffset = CCPoint{x, y};
        if (m_isSideways) std::swap(slopeOffset.x, slopeOffset.y);

        m_playerGroundParticles->setPosition(position - slopeOffset);
    }

    if (m_unkA29) {
        auto offset = CCPoint{reverseMod_() * m_vehicleSize, flipMod_() * m_vehicleSize * -15.f};
        if (m_isSideways) std::swap(offset.x, offset.y);
        
        m_vehicleGroundParticles->setPosition(position + offset);
        m_vehicleGroundParticles->update(0);
        m_vehicleGroundParticles->setGravity({-350.f, flipMod_() * -300.f});
        m_vehicleGroundParticles->setRotation(m_isSideways ? 90.f : 0.f);
    }

    if (m_isShip) {
        if (m_isPlatformer) {
            auto world = m_mainLayer->convertToWorldSpace({m_vehicleSize * -10.f, m_vehicleSize * -12.f});
            auto local = m_regularTrail->getParent()->convertToNodeSpace(world);

            m_regularTrail->setPosition(local);
        }
        else {
            auto trailOffset = CCPoint{reverseMod_() * 4.f, 0.f};
            if (m_isSideways) std::swap(trailOffset.x, trailOffset.y);
            
            m_regularTrail->setPosition(m_trailingParticles->getPosition() + trailOffset);

            if (m_shipStreak) {
                auto streakOffset = offsetForStreak__(m_shipStreakType);
                streakOffset.x *= reverseMod_();

                auto world = m_mainLayer->convertToWorldSpace(m_vehicleSprite->getPosition() + streakOffset);
                auto local = m_regularTrail->getParent()->convertToNodeSpace(world);

                m_shipStreak->setPosition(local);
            }
        }
    }
    else if (m_isSwing) {
        m_regularTrail->setPosition(m_trailingParticles->getPosition());
    }
    else if (m_isBird) {
        if (m_regularTrail->getParent()) {
            auto world = m_mainLayer->convertToWorldSpace(m_vehicleSprite->getPosition());
            auto local = m_regularTrail->getParent()->convertToNodeSpace(world);

            m_regularTrail->setPosition(local);
        }
    }
    else {
        auto trailOffset = CCPoint{reverseMod_() * m_vehicleSize * -2.f, 0.f};
        if (m_isSideways) std::swap(trailOffset.x, trailOffset.y);

        m_regularTrail->setPosition(getPosition() + trailOffset);
    }

    if (m_fadeOutStreak && m_isDart) {
        m_waveTrail->m_currentPoint = getPosition();
    }

    m_waveTrail->setPosition(getPosition());
}

CCPoint offsetForStreak__(ShipStreak type) {
    switch (type) {
        case ShipStreak::ShipFire2: return {-8.f, -3.f};
        case ShipStreak::ShipFire3: return {-8.f, -3.5f};
        case ShipStreak::ShipFire5: return {-14.f, -3.f};
        case ShipStreak::ShipFire6: return {-14.f, -2.5f};
        default: return {0, 0};
    }
}

void REPlayerObject::setVisible(bool visible) {
    bool isVisible = false;
    if (!m_isHidden) {
        isVisible = visible;
    }

    GameObject::setVisible(isVisible);
}

void REPlayerObject::setRotation(float rotation) {
    if (m_fRotationX != rotation || m_fRotationY != rotation) {
        m_isObjectRectDirty = true;
        m_isOrientedBoxDirty = true;
        m_isDirty = true;

        CCSpritePlus::setRotation(rotation);

        if (m_glowSprite) {
            m_glowSprite->setRotation(rotation);
        }
        if (m_colorSprite) {
            m_colorSprite->setRotation(rotation);
        }
        if (m_particle && !m_particleLocked) {
            m_particle->setRotation(rotation);
        }

        float rotationAbs = std::abs(rotation);

        m_isRotationAligned = rotationAbs == 90 || rotationAbs == 270;
    }
}

void REPlayerObject::setOpacity(unsigned char opacity) {
    GameObject::setOpacity(opacity);

    m_iconSprite->setOpacity(opacity);
    m_iconSpriteSecondary->setOpacity(opacity);
    m_iconSpriteWhitener->setOpacity(opacity);
    m_iconGlow->setOpacity(opacity);
    m_vehicleSprite->setOpacity(opacity);
    m_vehicleSpriteSecondary->setOpacity(opacity);
    m_birdVehicle->setOpacity(opacity);
    m_vehicleSpriteWhitener->setOpacity(opacity);
    m_vehicleGlow->setOpacity(opacity);
    m_robotSprite->setOpacity(opacity);
    m_spiderSprite->setOpacity(opacity);
}

void REPlayerObject::setColor(cocos2d::ccColor3B const& color) {
    CCSprite::setColor(color);

    m_iconSprite->setColor(color);
    m_vehicleSprite->setColor(color);

    m_robotSprite->updateColor01(color);
    m_spiderSprite->updateColor01(color);
}

void REPlayerObject::setFlipX(bool flipX) {
    if (m_isFlipX != flipX) {
        m_isFlipX = flipX;
        setScaleX(-getScaleX());
    }
    m_iconSprite->setFlipX(flipX);
    m_iconSpriteSecondary->setFlipX(flipX);
    m_vehicleSprite->setFlipX(flipX);
    m_vehicleSpriteSecondary->setFlipX(flipX);
    m_birdVehicle->setFlipX(flipX);
    m_vehicleGlow->setFlipX(flipX);
}

void REPlayerObject::setFlipY(bool flipY) {
    if (m_isFlipY != flipY) {
        m_isFlipY = flipY;
        setScaleY(-getScaleY());
    }
}

void REPlayerObject::resetObject() {
    m_maybeReducedEffects = true;

    m_actionManager->stopAllInternalActions();

    m_physDeltaRelated = 0.f;
    m_wasTeleported = false;
    m_isCollidingWithSlope = false;
    m_collidedObject = nullptr;
    m_lastGroundObject = nullptr;
    m_currentSlope2 = nullptr;
    m_preLastGroundObject = nullptr;
    m_unk3e0 = false;
    m_collidingWithLeft = nullptr;
    m_collidingWithRight = nullptr;
    m_scaleXRelated2 = 0.0;
    m_groundYVelocity = 0.f;
    m_scaleXRelated3 = 0.0;
    m_scaleXRelated4 = 0.0;
    m_yVelocityRelated = 0.f;
    m_scaleXRelated5 = 0.f;
    m_ringJumpRelated = false;
    m_maybeReverseSpeed = 0.f;
    m_maybeReverseAcceleration = 0.f;

    doReversePlayer(false);
    rotateGameplay(2, 4, false, 0.f, 0.f, false, false);

    m_platformerXVelocity = 0.f;
    m_maybeHasStopped = false;
    m_maybeGoingCorrectSlopeDirection = false;
    m_affectedByForces = false;
    m_isOnIce = false;
    m_scaleXRelated = 0.0;
    m_platformerMovingLeft = false;
    m_platformerMovingRight = false;
    m_maybeUpsideDownSlope = false;
    m_maybeSlidingTime = 0;
    m_maybeSlidingStartTime = 0.0;
    m_changedDirectionsTime = 0.0;
    m_controlsDisabled = false;
    m_gravityMod = 1.0;
    m_maybeIsColliding = false;
    m_isOnGround = false;
    m_isOnGround2 = true;
    m_lastLandTime = 0.0;
    m_scaleXRelatedTime = 0.0;
    m_fallSpeed = 0.0;
    m_dashRing = nullptr;
    m_dashStartTime = 0.0;
    m_platformerVelocityRelated = 0.0;
    m_decreaseBoostSlide = false;

    toggleVisibility(true);

    m_flashTime = -1.f;

    setColor(m_originalMainColor);
    setSecondColor(m_originalSecondColor);

    stopParticles();
    resetStateVariables_();
    resetTouchedRings(true);

    std::fill(m_playerFollowFloats.begin(), m_playerFollowFloats.end(), 0.f);

    m_followRelated = 0;
    m_unk838 = 0.0;

    stopDashing();
    deactivateStreak_(false);
    removePendingCheckpoint();

    if (m_spiderSprite) {
        m_spiderSprite->stopActionByTag(10);
        m_spiderSprite->setScale(1.f);
    }

    m_checkpointTimeout = false;
    m_canPlaceCheckpoint = false;

    m_lastGroundedPos = CCPoint{0, 0};
    m_lastActivatedPortal = nullptr;
    m_lastPortalPos = CCPoint{0, 0};

    m_isLocked = false;
    m_objectSnappedTo = nullptr;
    m_shouldTryPlacingCheckpoint = false;
    m_isOnSlope = false;
    m_wasOnSlope = false;
    m_slopeVelocity = 0.0;
    m_collidingWithSlopeId = -1;
    m_currentSlopeYVelocity = 0.0;
    m_isAccelerating = false;
    m_slopeRotation = 0.0;
    m_currentSlope = nullptr;
    m_currentPotentialSlope = nullptr;
    m_potentialSlopeMap.clear();
    m_unk669 = false;
    resetCollisionValues_();
    m_totalTime = 0.0;
    m_maybeIsBoosted = false;

    setYVelocity(0, 0);
    flipGravity(false, false);
    switchedToMode(GameObjectType::CubePortal);

    m_vehicleSize = 0.6f;
    togglePlayerScale(false, false);
    stopRotation_(false, 0);
    setRotation(0.f);

    m_isDead = false;

    stopActionByTag(4);
    setOpacity(255);

    toggleGhostEffect(GhostType::Disabled);
    updateTimeMod(0.9f, false);
    resetStreak();
    levelFlipFinished();

    m_maybeReducedEffects = false;

    if (m_alwaysShowStreak) {
        activateStreak();
    }
}

cocos2d::CCPoint REPlayerObject::getRealPosition() {
    return getPosition();
}

OBB2D* REPlayerObject::getOrientedBox() {
    updateOrientedBox();
    return m_orientedBox;
}

void REPlayerObject::animationFinished(char const* key) {
    if (!key) return;

    if (strcmp(key, "jumpstart") == 0) {
        if (m_robotSprite) {
            m_robotSprite->runAnimation("jump_loop");
        }
        return;
    }

    if (strcmp(key, "fall") == 0) {
        if (m_robotSprite) {
            m_robotSprite->runAnimation("fall_loop");
        }
        return;
    }

    if (strcmp(key, "idle") == 0) {
        if (m_isRobot) {
            if (m_robotSprite) {
                m_robotSprite->runAnimation("idle01");
            }
        }
        else if (m_isSpider) {
            if (m_spiderSprite) {
                m_spiderSprite->runAnimation("idle01");
            }
        }
        return;
    }
}

void REPlayerObject::activateStreak() {
    if (levelFlipping()) return;
    
    auto gameManager = GameManager::get();

    if (gameManager->m_editorEnabled) return;
    if (m_isHidden) return;

    m_fadeOutStreak = true;

    m_regularTrail->resumeStroke();

    if (m_isDart) {
        m_waveTrail->m_currentPoint = getPosition();
        m_waveTrail->stopAllActions();

        m_waveTrail->setOpacity(255);
        m_waveTrail->m_drawStreak = true;

        updateEffects_(0.f);
    }
}

void REPlayerObject::addAllParticles() {
    m_parentLayer->addChild(m_playerGroundParticles, 39);
    m_parentLayer->addChild(m_ufoClickParticles, 39);
    m_parentLayer->addChild(m_dashParticles, 39);
    m_parentLayer->addChild(m_robotBurstParticles, 39);
    m_parentLayer->addChild(m_trailingParticles, 39);
    m_parentLayer->addChild(m_shipClickParticles, 39);

    m_parentLayer->addChild(m_vehicleGroundParticles, 61);
    m_parentLayer->addChild(m_landParticles0, 61);
    m_parentLayer->addChild(m_landParticles1, 61);

    m_parentLayer->addChild(m_swingBurstParticles1, 39);
    m_parentLayer->addChild(m_swingBurstParticles2, 39);
}

void REPlayerObject::addToTouchedRings_(RingObject* object) {
    if (!m_touchingRings->containsObject(object)) {
        m_touchingRings->addObject(object);
    }
    m_touchedRings.insert(object->m_uniqueID);
}

void REPlayerObject::addToYVelocity_(double yVelocity, int type) {
    setYVelocity(m_yVelocity + yVelocity, type);
}

void REPlayerObject::animatePlatformerJump(float scale) {
    bool noInput = !m_holdingLeft && !m_holdingRight && !m_platformerMovingLeft && !m_platformerMovingRight;

    if (!noInput || !isInNormalMode_()) return;

    int rotation = static_cast<int>(getRotation());
    if (rotation < 0) rotation += 360;

    float x1 = 0.8f;
    float y1 = scale * 1.35f;
    float x2 = 1.1f;
    float y2 = 0.9f;

    if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    {
        auto a1 = CCEaseInOut::create(CCScaleTo::create(0.1f / m_gravityMod, x1, y1), 2.f);
        auto a2 = CCEaseInOut::create(CCScaleTo::create(0.25f / m_gravityMod, x2, y2), 2.f);
        auto a3 = CCEaseInOut::create(CCScaleTo::create(0.15f / m_gravityMod, 1.f, 1.f), 2.f);

        auto seq = CCSequence::create(a1, a2, a3, nullptr);

        seq->setTag(13);

        m_iconSprite->runAction(seq);
    }
    {
        auto a1 = CCEaseInOut::create(CCScaleTo::create(0.1f / m_gravityMod, x1, y1), 2.f);
        auto a2 = CCEaseInOut::create(CCScaleTo::create(0.25f / m_gravityMod, x2, y2), 2.f);
        auto a3 = CCEaseInOut::create(CCScaleTo::create(0.15f / m_gravityMod, 1.f, 1.f), 2.f);

        auto seq = CCSequence::create(a1, a2, a3, nullptr);

        seq->setTag(13);

        m_iconGlow->runAction(seq);
    }
}

void REPlayerObject::boostPlayer(float yVelocity) {
    m_maybeIsBoosted = true;
    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = true;
    m_isAccelerating = true;

    auto yVel = static_cast<double>(yVelocity);
    auto yVelInt = static_cast<double>(static_cast<int>(yVelocity));

    if (yVel != yVelInt) {
        yVel = static_cast<double>(std::round(yVel - yVelInt) * 1000.0);
        yVel = yVel / 1000.0 + yVelInt;
    }

    setYVelocity(yVelocity, 0);

    if (!m_isDashing) {
        if (isInNormalMode_()) {
            m_isBallRotating2 = false;
            m_isBallRotating = false;

            float rotationDivisor = m_vehicleSize == 1.f ? 0.8666667f : 0.6666667f;
            int rotationDegrees = flipMod_() * -180;

            m_rotationSpeed = rotationDegrees / rotationDivisor;
            m_isRotating = true;
        }
        else if (m_isSpider) {
            m_spiderSprite->tweenToAnimation("fall_loop", 0.1f);
        }
    }

    m_lastGroundedPos = getPosition();
}

void REPlayerObject::bumpPlayer(float bumpMod, int objectType, bool noEffects, GameObject* object) {
    if (m_isPlatformer || !m_fixRobotJump) {
        m_touchedPad = true;
    }

    if (objectType != 44) {
        propellPlayer(bumpMod, noEffects, objectType);

        if (m_isPlatformer && !m_isRotating) {
            float scale = objectType == 34 ? 1.2f : 1.f;
            animatePlatformerJump(scale);
        }

        m_isAccelerating = false;

        if (objectType == 34) {
            m_isAccelerating = true;
            m_lastGroundedPos = CCPoint{0.f, 0.f};
        }
        return;
    }

    bool didFlipGravity = false;

    if (object) {
        if (!m_isSideways) {
            if (object->isFacingDown() != m_isUpsideDown) {
                flipGravity(!m_isUpsideDown, true);
                didFlipGravity = true;
            }
        }
        else {
            if (object->isFacingLeft() != m_isUpsideDown) {
                flipGravity(!m_isUpsideDown, true);
                didFlipGravity = true;
            }
        }
    }

    spiderTestJump(false);
    playBumpEffect(44, nullptr);
}

bool REPlayerObject::buttonDown_(PlayerButton button) {
    return button == PlayerButton::Jump || m_jumpBuffered;
}

bool REPlayerObject::canStickToGround_() {
    return (!m_isShip && !m_isDart) || !m_stateRingJump;
}

void REPlayerObject::checkSnapJumpToObject(GameObject* object) {
    if (!object) return;
    
    auto snappedObject = m_objectSnappedTo;

    if (snappedObject && snappedObject->m_uniqueID != object->m_uniqueID && snappedObject->getType() == GameObjectType::Solid) {
        auto snappedPosition = snappedObject->getRealPosition();
        auto objectPosition = object->getRealPosition();

        float nearX = 120.f;
        float midX = 90.f;
        float farX = 150.f;
        float tolerance = 1.f;

        if (m_playerSpeed == 0.7f) {
            nearX = 90.f;
            midX = 60.f;
            farX = 120.f;
        }
        else if (m_playerSpeed == 1.1f) {
            nearX = 150.f;
            midX = 120.f;
            farX = 195.f;
            tolerance = 2.f;
        }
        else if (m_playerSpeed == 1.3f) {
            nearX = 180.f;
            midX = 135.f;
            farX = 225.f;
            tolerance = 2.f;
        }
        else if (m_playerSpeed == 0.9f) {
            nearX = 120.f;
            midX = 90.f;
            farX = 150.f;
        }

        if (m_vehicleSize != 1.f) {
            nearX = 90.f;
            midX = 90.f;
        }

        int gravitySign = flipMod_();

        bool matchesNear = std::abs(objectPosition.x - (snappedPosition.x + nearX)) <= tolerance && std::abs(objectPosition.y - (snappedPosition.y + gravitySign * 30.f)) <= tolerance;
        bool matchesFar = std::abs(objectPosition.x - (snappedPosition.x + farX)) <= tolerance && std::abs(objectPosition.y - (snappedPosition.y - gravitySign * 30.f)) <= tolerance;
        bool matchesMid = std::abs(objectPosition.x - (snappedPosition.x + midX)) <= tolerance && std::abs(objectPosition.y - (snappedPosition.y + gravitySign * 60.f)) <= tolerance;

        if (matchesNear || matchesFar || matchesMid) {
            auto objectPos = object->getPosition();
            double targetX = objectPos.x + m_snapDistance;

            auto playerPos = getPosition();
            double playerX = playerPos.x;

            if (std::abs(targetX - playerX) > tolerance) {
                if (targetX <= playerX) {
                    targetX = playerX - tolerance;
                }
                else {
                    targetX = playerX + tolerance;
                }
            }

            setPosition(CCPoint{static_cast<float>(targetX), playerPos.y});
        }
    }

    m_objectSnappedTo = object;
    m_snapDistance = getPosition().x - object->getPosition().x;
}

bool REPlayerObject::collidedWithObject_(float dt, GameObject* object) {
    return collidedWithObject(dt, object, {0.f, 0.f, 0.f, 0.f}, false);
}

bool REPlayerObject::collidedWithObject(float dt, GameObject* object, cocos2d::CCRect rect, bool skipCheck) {
    if (m_isSideways) {
        return handleRotatedCollisionInternal(dt, object, rect, skipCheck, false, false);
    }

    return collidedWithObjectInternal(dt, object, rect, skipCheck);
}

bool REPlayerObject::collidedWithObjectInternal(float dt, GameObject* object, cocos2d::CCRect rect, bool skipCheck) {
    //todo
    return PlayerObject::collidedWithObjectInternal(dt, object, rect, skipCheck);
}

void REPlayerObject::collidedWithSlope_(float dt, GameObject* object, bool skipPre) {
    if (m_isSideways) {
        handleRotatedSlopeCollision(dt, object, skipPre);
        return;
    }
    collidedWithSlopeInternal(dt, object, skipPre);
}

void REPlayerObject::collidedWithSlopeInternal(float dt, GameObject* object, bool forced) {
    //todo
    PlayerObject::collidedWithSlopeInternal(dt, object, forced);
}

float REPlayerObject::convertToClosestRotation(float rotation) {
    bool usesSnappedRotation = !isFlying_() && !m_isRobot && !m_isSpider && !m_isDashing;

    if (!usesSnappedRotation) return rotation;

    float currentRotation = static_cast<float>(static_cast<int>(getRotation()) % 360);
    float lower = rotation;
    float upper = rotation;

    if (rotation < currentRotation) {
        while (upper < currentRotation) {
            lower = upper;
            upper += 90.f;
        }
    }
    else if (rotation > currentRotation) {
        while (upper > currentRotation) {
            lower = upper;
            upper -= 90.f;
        }
    }

    if (std::abs(currentRotation - lower) <= std::abs(currentRotation - upper)) {
        return lower;
    }

    return upper;
}

void REPlayerObject::copyAttributes(PlayerObject* player) {
    m_maybeReducedEffects = true;
    m_totalTime = player->m_totalTime;

    m_position = player->m_position;
    setPosition(player->getPosition());

    m_shipRotation = player->m_shipRotation;

    flipGravity(player->m_isUpsideDown, true);
    doReversePlayer(player->m_isGoingLeft);

    toggleFlyMode(player->m_isShip, false);
    toggleBirdMode(player->m_isBird, false);
    toggleRollMode(player->m_isBall, false);
    toggleDartMode(player->m_isDart, false);
    toggleRobotMode(player->m_isRobot, false);
    toggleSpiderMode(player->m_isSpider, false);
    toggleSwingMode(player->m_isSwing, false);

    updateTimeMod(player->m_playerSpeed, false);
    togglePlayerScale(player->m_vehicleSize != 1.f, false);

    setYVelocity(player->getYVelocity(), 0);

    m_maybeReducedEffects = false;

    m_jumpBuffered = player->m_jumpBuffered;
    m_stateRingJump = player->m_stateRingJump;
}

void REPlayerObject::createFadeOutDartStreak() {
    auto fadeTrail = m_waveTrail->createDuplicate();

    m_waveTrail->getParent()->addChild(fadeTrail, m_waveTrail->getZOrder());
    fadeTrail->setPosition(m_waveTrail->getPosition());

    fadeTrail->resumeStroke();
    fadeTrail->scheduleAutoUpdate();

    auto fade = CCFadeTo::create(0.5f, 0);
    auto cleanup = CCCallFunc::create(fadeTrail, callfunc_selector(CCNode::removeFromParent));
    auto sequence = CCSequence::create(fade, cleanup, nullptr);

    fadeTrail->runAction(sequence);
}

void REPlayerObject::createRobot(int frame) {
    ccColor3B mainColor{};
    ccColor3B secondColor{};
    ccColor3B glowColor = m_playerColor2;

    bool glowVisible = false;
    std::string currentAnimation;

    auto oldRobot = m_robotSprite;
    if (oldRobot) {
        mainColor = oldRobot->m_color;
        secondColor = oldRobot->m_secondColor;
        glowVisible = oldRobot->m_glowSprite->isVisible();

        if (m_spiderSprite && m_spiderSprite->m_animationManager) {
            currentAnimation = m_spiderSprite->m_animationManager->m_currentAnimation;
        }

        m_robotBatchNode->removeFromParent();
        m_robotBatchNode->release();

        m_robotSprite = nullptr;
    }

    m_robotSprite = GJRobotSprite::create(frame);
    m_robotSprite->m_delegate = this;

    m_robotBatchNode = CCSpriteBatchNode::createWithTexture(m_robotSprite->getTexture(), 25);
    m_robotBatchNode->addChild(m_robotSprite);
    m_robotBatchNode->retain();

    if (oldRobot) {
        if (m_isRobot) {
            m_mainLayer->addChild(m_robotBatchNode, 2);
        }

        m_robotSprite->m_color = mainColor;
        m_robotSprite->m_secondColor = secondColor;
        m_robotSprite->updateColors();

        for (auto child : CCArrayExt<CCSprite, false>(m_robotSprite->m_glowSprite->getChildren())) {
            child->setColor(glowColor);
        }

        if (!currentAnimation.empty()) {
            m_robotSprite->runAnimation(currentAnimation);
        }

        if (glowVisible) {
            m_robotSprite->m_glowSprite->setVisible(true);
        }
    }
}

void REPlayerObject::createSpider(int frame) {
    ccColor3B mainColor{};
    ccColor3B secondColor{};
    ccColor3B glowColor = m_playerColor2;

    bool glowVisible = false;
    std::string currentAnimation;

    auto oldSpider = m_spiderSprite;
    if (oldSpider) {
        mainColor = oldSpider->m_color;
        secondColor = oldSpider->m_secondColor;
        glowVisible = oldSpider->m_glowSprite->isVisible();

        if (oldSpider->m_animationManager) {
            currentAnimation = oldSpider->m_animationManager->m_currentAnimation;
        }

        m_spiderBatchNode->removeFromParent();
        m_spiderBatchNode->release();

        m_spiderSprite = nullptr;
    }

    m_spiderSprite = GJSpiderSprite::create(frame);
    m_spiderSprite->m_delegate = this;

    m_spiderBatchNode = CCSpriteBatchNode::createWithTexture(m_spiderSprite->getTexture(), 25);
    m_spiderBatchNode->addChild(m_spiderSprite);
    m_spiderBatchNode->retain();

    if (oldSpider) {
        if (m_isSpider) {
            m_mainLayer->addChild(m_spiderBatchNode, 2);
        }

        m_spiderSprite->m_color = mainColor;
        m_spiderSprite->m_secondColor = secondColor;
        m_spiderSprite->updateColors();

        for (auto child : CCArrayExt<CCSprite, false>(m_spiderSprite->m_glowSprite->getChildren())) {
            child->setColor(glowColor);
        }

        if (!currentAnimation.empty()) {
            m_spiderSprite->runAnimation(currentAnimation);
        }

        if (glowVisible) {
            m_spiderSprite->m_glowSprite->setVisible(true);
        }
    }
}

void REPlayerObject::deactivateParticle_() { // FIX BINDINGS, is 0x38b8c0 on Windows 2.2084
    if (m_hasGroundParticles) {
        m_playerGroundParticles->stopSystem();
    }
    m_hasGroundParticles = false;
}

void REPlayerObject::deactivateStreak_(bool stop) {
    if (!m_alwaysShowStreak || stop) m_regularTrail->stopStroke();
    if (m_fadeOutStreak) {
        m_fadeOutStreak = false;
        fadeOutStreak2(m_playEffects ? 0.2f : 0.6f);
    }
}

bool REPlayerObject::destroyFromHitHead_() {
    return isInBasicMode_() && m_stateHitHead < 1;
}

void REPlayerObject::didHitHead() {
    if (m_stateFlipGravity <= 0) return;

    hardFlipGravity_();

    m_maybeIsBoosted = true;
    m_isOnGround2 = false;

    if (m_stateNoAutoJump > 0) {
        m_isOnGround = false;
        m_jumpBuffered = false;
        m_stateRingJump = false;
    }
}

void REPlayerObject::disableCustomGlowColor_() {
    m_hasCustomGlowColor = false;
}

void REPlayerObject::disablePlayerControls() {
    m_controlsDisabled = false;
    m_inputsLocked = true;

    releaseButton(PlayerButton::Jump);
    releaseButton(PlayerButton::Left);
    releaseButton(PlayerButton::Right);
    releaseButton(static_cast<PlayerButton>(5));

    m_inputsLocked = false;
    m_controlsDisabled = true;
}

void REPlayerObject::disableSwingFire() {
    m_swingFireMiddle->setVisible(false);
    m_swingFireMiddle->stopAllActions();

    m_swingFireBottom->setVisible(false);
    m_swingFireBottom->stopAllActions();

    m_swingFireTop->setVisible(false);
    m_swingFireTop->stopAllActions();

    m_swingBurstParticles1->stopSystem();
    m_swingBurstParticles2->stopSystem();
}

void REPlayerObject::doReversePlayer(bool reverse) {
    bool wasGoingLeft = m_isGoingLeft;
    bool directionChanged = wasGoingLeft != reverse;

    m_isGoingLeft = reverse;

    if (directionChanged) {
        m_reverseRelated = 2;
    }

    int sidewaysSign = m_isSideways ? -1 : 1;

    if (directionChanged && m_isDart) {
        createFadeOutDartStreak();
    }

    m_waveTrail->m_isFlipped = reverse;
    m_vehicleGroundParticles->setScaleX(sidewaysSign * reverseMod_());

    updatePlayerGlow();
    updatePlayerArt();

    if (m_fadeOutStreak && m_isDart && directionChanged) {
        m_waveTrail->clear();
        m_waveTrail->m_pointArray->removeAllObjects();
        placeStreakPoint();
    }

    if (m_isBall && !m_isLocked && !m_isDashing) {
        runRotateAction_(0, 1);
    }
}

void REPlayerObject::enableCustomGlowColor_(cocos2d::ccColor3B const& color) {
    m_hasCustomGlowColor = true;
    m_glowColor = color;
}

void REPlayerObject::enablePlayerControls() {
    m_controlsDisabled = false;

    bool holdingLeftButton = m_holdingButtons[static_cast<int>(PlayerButton::Left)];
    bool holdingRightButton = m_holdingButtons[static_cast<int>(PlayerButton::Right)];

    if (holdingLeftButton) {
        switchedDirTo(PlayerButton::Left);
    }
    else if (holdingRightButton) {
        switchedDirTo(PlayerButton::Right);
    }

    if (m_holdingButtons[static_cast<int>(PlayerButton::Jump)]) {
        pushButton(PlayerButton::Jump);
    }
}

void REPlayerObject::exitPlatformerAnimateJump() {
    if (!m_iconSprite->getActionByTag(13))  return;

    m_iconSprite->stopActionByTag(13);
    m_iconSprite->stopActionByTag(14);

    auto scale1 = CCScaleTo::create(0.1f, 1.f, 1.f);
    auto ease1 = CCEaseInOut::create(scale1, 2.f);
    ease1->setTag(14);

    m_iconSprite->runAction(ease1);

    m_iconGlow->stopActionByTag(13);
    m_iconGlow->stopActionByTag(14);

    auto scale2 = CCScaleTo::create(0.1f, 1.f, 1.f);
    auto ease2 = CCEaseInOut::create(scale2, 2.f);
    ease2->setTag(14);

    m_iconGlow->runAction(ease2);
}

void REPlayerObject::fadeOutStreak2(float duration) {
    if (m_waveTrail->getActionByTag(8)) return;

    auto fade = CCFadeOut::create(duration);
    auto stop = CCCallFunc::create(m_waveTrail, callfunc_selector(HardStreak::stopStroke));
    auto sequence = CCSequence::create(fade, stop, nullptr);

    sequence->setTag(8);
    m_waveTrail->runAction(sequence);
}

void REPlayerObject::flashPlayer_(float flashDuration, float flashDelay, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor) {
    m_flashMainColor = mainColor;
    m_flashSecondColor = secondColor;

    setColor(m_flashMainColor);
    setSecondColor(m_flashSecondColor);

    m_flashTime = m_totalTime;
    m_flashDuration = flashDuration;
    m_flashDelay = flashDelay;
}

void REPlayerObject::flipGravity(bool flip, bool noEffects) {
    if (m_isUpsideDown == flip) return;
    
    placeStreakPoint();
    m_isUpsideDown = flip;

    if (m_gameLayer) {
        auto event = flip ? GJGameEvent::GravityInverted : GJGameEvent::GravityRestored;
        gameEventTriggered_(event, 0);
    }

    m_lastFlipTime = m_totalTime;
    m_collidedBottomMaxY = 0.0;
    m_collidedTopMinY = 0.0;
    m_unkA29 = false;

    if (m_wasOnSlope || m_isOnSlope) {
        m_slopeFlipGravityRelated = !m_slopeFlipGravityRelated;
    }

    resetCollisionLog_(true);

    if (!m_maybeReducedEffects) {
        m_yVelocity *= 0.5;

        if (!noEffects) {
            auto color = flip ? ccColor3B{255, 200, 0} : ccColor3B{0, 150, 255};

            spawnPortalCircle(color, 45.0);
        }
    }

    updatePlayerScale();
    updatePlayerArt();

    if (!m_isBall) {
        auto gameManager = GameManager::get();

        if (gameManager->m_playLayer && !gameManager->m_playLayer->m_freezeStartCamera && !m_isDart && !m_maybeReducedEffects) { m_shouldTryPlacingCheckpoint = true;
            activateStreak();
        }
    }

    m_lastGroundedPos = m_lastPortalPos;
    m_isOnGround = false;

    if (m_isBall) {
        stopRotation_(false, 1);
        runBallRotation2();
        return;
    }

    if (m_isSwing) {
        updateSwingFire();
    }
}

int REPlayerObject::flipMod_() {
    return m_isUpsideDown ? -1 : 1;
}

void REPlayerObject::gameEventTriggered_(GJGameEvent gameEvent, int material) {
    if (m_gameLayer) {
        m_gameLayer->gameEventTriggered(gameEvent, material, static_cast<int>(m_uniqueID));
    }
}

GameObjectType REPlayerObject::getActiveMode_() {
    if (isFlying_()) {
        return GameObjectType::ShipPortal;
    }
    else if (m_isBall) {
        return GameObjectType::BallPortal;
    }
    else if (m_isSpider) {
        return GameObjectType::SpiderPortal;
    }
    else return GameObjectType::CubePortal;
}

double REPlayerObject::getCurrentXVelocity_() {
    return m_isPlatformer ? m_platformerXVelocity : m_playerSpeed * m_speedMultiplier;
}

float REPlayerObject::getModifiedSlopeYVel_() {
    auto diff = m_totalTime - m_slopeStartTime;
    if (diff > 0.1) {
        if (diff * 10.0 > 0.4) {
            return m_slopeVelocity * diff;
        }
        else {
            return m_slopeVelocity * 0.4f;
        }
    }
    else {
        return m_slopeVelocity * 1.f;
    }
}

float REPlayerObject::getOldPosition_(float dt) {
    if (dt <= 0.f) return m_obPosition.y;
    
    auto index = m_followRelated % 200 - std::clamp<int>(dt / 0.01f, 0, 199);
    return m_playerFollowFloats[index + (index < 0 ? 200 : 0)];
}

float REPlayerObject::getObjectRotation() {
    return getRotation();
}

cocos2d::ccColor3B REPlayerObject::getSecondColor_() {
    return m_iconSpriteSecondary->getColor();
}

double REPlayerObject::getYVelocity_() {
    return m_yVelocity;
}

void REPlayerObject::gravityDown_() {
    m_gravity -= 0.001;
    logValues();
}

void REPlayerObject::gravityUp_() {
    m_gravity += 0.001;
    logValues();
}

void REPlayerObject::handlePlayerCommand_(int command) {
    if (command == 543) {
        m_isAccelerating = false;
        m_affectedByForces = false;
    }
}

bool REPlayerObject::handleRotatedCollisionInternal(float dt, GameObject* object, cocos2d::CCRect rect, bool skipCheck, bool skipPre, bool slope) {
    auto originalPosition = getPosition();

    rotateGameplayObject(object);
    rotatePreSlopeObjects_();

    bool collided = false;

    if (slope) {
        collidedWithSlopeInternal(dt, object, skipPre);
    }
    else {
        collided = collidedWithObjectInternal(dt, object, rect, skipCheck);
    }

    auto pos = getPosition();
    auto restoredPosition = CCPoint{(pos.y - originalPosition.y) + originalPosition.x, originalPosition.y - (pos.x - originalPosition.x)};

    setPosition(restoredPosition);

    unrotateGameplayObject(object);
    unrotatePreSlopeObjects_();

    return collided;
}

bool REPlayerObject::handleRotatedObjectCollision_(float dt, GameObject* object, cocos2d::CCRect rect, bool skipCheck) {
    return handleRotatedCollisionInternal(dt, object, rect, skipCheck, false, false);
}

void REPlayerObject::handleRotatedSlopeCollision_(float dt, GameObject* object, bool skipPre) {
    handleRotatedCollisionInternal(dt, object, { 0.f, 0.f, 0.f, 0.f }, false, skipPre, true);
}

void REPlayerObject::hardFlipGravity_() {
    flipGravity(!m_isUpsideDown, true);
    setYVelocity(flipMod_() * -2, 42);
}

void REPlayerObject::hitGround(GameObject* object, bool notFlipped) {
    //todo
    PlayerObject::hitGround(object, notFlipped);
}

void REPlayerObject::hitGroundNoJump(GameObject* object, bool notFlipped) {
    bool wasOnGround = m_isOnGround;
    bool wasOnGround2 = m_isOnGround2;
    double lastLandTime = m_lastLandTime;

    hitGround(nullptr, notFlipped);

    m_lastLandTime = lastLandTime;
    m_isOnGround2 = wasOnGround2;
    m_isOnGround = wasOnGround;
}

void REPlayerObject::incrementJumps() {
    if (!m_playEffects) return; 

    auto playLayer = GameManager::get()->m_playLayer;

    playLayer->m_hasJumped = true;
    playLayer->m_uncommittedJumps++;
    playLayer->m_jumps++;
    playLayer->m_level->m_jumps = playLayer->m_level->m_jumps.value() + 1;
    m_hasEverJumped = true;
}

bool REPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
    //todo
    return PlayerObject::init(player, ship, gameLayer, layer, playLayer);
}

bool REPlayerObject::isBoostValid_(float yVelocity) {
    if (m_isUpsideDown) {
        if (!m_isCurrentSlopeTop && m_slopeFlipGravityRelated) {
            return yVelocity > m_yVelocity;
        }
        else {
            return yVelocity < m_yVelocity;
        }
    }
    else {
        if (!m_isCurrentSlopeTop || !m_slopeFlipGravityRelated) {
            return yVelocity > m_yVelocity;
        }
        else {
            return yVelocity < m_yVelocity;
        }
    }
}

bool REPlayerObject::isFlying_() {
    return m_isShip || m_isBird || m_isDart || m_isSwing;
}

bool REPlayerObject::isInBasicMode_() {
    return !isFlying_() && !m_isBall && !m_isSpider;
}

bool REPlayerObject::isInNormalMode_() {
    return !isFlying_() && !m_isBall && !m_isRobot && !m_isSpider;
}

bool REPlayerObject::isSafeFlip_(float flipTime) {
    return m_lastFlipTime != 0.0 && m_totalTime - m_lastFlipTime < flipTime;
}

bool REPlayerObject::isSafeHeadTest_() {
    return isSafeFlip_(0.2f) || isSafeMode_(0.2f) || m_stateHitHead > 0;
}

bool REPlayerObject::isSafeMode_(float changeTime) {
    return m_gameModeChangedTime != 0.0 && m_totalTime - m_gameModeChangedTime < changeTime;
}

bool REPlayerObject::isSafeSpiderFlip_(float flipTime) {
    return m_lastSpiderFlipTime != 0.0 && m_totalTime - m_lastSpiderFlipTime < flipTime;
}

void REPlayerObject::levelFlipFinished() {
    m_trailingParticles->setLife(m_trailingParticleLife);

    if (isFlying_() || m_alwaysShowStreak) {
        resetStreak();
        activateStreak();

        if (m_isDart) {
            placeStreakPoint();
        }
    }
}

bool REPlayerObject::levelFlipping() {
    if (!m_playEffects) return false;
    return GameManager::get()->m_playLayer->isFlipping();
}

void REPlayerObject::levelWillFlip() {
    m_trailingParticles->setLife(m_trailingParticleLife * 0.3f);
    deactivateStreak_(true);
}

void snapRotation360__(float& rotation) {
    if (rotation > 180.f) {
        rotation -= 360.f;
    }
    else if (rotation < -180.f) {
        rotation += 360.f;
    }
}

void REPlayerObject::limitDashRotation_(float& rotation) {
    float offset = 0.f;
    if (m_isSideways) {
        offset = m_isGoingLeft ? 90.f : -90.f;
        rotation += offset;
        snapRotation360__(rotation);
    }
    else {
        if (m_isGoingLeft) {
            offset = 180.f;
            rotation += offset;
            snapRotation360__(rotation);
        }
    }
    if (std::abs(rotation) > 90.f) {
        rotation = ((rotation <= 0.0f) ? -180.0f : 180.0f) - rotation;
        snapRotation360__(rotation);
    }
    if (rotation > 70.f || rotation < -70.f) {
        if (rotation > 0.f) {
            if (rotation <= 70.f || rotation >= 180.f) {
                rotation = -70.f;
            }
        }
        else if (rotation < 70.f || rotation > -180.f) {
            rotation = -70.f;
        }
        else {
            rotation = 70.f;
        }
    }
    if (offset != 0.f) {
        rotation -= offset;
        snapRotation360__(rotation);
    }
}

void REPlayerObject::loadFromCheckpoint(PlayerCheckpoint* object) {
    //todo
    PlayerObject::loadFromCheckpoint(object);
}

void REPlayerObject::lockPlayer() {
    if (m_isDart) {
        placeStreakPoint();
    }

    m_isLocked = true;
    stopRotation_(false, 1);
    releaseButton(PlayerButton::Jump);
    deactivateParticle_();

    m_isOnGround2 = false;

    if (m_isDart) {
        fadeOutStreak2(0.4f);
    }
}

void REPlayerObject::logValues_() {}

void REPlayerObject::modeDidChange_() {
    updatePlayerArt();
    updateDashArt();
}

void REPlayerObject::performSlideCheck_() {
    if (m_currentSlope2 && (m_isOnSlope || m_wasOnSlope) && !m_maybeUpsideDownSlope && !m_currentSlope2->m_isGripSlope && std::abs(m_slopeAngle * (180.f / M_PI)) > 40.f) {
        auto movingLeft = m_platformerMovingLeft;
        auto movingRight = m_platformerMovingRight;
        if (m_slopeSlidingMaybeRotated) {
            m_platformerMovingLeft = !m_isUpsideDown;
            m_platformerMovingRight = m_isUpsideDown;
        }
        else {
            m_platformerMovingLeft = m_isUpsideDown;
            m_platformerMovingRight = !m_isUpsideDown;
        }
        if (m_platformerMovingLeft != movingLeft || m_platformerMovingRight != movingRight) {
            m_changedDirectionsTime = m_totalTime;
        }
        m_maybeChangedDirectionAngle = std::abs(m_slopeAngle * (180.f / M_PI));
        m_maybeGoingCorrectSlopeDirection = true;
    }
    else {
        m_platformerMovingLeft = false;
        m_platformerMovingRight = false;
    }
}

void REPlayerObject::placeStreakPoint() {
    if (!m_maybeReducedEffects && m_isDart && !m_isLocked && m_fadeOutStreak && !levelFlipping()) {
        m_waveTrail->addPoint(getPosition());
    }

    m_justPlacedStreak = false;
}

void REPlayerObject::playBumpEffect(int objectType, GameObject* player) {
    auto gameManager = GameManager::get();
    if (gameManager->m_performanceMode || !m_playEffects) return;

    auto type = static_cast<GameObjectType>(objectType);

    float startRadius = 10.f;
    if (m_vehicleSize >= 1.f && type == GameObjectType::RedJumpPad) {
        startRadius = 12.f;
    }

    auto wave = CCCircleWave::create(startRadius, 40.f, 0.25f, false, true);

    auto color = ccColor3B{255, 200, 0};

    if (type == GameObjectType::PinkJumpPad) {
        color = ccColor3B{255, 0, 0};
    }
    else if (type == GameObjectType::GravityPad) {
        color = ccColor3B{0, 255, 255};
    }
    else if (type == GameObjectType::SpiderPad) {
        color = ccColor3B{255, 50, 255};
    }

    wave->m_color = color;
    wave->setPosition(m_lastPortalPos);

    if (m_lastActivatedPortal) {
        wave->followObject(m_lastActivatedPortal, true);
        wave->m_delegate = gameManager->m_playLayer;
        
        gameManager->m_playLayer->addCircle(wave);
    }

    m_parentLayer->addChild(wave, 0);
}

void REPlayerObject::playBurstEffect_() {
    if (!levelFlipping() && !m_isHidden) {
        m_ufoClickParticles->resumeSystem();
        stopActionByTag(7);

        auto action = CCSequence::create(
            CCDelayTime::create(.12f),
            CCCallFunc::create(this, callfunc_selector(PlayerObject::stopBurstEffect)),
            nullptr
        );
        action->setTag(7);
        runAction(action);
    }
}

void REPlayerObject::playCompleteEffect(bool noEffects, bool instant) {
    if (m_isDart) {
        fadeOutStreak2(0.2f);
    }

    stopDashing();
    deactivateParticle_();

    m_trailingParticles->stopSystem();
    m_shipClickParticles->stopSystem();
    m_vehicleGroundParticles->stopSystem();
    m_robotBurstParticles->stopSystem();

    if (m_robotFire) {
        m_robotFire->stopAllActions();
        m_robotFire->update(0.2f);
    }

    toggleGhostEffect(GhostType::Disabled);
    disableSwingFire();

    auto parent = getParent() ? static_cast<CCLayer*>(getParent()) : m_parentLayer;

    if (instant) {
        setOpacity(0);
    }
    else {
        auto fade = CCFadeTo::create(0.1f, 0);
        fade->setTag(4);
        runAction(fade);
    }

    if (noEffects) return;

    auto explosion = CCParticleSystemQuad::create("explodeEffectVortex.plist", false);
    explosion->setPositionType(kCCPositionTypeGrouped);
    m_parentLayer->addChild(explosion, 99);
    explosion->update(1.f);
    explosion->setPosition(getPosition());

    auto startColor = ccColor4F{m_playerColor1.r / 255.f, m_playerColor1.g / 255.f, m_playerColor1.b / 255.f, 1.f};
    auto endColor = ccColor4F{0.f, 0.f, 1.f, 1.f};

    explosion->setStartColor(startColor);
    explosion->setStartColorVar({0.f, 0.f, 0.f, 1.f});
    explosion->setEndColor(endColor);
    explosion->setEndColorVar({0.f, 0.f, 0.f, 1.f});

    explosion->setStartSize(explosion->getStartSize() * 3.f);
    explosion->setStartSizeVar(explosion->getStartSizeVar());

    explosion->setEndSize(explosion->getEndSize());
    explosion->setEndSizeVar(explosion->getEndSizeVar());
    explosion->resetSystem();

    auto wave1 = CCCircleWave::create(20.f, 80.f, 0.72f, false, true);
    wave1->m_color = m_playerColor1;
    wave1->setPosition(getPosition());
    parent->addChild(wave1, 99);

    auto wave2 = CCCircleWave::create(30.f, 50.f, 0.84000003f, false, true);
    wave2->m_color = m_playerColor2;
    wave2->setPosition(getPosition());
    wave2->m_opacityMod = 0.8f;
    m_parentLayer->addChild(wave2, 1000);

    auto wave3 = CCCircleWave::create(30.f, 20.f, 0.96000004f, false, true);
    wave3->m_color = m_playerColor1;
    wave3->setPosition(getPosition());
    wave3->m_opacityMod = 0.8f;
    m_parentLayer->addChild(wave3, 1000);
}

void REPlayerObject::playDeathEffect() {
    //todo
    PlayerObject::playDeathEffect();
}

void REPlayerObject::playDynamicSpiderRun() {
    if (m_isDashing) return;

    auto currentAnimation = m_spiderSprite->m_animationManager->m_currentAnimation;
    bool isWalk = currentAnimation == "walk";

    if (m_playerSpeed <= 1.1f) {
        if (m_playerSpeed > 0.9f && isWalk) {
            m_spiderSprite->runAnimation("run");
        }
        else {
            m_spiderSprite->runAnimation("walk");
        }
        return;
    }

    m_spiderSprite->runAnimation(m_spiderAnimationEnabled ? "run" : "run_fast");

    if (isWalk) {
        m_spiderSprite->m_animationManager->offsetCurrentAnimation(0.1f);
    }
}

void REPlayerObject::playerDestroyed(bool noEffects) {
    if (m_isDart) {
        fadeOutStreak2(0.2f);
    }

    stopDashing();

    m_isDead = true;
    stopRotation_(false, 1);
    stopParticles();
    m_lastPosition = getPosition();

    if (m_robotFire) {
        m_robotFire->stopAllActions();
        m_robotFire->setVisible(false);
    }

    toggleGhostEffect(GhostType::Disabled);
    disableSwingFire();

    if (m_playEffects && !noEffects && !m_isHidden) {
        playDeathEffect();
    }
    else {
        setOpacity(0);
    }
}

bool REPlayerObject::playerIsFalling_(float yVelocity) {
    return m_isUpsideDown ? -yVelocity < m_yVelocity : yVelocity > m_yVelocity;
}

bool REPlayerObject::playerIsFallingBugged() {
    double threshold = m_gravity * 2.0;

    if (m_isSideways || m_isPlatformer || m_isSwing || m_fixGravityBug) {
        return playerIsFalling_(threshold);
    }

    double adjustedGravity = m_unkA99 ? -m_gravity : m_gravity;
    double adjustedThreshold = adjustedGravity * 2.0;

    if (!m_isUpsideDown) {
        return m_yVelocity < adjustedThreshold;
    }

    return m_yVelocity > threshold;
}

bool REPlayerObject::playerIsMovingUp_() {
    return m_isUpsideDown ? m_yVelocity < 0.0 : m_yVelocity > 0.0;
}

void REPlayerObject::playerTeleported_() {
    m_isOnGround2 = false;
    m_lastGroundedPos = CCPoint{ 0.f, 0.f};
    placeStreakPoint();
}

void REPlayerObject::playingEndEffect_() {
    if (m_isSpider) m_spiderSprite->tweenToAnimation("fall_loop", 0.4f);
}

void REPlayerObject::playSpawnEffect() {
    if (m_isBeingSpawnedByDualPortal) return;

    stopActionByTag(11);

    auto blink = CCBlink::create(0.4f, 4);
    auto show = CCShow::create();
    auto sequence = CCSequence::create(blink, show, nullptr);

    sequence->setTag(11);
    runAction(sequence);

    auto gameManager = GameManager::get();
    if (gameManager->m_performanceMode) return;
    
    for (int i = 0; i < 4; ++i) {
        auto spawn = CCCallFunc::create(this, callfunc_selector(PlayerObject::spawnCircle));
        auto delay = CCDelayTime::create(i * 0.1f);
        auto delayedSpawn = CCSequence::create(delay, spawn, nullptr);

        runAction(delayedSpawn);
    }
}

void REPlayerObject::playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
    //todo
    PlayerObject::playSpiderDashEffect(from, to);
}

void REPlayerObject::postCollision(float dt, bool betweenSteps) {
    //todo
    PlayerObject::postCollision(dt, betweenSteps);
}

void REPlayerObject::preCollision_() {
    m_wasTeleported = false;
    m_ringJumpRelated = false;
    resetCollisionValues_();
    m_wasOnSlope = m_isOnSlope;
    m_isOnSlope = false;
    m_isOnGround4 = m_isOnGround2;

    if (m_isOnGround2 && !m_platformerMovingLeft && !m_platformerMovingRight && m_maybeSlidingTime > 0) {
        m_maybeSlidingTime = 0;
        m_maybeSlidingStartTime = m_totalTime;
    }

    if (!m_unk669) {
        m_unk669 = true;
    }
    else {
        m_currentPotentialSlope = nullptr;
    }
    m_potentialSlopeMap.clear();

    if (auto slope = m_currentPotentialSlope) {
        m_potentialSlopeMap[slope->m_uniqueID] = slope;
    }
    if (auto slope = m_currentSlope) {
        m_potentialSlopeMap[slope->m_uniqueID] = slope;
    }
}

bool REPlayerObject::preSlopeCollision(float dt, GameObject* object) {
    if (object->m_uniqueID == m_collidingWithSlopeId) return false;
    
    auto slopeRect = object->getObjectRect();
    auto playerRect = getObjectRect();

    bool checksLeftEdge = object->slopeWallLeft();
    bool checksTopEdge = object->slopeFloorTop();

    float slopeInset = 0.f;
    if (m_isPlatformer && (m_isOnSlope || m_wasOnSlope)) {
        slopeInset = 5.f;
    }

    auto objectDelta = object->getPosition() - object->m_lastPosition;
    float movementAlongSlope = m_isSideways ? objectDelta.y : objectDelta.x;

    float frameMove = static_cast<float>(m_playerSpeed * m_speedMultiplier * dt);

    bool handled = false;
    float edgeInset = 0.f;

    if (checksLeftEdge) {
        bool canCheckLeft = !m_isGoingLeft || m_isPlatformer || movementAlongSlope < frameMove;

        if (canCheckLeft && getPosition().x <= slopeRect.origin.x && getPosition().x != slopeRect.origin.x) {
            edgeInset = (m_isGoingLeft && !m_isPlatformer) ? 1.f : 0.f;

            float yInset = slopeInset + edgeInset;
            auto edgeRect = CCRect{slopeRect.origin.x, slopeRect.origin.y + yInset, 1.f, slopeRect.size.height - (yInset * 2.f)};

            handled = true;

            if (playerRect.intersectsRect(edgeRect)) {
                collidedWithObjectInternal(dt, object, edgeRect, false);
            }
        }
    }
    else {
        bool canCheckRight = m_isGoingLeft || m_isPlatformer || frameMove < movementAlongSlope;

        float slopeMaxX = slopeRect.getMaxX();

        if (canCheckRight && slopeMaxX < getPosition().x) {
            edgeInset = (!m_isGoingLeft && !m_isPlatformer) ? 1.f : 0.f;

            float x = slopeRect.origin.x + slopeRect.size.width - 1.f;
            float yInset = slopeInset + edgeInset;
            auto edgeRect = CCRect{x, slopeRect.origin.y + yInset, 1.f, slopeRect.size.height - (yInset * 2.f)};

            handled = true;

            if (playerRect.intersectsRect(edgeRect)) {
                collidedWithObjectInternal(dt, object, edgeRect, false);
            }
        }
    }

    slopeRect.size.width -= edgeInset * 2.f;

    if (checksTopEdge) {
        auto topOrBottomRect = CCRect{slopeRect.origin.x + edgeInset, slopeRect.origin.y + slopeRect.size.height - 1.f, slopeRect.size.width, 1.f};

        if (topOrBottomRect.origin.y + topOrBottomRect.size.height <= getPosition().y) {
            handled = true;

            if (playerRect.intersectsRect(topOrBottomRect)) {
                collidedWithObjectInternal(dt, object, topOrBottomRect, false);
            }
        }
    }
    else {
        auto topOrBottomRect = CCRect{slopeRect.origin.x + edgeInset, slopeRect.origin.y, slopeRect.size.width, 1.f};

        if (getPosition().y <= topOrBottomRect.origin.y + topOrBottomRect.size.height) {
            handled = true;

            if (playerRect.intersectsRect(topOrBottomRect)) {
                collidedWithObjectInternal(dt, object, topOrBottomRect, false);
            }
        }
    }

    if (!handled && !m_isOnSlope) {
        m_unk669 = false;
        m_potentialSlopeMap[object->m_uniqueID] = object;

        auto currentSlope = m_currentPotentialSlope;
        if (currentSlope) {

            bool checksTopEdgeCurrent = currentSlope->slopeFloorTop();
            bool currentIsPreferred = (!currentSlope->m_slopeUphill || checksTopEdgeCurrent);

            bool objectIsPreferred = (!object->m_slopeUphill || checksTopEdge);

            if (currentIsPreferred && objectIsPreferred) {
                if (object->getPosition().x > currentSlope->getPosition().x) {
                    m_currentPotentialSlope = object;
                }
            }
            else if (currentSlope->m_slopeUphill && !checksTopEdgeCurrent && object->getPosition().x > currentSlope->getPosition().x) {
                m_currentPotentialSlope = object;
            }
        }
        else {
            m_currentPotentialSlope = object;
        }
    }

    if (!slopeRect.intersectsRect(playerRect)) {
        handled = true;
    }

    return handled;
}

void REPlayerObject::propellPlayer(float yVelocity, bool noEffects, int objectType) {
    m_maybeIsBoosted = true;
    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = true;
    m_isOnSlope = false;
    m_wasOnSlope = false;

    float sizeScale = m_vehicleSize == 1.f ? 1.f : 0.8f;

    setYVelocity(static_cast<double>(flipMod_() * yVelocity * 16.f * sizeScale), 0);

    if (m_isBall || m_isSpider || m_isSwing) {
        m_yVelocity *= 0.6;
    }

    runRotateAction_(false, 0);

    if (!noEffects) {
        playBumpEffect(objectType, nullptr);
    }

    if (m_isRobot) {
        m_robotSprite->runAnimation("jump_start");
    }
    else if (m_isSpider) {
        m_spiderSprite->runAnimation("fall_loop");
    }

    activateStreak();
    m_lastGroundedPos = getPosition();
}

bool REPlayerObject::pushButton(PlayerButton button) {
    if (!m_inputsLocked) {
        m_holdingButtons[static_cast<int>(button)] = true;
    }

    if (m_isLocked || m_controlsDisabled) return false;

    if (button == PlayerButton::Jump) {
        if (!m_jumpBuffered) {
            placeStreakPoint();
        }

        m_jumpBuffered = true;
        m_stateRingJump = true;
        m_padRingRelated = false;

        m_ringRelatedSet.clear();

        updateJumpVariables_();

        if (m_touchingRings->count() == 0) {
            bool canGroundJump = m_isOnGround && (m_isBall || ((!m_isShip && !m_isBird && !m_isDart && !m_isSwing) && m_jumpBuffered));

            if (canGroundJump) {
                updateJump(0.f);
            }

            return true;
        }

        auto customRings = CCArray::createWithCapacity(m_touchingRings->count());
        auto otherRings = CCArray::createWithCapacity(m_touchingRings->count());

        for (auto object : CCArrayExt<RingObject, false>(m_touchingRings)) {
            if (!object) continue;
        
            if (object->m_claimTouch) {
                ringJump(object, false);
                m_jumpBuffered = false;
                return true;
            }

            auto targetArray = object->getType() == GameObjectType::CustomRing ? customRings : otherRings;
            targetArray->addObject(object);
        }

        for (auto ring : CCArrayExt<RingObject, false>(customRings)) {
            if (ring) {
                ringJump(ring, false);
            }
        }

        for (auto ring : CCArrayExt<RingObject, false>(otherRings)) {
            if (ring) {
                ringJump(ring, false);
            }
        }

        return true;
    }

    if (button == PlayerButton::Left || button == PlayerButton::Right) {
        switchedDirTo(button);
    }

    return true;
}

void REPlayerObject::pushDown_() {
    setYVelocity(0.0, 46);
    m_isOnGround2 = false;
    m_isOnGround = false;
}

void REPlayerObject::pushPlayer_(float yVelocity) {
    addToYVelocity_(yVelocity, 68);
}

void REPlayerObject::redirectDash_(float rotation) {
    if (!m_isDashing) return;

    auto dashPos = CCPoint{ static_cast<float>(m_dashX), static_cast<float>(m_dashY)};
    auto direction = GJBaseGameLayer::convertToClosestDirection(rotation * (M_PI / 180.f) - std::atan2f(dashPos.y, dashPos.x), M_PI);
    
    if (direction != 0.f) {
        auto cosDir = std::cosf(direction);
        auto sinDir = std::sinf(direction);
        dashPos.x = dashPos.x * cosDir - dashPos.y * sinDir;
        dashPos.y = dashPos.x * sinDir + dashPos.y * cosDir;
    }

    auto angle = ((int)-(atan2f(dashPos.y, dashPos.x) * (180.f / M_PI)) + (m_isSideways ? 90 : 0)) % 360;
    m_dashAngle = -(angle + (angle < 0 ? 360 : 0));
    m_dashX = m_isSideways ? dashPos.y : dashPos.x;
    m_dashY = m_isSideways ? dashPos.x : dashPos.y;

    if (m_dashX < 0.0) {
        doReversePlayer(!m_isSideways);
    }
    else if (m_dashX > 0.0) {
        doReversePlayer(m_isSideways);
    }
    updateDashArt();
}

void REPlayerObject::redirectPlayerForce(float rotation, float modifier, float minimum, float maximum) {
    float radians = rotation * 0.017453292f;

    auto velocity = CCPoint{static_cast<float>(m_platformerXVelocity), static_cast<float>(m_yVelocity)};

    float currentAngle = std::atan2f(velocity.y, velocity.x);
    float angleDelta = GJBaseGameLayer::convertToClosestDirection(radians - currentAngle, 3.1415927f);

    if (angleDelta != 0.f) {
        float c = std::cosf(angleDelta);
        float s = std::sinf(angleDelta);

        velocity = CCPoint{velocity.x * c - velocity.y * s, velocity.y * c + velocity.x * s};
    }

    velocity = velocity * modifier;

    float magnitudeSq = velocity.x * velocity.x + velocity.y * velocity.y;
    float magnitude = std::sqrtf(magnitudeSq);

    if (maximum > 0.f && magnitude > maximum) {
        if (magnitude != 0.f) {
            velocity = velocity * (maximum / magnitude);
        }
    }
    else if (minimum > 0.f && magnitude < minimum) {
        if (magnitude != 0.f) {
            velocity = velocity * (minimum / magnitude);
        }
        else {
            velocity = CCPoint{std::cosf(radians) * minimum, std::sinf(radians) * minimum};
        }
    }

    if (m_isSideways) {
        std::swap(velocity.x, velocity.y);
    }

    m_isAccelerating = true;

    updatePlayerForce_(velocity, false);
}

void REPlayerObject::releaseAllButtons() {
    releaseButton(PlayerButton::Jump);
    releaseButton(PlayerButton::Left);
    releaseButton(PlayerButton::Right);
    releaseButton(static_cast<PlayerButton>(5));
}

bool REPlayerObject::releaseButton(PlayerButton button) {
    if (static_cast<int>(button) == 5) { // FIX BINDINGS, PlayerButton enum needs something at 5
        releaseButton(PlayerButton::Left);
        releaseButton(PlayerButton::Right);
        return !m_controlsDisabled;
    }

    if (!m_inputsLocked) {
        m_holdingButtons[static_cast<int>(button)] = false;
    }

    if (m_controlsDisabled) return false;

    if (m_jumpBuffered) {
        placeStreakPoint();
    }

    if (button == PlayerButton::Jump) {
        m_jumpBuffered = false;
        m_stateRingJump = false;
        m_touchedPad = true;

        if (m_isDashing) {
            stopDashing();
        }

        return true;
    }

    if (button == PlayerButton::Left) {
        m_holdingLeft = false;

        if (m_platformerXVelocity < 0.0) {
            m_isMoving = false;
        }

        return true;
    }

    if (button == PlayerButton::Right) {
        m_holdingRight = false;

        if (m_platformerXVelocity > 0.0) {
            m_isMoving = false;
        }

        return true;
    }

    return true;
}

void REPlayerObject::removeAllParticles_() {
    for (auto particle : CCArrayExt<CCParticleSystem, false>(m_particleSystems)) {
        particle->resetSystem();
        particle->stopSystem();
        particle->removeFromParentAndCleanup(true);
    }
}

void REPlayerObject::removePendingCheckpoint() {
    if (!m_pendingCheckpoint) return;

    auto checkpointObject = m_pendingCheckpoint->m_physicalCheckpointObject;

    checkpointObject->removeGlow();

    checkpointObject->removeMeAndCleanup();
    m_pendingCheckpoint->release();
    m_pendingCheckpoint = nullptr;
}

void REPlayerObject::removePlacedCheckpoint_() {
    if (m_checkpointTimeout) {
        GameManager::get()->m_playLayer->removeCheckpoint(false);
        m_checkpointTimeout = false;
    }
}

void REPlayerObject::resetAllParticles() {
    for (auto particleSystem : CCArrayExt<CCParticleSystem, false>(m_particleSystems)) {
        particleSystem->resetSystem();
        particleSystem->stopSystem();
    }
}

void REPlayerObject::resetCollisionLog_(bool full) {
    m_collisionLogTop->removeAllObjects();
    m_collisionLogBottom->removeAllObjects();
    m_collisionLogLeft->removeAllObjects();
    m_collisionLogRight->removeAllObjects();

    if (full) {
        m_unk50C = -1;
        m_unk510 = -1;
    }
    else {
        m_unk50C = m_lastCollisionBottom;
        m_unk510 = m_lastCollisionTop;
    }

    m_lastCollisionBottom = -1;
    m_lastCollisionTop = -1;
    m_lastCollisionLeft = -1;
    m_lastCollisionRight = -1;
}

void REPlayerObject::resetCollisionValues_() {
    m_collidedTopMinY = 0.0;
    m_collidedBottomMaxY = 0.0;
    m_collidedLeftMaxX = 0.0;
    m_collidedRightMinX = 0.0;
}

void REPlayerObject::resetPlayerIcon() {
    stopPlatformerJumpAnimation();

    m_width = 30.f;
    m_height = 30.f;
    m_unkAngle1 = 30.f;

    runRotateAction(false, 0);

    m_iconSprite->setScale(1.f);
    m_iconSprite->setPosition(CCPoint{0.f, 0.f});

    m_vehicleSprite->setVisible(false);
    m_birdVehicle->setVisible(false);

    updatePlayerGlow();

    m_trailingParticles->stopSystem();
    m_shipClickParticles->stopSystem();
    m_vehicleGroundParticles->stopSystem();

    m_trailingParticles->setStartColor(ccColor4F{1.f, 0.39215687f, 0.f, 1.f});
    m_trailingParticles->setEndColor(ccColor4F{1.f, 0.f, 0.f, 1.f});

    auto trailColor = m_disableStreakTint ? ccColor3B{255, 255, 255} : m_playerColor2;
    m_regularTrail->setColor(trailColor);

    deactivateStreak_(false);

    if (m_shipStreak) {
        m_shipStreak->setVisible(false);
    }

    updatePlayerScale();
}

void REPlayerObject::resetStateVariables_() { // FIX BINDINGS, Final 4 members here should be in inline definition
    m_stateOnGround = 0;
    m_stateNoAutoJump = 0;
    m_stateDartSlide = 0;
    m_stateHitHead = 0;
    m_stateFlipGravity = 0;
    m_stateBoostX = 0;
    m_stateBoostY = 0;
    m_maybeStateForce2 = 0;
    m_stateScale = 0;
    m_stateForce = 0;
    m_stateForceVector = cocos2d::CCPoint {0.f, 0.f};
    m_jumpPadRelated.clear();
    m_stateUnk = '\0';
    m_stateNoStickX = '\0';
    m_stateNoStickY = '\0';
    m_stateUnk2 = '\0';
}

void REPlayerObject::resetStreak() {
    if (levelFlipping()) return;
    
    m_regularTrail->reset();

    auto trailOffset = CCPoint{-5.f, 0.f};
    m_regularTrail->setPosition(getPosition() + trailOffset);

    m_waveTrail->reset();
    m_waveTrail->setOpacity(255);
    m_waveTrail->stopAllActions();
    m_waveTrail->setPosition(-5.f, 0.f);
}

void REPlayerObject::resetTouchedRings(bool removeAll) {
    if (removeAll) {
        m_touchingRings->removeAllObjects();
        m_touchedRings.clear();
        return;
    }

    for (unsigned i = 0; i < m_touchingRings->count();) {
        auto ring = static_cast<RingObject*>(m_touchingRings->objectAtIndex(i));

        if (!m_touchedRings.contains(ring->m_uniqueID)) {
            m_touchingRings->removeObjectAtIndex(i, true);
        }
        else {
            ++i;
        }
    }

    m_touchedRings.clear();
}

int REPlayerObject::reverseMod_() {
    return (m_isGoingLeft ? -1 : 1);
}

void REPlayerObject::reversePlayer(EffectGameObject* object) {
    if (m_isPlatformer) return;

    if (object && m_reverseSync) {
        float reverseSpeed = static_cast<float>(m_maybeReverseSpeed);
        auto predictedPosition = getPosition() + CCPoint{reverseSpeed, reverseSpeed};
        auto delta = object->getPosition() - predictedPosition;

        float axisDelta = m_isSideways ? delta.y : delta.x;
        m_maybeReverseSpeed += axisDelta + axisDelta;
    }

    doReversePlayer(!m_isGoingLeft);
}

void REPlayerObject::ringJump(RingObject* object, bool skipCheck) {
    //todo
    PlayerObject::ringJump(object, skipCheck);
}

void REPlayerObject::rotateGameplay(int moveDirection, int groundDirection, bool editVelocity, float velocityModX, float velocityModY, bool overrideVelocity, bool dontSlide) {
    double oldYVelocity = m_yVelocity;
    double oldXVelocity = getCurrentXVelocity_();

    bool oldUpsideDown = m_isUpsideDown;
    bool oldSideways = m_isSideways;
    bool oldGoingLeft = m_isGoingLeft;

    bool newSideways = moveDirection == 3 || moveDirection == 4;
    m_isSideways = newSideways;

    bool shouldFlipGravity = moveDirection == 1 || moveDirection == 4;
    flipGravity(shouldFlipGravity, true);

    bool reverse = oldGoingLeft;
    if (!m_isPlatformer) {
        reverse = groundDirection == 2 || groundDirection == 3;
    }

    doReversePlayer(reverse);
    updatePlayerArt();

    bool sidewaysChanged = newSideways != oldSideways;
    bool gravityChanged = oldUpsideDown != m_isUpsideDown;
    bool directionChanged = oldGoingLeft != m_isGoingLeft;

    if (!sidewaysChanged && !gravityChanged && !directionChanged) return;

    if (sidewaysChanged) {
        if (!directionChanged && m_isDart) {
            createFadeOutDartStreak();

            if (m_fadeOutStreak) {
                m_waveTrail->clear();
                m_waveTrail->m_pointArray->removeAllObjects();
                placeStreakPoint();
            }
        }

        if (m_vehicleGroundParticles->isActive()) {
            m_vehicleGroundParticles->resetSystem();
        }

        resetCollisionLog_(true);

        auto velocity = CCPoint{static_cast<float>(oldXVelocity), static_cast<float>(oldYVelocity)};

        std::swap(velocity.x, velocity.y);

        if (editVelocity) {
            if (overrideVelocity) {
                velocity.x = velocityModX;
                velocity.y = velocityModY;
            }
            else {
                velocity.x *= velocityModX;
                velocity.y *= velocityModY;
            }
        }

        m_isAccelerating = true;

        updatePlayerForce_(velocity, false);

        m_maybeIsBoosted = true;

        playerTeleported_();

        if (dontSlide) {
            handlePlayerCommand_(543);
        }

        if (isInNormalMode_() && !m_isRotating) {
            runRotateAction_(false, 0);
        }

        if (m_isDashing) {
            std::swap(m_dashX, m_dashY);
        }
    }

    if (!m_isDashing) return;

    if (oldGoingLeft != m_isGoingLeft) {
        if (sidewaysChanged) {
            m_dashAngle += 180.0;
        }

        m_dashAngle = (m_dashAngle <= 0.0 ? -180.0 : 180.0) - m_dashAngle;
    }

    if (m_dashAngle > 180.0) {
        m_dashAngle -= 360.0;
    }
    else if (m_dashAngle < -180.0) {
        m_dashAngle += 360.0;
    }

    updateDashArt();
}

void REPlayerObject::rotateGameplayObject(GameObject* object) {
    if (m_rotateObjectsRelated.contains(object->m_uniqueID)) return;
    
    auto oldObjectPos = object->getRealPosition();
    auto playerPos = getPosition();

    float angle = 1.5707964f;
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    float dx = oldObjectPos.x - playerPos.x;
    float dy = oldObjectPos.y - playerPos.y;

    auto rotatedPos = GJPointDouble{c * dx - s * dy + playerPos.x, c * dy + s * dx + playerPos.y};
    auto delta = GJPointDouble{rotatedPos.m_x - oldObjectPos.x, rotatedPos.m_y - oldObjectPos.y};

    object->addToTempOffset(delta.m_x, delta.m_y);
    object->setLastPosition(object->m_lastPosition + CCPoint{static_cast<float>(delta.m_x), static_cast<float>(delta.m_y)});

    object->m_startRotationX -= 90.f;
    object->m_startRotationY -= 90.f;

    object->setRRotation(0.f);
    object->setObjectRectDirty(true);

    if (object->getType() == GameObjectType::Slope) {
        object->determineSlopeDirection();
    }

    m_rotateObjectsRelated[object->m_uniqueID] = delta;
}

void REPlayerObject::rotateGameplayOnly_(bool sideways) {
    m_isSideways = sideways;
    updatePlayerArt();
}

void REPlayerObject::rotatePreSlopeObjects_() {
    for (auto& pair : m_potentialSlopeMap) {
        rotateGameplayObject(pair.second);
    }
}

void REPlayerObject::runBallRotation(float speed) {
    if (m_isDashing || m_isPlatformer) return;
    
    m_isBallRotating = speed != 1.f;
    m_rotateSpeed = speed;

    float sizeScale = m_vehicleSize == 1.f ? 1.f : 0.8f;
    float speedScale = 1.f;

    if (m_playerSpeed == 0.7f) {
        speedScale = 1.2405638f;
    }
    else if (m_playerSpeed == 1.1f) {
        speedScale = 0.80424345f;
    }
    else if (m_playerSpeed == 1.3f) {
        speedScale = 0.6657693f;
    }
    else if (m_playerSpeed == 1.6f) {
        speedScale = 0.5409375f;
    }

    float rotationSpeed = (120.f * flipMod_() * reverseMod_()) / (sizeScale * 0.2f * speedScale);

    if (m_isSideways) {
        rotationSpeed = -rotationSpeed;
    }

    m_rotationSpeed = rotationSpeed;
    m_isRotating = true;
}

void REPlayerObject::runBallRotation2() {
    if (m_isDashing) return;
    
    float sizeScale = m_vehicleSize == 1.f ? 1.f : 0.8f;
    float speedScale = 1.f;

    if (m_playerSpeed == 0.7f) {
        speedScale = 1.2405638f;
    }
    else if (m_playerSpeed == 1.1f) {
        speedScale = 0.80424345f;
    }
    else if (m_playerSpeed == 1.3f) {
        speedScale = 0.6657693f;
    }
    else if (m_playerSpeed == 1.6f) {
        speedScale = 0.5409375f;
    }

    m_isBallRotating2 = true;
    m_rotationSpeed = ((340.f * -flipMod_() * reverseMod_()) * m_gravityMod) / (sizeScale * 0.8f * speedScale);
}

void REPlayerObject::runNormalRotation_() {
    runNormalRotation(false, 1.f);
}

void REPlayerObject::runNormalRotation(bool notNormalMode, float speed) {
    bool canRotate = notNormalMode || ((isInNormalMode_() || m_isBall) && !m_isDashing && (!m_isPlatformer || m_holdingLeft || m_holdingRight || m_platformerMovingLeft || m_platformerMovingRight));

    if (!canRotate) return;
    
    float sizeScale = m_vehicleSize == 1.f ? 0.43333334f : 0.33333334f;

    int sidewaysSign = m_isSideways ? -1 : 1;

    m_isRotating = true;
    m_rotationSpeed = (((flipMod_() * 180) * reverseMod_()) * sidewaysSign * m_gravityMod * speed) / sizeScale;
}

void REPlayerObject::runRotateAction_(bool ground, int type) {
    if (!m_isLocked && !m_isDashing) {
        stopRotation(ground, 22);
        if (m_isBall) {
            runBallRotation(1.f);
        }
        else {
            runNormalRotation_();
        }
    }
}

void REPlayerObject::saveToCheckpoint(PlayerCheckpoint* checkpoint) {
    //todo
    PlayerObject::saveToCheckpoint(checkpoint);
}

void REPlayerObject::setSecondColor(cocos2d::ccColor3B const& color) {
    m_iconSpriteSecondary->setColor(color);
    m_vehicleSpriteSecondary->setColor(color);

    m_robotSprite->m_secondColor = color;
    m_robotSprite->updateColors();

    m_spiderSprite->m_secondColor = color;
    m_spiderSprite->updateColors();
}

void REPlayerObject::setupStreak() {
    auto gameManager = GameManager::get();

    m_playerStreak = gameManager->m_playerStreak;
    m_hasGlow = gameManager->m_playerGlow;

    auto streakTexture = CCString::createWithFormat("streak_%02d_001.png", m_playerStreak)->getCString();

    m_streakStrokeWidth = 10.f;

    float fade = 0.3f;
    float strokeWidth = 10.f;

    switch (m_playerStreak) {
        case 2:
        case 7:
            strokeWidth = 14.f;
            m_disableStreakTint = true;
            m_streakStrokeWidth = 14.f;
            break;

        case 3:
            strokeWidth = 8.5f;
            m_streakStrokeWidth = 8.5f;
            break;

        case 4:
            fade = 0.4f;
            break;

        case 5:
            fade = 0.6f;
            strokeWidth = 5.f;
            m_streakStrokeWidth = 5.f;
            m_alwaysShowStreak = true;
            break;

        case 6:
            fade = 1.f;
            strokeWidth = 3.f;
            m_streakStrokeWidth = 3.f;
            m_alwaysShowStreak = true;
            break;
    }

    m_regularTrail = CCMotionStreak::create(fade, 5.f, strokeWidth, ccColor3B{255, 255, 255}, streakTexture);

    if (m_playerStreak == 6) {
        m_regularTrail->enableRepeatMode(0.1f);
    }

    m_regularTrail->m_fMaxSeg = 50.f;
    m_parentLayer->addChild(m_regularTrail);

    m_shipStreakType = static_cast<ShipStreak>(gameManager->m_playerShipFire.value());

    if (m_shipStreakType != ShipStreak::ShipFire1) {
        float shipFade = 0.f;
        float shipWidth = 0.f;

        getSettingsForStreak__(m_shipStreakType, 1.6f, 1.f, shipFade, shipWidth);

        auto textureName = getFrameForStreak__(m_shipStreakType, 0);
        auto texture = CCTextureCache::get()->addImage(textureName.c_str(), false);

        m_shipStreak = CCMotionStreak::create(shipFade, 1.f, shipWidth, ccColor3B{255, 255, 255}, texture);
        m_parentLayer->addChild(m_shipStreak, -3);

        m_shipStreak->m_fMaxSeg = 50.f;
        m_shipStreak->m_bDontOpacityFade = true;

        m_shipStreak->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    }
    else {
        m_shipStreakType = ShipStreak::ShipFire1;
    }

    m_regularTrail->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

    m_waveTrail = HardStreak::create();
    m_parentLayer->addChild(m_waveTrail, -3);

    if ((gameManager->m_playerColor == 15) && !m_switchWaveTrailColor) {
        m_waveTrail->m_isSolid = true;
    }
    else {
        m_waveTrail->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    }

    deactivateStreak_(true);
}

void REPlayerObject::setYVelocity(double velocity, int type) {
    double whole = static_cast<double>(static_cast<int>(velocity));

    if (velocity == whole) {
        m_yVelocity = velocity;
        return;
    }

    double fractional = velocity - whole;
    double rounded = std::round(fractional * 1000.0) / 1000.0;

    m_yVelocity = whole + rounded;
}

void REPlayerObject::spawnCircle() {
    auto gameManager = GameManager::get();
    if (gameManager->m_performanceMode) return;
    
    auto wave = CCCircleWave::create(70.f, 2.f, 0.3f, true, true);
    wave->m_color = m_playerColor1;
    wave->setPosition(getPosition());

    m_parentLayer->addChild(wave);

    wave->followObject(this, true);

    wave->m_delegate = gameManager->m_playLayer;

    gameManager->m_playLayer->addCircle(wave);
    wave->m_circleMode = CircleMode::Outline;
}

void REPlayerObject::spawnCircle2_() {
    if (GameManager::get()->m_performanceMode) return;

    auto circleWave = CCCircleWave::create(5.f, 50.f, 0.3f, false);
    circleWave->m_color = m_playerColor1;
    circleWave->setPosition(getPosition());
    m_parentLayer->addChild(circleWave, 0);
}

void REPlayerObject::spawnDualCircle() {
    if (m_maybeReducedEffects) return;
    
    auto gameManager = GameManager::get();

    if (gameManager->m_playLayer->m_skipArtReload || gameManager->m_performanceMode) return;

    auto wave = CCCircleWave::create(50.f, 2.f, 0.25f, true, true);
    wave->m_color = m_playerColor1;
    wave->setPosition(getPosition());

    m_parentLayer->addChild(wave);

    wave->followObject(this, true);

    wave->m_delegate = gameManager->m_playLayer;
    gameManager->m_playLayer->addCircle(wave);
}

void REPlayerObject::spawnFromPlayer_(PlayerObject* player, bool flip) {
    setVisible(true);
    setOpacity(255);
    copyAttributes(player);

    if (flip) {
        flipGravity(!player->m_isUpsideDown, true);
        setYVelocity(-player->getYVelocity(), 49);
    }
    else {
        flipGravity(player->m_isUpsideDown, true);
        setYVelocity(player->getYVelocity(), 49);
    }

    m_isOnGround = false;
    m_isOnGround2 = false;

    toggleVisibility(!player->m_isHidden);

    if (m_isDart) {
        placeStreakPoint();
    }
}

void REPlayerObject::spawnPortalCircle(cocos2d::ccColor3B color, float startRadius) {
    if (!m_playEffects || m_maybeReducedEffects) return;

    auto gameManager = GameManager::get();

    if (gameManager->m_playLayer->m_skipArtReload || gameManager->m_performanceMode) return;
    if (m_lastPortalPos.equals(CCPoint{0.f, 0.f})) return;

    auto wave = CCCircleWave::create(startRadius, 5.f, 0.3f, true, true);
    wave->m_color = color;
    wave->setPosition(m_lastPortalPos);

    if (m_lastActivatedPortal) {
        wave->followObject(m_lastActivatedPortal, true);
        wave->m_delegate = gameManager->m_playLayer;
        
        gameManager->m_playLayer->addCircle(wave);
    }

    m_parentLayer->addChild(wave, 0);
}

void REPlayerObject::spawnScaleCircle() {
    if (m_maybeReducedEffects) return;

    auto gameManager = GameManager::get();

    if (gameManager->m_playLayer->m_skipArtReload || gameManager->m_performanceMode) return;
    
    float startRadius = 50.f;
    float endRadius = 2.f;
    float duration = 0.25f;
    auto color = ccColor3B{255, 0, 150};

    bool isNormalSize = m_vehicleSize == 1.f;
    if (isNormalSize) {
        startRadius = 10.f;
        endRadius = 40.f;
        duration = 0.3f;
        color = ccColor3B{0, 255, 150};
    }

    auto wave = CCCircleWave::create(startRadius, endRadius, duration, !isNormalSize, true);
    wave->m_color = color;
    wave->setPosition(getPosition());

    m_parentLayer->addChild(wave);

    wave->followObject(this, true);

    wave->m_delegate = gameManager->m_playLayer;
    gameManager->m_playLayer->addCircle(wave);
}

void REPlayerObject::specialGroundHit_() {
    setYVelocity(flipMod_() * -5, 47);

    if (!m_isBall && !isFlying_()) {
        stopRotation(false, 21);
    }
    m_maybeSpriteRelated = true;
}

void REPlayerObject::speedDown_() {
    m_speedMultiplier -= 0.005;
    logValues();
}

void REPlayerObject::speedUp_() {
    m_speedMultiplier += 0.005;
    logValues();
}

void REPlayerObject::spiderTestJump(bool dynamic) {
    spiderTestJumpInternal(false);
    gameEventTriggered_(GJGameEvent::SpiderTeleport, 0);

    if (!isInBasicMode_()) return;

    float rotation = 0.f;

    if (!m_isSideways) {
        rotation = m_isUpsideDown ? 180.f : 0.f;
    }
    else {
        rotation = m_isUpsideDown ? 90.f : 270.f;
    }

    setRotation(rotation);
}

void REPlayerObject::spiderTestJumpInternal(bool dynamic) {
    //todo
    PlayerObject::spiderTestJumpInternal(dynamic);
}

void REPlayerObject::spiderTestJumpX_(bool dynamic) { // missing windows binding
    //todo
}

void REPlayerObject::spiderTestJumpY_(bool dynamic) { // missing windows binding
    //todo
}
 
void REPlayerObject::startDashing(DashRingObject* object) {
    //todo
    PlayerObject::startDashing(object);
}

void REPlayerObject::stopBurstEffect() {
    m_ufoClickParticles->stopSystem();
}

void REPlayerObject::stopDashing() {
    //todo
    PlayerObject::stopDashing();
}

void REPlayerObject::stopParticles() {
    deactivateParticle_();

    m_trailingParticles->stopSystem();
    m_shipClickParticles->stopSystem();
    m_vehicleGroundParticles->stopSystem();
    m_robotBurstParticles->stopSystem();
}

void REPlayerObject::stopPlatformerJumpAnimation() {
    if (m_isPlatformer) {
        m_iconSprite->stopActionByTag(13);
        m_iconSprite->stopActionByTag(14);
        m_iconSprite->setScale(1.f);

        m_iconGlow->stopActionByTag(13);
        m_iconGlow->stopActionByTag(14);
        m_iconGlow->setScale(1.f);
    }
}

void REPlayerObject::stopRotation_(bool ground, int type) {
    m_isRotating = false;
    m_isBallRotating2 = false;
    m_isBallRotating = false;
    m_rotationSpeed = 0.f;
}

void REPlayerObject::stopStreak2_() {
    m_waveTrail->stopActionByTag(8);
    m_waveTrail->setOpacity(0);
    m_waveTrail->stopStroke();
}

void REPlayerObject::storeCollision_(PlayerCollisionDirection direction, int id) {
    switch (direction) {
        case PlayerCollisionDirection::Top: {
            if (m_lastCollisionTop != id) {
                m_lastCollisionTop = id;
                if (m_collisionLogTop) {
                    m_collisionLogTop->setObject(m_maybeLastGroundObject, id);
                }
            }
            break;
        }
        case PlayerCollisionDirection::Bottom: {
            if (m_lastCollisionBottom != id) {
                m_lastCollisionBottom = id;
                if (m_collisionLogBottom) {
                    m_collisionLogBottom->setObject(m_maybeLastGroundObject, id);
                }
            }
            break;
        }
        case PlayerCollisionDirection::Left: {
            if (m_lastCollisionLeft != id) {
                m_lastCollisionLeft = id;
                if (m_collisionLogLeft) {
                    m_collisionLogLeft->setObject(m_maybeLastGroundObject, id);
                }
            }
            break;
        }
        case PlayerCollisionDirection::Right: {
            if (m_lastCollisionRight != id) {
                m_lastCollisionRight = id;
                if (m_collisionLogRight) {
                    m_collisionLogRight->setObject(m_maybeLastGroundObject, id);
                }
            }
            break;
        }
    }
}

bool REPlayerObject::switchedDirTo(PlayerButton button) {
    if (!m_inputsLocked) {
        m_holdingButtons[static_cast<int>(PlayerButton::Right)] = button == PlayerButton::Right;
        m_holdingButtons[static_cast<int>(PlayerButton::Left)] = button == PlayerButton::Left;
    }

    if (m_controlsDisabled) return false;

    if (button == PlayerButton::Right) {
        m_holdingRight = true;
        m_leftPressedFirst = false;

        if (m_isSliding && m_isSlidingRight) {
            m_isSliding = false;
            m_maybeSlopeForce = 0.0;
        }

        if (m_platformerXVelocity < 0.0) {
            m_isMoving = false;
        }
    }
    else if (button == PlayerButton::Left){
        m_holdingLeft = true;
        m_leftPressedFirst = true;

        if (m_isSliding && !m_isSlidingRight) {
            m_isSliding = false;
            m_maybeSlopeForce = 0.0;
        }

        if (m_platformerXVelocity > 0.0) {
            m_isMoving = false;
        }
    }

    return true;
}

void REPlayerObject::switchedToMode(GameObjectType type) {
    if (type != GameObjectType::ShipPortal) toggleFlyMode(false, false);
    if (type != GameObjectType::UfoPortal) toggleBirdMode(false, false);
    if (type != GameObjectType::BallPortal) toggleRollMode(false, false);
    if (type != GameObjectType::WavePortal) toggleDartMode(false, false);
    if (type != GameObjectType::RobotPortal) toggleRobotMode(false, false);
    if (type != GameObjectType::SpiderPortal) toggleSpiderMode(false, false);
    if (type != GameObjectType::SwingPortal) toggleSwingMode(false, false);
}

bool REPlayerObject::testForMoving_(float dt, GameObject* object) {
    auto lastPosition = object->getLastPosition();
    auto realPosition = object->getRealPosition();

    if (realPosition.x == lastPosition.x) {
        if (realPosition.y == lastPosition.y) {
            return false;
        }
        if (m_isUpsideDown) {
            if (realPosition.y < lastPosition.y) {
                return false;
            }
        }
        else {
            if (realPosition.y > lastPosition.y) {
                return false;
            }
        }
    }

    auto objectRect = object->getObjectRect();
    objectRect.size.height += dt * 5.f;

    if (m_isUpsideDown) {
        objectRect.origin.y -= dt * 5.f;
    }

    if (getObjectRect().intersectsRect(objectRect)) {
        m_collidedObject = object;
        return true;
    }
    else {
        return false;
    }
}

void REPlayerObject::toggleBirdMode(bool enable, bool noEffects) {
    if (m_isBird == enable) return;

    m_gameModeChangedTime = m_totalTime;
    m_isBird = enable;

    if (enable) {
        switchedToMode(GameObjectType::UfoPortal);
    }

    stopRotation_(false, 0);

    m_yVelocity *= 0.5;
    setRotation(0.f);

    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = false;

    removePendingCheckpoint();

    if (!m_isBird) {
        resetPlayerIcon();
        return;
    }

    updatePlayerBirdFrame(GameManager::get()->m_playerBird);

    stopPlatformerJumpAnimation();

    m_iconSprite->setScale(0.55f);
    m_iconSprite->setPosition({0.f, 5.f});

    m_vehicleSprite->setVisible(true);
    m_vehicleSprite->setPosition({0.f, -7.f});

    updatePlayerGlow();

    auto trailColor = ccColor4F{m_playerColor2.r / 255.f, m_playerColor2.g / 255.f, m_playerColor2.b / 255.f, 1.f};

    m_trailingParticles->setStartColor(trailColor);
    m_trailingParticles->setEndColor(trailColor);

    if (!m_isHidden) {
        m_trailingParticles->resetSystem();
    }

    deactivateParticle_();

    if (!noEffects) {
        spawnPortalCircle({255, 200, 0}, 50.f);
    }

    activateStreak();
    updatePlayerScale();

    m_birdVehicle->setVisible(true);

    if (enable) {
        modeDidChange_();
    }}

void REPlayerObject::toggleDartMode(bool enable, bool noEffects) {
    if (m_isDart == enable) return;
    
    m_gameModeChangedTime = m_totalTime;
    m_isDart = enable;

    if (enable) {
        switchedToMode(GameObjectType::WavePortal);
    }

    stopRotation_(false, 0);

    m_yVelocity *= 0.5;
    setRotation(0.f);

    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = false;

    removePendingCheckpoint();

    float streakScale = 1.f;

    if (!m_isDart) {
        int frame = 0;
        if (m_vehicleSize == 1.f || !m_defaultMiniIcon) {
            frame = m_maybeSavedPlayerFrame;
        }

        updatePlayerFrame(frame);
        resetPlayerIcon();
    }
    else {
        m_unkAngle1 = 20.f;
        m_width = 10.f;
        m_height = 10.f;

        auto gameManager = GameManager::get();
        updatePlayerDartFrame(gameManager->m_playerDart);

        if (!noEffects) {
            spawnPortalCircle({255, 200, 0}, 50.f);
        }

        activateStreak();
        updatePlayerScale();
        deactivateParticle_();

        m_regularTrail->setColor(m_maybeIsVehicleGlowing ? ccColor3B{168, 155, 41} : ccColor3B{172, 155, 41});

        m_waveTrail->reset();
        placeStreakPoint();

        if (m_playEffects && !m_maybeReducedEffects) {
            auto playLayer = GameManager::get()->m_playLayer;

            if (!playLayer->m_skipArtReload && !noEffects) {
                auto wave = CCCircleWave::create(10.f, 60.f, 0.4f, false, true);
                wave->m_color = m_playerColor1;
                wave->setPosition(m_lastPortalPos);
                wave->m_circleMode = CircleMode::Outline;
                wave->m_lineWidth = 4;

                m_parentLayer->addChild(wave, 0);
            }
        }

        streakScale = 0.8f;
    }

    m_regularTrail->setStroke(m_streakStrokeWidth * m_vehicleSize * streakScale);

    if (enable) {
        modeDidChange_();
    }
}

void REPlayerObject::toggleFlyMode(bool enable, bool noEffects) {
    if (m_isShip == enable) return;

    m_gameModeChangedTime = m_totalTime;
    m_isShip = enable;

    if (enable) {
        switchedToMode(GameObjectType::ShipPortal);
    }

    stopRotation_(false, 0);
    m_yVelocity *= 0.5;
    setRotation(0.f);

    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = false;

    removePendingCheckpoint();

    if (!m_isShip) {
        resetPlayerIcon();
        return;
    }

    stopPlatformerJumpAnimation();

    float vehicleOffsetY = 0.f;
    auto gameManager = GameManager::get();

    if (!m_isPlatformer) {
        updatePlayerShipFrame(gameManager->m_playerShip);

        m_iconSprite->setScale(0.55f);
        m_iconSprite->setPosition({0.f, 5.f});
        vehicleOffsetY = -5.f;
    }
    else {
        updatePlayerJetpackFrame(gameManager->m_playerJetpack);

        m_iconSprite->setScale(0.6f);
        m_iconSprite->setPosition({6.f, 4.f});
        vehicleOffsetY = 0.f;
    }

    m_vehicleSprite->setPosition({0.f, vehicleOffsetY});
    m_vehicleSprite->setVisible(true);

    updatePlayerGlow();

    if (!m_isHidden) {
        m_trailingParticles->resetSystem();
        m_shipClickParticles->resetSystem();
    }

    m_shipClickParticles->stopSystem();
    m_hasShipParticles = false;

    deactivateParticle_();

    if (!noEffects) {
        spawnPortalCircle({255, 0, 255}, 50.f);
    }

    activateStreak();
    updatePlayerScale();

    if (m_shipStreak && !m_isPlatformer && !m_isHidden) {
        m_shipStreak->setVisible(true);
    }

    if (enable) {
        modeDidChange_();
    }
}

void REPlayerObject::toggleGhostEffect(GhostType type) {
    if (m_ghostType == type) return;
    
    m_ghostType = type;

    if (m_ghostTrail) {
        m_ghostTrail->stopTrail();
        m_ghostTrail = nullptr;
    }

    if (m_ghostType != GhostType::Enabled) {
        return;
    }

    auto ghostTrail = GhostTrailEffect::create();
    m_ghostTrail = ghostTrail;

    ghostTrail->m_playerObject = this;
    ghostTrail->m_opacity = 200.f;

    auto iconColor = m_iconSprite->getColor();
    bool isBlack = iconColor.r == 0 && iconColor.g == 0 && iconColor.b == 0;

    if (isBlack) {
        ghostTrail->m_color = ccColor3B{0, 0, 0};
    }
    else {
        ghostTrail->doBlendAdditive();
        ghostTrail->m_color = m_playerColor1;
    }

    ghostTrail->runWithTarget(m_iconSprite, 0.05f, 0.4f, 0, 0.6f, false);

    auto objectLayer = GameManager::get()->m_playLayer->m_objectLayer;

    ghostTrail->m_objectLayer = objectLayer;
    objectLayer->addChild(ghostTrail);
}

void REPlayerObject::togglePlatformerMode_(bool val) {
    m_isPlatformer = val;
}

void REPlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    float oldSize = m_vehicleSize;

    if ((oldSize == 1.f && !enable) || (oldSize != 1.f && enable)) return;

    if (enable) {
        m_vehicleSize = 0.6f;

        if (m_defaultMiniIcon) {
            if (m_isBall) {
                updatePlayerRollFrame(0);
            }
            else if (!m_isRobot && !m_isDart) {
                updatePlayerFrame(0);
            }
        }
    }
    else {
        if (m_isPlatformer) {
            m_stateScale = 2;
        }

        m_vehicleSize = 1.f;

        if (m_defaultMiniIcon) {
            if (m_isBall) {
                updatePlayerRollFrame(GameManager::get()->m_playerBall);
            }
            else if (!m_isRobot && !m_isDart) {
                updatePlayerFrame(m_maybeSavedPlayerFrame);
            }
        }
    }

    m_spriteWidthScale = m_vehicleSize;
    m_spriteHeightScale = m_vehicleSize;

    m_landParticles0->loadScaledDefaults(m_vehicleSize);
    m_landParticles1->loadScaledDefaults(m_vehicleSize);
    m_playerGroundParticles->loadScaledDefaults(m_vehicleSize);
    m_vehicleGroundParticles->loadScaledDefaults(m_vehicleSize);
    m_trailingParticles->loadScaledDefaults(m_vehicleSize);
    m_shipClickParticles->loadScaledDefaults(m_vehicleSize);
    m_ufoClickParticles->loadScaledDefaults(m_vehicleSize);
    m_robotBurstParticles->loadScaledDefaults(m_vehicleSize);
    m_dashParticles->loadScaledDefaults(m_vehicleSize);
    m_swingBurstParticles1->loadScaledDefaults(m_vehicleSize);
    m_swingBurstParticles2->loadScaledDefaults(m_vehicleSize);

    float streakScale = m_isDart ? 0.8f : 1.f;
    m_regularTrail->setStroke(m_streakStrokeWidth * m_vehicleSize * streakScale);

    if (m_playEffects || m_isDart) {
        m_waveTrail->m_waveSize = m_vehicleSize;
    }

    if (m_ghostTrail) {
        m_ghostTrail->m_playerScale = m_vehicleSize;
    }

    if (m_shipStreak) {
        updateStreakSettings__(m_shipStreak, m_shipStreakType, this);
    }

    m_actionManager->stopInternalAction(6);

    float size = m_vehicleSize;
    bool shouldScale = true;

    if (m_playEffects && !m_maybeReducedEffects) {
        auto gameManager = GameManager::get();

        if (!gameManager->m_playLayer->m_skipArtReload) {
            auto scale = CCScaleTo::create(0.5f, size, size);
            auto ease = CCEaseElasticOut::create(scale);
            ease->setTag(6);

            m_actionManager->m_internalActions->setObject(ease, ease->getTag());
            ease->startWithTarget(this);

            auto effectColor = m_vehicleSize == 1.f ? ccColor3B{0, 255, 150} : ccColor3B{255, 0, 150};

            if (!noEffects) {
                auto portalPos = m_lastPortalPos;
                auto winSize = CCDirector::get()->getWinSize();
                float zoom = gameManager->m_playLayer->m_objectLayer->getScale();

                float minX = portalPos.x - 50.f;
                float maxX = portalPos.x + 50.f;
                float randomX = minX + (static_cast<float>(rand()) / 32767.f) * (maxX - minX);

                auto from = CCPoint{randomX, (winSize.height + 50.f) / zoom + gameManager->m_playLayer->m_gameState.m_cameraPosition.y};

                gameManager->m_playLayer->lightningFlash(from, portalPos, effectColor, 5.f, 0.5f, 0, true, 1.f);

                spawnPortalCircle(effectColor, 45.f);
                spawnScaleCircle();
            }

            shouldScale = false;
        }
    }

    if (shouldScale) {
        setScaleX(size);
        setScaleY(size);
    }

    if (m_isBall && m_isRotating) {
        runRotateAction_(false, 0);
    }

    placeStreakPoint();
    updateRobotAnimationSpeed();
}

void updateStreakSettings__(CCMotionStreak* streak, ShipStreak streakType, PlayerObject* player) {
    float fade = 0.f;
    float stroke = 0.f;

    getSettingsForStreak__(streakType, player->m_playerSpeed, player->m_vehicleSize, fade, stroke);

    streak->updateFade(fade);
    streak->setStroke(stroke);
}

void getSettingsForStreak__(ShipStreak type, float speed, float scale, float& outA, float& outB) {
    float scaleMod = scale == 1.f ? 1.f : 0.5f;
    float a = 0.f;
    float b = 0.f;

    if (type == ShipStreak::ShipFire2) {
        if (speed == 0.7f) {
            b = scaleMod * 14.f;
            a = scaleMod * 0.12f;
        }
        else if (speed == 0.9f) {
            b = scaleMod * 16.f;
            a = scaleMod * 0.115f;
        }
        else if (speed == 1.1f) {
            b = scaleMod * 18.f;
            a = scaleMod * 0.11f;
        }
        else if (speed == 1.3f) {
            b = scaleMod * 20.f;
            a = scaleMod * 0.108f;
        }
        else if (speed == 1.6f) {
            b = scaleMod * 22.f;
            a = scaleMod * 0.106f;
        }

        a *= 0.6f;
    }
    else if (type == ShipStreak::ShipFire3) {
        if (speed == 0.7f) {
            b = scaleMod * 12.f * 1.3f;
            a = scaleMod * 0.15f * 0.9f;
        }
        else if (speed == 0.9f) {
            b = scaleMod * 14.f * 1.3f;
            a = scaleMod * 0.148f * 0.9f;
        }
        else if (speed == 1.1f) {
            b = scaleMod * 16.f * 1.3f;
            a = scaleMod * 0.146f * 0.9f;
        }
        else if (speed == 1.3f) {
            b = scaleMod * 18.f * 1.3f;
            a = scaleMod * 0.144f * 0.9f;
        }
        else if (speed == 1.6f) {
            b = scaleMod * 22.f * 1.3f;
            a = scaleMod * 0.142f * 0.9f;
        }
    }
    else if (type == ShipStreak::ShipFire4) {
        if (speed == 0.7f) {
            b = scaleMod * 12.f * 1.3f;
            a = scaleMod * 0.145f * 0.7f;
        }
        else if (speed == 0.9f) {
            b = scaleMod * 14.f * 1.3f;
            a = scaleMod * 0.152f * 0.7f;
        }
        else if (speed == 1.1f) {
            b = scaleMod * 16.f * 1.3f;
            a = scaleMod * 0.155f * 0.7f;
        }
        else if (speed == 1.3f) {
            b = scaleMod * 18.f * 1.3f;
            a = scaleMod * 0.151f * 0.7f;
        }
        else if (speed == 1.6f) {
            b = scaleMod * 22.f * 1.3f;
            a = scaleMod * 0.15f * 0.7f;
        }
    }
    else if (type == ShipStreak::ShipFire5) {
        if (speed == 0.7f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 12.f * 1.1f;
        }
        else if (speed == 0.9f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 14.f * 1.1f;
        }
        else if (speed == 1.1f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 15.f * 1.1f;
        }
        else if (speed == 1.3f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 16.f * 1.1f;
        }
        else if (speed == 1.6f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 17.f * 1.1f;
        }

        a *= 0.6f;
    }
    else if (type == ShipStreak::ShipFire6) {
        if (speed == 0.7f) {
            a = scaleMod * 0.15f;
            b = scaleMod * 12.f * 1.5f;
        }
        else if (speed == 0.9f) {
            a = scaleMod * 0.151f;
            b = scaleMod * 14.f * 1.5f;
        }
        else if (speed == 1.1f) {
            a = scaleMod * 0.152f;
            b = scaleMod * 15.f * 1.5f;
        }
        else if (speed == 1.3f) {
            a = scaleMod * 0.155f;
            b = scaleMod * 16.f * 1.5f;
        }
        else if (speed == 1.6f) {
            a = scaleMod * 0.16f;
            b = scaleMod * 18.f * 1.5f;
        }

        a *= 0.6f;
    }

    outA = a;
    outB = b;

    if (speed == 0.7f) {
        outA *= 1.3f;
        outB *= 1.3f;
    }
    else if (speed == 0.9f) {
        outA *= 1.2f;
        outB *= 1.2f;
    }
    else if (speed == 1.1f) {
        outA *= 1.1f;
        outB *= 1.1f;
    }
    else if (speed == 1.3f) {
        outA *= 1.05f;
        outB *= 1.05f;
    }
}

void REPlayerObject::toggleRobotMode(bool enable, bool noEffects) {
    if (m_isRobot == enable) return;
    
    m_gameModeChangedTime = m_totalTime;
    m_isRobot = enable;

    if (enable) {
        m_mainLayer->addChild(m_robotBatchNode, 2);
        switchedToMode(GameObjectType::RobotPortal);

        m_accelerationOrSpeed = 1.5f;
        stopRotation_(false, 0);
        setRotation(0.f);

        auto gameManager = GameManager::get();
        int frame = gameManager->m_playerRobot;

        if (frame > 68) {
            frame = 68;
        }
        if (frame < 1) {
            frame = 1;
        }

        auto frameName = CCString::createWithFormat("robot_%02d_01_001.png", frame)->getCString();
        auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        m_iconSprite->setDisplayFrame(spriteFrame);

        if (!m_isPlatformer || m_platformerXVelocity != 0.0) {
            m_robotSprite->runAnimation(m_currentRobotAnimation);
        }
        else {
            m_robotSprite->runAnimation("idle01");
        }

        m_iconSprite->setVisible(false);

        if (!noEffects) {
            spawnPortalCircle({255, 50, 50}, 50.f);
        }

        updatePlayerScale();
    }
    else {
        m_mainLayer->removeChild(m_robotBatchNode, false);
        m_iconSprite->setVisible(true);

        auto manager = m_robotSprite->m_animationManager;
        manager->overridePrio();

        if (auto paSprite = manager->m_sprite->m_paSprite) {
            paSprite->stopAllActions();
        }

        if (auto fbfSprite = manager->m_sprite->m_fbfSprite) {
            fbfSprite->stopAllActions();
        }

        int frame = (m_vehicleSize == 1.f || !m_defaultMiniIcon) ? m_maybeSavedPlayerFrame : 0;
        updatePlayerFrame(frame);
    }

    updatePlayerGlow();
    stopRotation_(false, 0);

    if (enable) {
        modeDidChange_();
    }
}

void REPlayerObject::toggleRollMode(bool enable, bool noEffects) {
    if (m_isBall == enable) return;
    
    m_gameModeChangedTime = m_totalTime;
    m_isBall = enable;

    if (enable) {
        switchedToMode(GameObjectType::BallPortal);
    }

    float vehicleSize = m_vehicleSize;

    if (!m_isBall) {
        int frame = (vehicleSize == 1.f || !m_defaultMiniIcon) ? m_maybeSavedPlayerFrame : 0;
        updatePlayerFrame(frame);

        setRotation(m_isUpsideDown ? -180.f : 0.f);
    }
    else {
        int frame = 0;

        if (vehicleSize == 1.f || !m_defaultMiniIcon) {
            auto gameManager = GameManager::get();
            frame = gameManager->m_playerBall;
        }

        updatePlayerRollFrame(frame);

        if (!noEffects) {
            spawnPortalCircle({255, 50, 50}, 50.f);
        }
    }

    stopRotation_(false, 0);

    if (enable) {
        modeDidChange_();
    }
}

void REPlayerObject::toggleSpiderMode(bool enable, bool noEffects) {
    if (m_isSpider == enable) return;
    
    m_gameModeChangedTime = m_totalTime;
    m_isSpider = enable;

    if (enable) {
        m_mainLayer->addChild(m_spiderBatchNode, 2);
        switchedToMode(GameObjectType::SpiderPortal);

        m_unkAngle1 = 27.f;
        m_width = 27.f;
        m_height = 27.f;
        m_accelerationOrSpeed = 1.5f;

        stopRotation_(false, 0);
        setRotation(0.f);

        auto gameManager = GameManager::get();
        int frame = gameManager->m_playerSpider;

        if (frame > 69) {
            frame = 69;
        }
        if (frame < 1) {
            frame = 1;
        }

        auto frameName = CCString::createWithFormat("spider_%02d_01_001.png", frame)->getCString();
        auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        m_iconSprite->setDisplayFrame(spriteFrame);

        if (!m_isPlatformer || m_platformerXVelocity != 0.0) {
            playDynamicSpiderRun();
        }
        else {
            m_spiderSprite->runAnimation("idle01");
        }

        m_iconSprite->setVisible(false);

        if (!noEffects) {
            spawnPortalCircle({255, 50, 50}, 50.f);
        }

        updatePlayerScale();
        updatePlayerGlow();

        stopRotation_(false, 0);
        modeDidChange_();
    }
    else {
        m_mainLayer->removeChild(m_spiderBatchNode, false);
        m_iconSprite->setVisible(true);

        auto manager = m_spiderSprite->m_animationManager;
        manager->overridePrio();

        if (auto paSprite = manager->m_sprite->m_paSprite) {
            paSprite->stopAllActions();
        }

        if (auto fbfSprite = manager->m_sprite->m_fbfSprite) {
            fbfSprite->stopAllActions();
        }

        int frame = (m_vehicleSize == 1.f || !m_defaultMiniIcon) ? m_maybeSavedPlayerFrame : 0;
        updatePlayerFrame(frame);

        resetPlayerIcon();
        updatePlayerGlow();

        stopRotation_(false, 0);
    }
}

void REPlayerObject::toggleSwingMode(bool enable, bool noEffects) {
    if (m_isSwing == enable) return;
    
    m_gameModeChangedTime = m_totalTime;
    m_isSwing = enable;

    if (enable) {
        switchedToMode(GameObjectType::SwingPortal);
    }

    stopRotation_(false, 0);
    m_yVelocity *= 0.5;
    setRotation(0.f);

    m_isOnGround2 = false;
    m_isOnGround = false;
    m_shouldTryPlacingCheckpoint = false;

    removePendingCheckpoint();

    if (!m_isSwing) {
        resetPlayerIcon();
        updatePlayerFrame(m_maybeSavedPlayerFrame);
        setRotation(m_isUpsideDown ? -180.f : 0.f);
        disableSwingFire();
    }
    else {
        updatePlayerSwingFrame(GameManager::get()->m_playerSwing);

        if (!noEffects) {
            spawnPortalCircle({255, 200, 0}, 50.f);
        }

        if (!m_isHidden) {
            m_trailingParticles->resetSystem();
            m_shipClickParticles->resetSystem();
        }

        m_shipClickParticles->stopSystem();
        m_hasShipParticles = false;

        deactivateParticle_();
        activateStreak();

        m_swingFireMiddle->setVisible(true);
        m_swingFireMiddle->loopFireAnimation();

        m_swingFireBottom->setVisible(true);
        m_swingFireTop->setVisible(true);

        m_swingFireBottom->update(0.01f);
        m_swingFireTop->update(0.01f);

        m_swingBurstParticles1->resetSystem();
        m_swingBurstParticles2->resetSystem();
        m_swingBurstParticles1->stopSystem();
        m_swingBurstParticles2->stopSystem();

        updateSwingFire();
    }

    if (enable) {
        modeDidChange_();
    }
}

void REPlayerObject::toggleVisibility(bool visible) {
    bool hidden = !visible;
    if (m_isHidden == hidden) return;

    m_isHidden = hidden;
    setVisible(visible);

    if (!visible) {
        stopActionByTag(11);

        deactivateStreak_(true);

        if (m_shipStreak) {
            m_shipStreak->setVisible(false);
        }

        resetAllParticles();
        return;
    }

    if (isFlying_()) {
        resetStreak();
        activateStreak();
        m_trailingParticles->resetSystem();

        if (m_isDart) {
            placeStreakPoint();
            return;
        }

        if (m_isShip && !m_isPlatformer && m_shipStreak) {
            m_shipStreak->setVisible(true);
        }
    }
    else if (m_alwaysShowStreak) {
        resetStreak();
        activateStreak();
    }
}

void REPlayerObject::touchedObject_(GameObject* object) {
    switch (object->m_objectID) {
        case 1755: {
            m_stateDartSlide = 2;
            break;
        }
        case 1813: {
            m_stateNoAutoJump = 2;
            break;
        }
        case 1829: {
            if (m_isDashing) {
                stopDashing();
                m_jumpBuffered = false;
            }
        }
        case 1859: {
            m_stateHitHead = 2;
            break;
        }
        case 2069: case 3845: {
            m_stateForce = 2;
            auto forceObject = static_cast<ForceBlockGameObject*>(object);
            auto forceID = forceObject->m_forceID;

            if (forceID > 0) {
                if (m_jumpPadRelated[forceID]) {
                    break;
                }
                m_jumpPadRelated[forceID] = true;
            }

            m_stateForceVector = m_stateForceVector + forceObject->calculateForceToTarget(this);
            auto magnitude = sqrtf(m_stateForceVector.x * m_stateForceVector.x + m_stateForceVector.y * m_stateForceVector.y);

            if (magnitude > 9999.f) {
                m_stateForceVector = m_stateForceVector * (9999.f / magnitude);
            }
            break;
        }
        case 2866: {
            m_stateFlipGravity = 2;
            break;
        }
    }
}

void REPlayerObject::tryPlaceCheckpoint() {
    //todo
    PlayerObject::tryPlaceCheckpoint();
}

void REPlayerObject::unrotateGameplayObject(GameObject* object) {
    auto it = m_rotateObjectsRelated.find(object->m_uniqueID);
    if (it == m_rotateObjectsRelated.end()) return;
    
    object->setObjectRectDirty(true);

    auto delta = it->second;
    object->addToTempOffset(-delta.m_x, -delta.m_y);
    object->setLastPosition(object->m_lastPosition + CCPoint{static_cast<float>(-delta.m_x), static_cast<float>(-delta.m_y)});

    object->m_startRotationX += 90.f;
    object->m_startRotationY += 90.f;

    object->setRRotation(0.f);

    if (object->getType() == GameObjectType::Slope) {
        object->determineSlopeDirection();
    }

    m_rotateObjectsRelated.erase(it);
}

void REPlayerObject::unrotatePreSlopeObjects_() {
    for (auto& pair : m_potentialSlopeMap) {
        unrotateGameplayObject(pair.second);
    }
}

void REPlayerObject::updateCheckpointMode_(bool enable) {
    m_quickCheckpointMode = enable;
}

void REPlayerObject::updateCheckpointTest_() {
    if (m_canPlaceCheckpoint) {
        tryPlaceCheckpoint();
        m_canPlaceCheckpoint = false;
    }
    if (isFlying() && m_uniqueID == 1) {
        if (m_onFlyCheckpointTries + 1 < 20) {
            m_onFlyCheckpointTries++;
        }
        else {
            m_onFlyCheckpointTries = 0;
            tryPlaceCheckpoint();
        }
    }
    if (m_checkpointTimeout) {
        auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
        if (currentTime - m_lastCheckpointTime > 0.1f) m_checkpointTimeout = false;
    }
}

void REPlayerObject::updateCollide(PlayerCollisionDirection direction, GameObject* object) {
    auto rect = getObjectRect();
    float halfHeight = rect.size.height * 0.5f;
    float halfWidth = rect.size.width * 0.5f;
    auto position = getPosition();

    if (direction == PlayerCollisionDirection::Left) {
        updateCollideLeft(position.x - halfWidth, object);
        return;
    }

    if (direction == PlayerCollisionDirection::Right) {
        updateCollideRight(position.x + halfWidth, object);
        return;
    }

    if (direction == PlayerCollisionDirection::Top) {
        updateCollideTop_(position.y + halfHeight, object);
        return;
    }

    if (direction == PlayerCollisionDirection::Bottom) {
        updateCollideBottom_(position.y + halfHeight, object);
        return;
    }
}

void REPlayerObject::updateCollideBottom_(float y, GameObject* object) {
    int id = object ? object->m_uniqueID : 0;

    double bestY = y;

    if (m_collidedBottomMaxY != 0.0) {
        if (!m_isUpsideDown) {
            if (y < m_collidedBottomMaxY) {
                bestY = y;
            }
        }
        else {
            if (m_collidedBottomMaxY < y) {
                bestY = y;
            }
        }
    }

    m_collidedBottomMaxY = bestY;

    if (id != 0) {
        storeCollision_(PlayerCollisionDirection::Bottom, id);
    }
}

void REPlayerObject::updateCollideLeft(float x, GameObject* object) {
    int id = object ? object->m_uniqueID : 0;

    double bestX = x;

    if (m_collidedLeftMaxX != 0.0 && m_collidedLeftMaxX <= x) {
        bestX = m_collidedLeftMaxX;
    }

    m_collidedLeftMaxX = bestX;

    if (id != 0) {
        storeCollision_(PlayerCollisionDirection::Left, id);
        m_collidingWithLeft = object;
        m_collidingWithRight = nullptr;
    }
}

void REPlayerObject::updateCollideRight(float x, GameObject* object) {
    int id = object ? object->m_uniqueID : 0;

    double bestX = x;

    if (m_collidedRightMinX != 0.0 && x <= m_collidedRightMinX) {
        bestX = m_collidedRightMinX;
    }

    m_collidedRightMinX = bestX;

    if (id != 0) {
        storeCollision_(PlayerCollisionDirection::Left, id);
        m_collidingWithRight = object;
        m_collidingWithLeft = nullptr;
    }
}

void REPlayerObject::updateCollideTop_(float y, GameObject* object) {
    int id = object ? object->m_uniqueID : 0;

    double bestY = y;

    if (m_collidedTopMinY != 0.0) {
        if (!m_isUpsideDown) {
            if (m_collidedTopMinY < y) {
                bestY = y;
            }
        }
        else {
            if (y < m_collidedTopMinY) {
                bestY = y;
            }
        }
    }

    m_collidedTopMinY = bestY;

    if (id != 0) {
        storeCollision_(PlayerCollisionDirection::Top, id);
    }
}

void REPlayerObject::updateDashAnimation() {
    float rotation = m_dashFireSprite->getRotation();
    float speedScale = (1.f - (std::abs(rotation) / 70.f) * 0.4f) * (0.04f / m_playerSpeed);

    float frameTime = static_cast<float>(m_totalTime / speedScale);
    unsigned int frame = static_cast<unsigned int>(static_cast<int>(std::floor(frameTime)) % 12 + 1);

    if (frame == m_dashFireFrame) return;

    m_dashFireFrame = frame;

    auto children = m_dashFireSprite->getChildren();
    auto outlineSprite = children ? children->objectAtIndex(0) : nullptr;

    auto frameName = CCString::createWithFormat("playerDash2_%03d.png", frame)->getCString();
    auto outlineName = CCString::createWithFormat("playerDash2_outline_%03d.png", frame)->getCString();

    if (!frameName) return;
    
    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    auto dashFrame = cache->spriteFrameByName(frameName);
    m_dashFireSprite->setDisplayFrame(dashFrame);

    if (outlineSprite) {
        auto outlineFrame = cache->spriteFrameByName(outlineName);
        static_cast<CCSprite*>(outlineSprite)->setDisplayFrame(outlineFrame);

        auto size = m_dashFireSprite->getContentSize();
        static_cast<CCSprite*>(outlineSprite)->setPosition(CCPoint{size.width * 0.5f, size.height * 0.5f});
    }
}

void REPlayerObject::updateDashArt() {
    if (!m_isDashing) return;
    
    m_iconSprite->stopActionByTag(12);
    m_iconGlow->stopActionByTag(12);

    m_iconSprite->setRotation(0.f);
    m_iconGlow->setRotation(0.f);

    float rotation = (m_isSideways ? -90.f : 0.f) - m_dashAngle;
    if (m_isGoingLeft) {
        rotation += 180.f;
    }

    setRotation(rotation);

    m_dashFireSprite->stopAllActions();

    if (m_isShip) {
        m_dashFireSprite->setPosition({0.f, -2.f});
        m_dashFireSprite->setScaleX(1.f);
        m_dashFireSprite->setScaleY(0.85f);
    }
    else if (m_isBird) {
        m_dashFireSprite->setPosition({0.f, 0.f});
        m_dashFireSprite->setScaleX(0.95f);
        m_dashFireSprite->setScaleY(0.9f);
    }
    else if (m_isRobot) {
        m_dashFireSprite->setPosition({1.f, 0.f});
        m_dashFireSprite->setScaleX(0.9f);
        m_dashFireSprite->setScaleY(0.85f);
        m_robotSprite->runAnimation("fall_loop");
    }
    else if (m_isSpider) {
        m_dashFireSprite->setPosition({1.f, 0.f});
        m_dashFireSprite->setScaleX(0.95f);
        m_dashFireSprite->setScaleY(0.8f);
        m_spiderSprite->runAnimation("fall_loop");
    }
    else {
        m_dashFireSprite->setPosition({0.f, 0.f});
        m_dashFireSprite->setScaleX(0.9f);
        m_dashFireSprite->setScaleY(0.8f);
    }

    if (!isInNormalMode_() && !m_isBall) return;
    
    m_iconSprite->setScale(0.9f);
    m_iconGlow->setScale(0.9f);

    float direction = (m_isSideways != m_isUpsideDown) ? -1.f : 1.f;
    float rotateBy = direction * 288.f * m_gravityMod;

    if (m_isPlatformer) {
        auto dashVec = CCPoint{static_cast<float>(m_dashX), static_cast<float>(m_dashY)};

        float magnitude = std::sqrt(dashVec.x * dashVec.x + dashVec.y * dashVec.y);
        float speedScale = magnitude <= 17.310005f ? (magnitude / 17.310005f) * 1.7f + 0.3f : 2.f;

        rotateBy *= speedScale;
    }

    auto iconRotate = CCRepeatForever::create(CCRotateBy::create(0.3f, rotateBy));
    iconRotate->setTag(12);
    m_iconSprite->runAction(iconRotate);

    auto glowRotate = CCRepeatForever::create(CCRotateBy::create(0.3f, rotateBy));
    glowRotate->setTag(12);
    m_iconGlow->runAction(glowRotate);
}

void REPlayerObject::updateEffects_(float param) {
    m_waveTrail->updateStroke(param);
}

void REPlayerObject::updateGlowColor() {
    //todo
    PlayerObject::updateGlowColor();
}

void REPlayerObject::updateInternalActions(float dt) {
    auto internalActions = m_actionManager->m_internalActions;
    auto keys = internalActions->allKeys();

    if (!keys) return;

    for (auto key : CCArrayExt<CCInteger, false>(keys)) {
        if (!key) continue;
        
        auto action = static_cast<CCAction*>(internalActions->objectForKey(key->getValue()));
        if (!action) continue;

        if (!action->isDone()) {
            action->step(dt);
        }
        else {
            action->stop();
            internalActions->removeObjectForKey(key->getValue());
        }
    }
}

void REPlayerObject::updateJump(float dt) {
    //todo
    PlayerObject::updateJump(dt);
}

void REPlayerObject::updateJumpVariables_() {
    m_stateJumpBuffered = m_jumpBuffered;
    m_stateRingJump2 = m_stateRingJump;
    m_touchedRing = false;
    m_touchedCustomRing = false;
    m_touchedGravityPortal = false;
    m_maybeTouchedBreakableBlock = false;
}

void REPlayerObject::updateLastGroundObject_(GameObject* object) {
    if (!object) return;

    m_lastGroundObject = object;

    if (m_isDontBoostY) {
        m_stateBoostX = 2;
    }
    if (m_isDontBoostX) {
        m_stateBoostY = 2;
    }
}

void REPlayerObject::updateMove(float dt) {
    //todo
    PlayerObject::updateMove(dt);
}

void REPlayerObject::updatePlayerArt() {
    m_mainLayer->setScaleX(reverseMod_());

    if (!m_isSwing && !m_isBall && !isInNormalMode_()) {
        m_mainLayer->setScaleY((m_isSideways ? -1 : 1) * flipMod_());
    }
    else {
        m_mainLayer->setScaleY(1);
    }

    float baseRotation = m_isSideways ? -90.f : 0.f;

    m_mainLayer->setRotation(baseRotation);

    int flip = flipMod_();
    float angle1;

    if (!m_isSideways) {
        angle1 = static_cast<float>(flip * 90);
    } 
    else {
        angle1 = 0.f;
        if (!m_isGoingLeft) {
            angle1 = 180.f;
        }
    }

    m_playerGroundParticles->setAngle(angle1);
    m_trailingParticles->setAngle(angle1);
    m_shipClickParticles->setAngle(angle1);

    float gravityY = flipMod_() * -300.f;
    auto gravity = CCPoint{0.f, gravityY};

    if (m_isSideways) {
        std::swap(gravity.x, gravity.y);
    }

    m_playerGroundParticles->setGravity(gravity);
    m_trailingParticles->setGravity(gravity);
    m_shipClickParticles->setGravity(gravity);

    auto variance1 = CCPoint{0.f, 2.f};
    if (m_isSideways) {
        std::swap(variance1.x, variance1.y);
    }

    m_trailingParticles->setPosVar(variance1);
    m_shipClickParticles->setPosVar(variance1);

    float angle2;
    if (!m_isSideways) {
        angle2 = flip * 330;
    }
    else if (!m_isGoingLeft) {
        angle2 = 240.f;
    }
    else {
        angle2 = 60.f;
    }

    m_ufoClickParticles->setAngle(angle2);
    m_robotBurstParticles->setAngle(angle2);

    auto variance2 = CCPoint{5.f, 1.f};
    if (m_isSideways) {
        std::swap(variance2.x, variance2.y);
    }

    m_ufoClickParticles->setPosVar(variance2);
    m_robotBurstParticles->setPosVar(variance2);

    auto variance3 = CCPoint{5.f, 1.f};
    if (m_isRobot || m_isSpider) {
        variance3 = CCPoint{15.f, 0.f};
    }

    if (m_isSideways) {
        std::swap(variance3.x, variance3.y);
    }

    m_playerGroundParticles->setPosVar(variance3);
}

void REPlayerObject::updatePlayerBirdFrame(int frame) {
    unsigned int id = frame < 149 ? frame : 149;
    if (id < 1) {
        id = 1;
    }

    GameManager::get()->loadIcon(id, 3, m_iconRequestID);

    auto mainFrameName = CCString::createWithFormat("bird_%02d_001.png", id)->getCString();
    auto secondFrameName = CCString::createWithFormat("bird_%02d_2_001.png", id)->getCString();
    auto detailFrameName = CCString::createWithFormat("bird_%02d_3_001.png", id)->getCString();
    auto glowFrameName = CCString::createWithFormat("bird_%02d_glow_001.png", id)->getCString();

    auto cache = CCSpriteFrameCache::get();

    m_vehicleSprite->setDisplayFrame(cache->spriteFrameByName(mainFrameName));
    m_vehicleSpriteSecondary->setDisplayFrame(cache->spriteFrameByName(secondFrameName));
    m_birdVehicle->setDisplayFrame(cache->spriteFrameByName(detailFrameName));
    m_vehicleGlow->setDisplayFrame(cache->spriteFrameByName(glowFrameName));

    auto size = m_vehicleSprite->getContentSize();
    m_vehicleSpriteSecondary->setPosition({size.width * 0.5f, size.height * 0.5f});
    m_birdVehicle->setPosition(m_vehicleSpriteSecondary->getPosition());

    auto extraFrameName = CCString::createWithFormat("bird_%02d_extra_001.png", id)->getCString();
    updateShipSpriteExtra(extraFrameName);
}

void REPlayerObject::updatePlayerDartFrame(int frame) {
    unsigned int id = frame < 96 ? frame : 96;
    if (id < 1) {
        id = 1;
    }

    GameManager::get()->loadIcon(id, 4, m_iconRequestID);

    auto mainFrameName = CCString::createWithFormat("dart_%02d_001.png", id)->getCString();
    auto secondFrameName = CCString::createWithFormat("dart_%02d_2_001.png", id)->getCString();
    auto glowFrameName = CCString::createWithFormat("dart_%02d_glow_001.png", id)->getCString();

    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    m_iconSprite->setDisplayFrame(cache->spriteFrameByName(mainFrameName));
    m_iconSpriteSecondary->setDisplayFrame(cache->spriteFrameByName(secondFrameName));
    m_iconGlow->setDisplayFrame(cache->spriteFrameByName(glowFrameName));

    auto size = m_iconSprite->getContentSize();
    m_iconSpriteSecondary->setPosition({size.width * 0.5f, size.height * 0.5f});

    auto extraFrameName = CCString::createWithFormat("dart_%02d_extra_001.png", id)->getCString();
    updatePlayerSpriteExtra(extraFrameName);
}

void REPlayerObject::updatePlayerForce_(cocos2d::CCPoint velocity, bool additive) {
    if (additive) {
        m_yVelocity += velocity.y;
        if (m_isPlatformer) {
            m_platformerXVelocity += velocity.x;
            m_affectedByForces = true;
        }
    }
    else {
        m_yVelocity = velocity.y;
        if (m_isPlatformer) {
            m_platformerXVelocity = velocity.x;
            m_affectedByForces = true;
        }
    }
}

void REPlayerObject::updatePlayerFrame(int frame) {
    unsigned int id = frame < 485 ? frame : 485;

    if (id < 1) {
        id = 0;
    }
    else {
        m_maybeSavedPlayerFrame = id;
    }

    GameManager::get()->loadIcon(id, 0, m_iconRequestID);

    auto mainFrameName = CCString::createWithFormat("player_%02d_001.png", id)->getCString();
    auto secondFrameName = CCString::createWithFormat("player_%02d_2_001.png", id)->getCString();
    auto glowFrameName = CCString::createWithFormat("player_%02d_glow_001.png", id)->getCString();

    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    m_iconSprite->setDisplayFrame(cache->spriteFrameByName(mainFrameName));
    m_iconSpriteSecondary->setDisplayFrame(cache->spriteFrameByName(secondFrameName));
    m_iconGlow->setDisplayFrame(cache->spriteFrameByName(glowFrameName));

    auto size = m_iconSprite->getContentSize();
    m_iconSpriteSecondary->setPosition({size.width * 0.5f, size.height * 0.5f});

    auto extraFrameName = CCString::createWithFormat("player_%02d_extra_001.png", id)->getCString();
    updatePlayerSpriteExtra(extraFrameName);
}

void REPlayerObject::updatePlayerGlow() {
    //todo
    PlayerObject::updatePlayerGlow();
}

void REPlayerObject::updatePlayerJetpackFrame(int frame) {
    //todo
    PlayerObject::updatePlayerJetpackFrame(frame);
}

void REPlayerObject::updatePlayerRobotFrame_(int frame) {
    createRobot(std::clamp(frame, 1, 68));
}

void REPlayerObject::updatePlayerRollFrame(int frame) {
    //todo
    PlayerObject::updatePlayerRollFrame(frame);
}

void REPlayerObject::updatePlayerScale() {
    //todo
    PlayerObject::updatePlayerScale();
}

void REPlayerObject::updatePlayerShipFrame(int frame) {
    //todo
    PlayerObject::updatePlayerShipFrame(frame);
}

void REPlayerObject::updatePlayerSpiderFrame_(int frame) {
    createSpider(std::clamp(frame, 1, 69));
}

void REPlayerObject::updatePlayerSpriteExtra(gd::string frameName) {
    //todo
    PlayerObject::updatePlayerSpriteExtra(frameName);
}

void REPlayerObject::updatePlayerSwingFrame(int frame) {
    //todo
    PlayerObject::updatePlayerSwingFrame(frame);
}

void REPlayerObject::updateRobotAnimationSpeed() {
    //todo
    PlayerObject::updateRobotAnimationSpeed();
}

void REPlayerObject::updateRotation(float dt) {
    //todo
    PlayerObject::updateRotation(dt);
}

void REPlayerObject::updateRotation(float dt, float rotation) {
    //todo
    PlayerObject::updateRotation(dt, rotation);
}

void REPlayerObject::updateShipRotation(float dt) {
    //todo
    PlayerObject::updateShipRotation(dt);
}

void REPlayerObject::updateShipSpriteExtra(gd::string frameName) {
    auto spriteFrame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str());
    if (!spriteFrame) {
        m_vehicleSpriteWhitener->setVisible(false);
    }
    else {
        m_vehicleSpriteWhitener->setDisplayFrame(spriteFrame);
        m_vehicleSpriteWhitener->setVisible(true);

        m_vehicleSpriteWhitener->setPosition(m_vehicleSprite->getContentSize() / 2);
    }
}

void REPlayerObject::updateSlopeRotation(float dt) {
    float rotation = convertToClosestRotation(m_slopeRotation * 57.295780181884766);
    if (m_isPlatformer && m_maybeChangedDirectionAngle > 80.0 && (m_platformerMovingLeft || m_platformerMovingRight)) {
        rotation = 0;
    }
    updateRotation(dt, rotation);
}

void REPlayerObject::updateSlopeYVelocity_(float yVelocity) {}

void REPlayerObject::updateSpecial_(float dt) {
    m_unk838 += dt;
    if (m_unk838 >= 0.01f) {
        m_followRelated++;
        m_unk838 -= 0.01f;
    }
    m_playerFollowFloats[m_followRelated % 200] = m_obPosition.y;
}

void REPlayerObject::updateStateVariables_() { // FIX BINDINGS, Final 4 members here should be in inline definition
    m_stateNoAutoJump--;
    m_stateDartSlide--;
    m_stateFlipGravity--;
    m_stateHitHead--;
    m_stateOnGround--;
    m_stateBoostX--;
    m_stateBoostY--;
    m_maybeStateForce2--;
    m_stateScale--;
    m_stateForce--;
    m_stateForceVector = cocos2d::CCPoint { 0.f, 0.f };
    m_jumpPadRelated.clear();
    m_stateUnk = '\0';
    m_stateNoStickX = '\0';
    m_stateNoStickY = '\0';
    m_stateUnk2 = '\0';
}

void REPlayerObject::updateStaticForce_(float rotation, float staticForce, bool additive) {
    if (staticForce != 0.f || additive) {
        auto point = cocos2d::ccpForAngle(rotation * (M_PI / 180.f));
        auto factor = std::sqrtf(point.x * point.x + point.y * point.y);

        if (factor > 0.f) {
            point = point * (staticForce / factor);
            if (m_isSideways) {
                std::swap(point.x, point.y);
            }
            updatePlayerForce(point, additive);
        }
    }
    else {
        m_yVelocity = 0.0;
        if (m_isPlatformer) {
            m_platformerXVelocity = 0.0;
            m_affectedByForces = false;
        }
    }
}

void REPlayerObject::updateStreakBlend(bool blend) {
    GLenum src = GL_SRC_ALPHA;
    GLenum dst = blend ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA;

    m_regularTrail->setBlendFunc({src, dst});
    m_waveTrail->m_isSolid = !blend;
    m_waveTrail->setBlendFunc({src, dst});
}

void REPlayerObject::updateStreaks_(float dt) {
    if (m_shipStreak) m_shipStreak->update(dt);
}

void REPlayerObject::updateSwingFire() {
    PlayerFireBoostSprite* top;
    PlayerFireBoostSprite* bottom;
    CCParticleSystemQuad* topParticles;
    CCParticleSystemQuad* bottomParticles;

    if (m_isUpsideDown == m_isSideways) {
        bottom = m_swingFireTop;
        bottomParticles = m_swingBurstParticles2;
        top = m_swingFireBottom;
        topParticles = m_swingBurstParticles1;
    }
    else {
        bottom = m_swingFireBottom;
        bottomParticles = m_swingBurstParticles1;
        top = m_swingFireTop;
        topParticles = m_swingBurstParticles2;
    }

    bottom->animateFireIn();
    bottomParticles->resumeSystem();
    top->animateFireOut();
    topParticles->stopSystem();
}

void REPlayerObject::updateTimeMod(float speed, bool noEffects) {
    if (!m_maybeReducedEffects && m_playEffects && !noEffects && m_playerSpeed != speed) {
        GameManager::get()->m_playLayer->playSpeedParticle(speed);
    }

    m_playerSpeed = speed;

    if (speed == 0.9f) {
        m_yStart = 11.180031776428223;
        m_gravity = 0.9581990242004395;
        m_speedMultiplier = 5.7700018882751465;
    }
    else if (speed == 0.7f) {
        m_yStart = 10.620032;
        m_gravity = 0.940199;
        m_speedMultiplier = 5.980002;
    }
    else if (speed == 1.1f) {
        m_yStart = 11.420032;
        m_gravity = 0.957199;
        m_speedMultiplier = 5.870002;
    }
    else if (speed == 1.3f || speed == 1.6f) {
        m_yStart = 11.230032;
        m_gravity = 0.961199;
        m_speedMultiplier = 6.000002;
    }
    updateRobotAnimationSpeed();

    if (m_isBall) {
        runRotateAction_(false, 0);
    }

    if (m_shipStreak) {
        updateStreakSettings__(m_shipStreak, m_shipStreakType, this);
    }
}

bool REPlayerObject::usingWallLimitedMode_() {
    return isFlying_() || m_isBall || m_isSpider;
}

void REPlayerObject::yStartDown_() {
    m_yStart -= 0.01;
    logValues();
}

void REPlayerObject::yStartUp_() {
    m_yStart += 0.01;
    logValues();
}

/*
known issues todo
*/
