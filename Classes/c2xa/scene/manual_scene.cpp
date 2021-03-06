﻿/************************************************************************************//**
    @file   c2xa/scene/manual_scene.cpp
    @author 新ゝ月かりな
    @date   created on 2015/09/29
****************************************************************************************/
#include <AudioEngine.h>

#include <c2xa/scene/manual_scene.hpp>
#include <c2xa/scene/main_scene.hpp>

#include <c2xa/c2xa_config.hpp>

using namespace cocos2d;
using namespace c2xa::scene;

bool manual_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    auto sprite_bg_ = Sprite::create( "img/manual_scene_bg.png" );
    auto sprite_next_ = Sprite::create( "img/manual_scene_next.png" );

    auto s1 = Sprite::create( "img/manual_scene_1.png" );
    auto s21 = Sprite::create( "img/manual_scene_2_1.png" );
    auto s22 = Sprite::create( "img/manual_scene_2_2.png" );
    auto s23 = Sprite::create( "img/manual_scene_2_3.png" );
    auto s31 = Sprite::create( "img/manual_scene_3_1.png" );
    auto s32 = Sprite::create( "img/manual_scene_3_2.png" );
    auto s33 = Sprite::create( "img/manual_scene_3_3.png" );
    auto s34 = Sprite::create( "img/manual_scene_3_4.png" );
    auto s4 = Sprite::create( "img/manual_scene_4.png" );

    Sprite* ss[] = { s1, s21, s22, s23, s31, s32, s33, s34, s4 };

    sprite_bg_->setPosition( Vec2{ app_width / 2, app_height / 2 } );
    addChild( sprite_bg_ );

    sprite_next_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
    sprite_next_->setPosition( Vec2{ app_width - 15, 10 } );
    sprite_next_->setOpacity( 0 );
    addChild( sprite_next_ );

    for( auto i : ss )
    {
        i->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        i->setPosition( Vec2{ app_width / 2, app_height - 30 } );
        i->setOpacity( 0 );
        addChild( i );
    }

    auto dispatcher = Director::getInstance()->getEventDispatcher();

    auto on_input = [ = ]( Action* action_ )
    {
        sprite_next_->runAction( FadeIn::create( 0.3f ) );
        action_->retain();
        auto keyboard_listener_ = EventListenerKeyboard::create();
        auto touch_listener_    = EventListenerTouchOneByOne::create();
        keyboard_listener_->onKeyPressed = [ = ]( EventKeyboard::KeyCode key_, Event* event_ )
        {
            sprite_next_->runAction( FadeOut::create( 0.3f ) );
            this->runAction( action_ );
            action_->release();
            dispatcher->removeEventListenersForTarget( this );
        };
        touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
        {
            sprite_next_->runAction( FadeOut::create( 0.3f ) );
            this->runAction( action_ );
            action_->release();
            dispatcher->removeEventListenersForTarget( this );
            return true;
        };
        dispatcher->addEventListenerWithSceneGraphPriority( touch_listener_, this );
        dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );
    };

    auto pon = []
    {
        cocos2d::experimental::AudioEngine::play2d( "sounds/pon.mp3", false, 0.3f, nullptr );
    };

    auto bgm_id = cocos2d::experimental::AudioEngine::play2d( "sounds/manual_bgm.mp3", true, 0.3f, nullptr );
    runAction(
        Sequence::create(
            CallFunc::create( [ = ]
            {
                s1->runAction( FadeIn::create( 0.3f ) );
            } ),
            DelayTime::create( 2.f ),
            CallFunc::create( [ = ]
            {
                on_input( Sequence::create(
                    CallFunc::create( [ = ]
                    {
                        s1->runAction( FadeOut::create( 0.3f ) );
                    } ),
                    DelayTime::create( 1.f ),
                    CallFunc::create( [ = ]
                    {
                        pon();
                        s21->runAction( FadeIn::create( 0.f ) );
                    } ),
                    DelayTime::create( 1.f ),
                    CallFunc::create( [ = ]
                    {
                        pon();
                        s21->runAction( FadeOut::create( 0.f ) );
                        s22->runAction( FadeIn::create( 0.f ) );
                    } ),
                    DelayTime::create( 1.f ),
                    CallFunc::create( [ = ]
                    {
                        pon();
                        s22->runAction( FadeOut::create( 0.f ) );
                        s23->runAction( FadeIn::create( 0.f ) );

                        on_input( Sequence::create(
                            CallFunc::create( [ = ]
                            {
                                s23->runAction( FadeOut::create( 0.3f ) );
                            } ),
                            DelayTime::create( 1.f ),
                            CallFunc::create( [ = ]
                            {
                                s31->runAction( FadeIn::create( 0.3f ) );
                            } ),
                            DelayTime::create( 1.f ),
                            CallFunc::create( [ = ]
                            {
                                s31->runAction( FadeOut::create( 0.f ) );
                                s32->runAction( FadeIn::create( 0.f ) );
                                
                                on_input( Sequence::create(
                                    CallFunc::create( [ = ]
                                    {
                                        s32->runAction( FadeOut::create( 0.3f ) );
                                    } ),
                                    DelayTime::create( 1.f ),
                                    CallFunc::create( [ = ]
                                    {
                                        s33->runAction( FadeIn::create( 0.3f ) );
                                    } ),
                                    DelayTime::create( 1.f ),
                                    CallFunc::create( [ = ]
                                    {
                                        s33->runAction( FadeOut::create( 0.f ) );
                                        s34->runAction( FadeIn::create( 0.f ) );

                                        
                                        on_input( Sequence::create(
                                            CallFunc::create( [ = ]
                                            {
                                                s34->runAction( FadeOut::create( 0.3f ) );
                                            } ),
                                            DelayTime::create( 1.f ),
                                            CallFunc::create( [ = ]
                                            {
                                                s4->runAction( FadeIn::create( 0.3f ) );
                                            } ),
                                            DelayTime::create( 1.f ),
                                            CallFunc::create( [ = ]
                                            {
                                                on_input( CallFunc::create( [ = ]
                                                {
                                                    cocos2d::experimental::AudioEngine::stop( bgm_id );
                                                    Director::getInstance()->replaceScene( TransitionFade::create( 2.0f, scene::main_scene::create() ) );
                                                } ) );
                                            } ),
                                            nullptr ) );
                                    } ),
                                    nullptr ) );
                            } ),
                            nullptr ) );
                    } ),
                    nullptr ) );
            } ),
            nullptr
        )
    );


    return true;
}