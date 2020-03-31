#include<iostream>

using namespace std;

int board[9][9]={   {8,0,0,   0,0,0,   0,0,0},  // Sudoku board. This board is the hardest sudoku in the world. Enter your values here to change the board.
                    {0,0,3,   6,0,0,   0,0,0},
                    {0,7,0,   0,9,0,   2,0,0},

                    {0,5,0,   0,0,7,   0,0,0},
                    {0,0,0,   0,4,5,   7,0,0},
                    {0,0,0,   1,0,0,   0,3,0},

                    {0,0,1,   0,0,0,   0,6,8},
                    {0,0,8,   5,0,0,   0,1,0},
                    {0,9,0,   0,0,0,   4,0,0}
                };

int num_list[9][9][9]; //To store possible values for each index

void initializeNumList(){ //Initializing the possible num_list
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                if(board[i][j])
                    num_list[i][j][k]=0; //For non-zero elements, i.e, number already present on board
                else
                    num_list[i][j][k]=k+1; //For zero elements, i.e, number not present on board
            }
        }
    }
}

void disp(int f){ // Displaying the board
    int cnt1=0,cnt2;
    system("cls");
    if(f)
        cout<<endl<<"\t\t\t\t\t\t\t INITIAL BOARD"<<endl<<endl;
    else
        cout<<endl<<"\t\t\t\t\t\t\t SOLVED BOARD"<<endl<<endl;
    for(int i=0;i<9;i++){
        cnt2=0;
        cout<<"\t\t\t\t\t\t";
        for(int j=0;j<9;j++){
            cnt2++;
            cout<<board[i][j];
            if(cnt2==3){
                cout<<"    ";
                cnt2=0;
            }
            else
                cout<<"  ";
        }
        cnt1++;
        if(cnt1==3){
            cout<<endl<<endl;
            cnt1=0;
        }
        else
            cout<<endl;
    }
    if(f)
        system("pause");
}

int* limits(int a){ // Calculation of indices limit for 3x3 matrix for every location
    int* pos=new int[2];
    if(a>=0 && a<=2){
        pos[0]=0;
        pos[1]=2;
    }
    else if(a>=3 && a<=5){
        pos[0]=3;
        pos[1]=5;
    }
    else{
        pos[0]=6;
        pos[1]=8;
    }
    return pos;
}

int checkAndModify(int x, int y){ //Modifies the num_list of the row, column and 3x3 matrix for index (x,y) and checks if the num_list is empty for index (x,y)
    int* xpos=limits(x),*ypos=limits(y);
    int cnt;
    for(int i=0;i<9;i++){ //Row checking and modification
        cnt=0;
        if(i!=y && board[x][i]==board[x][y])
            return 1;
        for(int k=0;k<9;k++){
            if(num_list[x][i][k]==board[x][y]) //Removing values from num_list row-wise that had the last placed value
                num_list[x][i][k]=0;
            if(!board[x][i] && num_list[x][i][k]) //For empty check
                cnt++;
        }
        if(!board[x][i] && !cnt)
            return 1;
    }
    for(int i=0;i<9;i++){ //Column checking and modification
        cnt=0;
        if(i!=x && board[i][y]==board[x][y])
            return 1;
        for(int k=0;k<9;k++){
            if(num_list[i][y][k]==board[x][y]) //Removing values from num_list column-wise that had the last placed value
                num_list[i][y][k]=0;
            if(!board[i][y] && num_list[i][y][k]) //For empty check
                cnt++;
        }
        if(!board[i][y] && !cnt)
            return 1;
    }
    for(int i=xpos[0];i<=xpos[1];i++){ //3x3 matrix checking and modification
        for(int j=ypos[0];j<=ypos[1];j++){
            if(i!=x && j!=y && board[i][j]==board[x][y])
                return 1;
            cnt=0;
            for(int k=0;k<9;k++){
                if(num_list[i][j][k]==board[x][y]) //Removing values from num_list 3x3 matrix-wise that had the last placed value
                    num_list[i][j][k]=0;
                if(!board[i][j] && num_list[i][j][k]) //For empty check
                    cnt++;
            }
            if(!board[i][j] && !cnt)
                return 1;
        }
    }
    return 0;
}

void findNumList(){ // Calculating the possibilities for every index
    int* xpos,*ypos;
    initializeNumList();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(!board[i][j]){
                for(int m=0;m<9;m++){ //Row and column wise checking
                    for(int k=1;k<=9;k++){
                        if(board[i][m]==k || board[m][j]==k)
                            num_list[i][j][k-1]=0;
                    }
                }
                xpos=limits(i);
                ypos=limits(j);
                for(int m=xpos[0];m<=xpos[1];m++){ //3x3 matrix wise checking
                    for(int n=ypos[0];n<=ypos[1];n++){
                        for(int k=1;k<=9;k++){
                            if(board[m][n]==k){
                                num_list[i][j][k-1]=0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void place(){ //Places the only solution possible for every index
    int pos,place_cnt;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            place_cnt=0;
            pos=-1;
            for(int k=0;k<9;k++){
                if(num_list[i][j][k]){
                    place_cnt++;
                    pos=k;
                }
            }
            if(place_cnt==1){ //If single element found, it is the forced solution
                board[i][j]=num_list[i][j][pos];
                num_list[i][j][pos]=0;
                checkAndModify(i,j);
            }
        }
    }
}

int solver(){ //Backtracking by placing all possible values at every index
    int f=0; //Flag
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(!board[i][j]){ //Values which are 0
                for(int k=0;k<9;k++){
                    if(num_list[i][j][k]){ //Possible values other than 0
                        board[i][j]=num_list[i][j][k]; //Place the value and test
                        for(int m=0;m<9;m++) //Removing possibilities for the placed index
                            num_list[i][j][m]=0;
                        if(checkAndModify(i,j)){ //Failure found
                            board[i][j]=0; //Restore board
                            findNumList(); //Restore num_list
                            continue;
                        }
                        if(solver()){ //Recursion and success
                            f=1; //Flag triggered to break
                            break;
                        }
                        board[i][j]=0; //Failure found and hence restore board
                        findNumList(); //Restore num_list
                    }
                }
                if(!board[i][j]) //Return failure if the value is not found even after trying all possibilities
                    return 0;
            }
            if(f)
                break;
        }
        if(f)
            break;
    }
    return 1; //Return success if the whole board was found correctly
}

int main(){
    disp(1);
    findNumList(); //Finding the possible values
    place(); //Placing forced solutions
    solver();
    disp(0);
}
