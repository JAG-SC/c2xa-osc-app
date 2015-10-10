/************************************************************************************//**
    @file   c2xa/bullet/enemy_bullet.hpp
    @author 新ゝ月かりな
    @date   created on 2015/10/09
****************************************************************************************/
#ifndef C2XA_BULLET_ENEMY_BULLET_HPP
#define C2XA_BULLET_ENEMY_BULLET_HPP

#include <cocos2d.h>

#include <AudioEngine.h>

#include <stdlib.h>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/bullet/player_bullet.hpp>
#include <c2xa/object/object.hpp>
#include <c2xa/counter.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace bullet
    {
        struct data
        {
            lua_State* state;
            int move_id;
            double duration = 0; // 初期化必須
            float first_x;
            float first_y;
            unsigned int point;
        };
        /// @author 新ゝ月かりな
        /// @date created on 2015/10/04
        /// このクラスは基本的にLuaからのみ作成できます
        class enemy_bullet
            : public bullet_interface
        {
        private:
            std::unique_ptr<data> data_;
            double progress_ = 0.;
            collision collision_;

        public:
            static enemy_bullet* create( std::unique_ptr<data>&& d_ )
            {
                return create_template<enemy_bullet>( std::move( d_ ) );
            }
            virtual bool init( std::unique_ptr<data>&& d_ ) // no-override
            {
                if( !Node::init() )
                {
                    return false;
                }
                data_ = std::move( d_ );
                scheduleUpdate();

                auto sprite_ = create_sprite_from_batch( get_current_scene(), "img/enemy_bullet.png" );
                sprite_->setPosition( get_position() );
                sprite_->setName( "sprite" );
                addChild( sprite_ );

                collision_ = create_collision_circul( sprite_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                progress_ += get_count();

                if( progress_ > data_->duration * 60 )
                {
                    clean();
                    return;
                }

                get_child<cocos2d::Sprite>( this, "sprite" )->setPosition( get_position() );
            }

        private:
            void clean()
            {
                unscheduleUpdate();
                luaL_unref( data_->state, LUA_REGISTRYINDEX, data_->move_id );
                removeFromParent();
            }
            cocos2d::Vec2 get_position() const
            {
                // 呼び出す関数: move_idの参照先
                lua_rawgeti( data_->state, LUA_REGISTRYINDEX, data_->move_id );

                // 第一引数: 初期位置(xとyのテーブル)
                lua_createtable( data_->state, 0, 2 );
                lua_pushnumber( data_->state, data_->first_x );
                lua_setfield( data_->state, -2, "x" );
                lua_pushnumber( data_->state, data_->first_y );
                lua_setfield( data_->state, -2, "y" );

                // 第二引数: 進捗率(0～100までのdouble)
                lua_pushnumber( data_->state, progress_ * 100 / ( data_->duration * 60 ) );

                // 呼び出し: 引数2: 戻り値1: 座標(xとyを持つテーブル)
                lua::call( data_->state, 2, 1 );

                double x = lua::to< lua::type::number >::from_table( data_->state, "x" );
                double y = lua::to< lua::type::number >::from_table( data_->state, "y" );

                lua_settop( data_->state, -2 );

                return std::move( cocos2d::Vec2 {
                    static_cast<float>( x ),
                    static_cast<float>( y )
                } );
            }
            unsigned int get_point() const override
            {
                return data_->point;
            }
            collision get_collision() const override
            {
                return collision_;
            }
            void collide( object_type type_ ) override
            {
                // 自機の弾か自機本体
                // ここもLuaから指定できるようにする？とりあえずまずはC++で実装
                switch( type_ )
                {
                case object_type::player:
                {
                    break;
                }
                case object_type::player_bullet:
                {
                    clean();
                }
                }
            }
        };
    }
}

#endif//C2XA_BULLET_ENEMY_BULLET_HPP