#ifndef L_GAME
#define L_GAME
class lGame
{
public:
    lGame(){};
    virtual ~lGame(){};
    virtual int MainLoop() = 0;
};

#endif // L_GAME
