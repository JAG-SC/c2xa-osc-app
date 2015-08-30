/************************************************************************************//**
    @file	c2xa/bullet/test_player_bullet.hpp
    @brief	Test Bullet

    @author    �V�T��(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_BULLET_TEST_PLAYER_BULLET_HPP
#define C2XA_BULLET_TEST_PLAYER_BULLET_HPP

#include <cocos2d.h>

#include <c2xa/create_template.hpp>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/c2xa_config.hpp>

namespace c2xa
{
    namespace bullet
    {
        class test_player_bullet
            : public cocos2d::Node
        {
        private:
            float   duration_ = 320.f; // frame
            float   count_ = 0.f;
            cocos2d::Vec2    target_position_;
            cocos2d::Vec2    first_position_;
            cocos2d::Sprite* bullet_;

            /*
            Action�͊��荞�݂Ɏア(�ł��Ȃ��͂Ȃ����A�R�[���o�b�N�n���ɂȂ�̂Ŋ댯)
            �e�͈ړ����ɐڐG���ɏ�����Ƃ������荞�݂̉\�������邽�߁A�f���ɍ��W�����Ď����œ�����
            �܂��́A���荞�݉\��Action����邩(��Ɨʑ��A�g�������A���ʂ���)
            */

        public:
            static test_player_bullet* create( float player_x_ )
            {
                return create_template<test_player_bullet>( player_x_ );
            }
            virtual bool init( float player_x_ ) // !!! no-override !!!
            {
                if( !Node::init() )
                {
                    return false;
                }
                this->scheduleUpdate();

                target_position_ = { player_x_, app_height }; // �Ƃ肠������ʊO
                first_position_  = { player_x_, 100 };

                bullet_ = cocos2d::Sprite::create( "CloseSelected.png" );
                bullet_->setPosition( first_position_ );
                addChild( bullet_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                count_ += 1.f * ( delta_ * 100 );

                if( count_ > duration_ )
                {
                    removeFromParent();
                }
                else
                {
                    bullet_->setPosition( ( target_position_ - first_position_ ) * count_ / duration_ + first_position_ );
                }
            }
        };
    }
}

#endif//C2XA_BULLET_TEST_PLAYER_BULLET_HPP