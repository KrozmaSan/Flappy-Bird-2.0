#include "Saving.h"

PlayerData::PlayerData(const char* fileName)
:
    m_File(bootFile(fileName))
{
    load();
}

DataFile PlayerData::bootFile(const char* fileName)
{
    if(!myUtilities::fileExists(fileName))
    {
        DataFile::create(fileName);
        DataFile keep(fileName);
        keep.clearFile();
        keep.emptyBuffer();
        keep.write("score", 0);
        keep.write("skin", 0);
        keep.write("background", 0);
        keep.commit();
    }
    return DataFile(fileName);
}

void PlayerData::load()
{
    m_Score = m_File.parse<int>("score");
    m_Skin = m_File.parse<int>("skin");
    m_Background = m_File.parse<int>("background");
}

void PlayerData::clean()
{
    m_Score = m_Skin = m_Background = 0;
    save();
}

void PlayerData::save()
{
    m_File.clearFile();
    m_File.write("score", m_Score);
    m_File.write("skin", static_cast<int>(m_Skin));
    m_File.write("background", m_Background);
    m_File.commit();
}
