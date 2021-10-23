#include "SkinNode.h"

SkinNode::SkinNode(WindowInstance& app, Font& font, SkinPositions identifier, TextureAtlas& playerAtlas, DataFile& data, SDL_Rect const& topRect)
:
    AbstractGraphics(app, Position::NONE),
    Sprite(app, playerAtlas, SDL_VOID_RECT),
    m_Identifier(identifier),
    m_Name(app, "DEF_NAME", font, 100, Colors::WHITE, 2, TextQuality::HIGH, Position::NONE),
    m_MainString(app, "DEF_MAINSTR", font, 30, Colors::WHITE, 2, TextQuality::HIGH, Position::NONE),
    m_SubString(app, "DEF_SUBSTR", font, 30, Colors::WHITE, 2, TextQuality::HIGH, Position::NONE),
    m_Conditional(""),
    m_GraphConditional(app, "DEF_COND", font, 30, Colors::WHITE, 2, TextQuality::HIGH, Position::NONE),
    m_Unlocked(true)
{
    //std::string key = std::format("C{]R{}Name", m_Identifier & 0x0F, m_Identifier >> 4); //Seems like std::format isn't implemented yet by any compiler. Keeping it in case it gets revised...

    //Mask highest nibble to get rows, and bit-shift to get columns, with casting to avoid the sign bit from spreading all over the place
    std::string key; key += "R" + std::to_string(m_Identifier & 0x0F) + "C" + std::to_string(static_cast<unsigned char>(m_Identifier) >> 4) +  "Name";
    m_Name.setString(data.parse<std::string>(key));

    key = "R" + std::to_string(m_Identifier & 0x0F) + "C" + std::to_string(static_cast<unsigned char>(m_Identifier) >> 4) +  "MainString";
    m_MainString.setString(data.parse<std::string>(key));

    key = "R" + std::to_string(m_Identifier & 0x0F) + "C" + std::to_string(static_cast<unsigned char>(m_Identifier) >> 4) +  "SubString";
    m_SubString.setString(data.parse<std::string>(key));

    key = "R" + std::to_string(m_Identifier & 0x0F) + "C" + std::to_string(static_cast<unsigned char>(m_Identifier) >> 4) +  "Condition";
    m_Conditional = data.parse<std::string>(key);
    m_GraphConditional.setString(decodeConditional());


    m_AtlasPosition.w = Player::PLAYER_SPRITE_WIDTH; m_AtlasPosition.h = Player::PLAYER_SPRITE_HEIGHT;
    m_AtlasPosition.x = m_AtlasPosition.w * (static_cast<unsigned char>(identifier) >> 4); m_AtlasPosition.y = m_AtlasPosition.h * (identifier & 0x0F);

    scale(30);
    setNextTo(topRect, 0, 50);
    setPosition(Position::HORIZONTAL);
    m_Name.setNextTo(m_ScreenPosition, 0, 50);
    m_Name.setPosition(Position::HORIZONTAL);

    m_MainString.setNextTo(m_Name, 0, 50);
    m_MainString.setPosition(Position::HORIZONTAL);

    m_SubString.setNextTo(m_MainString, 0, 10);
    m_SubString.setPosition(Position::HORIZONTAL);

    m_GraphConditional.setPosition(Position::HORIZONTAL|Position::BOTTOM);
}

SkinNode::~SkinNode()
{
    //dtor
}

void SkinNode::show() const
{
    Sprite::show();

    if(m_Unlocked)
    {
        m_Name.show();
        m_MainString.show();
        m_SubString.show();
    }

    m_GraphConditional.show();
}

std::string SkinNode::decodeConditional()
{
    if(m_Conditional[0] == 'D')
    {
        return std::string("It's all yours, 'til the end of time !");
    }

    std::string str = "Unlocked by reaching " + m_Conditional + " points.";
    return str;
}

void SkinNode::determineIfUnlocked(const int score)
{
    if(m_Conditional[0] == 'D'){m_Unlocked = true; return;}
    m_Unlocked = score >= std::stoi(m_Conditional);

    if(!m_Unlocked)
    {
        m_AtlasPosition.x = 15 * m_AtlasPosition.w;
        m_AtlasPosition.y = 15 * m_AtlasPosition.h;
    }
}
