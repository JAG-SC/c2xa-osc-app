/************************************************************************************//**
    @file    c2xa/judgement.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_JUDGEMENT_HPP
#define C2XA_JUDGEMENT_HPP

#include <cocos2d.h>

#include <c2xa/scene/main/layer/ui_layer.hpp>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    class judgement
        : public cocos2d::Node
    {
    private:
        class remaining
        {
        private:
            unsigned int number_;

        public:
            remaining( unsigned int num_ )
                : number_{ num_ }
            {
            }

        public:
            void decrease()
            {
                --number_;
            }
            unsigned int get() const
            {
                return number_;
            }
            bool is_dead()
            {
                return number_ == 0;
            }
        } remaining_ = initial_player_remaining;
        class score
        {
        private:
            unsigned int number_;

        public:
            score( unsigned int num_ )
                : number_{ num_ }
            {
            }

        public:
            void add( unsigned int p_ )
            {
                number_ += p_;
                if( number_ > max_score )
                {
                    number_ = max_score;
                }
            }
            unsigned int get() const
            {
                return number_;
            }
        } score_ = 0;

    private:
        bool game_over_ = false;
        float no_judge_player = 0; // ダメージ後の無敵時間カウント

    public:
        static judgement* create( cocos2d::Node* parent_ )
        {
            return create_template<judgement>( parent_ );
        }
        virtual bool init( cocos2d::Node* parent_ ); //no-override;
        virtual void update( float ) override;
        bool is_game_over() const
        {
            return game_over_;
        }
        unsigned int get_score() const
        {
            return score_.get();
        }

    private:
        void damage_player();
    };
}

#endif//C2XA_JUDGEMENT_HPP