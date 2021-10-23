#ifndef SKINNODE_H
#define SKINNODE_H

#include <SDL_classes.h>

#include "Player.h"
#include "../Constants.h"
#include "../Saving.h"

class SkinNode : public Sprite
{
    public:
        explicit SkinNode(WindowInstance& app, Font& font, const SkinPositions identifier, TextureAtlas& playerAtlas, DataFile& data, SDL_Rect const& topRect);
        virtual ~SkinNode();

        SkinPositions getIdentifier() const{return m_Identifier;}
        bool isUnlocked() const {return m_Unlocked;}

        void show() const;

    protected:
        SkinPositions m_Identifier;
        Text m_Name;
        Text m_MainString;
        Text m_SubString;
        std::string m_Conditional;
        Text m_GraphConditional;
        bool m_Unlocked;

        std::string decodeConditional();
        void determineIfUnlocked(const int score);

        friend class SelectManager;

    private:
};

#endif // SKINNODE_H
