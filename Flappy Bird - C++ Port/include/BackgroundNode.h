#ifndef BACKGROUNDNODE_H
#define BACKGROUNDNODE_H

#include <SDL_classes.h>

#include "../Saving.h"

class BackgroundNode
{
    public:
        BackgroundNode(WindowInstance& app, const int id, PlayerData const& playerData, Font& font, DataFile const& data);
        virtual ~BackgroundNode();

        bool isUnlocked() const {return m_Unlocked;}
        int getId() const {return m_Id;}

        void show() const;

    protected:
        WindowInstance& m_AppBind;
        const int m_Id;
        ScrollablePicture m_Picture;
        Text m_Name;
        Text m_GraphConditional;
        bool m_Unlocked;

        std::string extractPath() const;
        void extractInfo(PlayerData const& playerData, DataFile const& data);

    private:
};

#endif // BACKGROUNDNODE_H
