//Andrea Del Corto e Gabriele Giovanelli 3IA
//Gioco della vita ver6.0 - con toro
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>//per utilizzare la funzione time()
#define NR 30
#define NC 70
//PROTOTYPE funzioni
char controlSelectMenu(char Start,char Fine,char esci);
int stampMat(int mat[NR][NC],int nStamp,FILE *f,int tipo);
int viciniToro(int posRT,int posCT,int mondo[NR][NC]);
int statoCell(int posR,int posC,int mondo[NR][NC]);
void menu(int tip);
void controllMac(float *x,long *y,long mini,long mas);
void randDefine(int mat[NR][NC],int n);
void puliziaMat(int mat[NR][NC],int n);
void cellPrec(int *i,int *j);
char manDefine(int mat[NR][NC]);
char controlCaratManuale(int *cella);
void mondoSucc(int mat1[NR][NC],int mat2[NR][NC]);
void mondoPartenza(int mat1[NR][NC],int mat2[NR][NC]);
void letturaFile(char *name,int mat[NR][NC]);
char configPart(int mat[NR][NC]);
int getch();
//-----------------------------------------------
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int getch() {
    struct termios oldtc, newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}


char controlCaratManuale(int *cella){
  char appoggio;
      do{
        appoggio = getch();
      }while(appoggio!='0' && appoggio!='1' && appoggio!='f' && appoggio!='F' && appoggio!='x' && appoggio!='X' && appoggio!='c' && appoggio!='C' && appoggio!='r' && appoggio!='R');
      switch(appoggio){
        case '0':
          *cella = 0;
          return appoggio;
        break;

        case '1':
          *cella = 1;
          return appoggio;
        break;
        case 'r':
        case 'R':
          *cella = 0;
          return appoggio;
        break;
        case 'f':
        case 'F':
          *cella = 0;
          return appoggio;
        break;
        case 'C':
        case 'c':
          return appoggio;
        break;

        case 'x':
        case 'X':
          *cella = 0;
          return appoggio;
        break;
      }
}
char controlSelectMenu(char Start,char Fine,char esci){
  char carat;
      printf("\n");
      do{
        carat = getch();
        if( (carat>=Start && carat<=Fine) || carat==esci ){
          printf(" %c",carat);
          return carat;
        }
        else{
          if( (carat>=Start-32 && carat<=Fine-32) || carat==esci-32 ){
            printf(" %c",carat);
            return carat;
          }
        }
      }while( (carat<Start || carat>Fine) && ( carat<Start-32 || carat>Fine-32 ) && (carat!=Fine && carat!=Fine-32) );

}
int stampMat(int mat[NR][NC],int nStamp,FILE *f,int tipo){
  int  i,j,vive;
  vive=0;
  system("clear");
  if(tipo==1)printf("SIMULAZIONE\n");
  else printf("INSERIMENTO MANUALE\n");
  printf("   ");
  for(i=0;i<NC;i++){
    printf("-");
  }
  printf("\n");
  for(i=0;i<NR;i++){
    printf("  |");
    for(j=0;j<NC;j++){
      if(mat[i][j]==1){
        if(tipo==1)printf("#");
        else printf("1");
        vive++;
      }
      else{
        if(mat[i][j]==0){
          if(tipo==1)printf(" ");
          else printf("0");
        }
        else{
          if(tipo==1)printf("e");
          else printf(" ");
        }
      }
      if(j==NC-1){
        printf("|\n");
      }
    }
  }
  printf("   ");
  for(i=0;i<NC;i++){
    printf("-");
  }
  if(tipo==1){
    printf("\n\nAnni trascorsi %d\nCelle vive %d\n",nStamp,vive);
    fprintf(f,"%d\n",vive);
    return vive;
  }
  else printf("\n\nCelle vive %d\n\n1/0: cella viva/morta\nc,C: cancella ultimo inserimento\nf,F: completa automaticamente la matrice con degli 0 a partire dal punto\n     in cui viene inserito il carrattere fino all'ultima cella\nr,R: completa automaticamente la riga corrente con degli 0\nx,X: torna al menu' 'Simulazione Microscopica/Inserimento viventi'\n",vive);
}
int viciniToro(int posRT,int posCT,int mondo[NR][NC]){
  int i,j,viciniT=0,cont;
  //Anlisi angoli
  cont=0;
  switch(posRT){
    case 0:
      if(posCT==0){
        //cella in alto a sx
        viciniT = mondo[NR-1][NC-1] + mondo[NR-1][posCT] + mondo[NR-1][posCT+1] + mondo[posRT][NC-1] + mondo[posRT][posCT+1] + mondo[posRT+1][NC-1] + mondo[posRT+1][posCT] + mondo[posRT+1][posCT+1];
        cont++;
      }
      if(posCT==NC-1){
        //cella in alto a dx
        viciniT = mondo[NR-1][posCT-1] + mondo[NR-1][posCT] + mondo[NR-1][0] + mondo[posRT][posCT-1] + mondo[posRT][0] + mondo[posRT+1][posCT-1] + mondo[posRT+1][posCT] + mondo[posRT+1][0];
        cont++;
      }
    break;
    case NR-1:
      if(posCT==0){
        //cella in basso a sx
        viciniT = mondo[posRT-1][NC-1] + mondo[posRT-1][posCT] + mondo[posRT-1][posCT+1] + mondo[posRT][NC-1] + mondo[posRT][posCT+1] + mondo[0][NC-1] + mondo[0][posCT] + mondo[0][posCT+1];
        cont++;
      }
      if(posCT==NC-1){
          //cella in bassso a dx
          viciniT = mondo[posRT-1][posCT-1] + mondo[posRT-1][posCT] + mondo[posRT-1][0] + mondo[posRT][posCT-1] + mondo[posRT][0] + mondo[0][posCT-1] + mondo[0][posCT] + mondo[0][0];
          cont++;
      }
    break;
  }
    if(cont==0){
        //printf("BORDI SENZA ANGOLI");//----------------------------------------------------------------DA LEVARE
      if( (posRT>0 && posRT<NR-1) && posCT==0 ){
        //sx bordo esclusi gli angoli
        viciniT = mondo[posRT-1][NC-1] + mondo[posRT-1][posCT] + mondo[posRT-1][posCT+1] + mondo[posRT][NC-1] + mondo[posRT][posCT+1] + mondo[posRT+1][NC-1] + mondo[posRT+1][posCT] + mondo[posRT+1][posCT+1];
      }
      if( (posRT>0 && posRT<NR-1) && posCT==NC-1){
        //dx bordo esclusi gli angoli
        viciniT = mondo[posRT-1][posCT-1] + mondo[posRT-1][posCT] + mondo[posRT-1][0] + mondo[posRT][posCT-1] + mondo[posRT][0] + mondo[posRT+1][posCT-1] + mondo[posRT+1][posCT] + mondo[posRT+1][0];
      }
      if(posRT==0 && (posCT>0 && posCT<NC-1) ){
        //up bordo esclusi gli angoli
        viciniT = mondo[NR-1][posCT-1] + mondo[NR-1][posCT] + mondo[NR-1][posCT+1] + mondo[posRT][posCT-1] + mondo[posRT][posCT+1] + mondo[posRT+1][posCT-1] + mondo[posRT+1][posCT] + mondo[posRT+1][posCT+1];
      }
      if(posRT==NR-1 && (posCT>0 && posCT<NC-1) ){
        //down bordo esclusi gli angoli
        viciniT = mondo[posRT-1][posCT-1] + mondo[posRT-1][posCT] + mondo[posRT-1][posCT+1] + mondo[posRT][posCT-1] + mondo[posRT][posCT+1] + mondo[0][posCT-1] + mondo[0][posCT] + mondo[0][posCT+1];
      }
    }
  return viciniT;
}
int statoCell(int posR,int posC,int mondo[NR][NC]){
  int i,j,vicini=0;
  //Quanti vicini nelle 8 celle adiacenti
  if( ((posR==0 || posR==NR-1) && (posC>=0 && posC<=NC-1)) || ((posC==0 || posC==NC-1) && (posR>=0 && posR<=NR-1)) ){
       //printf("FUNZIONE TOROO  ");//-----------------------------------------------------------------------------------------------DA LEVAREEEEEE
       vicini = viciniToro(posR,posC,mondo);
  }
  else{
    for(i=posR-1;i<=posR+1;i++){
      for(j=posC-1;j<=posC+1;j++){
        if(i!=posR || j!=posC){
          if(mondo[i][j]==1){
            vicini++;
          }
        }
      }
    }
  }
  //printf("vicini di mondo[%d][%d] = %d\n",posR,posC,vicini);//----------------------------------------------------------------------DA LEVAREEEEEEEEEEEEEEEEEEE
  switch(mondo[posR][posC]){
    case 0:
      //Cella morta
      if(vicini==3){
        return 1;//Nasce
      }
      else{
        return 0;//Non nasce
      }
    break;
    case 1:
      //Cella viva
      if(vicini==2 || vicini==3){
        return 1;//Sopravvive
      }
      else{
        return 0;//Non sopravvive per isolamento o sovraffollamento
      }
    break;
  }
}
void menu(int tip){
  switch(tip){
    case 1://Menu principale
      system("clear");
      printf("\n");
      printf("\n                            ####################\n");
      printf("                            #                  #");
      printf("\n                            # GIOCO DELLA VITA #\n");
      printf("                            #                  #");
      printf("\n                            ####################\n\n\n");
      printf("=============MENU' PRINCIPALE=============\n\n");
      printf("  A) Simulazione microscopica\n");
      printf("  B) Simulazione macroscopica\n");
      printf("  X) Termina programma\n");
      printf("\n==========================================");
    break;
    case 2://Menu' secondario - Simulazione microscopica
      system("clear");
      printf("\n");
      printf("                          ________________________\n\n");
      printf("                          Simulazione microscopica\n\n");
      printf("                          ________________________\n\n\n");
      printf("=============Inserimento viventi=============\n\n");
      printf("  A) Random\n");
      printf("  B) Manuale\n");
      printf("  C) Configurazioni di partenza\n");
      printf("  X) Torna al menu' principale\n");
      printf("\n=============================================");
    break;
    case 3://Menu' secondario - Simulazione macroscopica
      system("clear");
      printf("\n");
      printf("                          ________________________\n\n");
      printf("                          Simulazione macroscopica\n\n");
      printf("                          ________________________\n\n");
    break;
    case 4://Menu' secondario - configurazioni di partenza
      system("clear");
      printf("\n");
      printf("                          ________________________\n\n");
      printf("                          Simulazione microscopica\n\n");
      printf("                          ________________________\n\n\n");
      printf("=============Configurazioni di partenza==========\n\n");
      printf("  A) Ghianda (Methuselah- long 5206)\n");
      printf("  B) Gilder (navicella) \n");
      printf("  C) Gosper Gilder Gun\n");
      printf("  D) Lightweight space ship(navicella)\n");
      printf("  E) F-pentomino (Methuselah - long 1103)\n");
      printf("  F) Blocco (still life)\n");
      printf("  G) Beehive (still life)\n");
      printf("  H) Hive Marker (fuse)\n");
      printf("  I) Cross (oscillatore)\n");
      printf("  L) Lampeggiatore (oscillatore)\n");
      printf("  M) Bi-gun (oscillatore)\n");
      printf("  N) Flower of Eden (Gardens of Eden)\n");
      printf("  X) torna al menu' 'Simulazione Microscopica/Inserimento viventi'\n");
      printf("\n=============================================");
    break;
    default:
      printf("\nERRORRE: passaggio parametro!!!");
  }
}
void controllMac(float *x,long *y,long mini,long mas){
  int cont;
  cont=0;
  if(x!=NULL){
    do{
      if(cont>0)printf("\nValore inserito fuori range: ");
      scanf("%f",x);
      cont++;
    }while(*x<mini || *x>mas);
  }
  else{
    do{
      if(cont>0)printf("\nValore inserito fuori range: ");
      scanf("%d",y);
      cont++;
    }while(*y<mini || *y>mas);
  }
}
void randDefine(int mat[NR][NC],int n){
  int i,j;
  srand(time(NULL));
  for(i=0;i<NR;i++){
    for(j=0;j<NC;j++){
      mat[i][j] = rand( ) % n;// 0 <---> n-1
    }
  }
}
void puliziaMat(int mat[NR][NC],int n){
  int i,j;
  for(i=0;i<NR;i++){
    for(j=0;j<NC;j++){
      mat[i][j]=n;
    }
  }
}
void cellPrec(int *i,int *j){
  if(*i>0 && *j==0){//Riga precedente
    *i = *i - 1;
    *j = NC-1;
  }
  else{
    if(*j!=-1){
      *j = *j - 1;
    }
  }
}
char manDefine(int mat[NR][NC]){
  int i,j,cont;
  char compAut;
  compAut=' ';
  puliziaMat(mat,2);
  stampMat(mat,0,NULL,2);
  cont=0;
  for(i=0;i<NR;i++){
    //printf("\n  ");
    for(j=0;j<NC && compAut!='x' && compAut!='X';j++){
      if(cont==0)compAut = controlCaratManuale(&mat[i][j]);
      if(compAut=='c' || compAut=='C'){
        cellPrec(&i,&j);
        mat[i][j] = 2;
        cellPrec(&i,&j);
        stampMat(mat,0,NULL,2);
      }
      else{
        if(compAut=='1' || compAut=='0'){
          stampMat(mat,0,NULL,2);
        }
        else{//compAut = 'f',F || compAut = 'r,R'
          if(compAut=='r' || compAut=='R'){
            cont++;
            if(j!=NC-1){
              mat[i][j] = 0;
            }
            else{
              mat[i][j] = 0;
              cont=0;
              stampMat(mat,0,NULL,2);
            }
          }
          else{
            cont++;
            mat[i][j] = 0;
          }
        }
      }
    }
  }
  return compAut;
}
void mondoSucc(int mat1[NR][NC],int mat2[NR][NC]){
  int i,j;
  for(i=0;i<NR;i++){
    for(j=0;j<NC;j++){
      mat1[i][j] = statoCell(i,j,mat2);
    }
  }
}
void mondoPartenza(int mat1[NR][NC],int mat2[NR][NC]){
  int i,j;
  for(i=0;i<NR;i++){
    for(j=0;j<NC;j++){
      mat1[i][j] = mat2[i][j];
    }
  }
}
void letturaFile(char *name,int mat[NR][NC]){
  int i,j;
  FILE *f;
  char c;
  f = fopen(name,"r");
    for(i=0;i<NR;i++){
      for(j=0;j<NC;j++){
        fscanf(f,"%c",&c);
        //if(j==0 || j==69){printf("i= %d  j = %d  c = %d",i,j,c);system("pause");}
        if(c == 10 || c==13){//10 = LF - line feed  13 = CR - carriage return
          j--;
        }
        else{
          //c = fgetc(f);
          //printf("%c",c);
          switch(c){
            case ' ':
              mat[i][j]=0;
            break;
            case '1':
              mat[i][j]=1;
            break;
          }
       }
      }
    }
  fclose(f);
}
char configPart(int mat[NR][NC]){
  int i,j;
  char sel=' ',c;
  FILE *f;
  menu(4);
  sel = controlSelectMenu('a','n','x');
  switch(sel){
    case 'a':
    case 'A':
      //Ghianda
      letturaFile("./configurations/ghianda.txt",mat);
    break;
    case 'b':
    case 'B':
      //Gilder
      letturaFile("./configurations/.configurations/gilder.txt",mat);
    break;
    case 'c':
    case 'C':
      //Gosper Gilder Gun
      letturaFile("./configurations/gosperGilderGun.txt",mat);
    break;
    case 'd':
    case 'D':
      //Ligthweigth spaceship
      letturaFile("./configurations/ligthweigthSpaceship.txt",mat);
    break;
    case 'e':
    case 'E':
      //F-pentomino
      letturaFile("./configurations/fPentomino.txt",mat);
    break;
    case 'f':
    case 'F':
      //Block - blocco
      letturaFile("./configurations/block.txt",mat);
    break;
    case 'g':
    case 'G':
      //Beehive - alveare
      letturaFile("./configurations/beehive.txt",mat);
    break;
    case 'h':
    case 'H':
      //hiveMarker
      letturaFile("./configurations/hiveMarker.txt",mat);
    break;
    case 'i':
    case 'I':
      //Cross
      letturaFile("./configurations/cross.txt",mat);
    break;
    case 'l':
    case 'L':
      //Blinker - lampeggiatore
      letturaFile("./configurations/blinker.txt",mat);
    break;
    case 'm':
    case 'M':
      //Bi-gun
      letturaFile("./configurations/bi_gun.txt",mat);
    break;
    case 'n':
    case 'N':
      //Flower of Eden
      letturaFile("./configurations/flowerOfEden.txt",mat);
    break;
  }
  return sel;
}
int main()
{
    FILE *fMac,*fMic;
    char select=' ',select2=' ',select3=' ',select4=' ',select5=' ',exit=' ';
    int i,j,epoche,cont,vive;
    long individui,nEpoc,il;
    float costM,costN,k0,alfa;
    int mondo[NR][NC];
    int mondoModif[NR][NC];
    do{
      menu(1);//Inserisce il menu principale
      select = controlSelectMenu('a','b','x');//scelta menu
      switch(select){
        case 'a':
        case 'A':
          //Simulazione microscopica
          do{
            menu(2);//Inserisce il menu secondario
            select2 = controlSelectMenu('a','c','x');//scelta menu
            select5 = ' ';
            select4 = ' ';
              switch(select2){
                case 'a':
                case'A':
                  randDefine(mondo,2);
                break;
                case 'b':
                case 'B':
                  select5 = manDefine(mondo);
                break;
                case 'c':
                case 'C':
                  select4 = configPart(mondo);
                break;
              }
            }while(select5=='x' || select5=='X' || select4=='x' || select4=='X');
            if(select2!='x' && select2!='X' && select4!='x' && select4!='X' && select5!='x' && select5!='X'){
              cont=0;
              epoche=0;
              fMic = fopen("individui_Mic.txt","w");
              fprintf(fMic,"Individui:\n");
              do{
                if(cont==0){
                  vive = stampMat(mondo,epoche,fMic,1);//Stampa della popolazione di partenza
                  printf("s,S: avanti   /   x,X: fine");
                  select3 = controlSelectMenu('s','s','x');
                  cont++;
                }
                else{
                  //Simulazione mondo epoca successiva
                  mondoSucc(mondoModif,mondo);
                  epoche++;
                  vive = stampMat(mondoModif,epoche,fMic,1);
                  //Inizializzazione nuovo mondo di partenza
                  mondoPartenza(mondo,mondoModif);
                  printf("s,S: avanti   /   x,X: fine");
                  select3 = controlSelectMenu('s','s','x');
                  cont++;
                }
              }while( (select3=='s' || select3=='S') && vive!=0);
            fclose(fMic);
            printf("\n\nLa simulazione e' stata ducumentata nel file 'individui_Mic.txt'\nPeremi 'E,e' per tornare al menu' principale.");
            exit = controlSelectMenu('e','e','e');
          }
        break;
        case 'b':
        case 'B':
          //Simulazione macroscopica
          menu(3);
          printf("Costante di natalita' (numero decimale): ");
          controllMac(&costN,NULL,-16000000,16000000);//float costN
          printf("Costante di mortalita' (numero decimale): ");
          controllMac(&costM,NULL,-16000000,16000000);//float costM
          k0 = costN-costM;
          printf("K0 = %f\n",k0);
          printf("Coeficente di autoinibizione (numero decimale fra 0 e 1): ");
          controllMac(&alfa,NULL,0,1);//float alfa
          printf("Numero di individui (numero intero maggiore di 0): ");
          controllMac(NULL,&individui,1,20000000);//long individui
          printf("Numero di epoche (numero intero maggiore di 0): ");
          controllMac(NULL,&nEpoc,1,20000000);//long nEpoc
          fMac = fopen("individui_Mac.txt","w");
          fprintf(fMac,"Individui:\n");
          for(il=0;il<nEpoc && (individui>0 && individui<20000000);il++){
            //individui = (1 + k0*(1-alfa*individui*il))*individui;
            individui = (1 + k0*(1-alfa*individui)*0.01)*individui;
            //system("clear");
            if(individui>0 && individui<20000000){
              printf("\nIndividui: %ld    Anni trascorsi: %ld",individui,il+1);
              fprintf(fMac,"%ld\n",individui);
            }
          }
          fclose(fMac);
          //printf("\nprima di if individui = %ld\n",individui);//DA LEVAREEEEEEEE
          if(individui>=20000000){printf("\nSimulazione interrotta: Raggiunto limite massimo di individui (20'000'000)\nAnni trascorsi: %ld",il);}
          if(individui<=0){printf("Simulazione interrotta: Popolazione estinta\nAnni trascorsi: %ld",il);}
          printf("\n\nLa simulazione e' stata ducumentata nel file 'individui_Mac.txt'\nPeremi 'E,e' per tornare al menu' principale.");
          exit = controlSelectMenu('e','e','e');

        break;
      }
    }while(select!='x' && select!='X');
    printf("\n\n                              FINE ELABORAZIONE\n");
    return 0;
}
