/************************************************************************************//**
    @file	 c2xa/judgement.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_OBJECT_COIN_HPP
#define C2XA_OBJECT_COIN_HPP

#include <cocos2d.h>

#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace object
    {
        class coin
            : public cocos2d::Node
        {
        private:
            cocos2d::Sprite* sprite_;
            collision collision_;
            float fall_speed_ = 1.f;

        public:
            ~coin()
            {
                sprite_->release();
            }

            CREATE_FUNC( coin );
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }

                struct once_init
                {
                    once_init()
                    {
                        add_sprite_batch( get_current_scene(), "CloseNormal.png", "coin" );
                    }
                } static once_;

                scheduleUpdate();

                sprite_ = create_sprite_from_batch( get_current_scene(), "coin" );
                sprite_->retain();

                static std::mt19937 engine_( random_seed );
                std::uniform_real_distribution<> dist_( 0, app_width );
                auto x =  dist_( engine_ );
                sprite_->setPosition( x, app_height );

                addChild( sprite_ );

                collision_ = create_collision_circul( sprite_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                auto y_ = sprite_->getPositionY() - ( fall_speed_ * delta_ * 100.f );
                sprite_->setPositionY( y_ );
                if( y_ < 0 )
                {
                    removeFromParent();
                }
            }
            collision get_collision() const
            {
                return collision_;
            }
        };
    }
}

#endif//C2XA_OBJECT_COIN_HPP