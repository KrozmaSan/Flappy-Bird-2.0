#include "Player.h"

Player::Player(WindowInstance& app, TextureAtlas& atlas, SkinPositions selectedSkin)
:
    AbstractGraphics(app, Position::NONE),
    Sprite(app, atlas, SDL_VOID_RECT),
    m_Rotation(0),
    m_JumpCount(0),
    m_IsAlive(true),
    m_HasCrashed(false),
    m_Buried(false),
    m_Flap("assets/sfx/flap.wav"),
    m_Crash("assets/sfx/hit.wav")
{
    setSkin(selectedSkin);
    m_ScreenPosition.w = PLAYER_SPRITE_WIDTH;
    m_ScreenPosition.h = PLAYER_SPRITE_HEIGHT;
    scale(3);
}

Player::~Player()
{

}

void Player::show() const
{
    if(!m_Displayed){return;}

    m_AppBind.saveRendererSettings();

    prepareAtlas();
    SDL_RenderCopyEx(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &m_AtlasPosition, &m_ScreenPosition, m_Rotation, NULL, SDL_FLIP_NONE);
    restoreAtlas();
    showFrame();
    showHitbox();

    m_AppBind.restoreRendererSettings();
}

void Player::setSkin(SkinPositions skin)
{
    SDL_Rect pos; pos.w = PLAYER_SPRITE_WIDTH; pos.h = PLAYER_SPRITE_HEIGHT;
    pos.y = PLAYER_SPRITE_HEIGHT * (static_cast<uint8_t>(skin) & 0x0F); //Delete highest nibble
    pos.x = PLAYER_SPRITE_WIDTH * (static_cast<uint8_t>(skin) >> 4); //Delete lowest nibble, and set the highest in its place - giving it usable values
    setAtlasPosition(pos);
}

int Player::update(Floor const& floor, PipeManager const& pipeManager)
{
    int score = 0;
    m_JumpCount = std::min(static_cast<int>(JUMP_HEIGHT * (-1) * SQUARE_FACTOR), m_JumpCount + 1); //Increment jump counter up to JUMP_HEIGHT.
    //Doing it first prevents calling on negatives

    /*
    This formula, albeit not very optimized, allows for changing the inner equation and constants without breaking.
    Once a proper formula has been settled on, this can be optimized away.
    */
    m_ScreenPosition.y += GRAVITY - (jumpEquation(m_JumpCount) - jumpEquation(m_JumpCount - 1));

    m_ScreenPosition.y = std::max(m_ScreenPosition.y, 0); //Keep it in the screen
    m_ScreenPosition.y = std::min(m_ScreenPosition.y, floor.getVerticalPosition() - m_ScreenPosition.w / 2); //Keep it above floor

    m_Rotation = DEFAULT_ROTATION - (JUMP_HEIGHT - m_JumpCount) * 10;

    if(m_IsAlive) //If player is still alright...
    {
        score = pipeManager.awardPoints(*this);
        checkForDeath(floor, pipeManager);
        if(!m_IsAlive)
        {
            m_Crash.play();
        }
    }

    if(!m_IsAlive && !m_HasCrashed) //If he hit something, wait until he hits the floor...
    {
        if((m_HasCrashed = floor.playerHitFloor(*this)))
        {
            m_DeathClock.start();
        }
    }

    if(m_HasCrashed)  //If he finally hit the floor, wait a second until counting it as dead
    {
        m_DeathClock.updateTime();
        m_Buried = m_DeathClock.getIntervalSeconds() > 1;
    }

    return score;
}

void Player::checkForDeath(Floor const& floor, PipeManager const& pipeManager)
{
    m_IsAlive = !(floor.playerHitFloor(*this) || pipeManager.playerHitPipes(*this));
}

void Player::reset()
{
    m_Rotation = 0;
    m_JumpCount = 0;
    m_IsAlive = true;
    m_HasCrashed = false;
    m_Buried = false;
    m_DeathClock.stop();
}

