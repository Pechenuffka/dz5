//made by serGEY burakov
//(i am suffering)



#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <graphviz/gvc.h>

#define CTRL(A) (A & 037)
#define DELL_KEY 127
#define ENTER_KEY 10
void reverse(char s[]){
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
void itoa(int n, char s[]){
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
long int geta(int *counter){
    int cond = 0;
    int count = *counter;
    if (count == 0){
        cond = 1;
    }
    int c;
    long int a = 0;
    while(count || cond)
        {       
                c = getch();
                if (c == CTRL('c'))
                {
                        endwin();
                        exit(1);
                }
                if (c == '1' | c == '0')
                {
                        addch(c);
                        refresh();
                        a<<=1;
                        a += (c == '1')? 1 : 0;
                        if (cond)
                            (*counter)++;
                }
                if (c == DELL_KEY) 
                {       
                        int x,y;
                        for (getyx(stdscr, y, x);;)
                        {
                                move(y,x-1);
                                break;
                        }
                        delch();
                        a >>= 1;
                        refresh();
						count++;
                }
                
                if (c == CTRL('d') | c == ENTER_KEY)
                {
                        break;
                }
		if(!cond)
			count--;
        }
    	addch('\n');
        refresh();

        return a;
}

int main(int argc, char const *argv[])
{
    initscr();
    noecho();
    raw();

    mvprintw(getmaxy(stdscr)-2,0, "Input binary matrix without sparators, Enter for new line...");
    mvprintw(getmaxy(stdscr)-1,0, "Ctrl+C to iterupt!");
    move(0, 0);
    refresh();

    int counter=0;
    long int a = geta(&counter);
    long int arr[counter];
    arr[0] = a;
    for(int i = 1; i< counter; i++){
        arr[i] = geta(&counter);
    }
	
	int ap,bp;
	echo();
	scanw("%d", &ap);
	scanw("%d", &bp);
	noecho();
	
	arr[ap]|=1<<(counter-bp-1);
	arr[bp]|=1<<(counter-ap-1);

	int max_num, max_count=0, count=0;
	for(int i=0;i<counter;i++){
		int tmp = arr[i];
		int pos=0;
		while(tmp){
			if(tmp & 1<<pos){
				tmp^=1<<pos;
				count++;
			}
			pos++;
		}
		if(count>max_count){
			max_count=count;
			max_num=i;
		}
		count=0;
	}
	printw("%d\n",max_num);
    move(getmaxy(stdscr)-2,0);
    clrtoeol();
    move(getmaxy(stdscr)-1,0);
    clrtoeol();
    printw("Enter to exit; Ctrl+C to iterupt!; output > out.png, out.svg");

    GVC_t *gvc;
    gvc = gvContext();

    Agraph_t *g;
    g = agopen("g", Agundirected, 0);

    Agnode_t* nodes[counter];
    for(int i = 0; i < counter; i++){
        char tmp[5];
        itoa(i, tmp);
        nodes[i] = agnode(g, tmp, 1);
    }

    for(int i = 0; i < counter; i++){
        for(int j = i; j < counter; j++){
            if(arr[i] & (1<<(counter-j-1))){
                agedge(g, nodes[i], nodes[j], 0, 1);
            }
        }
    }

    gvLayout (gvc, g, "dot");
    gvRenderFilename (gvc, g, "png", "out.png");
    gvRenderFilename (gvc, g, "svg", "out.svg");
    gvFreeLayout(gvc, g);
    agclose(g);

    int c;
    for (c = getch(); (c != CTRL('c')) & (c != ENTER_KEY); c = getch()){}  
        if (c == CTRL('c'))
        {       
                endwin();
                exit(1);
        }
        if (c == ENTER_KEY)
        {
                endwin();
                exit(0);
        }
    return 0;
}
