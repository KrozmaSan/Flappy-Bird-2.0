#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include <SDL_classes.h>

#include "BackgroundNode.h"
#include "../Saving.h"

class BackgroundManager
{
    public:
        BackgroundManager(WindowInstance& app, PlayerData& playerData, const char* dataPath, Font& font);
        virtual ~BackgroundManager();

        void show();
        bool requestingSelect(SDL_Event const& e);
        void trySelect();

        void scrollLeft();
        void scrollRight();

    protected:
        WindowInstance& m_AppBind;
        std::vector<BackgroundNode*> m_Nodes;
        int m_SelectedNode;
        TextButton m_Select;
        PlayerData& m_Data;

    private:
};

#endif // BACKGROUNDMANAGER_H
