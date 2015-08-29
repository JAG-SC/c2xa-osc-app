/************************************************************************************//**
    @file	c2xa/scene/main_scene/layer/object_layer.hpp
    @brief	object_layer

    @author    �V�T��(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP
#define C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP

#include <cocos2d.h>

#include <c2xa/object/player.hpp>

namespace c2xa
{
    namespace scene
    {
        namespace main
        {
            namespace layer
            {
                class object_layer
                    : public cocos2d::Layer
                {
                public:
                    CREATE_FUNC( object_layer );
                    virtual bool init() override
                    {
                        if( !Layer::init() )
                        {
                            return false;
                        }
                        this->addChild( object::player::create() );
                        return true;
                    }
                };
            }
        }
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP