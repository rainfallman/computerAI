#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
struct Othellomap
{
    int x;
    int y;
    char color;
    bool canplace;
    int openpoint;
};

int ansx,ansy;
struct Othellomap playmap[9][9];

int direct[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

bool searchcanswap(int dir,int x,int y)
{
    int tx=x,ty=y;
    for(;(tx<9&&tx>0)&&(ty<9&&ty>0);)
    {

        tx+=direct[dir][0];
        ty+=direct[dir][1];
        if(playmap[tx][ty].color=='E')
        {
            break;
        }
        if(playmap[x][y].color==playmap[tx][ty].color)
        {   
            return true;
        }
    }
    return false;
}

void swapchess(int dir,int x,int y)
{
    int tx=x+direct[dir][0];
    int ty=y+direct[dir][1];
    while((playmap[x][y].color!=playmap[tx][ty].color)&&playmap[tx][ty].color!='E')
    {
        playmap[tx][ty].color=playmap[x][y].color;
        tx+=direct[dir][0];
        ty+=direct[dir][1];
    }
}

void openpointpredict(int dir,int x,int y)
{
    int tx=x+direct[dir][0];
    int ty=y+direct[dir][1];
    
    //cout<<"predict from"<<x<<" "<<y<<"to"<<direct[dir][0]<<direct[dir][1]<<endl;
    while((playmap[x][y].color!=playmap[tx][ty].color)&&playmap[tx][ty].color!='E')
    {
        int count=0;
        for(int i=0;i<8;i++)
        {
            int ttx=tx+direct[i][0],tty=ty+direct[i][1];
            if(playmap[ttx][tty].color=='E'&&(ttx>0&&ttx<9&&tty>0&&tty<9))
                count++;
        }
        cout<<tx<<ty<<": "<<count<<"point"<<endl;
        playmap[x][y].openpoint+=count;
        tx+=direct[dir][0];
        ty+=direct[dir][1];
    }
}

void putplace(int x,int y)
{
    for(int i=0;i<8;i++)
    {
        bool check=searchcanswap(i,x,y);
        if(check==true)
        {
            swapchess(i,x,y);
        }

    }
}

bool canplacetry(int dir,int x,int y)
{
    int tx=x,ty=y,count=0;
    //cout<<"search from"<<x<<" "<<y<<"to"<<direct[dir][0]<<direct[dir][1]<<endl;
    tx+=direct[dir][0];
    ty+=direct[dir][1];
    for(;(tx<9&&tx>0)&&(ty<9&&ty>0);)
    {
        if(playmap[tx][ty].color==playmap[x][y].color)
        {
            //cout<<"others will do this for me"<<endl;
            break;
        }
        if(playmap[tx][ty].color!=playmap[x][y].color&&playmap[tx][ty].color!='E')
        {
            //cout<<"here might can place"<<endl;
            count++;
        }
        if(playmap[tx][ty].color=='E')
        {
            if(count>0)
            {
                playmap[tx][ty].canplace=true;
                //cout<<"here can place"<<endl;
            }
            //cout<<"i cant place"<<endl;
            break;
        }
        tx+=direct[dir][0];
        ty+=direct[dir][1];
    }
}

void searchcanplace(int x,int y)
{
    //cout<<"search"<<x<<" "<<y<<endl;
    for(int i=0;i<8;i++)
    {
        canplacetry(i,x,y);
    }
}

void countscore(int x,int y)
{
    for(int i=0;i<8;i++)
    {
        bool check=searchcanswap(i,x,y);
        if(check==true)
        {
            openpointpredict(i,x,y);
        }
    }
}

void AIsearch(char color)
{
    int tmpscore=10000;
    
    for(int x=1;x<9;x++)
    {
        for(int y=1;y<9;y++)
        {
            if(playmap[x][y].canplace==true)
            {
                playmap[x][y].color=color;
                countscore(x,y);
                if(tmpscore>playmap[x][y].openpoint)
                {
                    tmpscore=playmap[x][y].openpoint;
                    ansx=x;
                    ansy=y;
                }
                else if(tmpscore==playmap[x][y].openpoint)
                {
                    int tc=abs(x-4.5)+abs(y-4.5);
                    int ac=abs(ansx-4.5)+abs(ansy-4.5);
                    if(tc>ac)
                    {
                        ansx=x;
                        ansy=y;
                    }
                }
                playmap[x][y].color='E';
            }
        }
    }
    if(playmap[1][1].canplace==true||playmap[1][8].canplace==true||playmap[8][1].canplace==true||playmap[8][8].canplace==true)
    {
        if(playmap[1][1].canplace==true)
        {
            ansx=1;
            ansy=1;
            return;
        }
        if(playmap[8][1].canplace==true)
        {
            ansx=8;
            ansy=1;
            return;
        }
        if(playmap[1][8].canplace==true)
        {
            ansx=1;
            ansy=8;
            return;
        }
        if(playmap[8][8].canplace==true)
        {
            ansx=8;
            ansy=8;
            return;
        } 
    }
    if(playmap[3][1].canplace==true||playmap[1][3].canplace==true||playmap[3][3].canplace==true||playmap[6][1].canplace==true||playmap[6][3].canplace==true||playmap[8][3].canplace==true||playmap[1][6].canplace==true||playmap[3][6].canplace==true||playmap[3][8].canplace==true||playmap[6][6].canplace==true||playmap[6][8].canplace==true||playmap[8][6].canplace==true)
    {
        //left up
        if(playmap[1][3].canplace==true)
        {
            if(playmap[1][3].openpoint<(tmpscore+10))
            ansx=1;
            ansy=3;
            return;
        }
        if(playmap[3][1].canplace==true)
        {
            if(playmap[3][1].openpoint<(tmpscore+10))
            ansx=3;
            ansy=1;
            return;
        }
        if(playmap[3][3].canplace==true)
        {
            if(playmap[3][3].openpoint<(tmpscore+10))
            ansx=3;
            ansy=3;
            return;
        }
        //right up
        if(playmap[1][6].canplace==true)
        {
            if(playmap[1][6].openpoint<(tmpscore+10))
            ansx=1;
            ansy=6;
            return;
        }
        if(playmap[3][8].canplace==true)
        {
            if(playmap[3][8].openpoint<(tmpscore+10))
            ansx=3;
            ansy=8;
            return;
        }
        if(playmap[3][6].canplace==true)
        {
            if(playmap[3][6].openpoint<(tmpscore+10))
            ansx=3;
            ansy=6;
            return;
        }
        //left down
        if(playmap[6][1].canplace==true)
        {
            if(playmap[6][1].openpoint<(tmpscore+10))
            ansx=6;
            ansy=1;
            return;
        }
        if(playmap[6][3].canplace==true)
        {
            if(playmap[6][3].openpoint<(tmpscore+10))
            ansx=6;
            ansy=3;
            return;
        }
        if(playmap[8][3].canplace==true)
        {
            if(playmap[8][3].openpoint<(tmpscore+10))
            ansx=8;
            ansy=3;
            return;
        }
        //right down
        if(playmap[6][6].canplace==true)
        {
            if(playmap[6][6].openpoint<(tmpscore+10))
            ansx=6;
            ansy=6;
            return;
        }
        if(playmap[6][8].canplace==true)
        {
            if(playmap[6][8].openpoint<(tmpscore+10))
            ansx=6;
            ansy=8;
            return;
        }
        if(playmap[8][6].canplace==true)
        {
            if(playmap[8][6].openpoint<(tmpscore+10))
            ansx=8;
            ansy=6;
            return;
        }
    }
    while((ansx*ansy)==49||(ansx*ansy)==14||(ansx==2&&ansy==2))
    {
        int tmpx,tmpy;
        playmap[ansx][ansy].openpoint+=10;
        for(int x=1;x<9;x++)
        {
            for(int y=1;y<9;y++)
            {
                if(playmap[x][y].canplace==true)
                {
                    playmap[x][y].color=color;
                    countscore(x,y);
                    if(tmpscore>playmap[x][y].openpoint)
                    {
                        tmpscore=playmap[x][y].openpoint;
                        tmpx=x;
                        tmpy=y;
                    }
                    playmap[x][y].color='E';
                }
            }
        }
        if(tmpx!=ansx||tmpy!=ansy)
        {
            ansx=tmpx;
            ansy=tmpy;
        }
        else
            break;
    }

}


int main()
{
    char myside;

    for(int i=1;i<9;i++)
    {
        for(int j=1;j<9;j++)
        {
            playmap[i][j].x=i;
            playmap[i][j].y=j;
            playmap[i][j].color='E';
            playmap[i][j].canplace=false;
            playmap[i][j].openpoint=0;
        }
    }
    playmap[4][4].color='W';
    playmap[5][5].color='W';
    playmap[4][5].color='B';
    playmap[5][4].color='B';
    playmap[3][4].canplace=true;
    playmap[4][3].canplace=true;
    playmap[5][6].canplace=true;
    playmap[6][5].canplace=true;

    for(int i=1;i<9;i++)
    {
        for(int j=1;j<9;j++)
        {
            cout<<playmap[i][j].color<<" ";
        }
        cout<<endl;
    }
    cout<<"choose side"<<endl;
    cin>>myside;
    char inputx,inputy,intype;
    int placex,placey;
    int black=0,white=0;
    while(1)
    {
        cin>>intype>>inputy>>inputx;
        placey=inputy-'A'+1;
        placex=inputx-'0';
        
        if(playmap[placex][placey].canplace==true)
        {
            playmap[placex][placey].color=intype;
            putplace(placex,placey);


            //clear canplace
            for(int i=1;i<9;i++)
            {
                for(int j=1;j<9;j++)
                {
                    playmap[i][j].canplace=false;
                    playmap[i][j].openpoint=0;
                }
            }
            //search where can place
            int canplacecheck=0;
            for(int i=1;i<9;i++)
            {
                for(int j=1;j<9;j++)
                {
                    if(playmap[i][j].color!=intype&&playmap[i][j].color!='E')
                    {
                        searchcanplace(i,j);
                        canplacecheck++;
                    }
                }
            }
            if(canplacecheck==0)
            {
                if(intype=='B') intype='W';
                else if(intype=='W') intype='B';
                for(int i=1;i<9;i++)
                {
                    for(int j=1;j<9;j++)
                    {
                        if(playmap[i][j].color!=intype&&playmap[i][j].color!='E')
                        {
                            searchcanplace(i,j);
                        }
                    }
                }
            }
            for(int i=1;i<9;i++)
            {
                for(int j=1;j<9;j++)
                {
                    if(playmap[i][j].color=='B')
                        black++;
                    if(playmap[i][j].color=='W')
                        white++;
                    cout<<playmap[i][j].color<<" ";
                }
                cout<<endl;
            }

            for(int i=1;i<9;i++)
            {
                for(int j=1;j<9;j++)
                {
                    if(playmap[i][j].canplace==true)
                        cout<<"1"<<" ";
                    else
                        cout<<"0"<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
            //search where to put
            if(intype=='B')
                AIsearch('W');
            if(intype=='W')
                AIsearch('B');
            for(int i=1;i<9;i++)
            {
                for(int j=1;j<9;j++)
                {
                    cout<<playmap[i][j].openpoint<<" ";
                }
                cout<<endl;
            }
            if(intype=='B')
                cout<<"It's turn to White"<<endl;
            if(intype=='W')
                cout<<"It's turn to Black"<<endl;

            cout<<"best next choose step is :"<<char(ansy+64)<<ansx<<endl;
            cout<<"black"<<black<<"white"<<white<<endl;
            black=0;
            white=0;
        }
        else cout<<"Wrong place to put pleace check again"<<endl;
    }
}
