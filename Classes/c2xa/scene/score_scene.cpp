﻿/************************************************************************************//**
    @file   c2xa/scene/score_scene.cpp
    @author 新ゝ月かりな
    @date   created on 2015/09/29
****************************************************************************************/
#include <AudioEngine.h>

#include <c2xa/to_string_from_score.hpp>
#include <c2xa/utility.hpp>

#include <c2xa/scene/score_scene.hpp>
#include <c2xa/scene/end_scene.hpp>

using namespace cocos2d;
using namespace c2xa::scene;

score_scene* score_scene::create( unsigned int socre_ )
{
    return create_template<score_scene>( socre_ );
}

bool score_scene::init( unsigned int s_ )
{
    using namespace cocos2d;

    if( !Scene::init() )
    {
        return false;
    }

    score_ = s_;

    setName( "score_scene" );
    
    auto sf_ = Sprite::create( "img/score_scene_score_font.png" );
    for( int i = 0; i <= 9; ++i )
    {
        auto frame_ = SpriteFrame::createWithTexture(
            sf_->getTexture(),
            Rect{ i * score_scene_score_font_width, 0, score_scene_score_font_width, score_scene_score_font_height } );
        SpriteFrameCache::getInstance()
            ->addSpriteFrame( frame_, std::move( get_cache_frame_name( i ) ) );
    }
                
    auto score_display_ = Layer::create();
    score_display_->setName( "score_display" );
    score_display_->setCascadeOpacityEnabled( true );
    score_display_->setOpacity( 0 );

    for( int i = 0; i < score_digit; ++i )
    {
        auto sprite_ = Sprite::createWithSpriteFrameName( get_cache_frame_name( 0 ) );
        sprite_->setTag( i );
        sprite_->setPosition( Vec2{
            110 + i * ( score_scene_score_font_width ),
            730 } );
        sprite_->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        score_display_->addChild( sprite_ );
    }

    auto background_ = Sprite::create( "img/score_scene_background.png" );
    background_->setPosition( Vec2( 0, 0 ) );
    background_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );

    auto result_font_ = Sprite::create( "img/score_scene_result_font.png" );
    result_font_->setPosition( Vec2( -result_font_->getContentSize().width, app_height ) );
    result_font_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    result_font_->setOpacity( 0 );

    auto comment_ = Sprite::create( "img/score_scene_good.png" );
    comment_->setPosition( Vec2( 0, 0 ) );
    comment_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    comment_->setOpacity( 0 );

    addChild( score_display_, 4 );
    addChild( result_font_, 3 );
    addChild( comment_, 2 );
    addChild( background_, 1 );
                

    auto count_                = std::make_shared<float>( 0 );
    auto audio_id_drum_        = std::make_shared<int>( 0 );
    auto audio_id_drum_finish_ = std::make_shared<int>( 0 );

    result_font_->runAction(
        Sequence::create(
            Spawn::createWithTwoActions(
                EaseOut::create( MoveTo::create( 1.f, Vec2( 0, app_height ) ), 3.f ),
                FadeTo::create( 1.0f, 255 ) ),
            DelayTime::create( 0.5f ),
            CallFunc::create( [ score_display_ ]
            {
                score_display_->runAction( EaseSineInOut::create( FadeTo::create( 0.3f, 255 ) ) );
            } ),
            // score roulette 2.5s
            CallFunc::create( [ this, count_, score_display_, comment_, audio_id_drum_, audio_id_drum_finish_ ]
            {
                *audio_id_drum_ = cocos2d::experimental::AudioEngine::play2d( "sounds/drum_roll.mp3", false, 0.1f, nullptr );
                schedule( [ this, count_, score_display_, comment_, audio_id_drum_, audio_id_drum_finish_ ]( float t_ )
                {
                    *count_ += t_ * 100.f;
                    set_score( get_score() * *count_ / 150.f );
                    if( *count_ >= 150.f )
                    {
                        set_score( get_score() );
                        unschedule( "roulette" );
                        
                        cocos2d::experimental::AudioEngine::stop( *audio_id_drum_ );
                        *audio_id_drum_finish_ = cocos2d::experimental::AudioEngine::play2d( "sounds/drum_roll_finish.mp3", false, 0.1f, nullptr );

                        auto keyboard_listener_ = EventListenerKeyboard::create();
                        auto touch_listener_    = EventListenerTouchOneByOne::create();

                        auto next_scene_ = [ = ]
                        {
                            cocos2d::experimental::AudioEngine::stop( *audio_id_drum_finish_ );
                            cocos2d::experimental::AudioEngine::stop( *audio_id_drum_ );
                            Director::getInstance()
                                ->replaceScene(
                                    TransitionFade::create(
                                        1.0f,
                                        scene::end_scene::create() ) );
                        };

                        keyboard_listener_->onKeyPressed = [ next_scene_ ]( EventKeyboard::KeyCode key_, Event* event_ )
                        {
                            next_scene_();
                        };

                        touch_listener_->onTouchBegan = [ next_scene_ ]( Touch* t_, Event* )
                        {
                            next_scene_();
                            return true;
                        };

                        auto dispatcher = Director::getInstance()->getEventDispatcher();
                        dispatcher->addEventListenerWithSceneGraphPriority( touch_listener_, this );
                        dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );

                        comment_->runAction( EaseSineInOut::create( FadeTo::create( 1.f, 255 ) ) );
                    }
                }, "roulette" );
            } ),
            nullptr ) );
    return true;
}

void score_scene::set_score( unsigned int score_number_ )
{
    using namespace cocos2d;
    auto score_ = to_string_from_score( score_number_ );
    auto layer_ = get_child( this, "score_display" );
    for( int i = 0; i < score_digit; ++i )
    {
        auto number_ = score_[ i ] - '0';
        auto frame_ = SpriteFrameCache::getInstance()
            ->getSpriteFrameByName( get_cache_frame_name( number_ ) );
        auto sprite_ = static_cast<Sprite*>( layer_->getChildByTag( i ) );
        sprite_->setTexture( frame_->getTexture() );
        sprite_->setTextureRect( frame_->getRect() );
        sprite_->setSpriteFrame( frame_ );
    }
}

std::string score_scene::get_cache_frame_name( int i_ )
{
    return std::string{ "score_font_" } +one_digit_to_string( i_ );
}