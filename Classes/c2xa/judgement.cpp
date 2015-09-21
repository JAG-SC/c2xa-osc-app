
#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/object/player.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa;

void judgement::update( float )
{
    if( game_over_ )
    {
    }
    else
    {
        // 接触判定
        auto root_         = get_parent( this );
        auto object_layer_ = get_child( root_, "object_layer" );
        auto player_       = get_child<object::player>( object_layer_, "player" );
        auto coins_        = get_child( object_layer_, "coins" )->getChildren();
        
        for( auto i : coins_ )
        {
            if( player_->get_collision()->judge( static_cast<object::coin*>( i )->get_collision().get() ) )
            {
                score_.add( coin_point );
                i->removeFromParent();
            };
        }

        // スコアカウント
        // コリジョンより後ろ
        get_child<scene::main::layer::ui_layer>( get_current_scene(), "ui_layer" )->set_score( score_.get() );
        // 自機数カウント
        // ゲーム終了判断
        if( remaining_.is_dead() )
        {
            game_over_ = true;
            get_parent<scene::main_scene>( this )->game_over();
        }
    }
}