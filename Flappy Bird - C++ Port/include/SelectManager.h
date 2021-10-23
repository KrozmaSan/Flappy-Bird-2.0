#ifndef SELECTMANAGER_H
#define SELECTMANAGER_H

#include <SDL_classes.h>

#include "../Saving.h"
#include "../Constants.h"
#include "Player.h"
#include "SkinNode.h"


class SelectManager
{
public:
    explicit SelectManager(WindowInstance& app, const char* playerAtlasPath, const char* cosmeticStringsPath, const char* nodesFontPath, PlayerData& playerData);
    explicit SelectManager(WindowInstance& app, std::string const& playerAtlasPath, std::string const& cosmeticStringsPath, std::string const& nodesFontPath, PlayerData& playerData);

    virtual ~SelectManager();

    bool requestingSelect(SDL_Event const& e);
    void setSelection();

    void show();
    void scrollRight();
    void scrollLeft();

protected:
    WindowInstance& m_AppBind;
    TextureAtlas m_SkinSheet;
    Font m_NodesFont;
    std::vector<SDL_Point> m_SkinPositions;
    const unsigned int m_NumberOfNodes;
    DataFile m_CosmeticStrings;
    std::vector<SkinNode*> m_Nodes;
    PlayerData& m_Data;
    int m_SelectedNode;
    TextButton m_Select;

    unsigned int computeNumberOfNodes(const char* nodePath);

};
#endif // SELECTMANAGER_H
