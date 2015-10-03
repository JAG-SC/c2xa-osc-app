/************************************************************************************//**
    @file   c2xa/scene/main_scene.cpp
    @author �V�T�������
    @date   created on 2015/09/29
****************************************************************************************/
#include <c2xa/judgement.hpp>

#include <c2xa/scene/main_scene.hpp>

#include <c2xa/scene/main/layer/ui_layer.hpp>
#include <c2xa/scene/main/layer/object_layer.hpp>
#include <c2xa/scene/main/layer/background_layer.hpp>

#include <c2xa/scene/score_scene.hpp>

#include <scripting/lua-bindings/manual/CCLuaEngine.h>

using namespace cocos2d;
using namespace c2xa::scene;

bool main_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    setName( "main_scene" );
    addChild( main::layer::ui_layer::create(), 3 );
    addChild( main::layer::object_layer::create(), 2 );
    addChild( main::layer::background_layer::create(), 1 );

    addChild( judgement::create() );

    auto lua_engine_ = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine( lua_engine_ );
    lua_engine_->addSearchPath( "scripts" );
    lua_engine_->executeScriptFile( "scripts/test.lua" );

    lua_State* l = lua_engine_->getLuaStack()->getLuaState();
    
    lua_getglobal( l, "helloLua" );
    tolua_pushusertype( l, this, "cc.Scene" );
    if( lua_pcall( l, 1, 0, 0 ) ){
        CCLOG( "error=%s", lua_tostring( l, lua_gettop( l ) ) );
    }

    return true;
}
void main_scene::game_over()
{
    if( !is_transitioning_ )
    {
        // ����:
        //  update����replase���ăo�O�ɔY�񂾁B
        //  �g�����W�V�����̏ꍇ�Areplace�͑����ɐ؂�ւ��킯�ł͂Ȃ��̂�
        //  update���Ă΂ꑱ����B�����đ�ʂ�replace����đ�ςȎ��ɂȂ�B
        //  �����݂����ɃR�[���o�b�N�̂悤�ȏ󋵂��A�g�����W�V�������̃t���O��p��
        //  ����ȂǋC������K�v������B��҂��K�؂��B
        Director::getInstance()
            ->replaceScene(
                TransitionFade::create(
                    1.0f,
                    scene::score_scene::create(
                        get_child<judgement>( this, "judgement" )->get_score() ) ) );
        is_transitioning_ = true;
    }
}