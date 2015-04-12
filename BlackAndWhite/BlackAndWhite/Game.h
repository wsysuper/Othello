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
    int state;//���Է�ʲô��ɫ������
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
    Mapinfo map[9][9];//����
    int cb,cw;//�ڰ׿����µ�λ����
    int totb,totw;//Ŀǰ�ڰ������Ŀ
public:
    int state;
	friend class AiHm;
	friend class AI;
    CChildView* pwnd;
    ChessBoard();
    ~ChessBoard();
    bool CanSet(int color);//�Ƿ���λ����
    void SetXY(int x, int y, int color);//��������ĳλ�õ���ɫ
	void SimSetXY(int x, int y, int color);
    void TurnColor(int x, int y);
	void SimTurnColor(int x, int y);
    bool PutChess(int x, int y, int color);//����������xyλ��
	bool SimPutChess(int x, int y, int color);
    void UpDate();//����x��y�������кڰ׿��µ�λ��
    int GetNum(int color);//��ȡĿǰ���ӵ���Ŀ
    int GenAble(int color);//��ȡ�����µ���Ŀ
    int WhoWin();
    void DrawAll();
};

class GameControl{
protected:
    ChessManual chessmanual;
    int state;//״̬��Ŀǰ˭����
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
	Mapinfo *imap[9];//������Ϣ
public:
	AiHm() {for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];};
	int TurnNum(int x, int y) const;//���㵱ǰλ�ó�����
	void CalculateAIPlace();//�����㷨
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
	Mapinfo *imap[9];//������Ϣ
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