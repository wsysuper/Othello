#ifndef GameBW
#define GameBW
#define BLACK 1
#define WHITE 0
#define BLANK 2
#define BLACK_ABLE 1
#define WHITE_ABLE 2
#define DIS_ABLE 0
#define GAMEOVER 4
#define TIED 2
#include <time.h>
#include <vector>
#include <fstream>
class CChildView;
using namespace std;

#define MAXNUM 32000

//---------------------------------------------------------------------------
const int posscore[9][9]=
{
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,50,-10,  1,  1,  1,  1,-10, 50},
    {0,-10,-30,-10,-10,-10,-10,-30,-10},
    { 0, 1,-10,  5,  5,  5,  5,-10,  0},
    { 0, 1,-10,  5,  1,  1,  5,-10,  0},
    { 0, 1,-10,  5,  1,  1,  5,-10,  0},
    { 0, 1,-10,  5,  5,  5,  5,-10,  0},
    {0,-10,-30,-10,-10,-10,-10,-30,-10},
    { 0,50,-10,  1,  1,  1,  1,-10, 50},
}; 

struct Mapinfo{
    int color;
    int state;//可以放什么颜色的棋子
};

struct Manualinfo{
    int x,y;
    int color;
};

class ChessManual{
private:
    int num,sum;
    Manualinfo cm[65];
public:
    friend class GameControl;
    bool Back();
    bool Forward();
    bool Add(int x, int y, int color);
    bool Del();
    Manualinfo Info(int n);
    ~ChessManual();
    ChessManual();
};

class ChessBoard{
private:
    Mapinfo map[9][9];//棋盘
    int cb,cw;//黑白可以下的位置数
    int totb,totw;//目前黑白棋的数目
public:
    int state;
	friend class AiHm;
	friend class AI;
    CChildView* pwnd;
    ChessBoard();
    ~ChessBoard();
    bool CanSet(int color);//是否有位置下
    void SetXY(int x, int y, int color);//设置棋盘某位置的颜色
	void SimSetXY(int x, int y, int color);
    void TurnColor(int x, int y);
	void SimTurnColor(int x, int y);
    bool PutChess(int x, int y, int color);//讲棋子下在xy位置
	bool SimPutChess(int x, int y, int color);
    void UpDate();//更新x，y所在行列黑白可下的位置
    int GetNum(int color);//获取目前棋子的数目
    int GenAble(int color);//获取可以下的数目
    int WhoWin();
    void DrawAll();
};

class GameControl{
protected:
    ChessManual chessmanual;
    int state;//状态，目前谁在下
    int winner;
public:
    int color;
    int step;
    bool WriteCM(std::ofstream &ofile);
    bool ReadCM(std::ifstream &ifile);
    ChessBoard * chessboard;
    GameControl();
    ~GameControl();
    int GetState();
    virtual void Reloadmanual();
    virtual void Regret();
    virtual void TryPut(int x,int y);
};

class AiHm:public GameControl{
	Mapinfo *imap[9];//棋盘信息
public:
	AiHm() {for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];};
	int TurnNum(int x, int y) const;//计算当前位置吃子数
	void CalculateAIPlace();//下棋算法
	virtual void TryPut(int x,int y);
    virtual void Regret();
	~AiHm(){};
};

class NetHm:public GameControl{
public:
	virtual void TryPut(int x,int y);
    virtual void Regret();
	NetHm();
    ~NetHm();
private:
	HANDLE Inithread;
};

class Watch:public NetHm{
public:
    virtual void TryPut(int x, int y);
    virtual void Regret();
    Watch();
    ~Watch();
};

class AI:public GameControl{
	Mapinfo *imap[9];//棋盘信息
	int max;
	int score;
	int dif;
public:
	AI() {for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];max = 0;score = 0;};
	int Eval(int x, int y);
	int Search(int depth, int alpha, int beta, int &max, int color);
	void Cal();
	virtual void TryPut(int x,int y);
    virtual void Regret();
	~AI(){};
};

#endif