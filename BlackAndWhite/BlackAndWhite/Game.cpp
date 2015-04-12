#include "stdafx.h"
#include "Game.h"
#include "BlackAndWhite.h"
#include "ChildView.h"
#include "Multicast.h"

GameControl::~GameControl(){
    if(chessboard != NULL) delete chessboard;
}
GameControl::GameControl()
{
    state = BLACK;
    winner = -1;
    //state = WHITE;
    chessboard = new ChessBoard();
}

int GameControl::GetState()
{
    return state;
}

void GameControl::TryPut(int x, int y)
{
    TRACE("me");
    if(state == GAMEOVER) 
    {
        return;
    }
    if(!chessboard->PutChess(x,y,state)) return;
    chessmanual.Add(x,y,state);
    chessboard->UpDate ();
    state = 1 - state;
    if(!chessboard->CanSet(state))
    {
        //某方无子可下
        if(!chessboard->CanSet(1-state)) 
        {
            winner = chessboard->WhoWin();
            state = GAMEOVER;
            if(winner == BLACK)
            {
                CDialog setDlg(IDD_BWIN);
                setDlg.DoModal();
            }else if(winner == WHITE)
            {
                CDialog setDlg(IDD_WWIN);
                setDlg.DoModal();
            }
        }else
        {
            CDialog setDlg(IDD_NOCHESS);
            setDlg.DoModal();
            state = 1 - state;
        }
    }
    chessboard->state = state;
    chessboard->pwnd->InvalidateRect(NULL);
    chessboard->pwnd->UpdateWindow();
}

void GameControl::Reloadmanual()
{
    if(chessboard == NULL) return;

    ChessBoard *chessboardtemp = new ChessBoard();
    chessboardtemp->pwnd = chessboard->pwnd;
    delete chessboard;
    chessboard = chessboardtemp;
    for(int i = 1; i <= chessmanual.sum; i++)
    {
        chessboard->PutChess(chessmanual.cm[i].x,chessmanual.cm[i].y,chessmanual.cm[i].color);
        chessboard->state = state;
        chessboard->UpDate ();
    }
    chessboard->pwnd->InvalidateRect(NULL);
    chessboard->pwnd->UpdateWindow();
}
void GameControl::Regret()
{
    if(state == GAMEOVER) return;
    if(chessmanual.Del())
    {
        Reloadmanual();
        state = 1 - state;
        if(!chessboard->CanSet(state))
        {
            state = 1 - state;
        }
        chessboard->state = state;
    }
    if(chessboard != NULL) 
    {
        chessboard->pwnd->InvalidateRect(NULL);
        chessboard->pwnd->UpdateWindow();
    }
}
bool GameControl::WriteCM(std::ofstream &ofile)
{
    int sum = chessmanual.sum;
    ofile << sum << std::endl;
    for(int i = 1; i <= sum; ++i)
    {
        ofile << chessmanual.cm[i].x << ' ' << chessmanual.cm[i].y << ' ' << chessmanual.cm[i].color << std::endl;
    }
    return true;
}

bool GameControl::ReadCM(std::ifstream &ifile)
{
    int sum;
    ifile >> sum;
    for(int i = 1; i <= sum; ++i)
    {
        ifile >> chessmanual.cm[i].x >> chessmanual.cm[i].y >> chessmanual.cm[i].color;
    }
    chessmanual.sum = sum;
    chessmanual.num = sum;
    if(chessmanual.cm[sum].color == WHITE)
    {
        state = BLACK;
    }
    else
    {
        state = WHITE;
    }
    if(!chessboard->CanSet(state))
        {
            state = 1 - state;
        }
    Reloadmanual();
    chessboard->state = state;
    return true;
}
bool ChessBoard::PutChess(int x, int y, int color)
{
    if(map[x][y].color != BLANK) return false;
    if(WHITE == color)
    {
        if(!(map[x][y].state & WHITE_ABLE)) return false;
    }else
    {
        if(!(map[x][y].state & BLACK_ABLE)) return false;
    }
    SetXY(x,y,color);
    int i;
    for(i = x-1; i >= 1; --i)
    {
        if(i < 1 || map[i][y].color != 1 - color) break;
    }
    if(i >= 1 && map[i][y].color == color)
    {
        for(int j = i+1; j < x; ++j)
        {
            TurnColor(j,y);
        }
    }

    for(i = x+1; i <= 8; ++i)
    {
        if(i > 8 || map[i][y].color != 1 - color) break;
    }
    if(i <= 8 && map[i][y].color == color)
    {
        for(int j = i-1; j > x; --j)
        {
            TurnColor(j,y);
        }
    }

    for(i = y-1; i >= 1; --i)
    {
        if(i < 1 || map[x][i].color != 1 - color) break;
    }
    if(i >= 1 && map[x][i].color == color)
    {
        for(int j = i+1; j < y; ++j)
        {
            TurnColor(x,j);
        }
    }

    for(i = y+1; i <= 8; ++i)
    {
        if(i > 8 || map[x][i].color != 1 - color) break;
    }
    if(i <= 8 && map[x][i].color == color)
    {
        for(int j = i-1; j > y; --j)
        {
            TurnColor(x,j);
        }
    }

    for(i = 1; x+i<=8 && y+i <= 8; ++i)
    {
        if(x+i>8 || y +i > 8 || map[x+i][y+i].color != 1 - color) break;
    }

    if(x+i <=8 && y+i <= 8 && map[x+i][y+i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            TurnColor(x+j,y+j);
        }
    }

    for(i = 1; x+i<=8 && y-i >= 1; ++i)
    {
        if(x+i>8 || y -i < 1 || map[x+i][y-i].color != 1 - color) break;
    }

    if(x+i <=8 && y-i >= 1 && map[x+i][y-i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            TurnColor(x+j,y-j);
        }
    }

    for(i = 1; x-i>=1 && y+i <= 8; ++i)
    {
        if(x-i<1 || y +i > 8 || map[x-i][y+i].color != 1 - color) break;
    }

    if(x-i >=1 && y+i <= 8 && map[x-i][y+i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            TurnColor(x-j,y+j);
        }
    }

    for(i = 1; x-i>=1 && y-i >= 1; ++i)
    {
        if(x-i<1 || y -i < 1 || map[x-i][y-i].color != 1 - color) break;
    }

    if(x-i >=1 && y-i >= 1 && map[x-i][y-i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            TurnColor(x-j,y-j);
        }
    }

    return true;
}

bool ChessBoard::SimPutChess(int x, int y, int color)
{
    if(map[x][y].color != BLANK) return false;
    if(WHITE == color)
    {
        if(!(map[x][y].state & WHITE_ABLE)) return false;
    }else
    {
        if(!(map[x][y].state & BLACK_ABLE)) return false;
    }
    SimSetXY(x,y,color);
    int i;
    for(i = x-1; i >= 1; --i)
    {
        if(i < 1 || map[i][y].color != 1 - color) break;
    }
    if(i >= 1 && map[i][y].color == color)
    {
        for(int j = i+1; j < x; ++j)
        {
            SimTurnColor(j,y);
        }
    }

    for(i = x+1; i <= 8; ++i)
    {
        if(i > 8 || map[i][y].color != 1 - color) break;
    }
    if(i <= 8 && map[i][y].color == color)
    {
        for(int j = i-1; j > x; --j)
        {
            SimTurnColor(j,y);
        }
    }

    for(i = y-1; i >= 1; --i)
    {
        if(i < 1 || map[x][i].color != 1 - color) break;
    }
    if(i >= 1 && map[x][i].color == color)
    {
        for(int j = i+1; j < y; ++j)
        {
            SimTurnColor(x,j);
        }
    }

    for(i = y+1; i <= 8; ++i)
    {
        if(i > 8 || map[x][i].color != 1 - color) break;
    }
    if(i <= 8 && map[x][i].color == color)
    {
        for(int j = i-1; j > y; --j)
        {
            SimTurnColor(x,j);
        }
    }

    for(i = 1; x+i<=8 && y+i <= 8; ++i)
    {
        if(x+i>8 || y +i > 8 || map[x+i][y+i].color != 1 - color) break;
    }

    if(x+i <=8 && y+i <= 8 && map[x+i][y+i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            SimTurnColor(x+j,y+j);
        }
    }

    for(i = 1; x+i<=8 && y-i >= 1; ++i)
    {
        if(x+i>8 || y -i < 1 || map[x+i][y-i].color != 1 - color) break;
    }

    if(x+i <=8 && y-i >= 1 && map[x+i][y-i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            SimTurnColor(x+j,y-j);
        }
    }

    for(i = 1; x-i>=1 && y+i <= 8; ++i)
    {
        if(x-i<1 || y +i > 8 || map[x-i][y+i].color != 1 - color) break;
    }

    if(x-i >=1 && y+i <= 8 && map[x-i][y+i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            SimTurnColor(x-j,y+j);
        }
    }

    for(i = 1; x-i>=1 && y-i >= 1; ++i)
    {
        if(x-i<1 || y -i < 1 || map[x-i][y-i].color != 1 - color) break;
    }

    if(x-i >=1 && y-i >= 1 && map[x-i][y-i].color == color)
    {
        for(int j = 1; j < i; ++j)
        {
            SimTurnColor(x-j,y-j);
        }
    }

    return true;
}

void ChessBoard::SetXY(int x, int y, int color)
{
    map[x][y].color = color;
    map[x][y].state = DIS_ABLE;
    pwnd->DrawChess(x,y,map[x][y].color);
}

void ChessBoard::SimSetXY(int x, int y, int color)
{
    map[x][y].color = color;
    map[x][y].state = DIS_ABLE;
}

void ChessBoard::TurnColor(int x, int y)
{
    if(map[x][y].color == BLANK) return;
    map[x][y].color = 1 - map[x][y].color;
    pwnd->DrawChess(x,y,map[x][y].color);
}

void ChessBoard::SimTurnColor(int x, int y)
{
    if(map[x][y].color == BLANK) return;
    map[x][y].color = 1 - map[x][y].color;
}

bool ChessBoard::CanSet(int color)
{
    TRACE3("%d%d%d",color,cw,cb);
    if(color == WHITE)
    {
        if(cw > 0) return true;
        else return false;
    }
    {
        if(cb > 0) return true;
        else return false;
    }
}

int ChessBoard::GetNum(int color)
{
    if(color == WHITE)
    {
        return totw;
    }
    if(color == BLACK)
    {
        return totb;
    }
    return 0;
}

void ChessBoard::DrawAll()
{
    if(pwnd == NULL) return;
    pwnd->DrawState(state);
    pwnd->DrawNum(totb,totw);
    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            pwnd->DrawChess(i,j,map[i][j].color);
            if(state == BLACK)
            {
                if(map[i][j].state & BLACK_ABLE) pwnd->DrawTip(i,j);
            }else
            {
                if(map[i][j].state & WHITE_ABLE) pwnd->DrawTip(i,j);
            }
        }
    }
}

void ChessBoard::UpDate ()
{
    totb = 0;
    totw = 0;
    cb = 0;
    cw = 0;
    bool flag;
    int i;
    int color;
    for(int x = 1; x <= 8; ++x)
    {
        for(int y = 1; y <= 8; ++y)
        {
            if(map[x][y].color != BLANK)
            {
                if(map[x][y].color == WHITE) totw++;
                else totb++;
            }
            else
            {
                map[x][y].state = DIS_ABLE;
                if(x>=3)
                {
                    color = map[x-1][y].color;
                    i = x-2;
                    while(i > 0 && map[i][y].color == color) --i;
                    if(i>0 && map[i][y].color == 1 - color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }
                if(y>=3)
                {
                    color = map[x][y-1].color;
                    i = y-2;
                    while(i > 0 && map[x][i].color == color) --i;
                    if(i>0 && map[x][i].color == 1 - color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }
                if(x<=6)
                {
                    color = map[x+1][y].color;
                    i = x+2;
                    while(i <= 8 && map[i][y].color == color) ++i;
                    if(i<=8 && map[i][y].color == 1 - color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }
                if(y<=6)
                {
                    color = map[x][y+1].color;
                    i = y+2;
                    while(i <= 8 && map[x][i].color == color) ++i;
                    if(i<=8 && map[x][i].color == 1 - color){
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }

                if(x >= 3 && y >= 3)
                {
                    color = map[x-1][y-1].color;
                    i = 2;
                    while(x-i >= 1 && y - i >= 1 && map[x-i][y-i].color == color) ++i;
                    if(x-i >= 1 && y - i >= 1 && map[x-i][y-i].color == 1-color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }

                if(x >= 3 && y <= 6)
                {
                    color = map[x-1][y+1].color;
                    i = 2;
                    while(x-i >= 1 && y + i <= 8 && map[x-i][y+i].color == color) ++i;
                    if(x-i >= 1 && y + i <= 8 && map[x-i][y+i].color == 1-color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }

                if(x <= 6 && y >= 3)
                {
                    color = map[x+1][y-1].color;
                    i = 2;
                    while(x+i <= 8 && y - i >= 1 && map[x+i][y-i].color == color) ++i;
                    if(x+i <= 8 && y - i >= 1 && map[x+i][y-i].color == 1-color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }

                if(x <= 6 && y <= 6)
                {
                    color = map[x+1][y+1].color;
                    i = 2;
                    while(x+i <= 8 && y + i <= 8 && map[x+i][y+i].color == color) ++i;
                    if(x+i <= 8 && y + i <= 8 && map[x+i][y+i].color == 1-color)
                    {
                        if(color == WHITE) map[x][y].state |= BLACK_ABLE;
                        else map[x][y].state |= WHITE_ABLE;
                    }
                }
                if(map[x][y].state & WHITE_ABLE) cw++;
                if(map[x][y].state & BLACK_ABLE) cb++;
            }
        }
    }
}

int ChessBoard::WhoWin()
{
    if(totb > totw) return BLACK;
    else if(totb < totw) return WHITE;
    else return TIED;
}
ChessBoard::ChessBoard()
{
    state = BLACK;
    pwnd = NULL;
    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            map[i][j].color = BLANK;
            map[i][j].state = DIS_ABLE;
        }
    }
    map[4][4].color = WHITE;
    map[4][5].color = BLACK;
    map[5][5].color = WHITE;
    map[5][4].color = BLACK;
    map[3][4].state = BLACK_ABLE;
    map[4][3].state = BLACK_ABLE;
    map[6][5].state = BLACK_ABLE;
    map[5][6].state = BLACK_ABLE;
    map[3][5].state = WHITE_ABLE;
    map[5][3].state = WHITE_ABLE;
    map[4][6].state = WHITE_ABLE;
    map[6][4].state = WHITE_ABLE;
    totb = 2;
    totw = 2;
    cb = 4;
    cw = 4;
}

ChessBoard::~ChessBoard()
{
    pwnd = NULL;
}


int AiHm::TurnNum(int x, int y) const{
	int num = 0;
	int xm[8] = {-1,1,0, 0,-1,1,-1, 1};
	int ym[8] = { 0,0,1,-1,-1,1, 1,-1};
	for (int i = 0; i < 8; i++) {
		int count = 1;
		if(x + count * xm[i] >= 1 && x + count * xm[i] <= 8 && y + count * ym[i] >= 1 && y + count * ym[i] <= 8)
		{
			while(imap[x + count * xm[i]][y + count * ym[i]].color + state == 1)
			{
				count++;
				if(!(x + count * xm[i] >= 1 && x + count * xm[i] <= 8 && y + count * ym[i] >= 1 && y + count * ym[i] <= 8))
				{count = 1; break;}
			}
		if(imap[x + count * xm[i]][y + count * ym[i]].color == BLANK)
			count = 1;
		}
		count--;
		num += count;
	}
	return num;
}

void AiHm::CalculateAIPlace() {
	vector<int> able;//记录哪些地方是能下的
	for (int i = 1; i < 9; i++)
		for (int j = 1; j < 9; j++)
		{
			if (!(imap[i][j].state & (state + 1)) && imap[i][j].state || imap[i][j].state == 3)
				able.push_back(i * 10 + j);
		}
	if (able.empty()) 
		return;
    if (CChildView::dif == 0)
    {srand( (unsigned)time( NULL ) ); 
	int result = rand() % able.size();
	TryPut(able[result] / 10, able[result] % 10);
    return;
    }
	int score[60];//记录对应点的分数
	for (int i = 0; i < able.size(); i++)
	{
		int x = able[i] / 10; int y = able[i] % 10;
		score[i] = 0;
		if (x * y == 1 || x * y == 64 || x == 1 && y == 8 || x == 8 && y == 1)
		{
			score[i] = 10000;
			continue;
		}
		if (x == 1)
		{
			if (imap[1][y - 1].color == BLANK && imap[1][y + 1].color == BLANK)
				score[i] = 100;
			else if (imap[1][y - 1].color + state == 1 && imap[1][y + 1].color + state == 1)
				score[i] = 1000;
		}
		if (x == 8)
		{
			if (imap[8][y - 1].color == BLANK && imap[8][y + 1].color == BLANK)
				score[i] = 100;
			else if (imap[8][y - 1].color + state == 1 && imap[8][y + 1].color + state == 1)
				score[i] = 1000;
		}
		if (y == 1)
		{
			if (imap[x - 1][1].color == BLANK && imap[x + 1][1].color == BLANK)
				score[i] = 100;
			else if (imap[x - 1][1].color + state == 1 && imap[x + 1][1].color + state == 1)
				score[i] = 1000;
		}
		if (y == 8)
		{
			if (imap[x - 1][8].color == BLANK && imap[x + 1][8].color == BLANK)
				score[i] = 100;
			else if (imap[x - 1][8].color + state == 1 && imap[x + 1][8].color + state == 1)
				score[i] = 1000;
		}
		if (x == 2 || x == 7)
			score[i] -= 100;
		if (y == 2 || y == 7)
			score[i] -= 100;
		score[i] += TurnNum(x, y);
	}
	vector<int> max;//记录分数最大值点
	int maxscore = score[0];
	for (int i = 0; i < able.size(); i++)
	{
		if (score[i] == maxscore)
			max.push_back(able[i]);
		else if (score[i] > maxscore)
		{
			maxscore = score[i];
			max.clear();
			max.push_back(able[i]);
		}
	}
	srand( (unsigned)time( NULL ) ); 
	int result = rand() % max.size();
	TryPut(max[result] / 10, max[result] % 10);
}

void AiHm::TryPut(int x, int y)
{
    if(state == GAMEOVER) 
    {
        return;
    }
    if(!chessboard->PutChess(x,y,state))
		return;
    chessmanual.Add(x,y,state);
    chessboard->UpDate ();
    state = 1 - state;
    if(!chessboard->CanSet(state))
    {
        //某方无子可下
        if(!chessboard->CanSet(1-state)) 
        {
            winner = chessboard->WhoWin();
            state = GAMEOVER;
            if(winner == BLACK)
            {
                CDialog setDlg(IDD_BWIN);
                setDlg.DoModal();
            }
			else if(winner == WHITE)
            {
                CDialog setDlg(IDD_WWIN);
                setDlg.DoModal();
            }
        }
		else
        {
            CDialog setDlg(IDD_NOCHESS);
            setDlg.DoModal(); 
			state = 1 - state;
        }
	}
	if (state == WHITE)
	{
		CalculateAIPlace();
	}
    chessboard->state = state;
    chessboard->pwnd->InvalidateRect(NULL);
    chessboard->pwnd->UpdateWindow();
}
void AiHm::Regret()
{
    if(state == GAMEOVER) return;
    if(chessmanual.Del())
    {
        if(chessmanual.Del())
        {
            Reloadmanual();
            for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];
            if(chessboard != NULL) 
            {
                chessboard->pwnd->InvalidateRect(NULL);
                chessboard->pwnd->UpdateWindow();
             }
        }
    }
}
ChessManual::ChessManual():num(0),sum(0)
{
    for(int i = 0; i < 65; i++)
    {
        cm[i].color = BLANK;
        cm[i].x = 0;
        cm[i].y = 0;
    }
}

ChessManual::~ChessManual()
{
}

bool ChessManual::Back()
{
    if(num > 1) 
    {
        num--;
        return true;
    }else
    {
        return false;
    }
}

bool ChessManual::Forward()
{
    if(num < sum) 
    {
        num++;
        return true;
    }else
    {
        return false;
    }
}

bool ChessManual::Add(int x, int y, int color)
{
    if(sum < 60)
    {
        sum++;
        num = sum;
        cm[sum].x = x;
        cm[sum].y = y;
        cm[sum].color = color;
        return true;
    }
    else
    {
        return false;
    }
}

bool ChessManual::Del()
{
    if(sum > 0) 
    {
        sum--;
        num = sum;
        return true;
    }else
    {
        return false;
    }
}

Manualinfo ChessManual::Info(int n)
{
    if(n > 0 && n < sum) return cm[n];
}

/////////////////////////////////////////////////////////////////

NetHm::NetHm()
{
    step = 0;
    Inithread = (HANDLE)_beginthread(initial,0,NULL);
}

void NetHm::TryPut(int x, int y)
{
    if(state == GAMEOVER) 
    {
        return;
    }
    if(color != state) return;
    if(!chessboard->PutChess(x,y,state)) return;
    chessmanual.Add(x,y,state);
    chessboard->UpDate ();
    state = 1 - state;
    if(!chessboard->CanSet(state))
    {
        //某方无子可下
        if(!chessboard->CanSet(1-state)) 
        {
            winner = chessboard->WhoWin();
            state = GAMEOVER;
            if(winner == BLACK)
            {
                CDialog setDlg(IDD_BWIN);
                setDlg.DoModal();
            }else if(winner == WHITE)
            {
                CDialog setDlg(IDD_WWIN);
                setDlg.DoModal();
            }
        }else
        {
            CDialog setDlg(IDD_NOCHESS);
            setDlg.DoModal();
            state = 1 - state;
        }
    }
    chessboard->state = state;
    chessboard->pwnd->InvalidateRect(NULL);
    chessboard->pwnd->UpdateWindow();

    char msg[4]={PUTCHESS,step,x,y};
    do_send(msg,4);
    step++;
}


void NetHm::Regret()
{
    if(state == GAMEOVER) return;
    if(chessmanual.Del())
    {
        Reloadmanual();
        state = 1 - state;
        if(!chessboard->CanSet(state))
        {
            state = 1 - state;
        }
        chessboard->state = state;
    }
    if(chessboard != NULL) 
    {
        chessboard->pwnd->InvalidateRect(NULL);
        chessboard->pwnd->UpdateWindow();
    }

    char msg[2]={REGRET,step};
    do_send(msg,2);
    step--;
}

void Watch::TryPut(int x, int y)
{
}
void Watch::Regret()
{
}
NetHm::~NetHm()
{
    _endthread();
   	WSACleanup();

}
Watch::~Watch()
{
}

Watch::Watch()
{
}


void AI::Regret(){
    if(chessmanual.Del())
    {
        if(chessmanual.Del())
        {
            Reloadmanual();
            for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];
            if(chessboard != NULL) 
            {
                chessboard->pwnd->InvalidateRect(NULL);
                chessboard->pwnd->UpdateWindow();
             }
        }
    }
}

void AI::TryPut(int x, int y) {
    if(state == GAMEOVER) 
    {
        return;
    }
    if(!chessboard->PutChess(x,y,state))
		return;
    chessmanual.Add(x,y,state);
    chessboard->UpDate ();
    state = 1 - state;
    if(!chessboard->CanSet(state))
    {
        //某方无子可下
        if(!chessboard->CanSet(1-state)) 
        {
            winner = chessboard->WhoWin();
            state = GAMEOVER;
            if(winner == BLACK)
            {
                CDialog setDlg(IDD_BWIN);
                setDlg.DoModal();
            }
			else if(winner == WHITE)
            {
                CDialog setDlg(IDD_WWIN);
                setDlg.DoModal();
            }
        }
		else
        {
            CDialog setDlg(IDD_NOCHESS);
            setDlg.DoModal(); 
			state = 1 - state;
        }
	}
	if (state == WHITE)
	{
		Cal();
	}
    chessboard->state = state;
    chessboard->pwnd->InvalidateRect(NULL);
    chessboard->pwnd->UpdateWindow();
}

int AI::Eval(int x, int y) {
	return posscore[x][y];
}

int AI::Search(int depth, int alpha, int beta, int &max, int color) { 
	ChessBoard temp;
	temp = *chessboard;
	if (state == GAMEOVER || depth <= 0) { 
		return 0; 
	} 
	vector<int> able;//记录哪些地方是能下的
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			if (!(imap[i][j].state & (state + 1)) && imap[i][j].state || imap[i][j].state == 3)
				able.push_back(i * 10 + j);
		}
	}
	if (able.empty()) 
	{
		state = 1 - state;
		score = -Search(depth - 1, -beta, -alpha, max, 1-color); 
		if(score > alpha) {
			alpha = score;
		}
		state = 1 - state;
	}
	else 
	{
		for (int i = 0; i < able.size(); i++)
		{
			int x = able[i] / 10; int y = able[i] % 10;
			chessboard->SimPutChess(x,y,color);
			chessboard->UpDate ();
			state = 1 - state;
			for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];
			score = Eval(x,y) - Search(depth - 1, -beta, -alpha, max, 1-color); 
			if (score > alpha) { 
				alpha = score; 
				if(depth == dif) 
				{
					max = x * 10 + y;
				}
			} 
			state = 1 - state;
			*chessboard = temp;
			for(int i = 0; i < 9; i++) imap[i] = chessboard->map[i];
			if (alpha >= beta) { 
				break; 
			} 
		}
	}
	return alpha;
}

void AI::Cal() {
	dif = 5;
	Search(dif,-MAXNUM,MAXNUM,max,0);
	TryPut(max/10,max%10);
}

