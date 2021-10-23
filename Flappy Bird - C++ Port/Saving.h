#ifndef SAVING_H_INCLUDED
#define SAVING_H_INCLUDED

#include <string>

#include <myUtilities.h>
#include "Constants.h"

class PlayerData
{
public:
    explicit PlayerData(const char* fileName);
    explicit PlayerData(std::string const& fileName);

    DataFile const& getFile() const {return m_File;}
    SkinPositions getSelectedSkin() const {return m_Skin;}
    int getHighScore() const {return m_Score;}
    std::string getBackgroundPath() const {std::string foo; foo = "assets/backgrounds/bg" + std::to_string(m_Background) + ".png"; return foo;}
    int getBackgroundId() const {return m_Background;}

    void setSelectedSkin(const SkinPositions skin) {m_Skin = skin;}
    void setHighScore(const int score) {m_Score = score;}
    void trySetHighScore(const int possibleScore) { m_Score = std::max(m_Score, possibleScore); }
    void setBackground(const int id) {m_Background = id;}

    void save();
    void clean();
    void load();

protected:
    DataFile m_File;
    int m_Score;
    SkinPositions m_Skin;
    int m_Background;

    DataFile bootFile(const char* fileName);
};

#endif // SAVING_H_INCLUDED
