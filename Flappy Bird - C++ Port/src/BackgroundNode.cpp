#include "BackgroundNode.h"

BackgroundNode::BackgroundNode(WindowInstance& app, const int id, PlayerData const& playerData, Font& font, DataFile const& data)
:
    m_AppBind(app),
    m_Id(id),
    m_Picture(app, extractPath(), app.getWindowRect()),
    m_Name(app, "DEF_NAME", font, 100, Colors::WHITE, 2, TextQuality::HIGH, Position::HORIZONTAL),
    m_GraphConditional(app, "DEF_COND", font, 30, Colors::WHITE, 2, TextQuality::HIGH, Position::HORIZONTAL)
{
    extractInfo(playerData, data);
    m_GraphConditional.setPosition(Position::HORIZONTAL|Position::BOTTOM);
    m_Name.setNextTo(m_GraphConditional, 0, -50);
    m_Name.setPosition(Position::HORIZONTAL);
}

std::string BackgroundNode::extractPath() const
{
    return std::string("assets/backgrounds/bg" + std::to_string(m_Id) + ".png");
}

void BackgroundNode::extractInfo(PlayerData const& playerData, DataFile const& data)
{
    std::string str = "bg" + std::to_string(m_Id) + "Name";
    m_Name.setString(data.parse<std::string>(str));

    str = "bg" + std::to_string(m_Id) + "Cond";
    std::string cond = data.parse<std::string>(str);

    if(cond[0] == 'D')
    {
        m_GraphConditional.setString("It's all yours, 'til the end of time !");
        m_Unlocked = true;
        return;
    }
    std::string result = "Unlocked by reaching " + cond + " points.";
    m_GraphConditional.setString(result);
    m_Unlocked = playerData.getHighScore() >= std::stoi(cond);
}

void BackgroundNode::show() const
{
    if(m_Unlocked)
    {
        m_Picture.show();
        m_Name.show();
    }

    m_GraphConditional.show();
}

BackgroundNode::~BackgroundNode()
{
    //dtor
}
