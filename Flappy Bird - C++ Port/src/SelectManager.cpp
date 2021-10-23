#include "SelectManager.h"

#include <sys/cdefs.h>

SelectManager::SelectManager(WindowInstance& app, const char* playerAtlasPath, const char* cosmeticStringsPath, const char* nodesFontPath, PlayerData& playerData)
:
    m_AppBind(app),
    m_SkinSheet(app, playerAtlasPath),
    m_NodesFont(nodesFontPath),
    m_SkinPositions(),
    m_NumberOfNodes(computeNumberOfNodes(playerAtlasPath)),
    m_CosmeticStrings(cosmeticStringsPath),
    m_Nodes(),
    m_Data(playerData),
    m_SelectedNode(0),
    m_Select(app, Colors::BLANK, Colors::BLANK, "Select", m_NodesFont, 100, Colors::BLACK, 2, TextQuality::HIGH, Position::NONE)
{
    m_Select.setPosition(0, 25);
    m_Select.setPosition(Position::HORIZONTAL);

    for(unsigned int i = 0 ; i < m_NumberOfNodes ; i++)
    {
        SkinPositions p;
        p = m_SkinPositions[i].x << 4 | m_SkinPositions[i].y;
        m_Nodes.push_back(new SkinNode(m_AppBind, m_NodesFont, p, m_SkinSheet, m_CosmeticStrings, m_Select.getScreenPosition()));
        m_Nodes[i]->determineIfUnlocked(playerData.getHighScore());
        if(m_Nodes[i]->getIdentifier() == m_Data.getSelectedSkin()){m_SelectedNode = i;}
    }
}

SelectManager::SelectManager(WindowInstance& app, std::string const& playerAtlasPath, std::string const& cosmeticStringsPath, std::string const& nodesFontPath, PlayerData& playerData)
:
    SelectManager(app, playerAtlasPath.c_str(), cosmeticStringsPath.c_str(), nodesFontPath.c_str(), playerData)
{

}

SelectManager::~SelectManager()
{
    for(unsigned int i = 0 ; i < m_NumberOfNodes ; i++)
    {
        delete m_Nodes[i];
    }
}

unsigned int SelectManager::computeNumberOfNodes(const char* nodePath)
{
    //Original approach used textures and SDL_ReadPixels, but it was so messy I'd rather reload the surface.
    //Original approach also used a bitmask, but since transparent pixels are just all zeros, it's useless
    SDL_Surface* surf = IMG_Load(nodePath);

    unsigned int numberOfSkins = 0;

    for(int i = 0 ; i < 16 ; i++) //Iterate over each row
    {
        for(int j = 0 ; j < 16 ; j++) //Iterate over each column
        {
            for(int k = 0 ; k < Player::PLAYER_SPRITE_HEIGHT ; k++) //Iterate over row sub-positions
            {
                for(int l = 0 ; l < Player::PLAYER_SPRITE_WIDTH ; l++) //Iterate over column sub-positions
                {
                    uint64_t val = i * surf->w * Player::PLAYER_SPRITE_HEIGHT
                    + j * Player::PLAYER_SPRITE_WIDTH
                    + k * surf->w
                    + l;
                    if(static_cast<Uint32*>(surf->pixels)
                       [val] != 0)
                    {
                        numberOfSkins++;
                        SDL_Point p; p.x = j; p.y = i;
                        m_SkinPositions.push_back(p);
                        goto MOVE_TO_NEXT_SKIN;
                    }
                }
            }
            MOVE_TO_NEXT_SKIN:
            asm("nop"); //Need to put something here for the label to be valid
        }
    }

    SDL_FreeSurface(surf);
    return numberOfSkins - 1; //Discard the last, black skin
}

void SelectManager::scrollRight()
{
    m_SelectedNode++;
    m_SelectedNode %= m_NumberOfNodes;
    if(m_Data.getSelectedSkin() == m_Nodes[m_SelectedNode]->getIdentifier())
    {
        m_Select.setBackgroundColor(Colors::BLANK);
        m_Select.setHighlightColor(Colors::BLANK);
        m_Select.setTextColor(Colors::BLACK);
    }
    else
    {
        m_Select.setBackgroundColor(Colors::TBLACK);
        m_Select.setHighlightColor(Colors::TBLACK);
        m_Select.setTextColor(Colors::WHITE);
    }

    m_Select.setVisibility(m_Nodes[m_SelectedNode]->isUnlocked());
}

void SelectManager::scrollLeft()
{
    m_SelectedNode--;
    m_SelectedNode = (m_SelectedNode < 0) ? m_NumberOfNodes - 1 : m_SelectedNode; //Wrap around
    if(m_Data.getSelectedSkin() == m_Nodes[m_SelectedNode]->getIdentifier())
    {
        m_Select.setBackgroundColor(Colors::BLANK);
        m_Select.setHighlightColor(Colors::BLANK);
        m_Select.setTextColor(Colors::BLACK);
    }
    else
    {
        m_Select.setBackgroundColor(Colors::TBLACK);
        m_Select.setHighlightColor(Colors::TBLACK);
        m_Select.setTextColor(Colors::WHITE);
    }

    m_Select.setVisibility(m_Nodes[m_SelectedNode]->isUnlocked());
}

void SelectManager::show()
{
    if(m_Nodes[m_SelectedNode]->isUnlocked())
    {
        m_Select.show();
    }

    m_Nodes[m_SelectedNode]->show();
}


bool SelectManager::requestingSelect(SDL_Event const& e)
{
    return m_Select.clickedOn(e);
}

void SelectManager::setSelection()
{
    if(!m_Nodes[m_SelectedNode]->isUnlocked()){return;}
    m_Data.setSelectedSkin(m_Nodes[m_SelectedNode]->getIdentifier());
    m_Select.setBackgroundColor(Colors::BLANK);
    m_Select.setHighlightColor(Colors::BLANK);
    m_Select.setTextColor(Colors::BLACK);
}

