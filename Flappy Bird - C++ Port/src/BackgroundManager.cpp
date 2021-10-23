#include "../BackdropSelector.h"
#include "BackgroundManager.h"

/*
Changed my mind.
I am NOT dealing with placement new again.
*/

BackgroundManager::BackgroundManager(WindowInstance& app, PlayerData& playerData, const char* dataPath, Font& font)
:
    m_AppBind(app),
    m_Nodes(),
    m_SelectedNode(0),
    m_Select(app, Colors::BLANK, Colors::BLANK, "Select", font, 100, Colors::BLACK, 2, TextQuality::HIGH, Position::TOP|Position::HORIZONTAL),
    m_Data(playerData)
{
    DataFile data(dataPath);
    for(unsigned int i = 0 ; i < determineAmountOfBackgrounds() ; i++)
    {
        m_Nodes.push_back(new BackgroundNode(app, i, playerData, font, data));
        if(m_Nodes[i]->getId() == playerData.getBackgroundId()){m_SelectedNode = i;}
    }
}

BackgroundManager::~BackgroundManager()
{
    for(unsigned int i = 0 ; i < m_Nodes.size() ; i++)
    {
        delete m_Nodes[i];
    }
}

bool BackgroundManager::requestingSelect(SDL_Event const& e)
{
    return m_Select.clickedOn(e);
}

void BackgroundManager::trySelect()
{
    if(m_Nodes[m_SelectedNode]->isUnlocked())
    {
        m_Data.setBackground(m_SelectedNode);
        m_Select.setBackgroundColor(Colors::BLANK);
        m_Select.setHighlightColor(Colors::BLANK);
        m_Select.setTextColor(Colors::BLACK);
    }
}

void BackgroundManager::scrollLeft()
{
    m_SelectedNode = (m_SelectedNode - 1 < 0) ? m_Nodes.size() - 1 : m_SelectedNode - 1;

    if(m_Data.getBackgroundId() == m_Nodes[m_SelectedNode]->getId())
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

void BackgroundManager::scrollRight()
{
    m_SelectedNode++; m_SelectedNode %= m_Nodes.size();

    if(m_Data.getBackgroundId() == m_Nodes[m_SelectedNode]->getId())
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

void BackgroundManager::show()
{
    m_AppBind.clean();
    m_AppBind.paint(Colors::GRAY);

    m_Nodes[m_SelectedNode]->show();

    if(m_Nodes[m_SelectedNode]->isUnlocked())
    {
        m_Select.show();
    }
}
