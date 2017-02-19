
#include "RM.h"

using namespace std;

void LoadQuotedString(std::istream &in,std::string &str)
{
    char c;

    do
    {
        in.get(c);
    }while(c != '\"');

    in.get(c);
    while(c != '\"')
    {
        str.push_back(c);
        in.get(c);
    }
}

GLuint RM::GetImage(const string &filename)
{
    for (unsigned int i=0; i < LoadedImages.size(); i++)
    {
        if (LoadedImages[i].first == filename)
        {
            return LoadedImages[i].second;
        }
    }

    GLuint ID = loadTexture(filename);
    pair<string,GLuint> newPair(filename, ID);

    LoadedImages.push_back(newPair);
    return LoadedImages.back().second;
}

RM::~RM()
{
    for (unsigned int i=0; i < LoadedImages.size(); i++)
    {
        glDeleteTextures(1,&LoadedImages[i].second);
    }

    FreeWaves();
}
