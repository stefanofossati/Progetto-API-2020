#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rg {
    int nRiga;               //   numero della riga
    char *text;              //   puntatore alla stringa della riga
    struct Rg * next;
} Riga;

typedef Riga * ListaRighe;    //  lista delle righe, è una lista semplice

typedef struct Cm {           //  struct dei nodi dei comandi
    char command;             //  comando
    int riga1;                //  primo numero (indirizzo) della riga del comando
    int riga2;                //  ultimo numero (indirizzo) della riga del comando
    struct Rg * riga;         //  puntatore alla prima riga eliminata o sovrascritta
    struct Rg * ultriga;      //  puntatore all'ultima riga eliminata o sovrascritta
    struct Rg * pririga;      //  puntatore alla riga precedente alla modifica della lista delle righe da parte del comando
    struct Cm * next;
    struct Cm * prev;
} Comando;

typedef Comando * ListaComandi;   // lista dei comandi, è una lista doppiamente concatenata


void SalvaComandi(ListaComandi listac,char com, int a, int b ){         // salva i comandi all'interno di un nodo dei comandi
    ListaComandi punt;
    punt = listac;
    punt->command = com;
    punt->riga1 = a;
    punt->riga2 = b;
    punt->next = NULL;
}

void Change(ListaRighe ultrigr, ListaComandi comcorr, ListaRighe listar, int a, int b, int z){
    ListaRighe RigaNuova;
    ListaRighe PuntRiga;           // puntatore alla riga della lista delle righe
    ListaComandi PuntCom;          // puntatore al nodo del comando corrente
    ListaRighe PuntTemp=NULL;
    ListaRighe UltimaRiga;         // puntatore all'ultima riga della lista delle righe
    char frase[1024];              // buffer
    int n=0;                       // contatore


    UltimaRiga = ultrigr;
    PuntCom = comcorr;
    PuntRiga = listar;


    while(z<a && PuntRiga->next!=NULL){
        PuntRiga=PuntRiga->next;
        z++;
    }
    PuntCom->pririga=PuntRiga;
    if(PuntRiga->next!=NULL){
      PuntTemp=PuntRiga->next;
    }
    while(z<=b){
      RigaNuova=malloc(sizeof(Riga));             //  creo nuovo nodo per le righe
      fgets(frase, sizeof(frase), stdin);         //  inserisco l'input della frase da salvare
      RigaNuova->text=malloc(strlen(frase)+1);    //  creo lo spazio giusto per la frase da salvare
      strcpy(RigaNuova->text, frase);             //  copio dal buffer al nodo la frase di input
      RigaNuova->nRiga=z;
      PuntRiga->next=RigaNuova;
      UltimaRiga->next=PuntRiga->next;
      PuntRiga=PuntRiga->next;
      z++;
    }
    PuntRiga->next=NULL;
    if(PuntTemp!=NULL){
      PuntCom->riga=PuntTemp;
      while(n<b-a && PuntTemp->next!=NULL){
        PuntTemp=PuntTemp->next;
        n++;
      }
      PuntCom->ultriga=PuntTemp;
      PuntRiga->next=PuntTemp->next;
    }
}



void Print(ListaRighe listar, int a, int b){
  ListaRighe PuntRiga;                 // puntatore alla riga della lista delle righe
  int n=1;
  PuntRiga=listar;

  while(n<a){
    if(PuntRiga->next!=NULL){           //  proseguo finchè non ho trovato la prima riga da stampare se esiste
      PuntRiga=PuntRiga->next;
      n++;
    }
    else{
      n++;
    }
  }
  while(n<=b){
    if(PuntRiga->next!=NULL){           //  stampo le righe fino all'ultima
      fputs(PuntRiga->next->text, stdout);
      PuntRiga=PuntRiga->next;
      n++;
    }
    else{
      printf(".\n");
      n++;
    }
  }
}



void RiordinaRighe(ListaRighe listar, int k){           //  sistema gli indici dei nodi dopo una delete
    ListaRighe PuntRiga;

    PuntRiga=listar;
    while(PuntRiga->next != NULL){
        PuntRiga->next->nRiga = k;
        PuntRiga=PuntRiga->next;
        k++;
    }
}

void Delete(ListaComandi comcorr, ListaRighe listar, int a, int b){
    ListaRighe PuntRiga;                      // puntatore alla riga della lista delle righe
    ListaRighe PuntTemp, PuntTemp1;
    ListaComandi PuntCom;                     // puntatore al nodo del comando corrente
    int n=1;

    PuntRiga=listar;
    PuntCom=comcorr;

    while(PuntRiga->next!=NULL && n<a){       //  proseguo finchè non ho trovato il nodo da eliminare
        PuntRiga=PuntRiga->next;
        n++;
    }
    PuntCom->pririga=PuntRiga;                //  salvo il nodo precendete al primo da eliminare
    if(PuntRiga->next!=NULL){
        PuntTemp=PuntRiga->next;
        PuntTemp1=PuntTemp;
        while(PuntTemp1->next!=NULL && n<b){
            PuntTemp1=PuntTemp1->next;
            n++;
        }
        PuntCom->ultriga=PuntTemp1;
        PuntRiga->next=PuntTemp1->next;
        PuntCom->riga=PuntTemp;
        RiordinaRighe(PuntRiga, a);
    }
    else{
        return;
    }
}


ListaComandi Undo(ListaComandi comcorr, ListaRighe listar, int a){
    ListaComandi PuntCom;                 // puntatore al nodo del comando corrente
    ListaRighe PuntRiga;                  // puntatore alla riga della lista delle righe
    ListaRighe PuntTemp, PuntTemp1, PuntTemp2;
    int k=0, z=0;

    PuntCom=comcorr;
    PuntRiga= listar;

    while(z<a){
        k=0;
        PuntRiga= listar;
        z++;
        if(PuntCom->command == 'c'){      //  undo di change
            if(PuntCom->riga==NULL){      //  undo di change che non hanno sovrascritto delle righe
                PuntRiga=PuntCom->pririga;
                PuntTemp=PuntRiga->next;
                PuntCom->riga=PuntTemp;
                PuntRiga->next=NULL;
            }
            else{                         //  undo di change che hanno sovrascritto delle righe
                PuntRiga=PuntCom->pririga;
                PuntTemp2=PuntCom->riga;
                PuntCom->riga = NULL;
                PuntTemp=PuntRiga->next;
                PuntRiga->next=PuntTemp2;
                PuntCom->riga=PuntTemp;
            }
        }
        if(PuntCom->command == 'd'){      //  undo di delete
            PuntRiga=PuntCom->pririga;
            if(PuntCom->riga!=NULL){
                PuntTemp=PuntCom->riga;
                PuntTemp1=PuntRiga->next;
                PuntRiga->next=PuntTemp;
                PuntTemp=PuntCom->ultriga;
                PuntTemp->next=PuntTemp1;
                PuntCom->riga=NULL;
                PuntCom->ultriga=NULL;
            }
        }
        if(PuntCom->prev != NULL){
            PuntCom=PuntCom->prev;
        }
        else{
            return PuntCom;
        }
    }
    return PuntCom;
}


ListaComandi Redo(ListaComandi comcorr, ListaRighe listar, int a){
    ListaComandi PuntCom;                     // puntatore al nodo del comando corrente
    ListaRighe PuntRiga;                      // puntatore alla riga della lista delle righe
    ListaRighe PuntTemp, PuntTemp1;
    int k=0, h=1, z=0;

    PuntCom=comcorr;
    PuntRiga= listar;

    while(z<a) {
        k=0, h=1;
        PuntRiga= listar;
        z++;
        if(PuntCom->command == 'c'){     // redo di change
            PuntRiga=PuntCom->pririga;
            PuntTemp=PuntRiga->next;
            PuntRiga->next=PuntCom->riga;
            PuntCom->riga=PuntTemp;
        }
        if(PuntCom->command =='d'){     //  redo di delete
            PuntRiga=PuntCom->pririga;
            if(PuntRiga->next!=NULL){
                PuntTemp=PuntRiga->next;
                PuntTemp1=PuntTemp;
                while(k<((PuntCom->riga2)-(PuntCom->riga1)) && PuntTemp1->next!=NULL){           //riguardo bene
                    PuntTemp1=PuntTemp1->next;
                    k++;
                }
                PuntRiga->next=PuntTemp1->next;
                PuntCom->riga=PuntTemp;
                PuntTemp1->next=NULL;
                PuntCom->ultriga=PuntTemp1;
            }

        }
        if(PuntCom->next!= NULL && z<a){
            PuntCom=PuntCom->next;
        }
        else{
            return PuntCom;
        }
    }

    return PuntCom;

}


int main() {
    ListaComandi NodoCom;
    ListaComandi NodoComNuovo;
    ListaComandi ListaC= malloc(sizeof(Comando));           // lista dei comandi con un primo nodo vuoto
    ListaComandi NodoComCorr=ListaC;                        // puntatore al nodo del comando corrente
    ListaComandi PuntCom;
    ListaRighe ListaR = malloc(sizeof(Riga));               // lista delle righe con un primo nodo vuoto
    ListaRighe UltimaRiga=malloc(sizeof(Riga));             // nodo vuoto che punta alla coda della lista delle righe
    char input[25], b[10], c[10];
    char istruz;
    char punto;
    int n, m;
    int x, y;
    int r, u;
    int ContNumCom=0, ContNumLim=0, ContComCorr=0;
    char ip;
    int k=0;

    ListaR->nRiga = 0;
    ListaR->next = NULL;
    ListaR->text=NULL;
    NodoCom=ListaC;
    NodoCom->prev=NULL;



    //printf("inserisci comando:");
    while(istruz != 'q'){
        memset(&b[0], 0, strlen(b));    //  reset memoria dell'array b
        memset(&c[0], 0, strlen(c));    //  reset memoria dell'array c
        ip=' ';
        n=0;
        while(ip!=',' && ip!='r' && ip!='u' && ip!='q' && ip!='c' && ip!='d' && ip!='p'){      // inizio dell'inserimento e riconoscimento dei comandi
          ip=getc_unlocked(stdin);
          if(ip!=',' && ip!='r' && ip!='u' && ip!='q'){
            b[n]=ip;
            n++;
          }
          else if(ip=='r' || ip=='u' || ip=='q'){
            istruz=ip;
          }
          else{
            n=0;
            while(ip!='c' && ip!='d' && ip!='p'){
              ip=getc_unlocked(stdin);
              if(ip!='c' && ip!='d' && ip!='p'){
                c[n]=ip;
                n++;
              }
              else{
                istruz=ip;
              }
            }
          }
        }
        while(getchar() != '\n');           // istruzione per pulire il buffer

        x = atoi(b);
        y = atoi(c);


        if(istruz != 'u' && istruz != 'r' && ContNumCom!=ContComCorr){        // esegue undo o redo solo se il comando successivo a una di esse non è ancora undo o redo
               if(ContNumCom<ContComCorr){
                    if(NodoComCorr->prev!=NULL){
                        NodoComCorr = Undo(NodoComCorr, ListaR, ContComCorr-ContNumCom);
                    }
                    ContComCorr=ContNumCom;
               }
               else if(ContNumCom>ContComCorr){
               if(NodoComCorr->next != NULL){
                    NodoComCorr = Redo(NodoComCorr->next, ListaR, ContNumCom-ContComCorr);
                    }
                    ContComCorr=ContNumCom;
                }


        }
        if(istruz == 'c'){              //Change
            //printf("Change\n");
            if(ListaC->next!=NULL && NodoComCorr->next != NULL ){
                NodoComCorr->next=NULL;
                NodoCom=NodoComCorr;
                ContNumLim=ContNumCom;

            }
            NodoComNuovo=malloc(sizeof(Comando));           //  creazione nuovo nodo comandi
            SalvaComandi(NodoComNuovo, istruz, x, y);
            NodoCom->next = NodoComNuovo;
            NodoComNuovo->prev = NodoCom;
            NodoComNuovo->riga=NULL;
            NodoComCorr = NodoCom->next;
            NodoCom=NodoCom->next;

            if(x==0){
                printf(".\n");
                x=1;
            }
            if(NodoComCorr->prev!=NULL && NodoComCorr->prev->command=='c' && UltimaRiga->next->nRiga==(x-1)){
                //  inizia dell'ultima riga a scrivere
                Change(UltimaRiga, NodoComCorr, UltimaRiga->next, x, y, x);
            }
            else{
                //  inizia la ricerca dall'inizio
                Change(UltimaRiga, NodoComCorr, ListaR, x, y, 1);
            }
            punto=getc_unlocked(stdin);
            if(punto!='.'){
                printf("error");
            }

            ContComCorr++;
            ContNumLim++;
            ContNumCom++;
        }

        if(istruz == 'p'){        //Print;
            //printf("Print\n");
            if(x==0 & y==0){
                printf(".\n");
            }
            else{
                Print(ListaR, x, y);
            }
        }

        if(istruz == 'd'){            //Delete;
            //printf("Delete\n");
            if(ListaC->next!=NULL && NodoComCorr->next != NULL ){
                NodoComCorr->next=NULL;
                NodoCom=NodoComCorr;
                ContNumLim=ContNumCom;
            }

            NodoComNuovo=malloc(sizeof(Comando));        //creazione nuovo nodo comandi
            SalvaComandi(NodoComNuovo, istruz, x, y);
            NodoCom->next = NodoComNuovo;
            NodoComNuovo->prev = NodoCom;
            NodoComNuovo->riga=NULL;
            NodoComCorr = NodoCom->next;
            NodoCom=NodoCom->next;
            if(x!=0 || y!=0){
                if(x==0){
                    x=1;
                }
                Delete(NodoComCorr, ListaR, x, y);
            }

            ContNumLim++;
            ContNumCom++;
            ContComCorr++;
        }

        if(istruz == 'u'){            //Undo;
            //printf("Undo\n");
            if(x>ContNumLim){
                x=ContNumLim;
            }
            ContNumCom=ContNumCom-x;
            if(ContNumCom<0){
                ContNumCom=0;
            }
            if(ContNumCom>ContNumLim){
                ContNumCom=ContNumLim;
            }
        }

        if(istruz == 'r'){          //Redo;
            //printf("Redo\n");
            if(x>ContNumLim){
                x=ContNumLim;
            }
            ContNumCom=ContNumCom+x;
            if(ContNumCom<0){
                ContNumCom=0;
            }
            if(ContNumCom>ContNumLim){
                ContNumCom=ContNumLim;
            }
        }

    }
    return 0;    //fine programma
}
