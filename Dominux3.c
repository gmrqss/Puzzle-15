#include<stdio.h>
#include "stdlib.h"
#include "conio.h"
#include "time.h"

#define RS "\033[0m"
#define OL "\033[1m"

#define FR "\033[31m" // Cor vermelha para letras
#define FG "\033[32m" // Cor verde para letras
#define FB "\033[34m" // Cor azul para letras
#define FW "\033[37m" // Cor branca para letras
#include "stdio.h"
#define BR "\033[41m" // Cor vermelha para fundo
#define BB "\033[44m" // Cor azul para fundo

/* Estrutura da pe�as*/
typedef struct P{
	int D;
	int E;
	 struct P *prox;
} P ;

P* inicializa(P *ptr);		/* inicializa as 28 pe�as do jogo */
P * insereFinal(P **lista, P **inserido);	/* insere um n� no final de uma lista simplesmete encadeada*/
P * FimLista(P *comeco);	/* fun�o que retorna um ponteiro para o ultimo n� da lista */
P* distribui(P **inicio1, P **inicio2 , int qtd_pecas, int ver_pecas);		/* distribui para inicio1 a qtd_pecas q deseja do inicio2 randomizando ou n�o depender� do ver_pecas */
P * escolheTira();		/* fun�o que interage com o usuario, para que ele possa escolher sua op�o */
P * invertePeca(P *peca);	/* esta fun�o recebe o ponteiro para um n� e retorna o mesmo, mas com seu conteudo invertido */
int conta(P *corre);		/* conta quantas pe�as possui o local e retorna esta quantidade */
int soma(P *corre);		/* soma todas as extremidades d todas as pe�as restantes */
int comparaPeca(P *ptr,P *comeco,P *final); /* Varredura de compatibilidade, busca em uma lista se achar retorna 1 */
int jogaPC(P **Pc,P *inicio, P *fim);		/* PC joga com a primeira op�o que aparecer, sem prioridade */
int joga_jogador(P *inicio, P *fim);		/* cont�m todo o controle de fluxo para que o jogador interaja com o jogo */
void comeco_jogo (P **corre, P **anda);		/* come�a o jogo j� distribuindo o maior carret�o ou pela maior soma */
void maior (P **corre, P **anda);		/* fun�o auxiliar para pe�a de maior soma */
void mostra(P *corre,int verifica);		/* mostra as pe�as de uma lista */
void mostraIndice(P *corre);	/* mostra as pe�as de uma lista com �ndices nas pe�as */
void esconde(P *corre,int verifica);		/* esconde as pe�as do PC e do monte guardando seus valores */
void fim_jogo();		/* fun�o encarregada de somar os pontos caso d� empate, PC ou Jogador ven�am */
void jogo();			/* fun�o onde organiza e ocorre os fluxos e eventos do jogo */
void mostra_jogo();		/* fun�o redutora serve para mostrar a tela do jogo utilizando outras fun�oes (mostra e esconde) */
int menu(); /* Fun�o de um pequeno menu */
void sair(); /* Fun�o para imprimir texto final */
void info(); /* Fun�o pra informa�o academica */

P *monte = NULL;
P *jogador = NULL;
P *tabuleiro = NULL;
P *PC = NULL;

int main ()
{
	int opc,aux=1;
		while(aux!=0)
		{
			opc=menu();
			if(opc==1)
			{
				jogo();
				aux=0;
			}
			if(opc==2)
			{
				info();
				aux=1;
				getchar();
			}
			if(opc==3)
			{
				sair();
				aux=0;
			}
		}
}
/* Fun�o primordial nela o usu�rio escolher� se deseja come�ar um novo jogo ou sair do programa */

void jogo()
{
 system("cls");
	P *pri,*ult;
	int cont1,cont2,cont3,vez_PC,vez_jogador,a,b;

	monte = inicializa(monte); // introduz as 28 pe�as no monte

	jogador = distribui(&jogador,&monte,7,0); // jogador compra 7 pe�as aleat�riamente
	PC = distribui(&PC,&monte,7,0); // pc compra 7 pe�as aleat�riamente

	comeco_jogo(&jogador,&PC);

	cont1=conta(PC);
	cont2=conta(jogador); // contadores para verificar quem foi o primeiro a jogar
	cont3=0;

	if (cont1<cont2){
		vez_PC=2; // Segundo a jogar
		vez_jogador=1; // Primeiro a jogar
	}else{
		vez_PC=1; // Primeiro a jogar
		vez_jogador=2; // Segundo a jogar
	}

	while (cont1!=0 && cont2!=0 && cont3<2)
	{
		pri=tabuleiro; // verifica a primeira pe�a no come�o do jogo e ap�s a jogada do usuario
		ult=FimLista(tabuleiro); // verifica a ultima pe�a no come�o do jogo e ap�s a jogada do usuario

		if (vez_PC < vez_jogador)
		{
		b=jogaPC(&PC,pri,ult);
			if(conta(PC)==0) break;
		system("cls");
		mostra_jogo();
		pri=tabuleiro; // verifica a primeira pe�a ap�s jogada do pc
		ult=FimLista(tabuleiro); // verifica a ultima pe�a ap�s jogada do pc
		a=joga_jogador(pri,ult);
		}
		else
		{
		system("cls");
		mostra_jogo();
		a=joga_jogador(pri,ult);
			if(conta(jogador)==0) break;
		pri=tabuleiro; // verifica a primeira pe�a ap�s jogada do jogador
		ult=FimLista(tabuleiro); // verifica a ultima pe�a ap�s jogada do jogador
		b=jogaPC(&PC,pri,ult);
		}


	if(a==0 && b==0)
	{
		cont3=2;
	}

	cont1=conta(PC);
	cont2=conta(jogador);

	if(cont1 == 0 || cont2 ==0) break;
	}
	system("cls");
	mostra_jogo();
	fim_jogo(); // chama o termino do jogo
	getchar();
	free(monte);
	free(jogador);
	free(tabuleiro);
	free(PC);
	getchar();
	sair();
}
/* fun�o principal do jogo. Nela se desenvolve o jogo. Onde s�o inseridass as 28 pe�as essas pe�as s�o introduzidas no monte e em seguida distribui a quantidade minima de pe�as para cada jogador randomicamente. Em seguida come�a um jogo e verefica qual foi o primeiro jogador a joga-la. Utilizando o vez_PC e o vez_jogador logo ap�s a verefica�o mantem a determinada ordem para o jogo. O jogo ter� continuidade a n�o ser q se alcance tr�s resultados (1-Jogador sem pe�as 2-Computador sem pe�as 3-Os dois n�o terem pe�as para jogar) neste ultimo caso a fun�o chamar� uma outra para verificar qual jogador possui menor somat�rio das pe�as. Por fim usando o free libera a mem�ria */


int jogaPC(P **Pc,P *inicio, P *fim)
{
	P *comp,*atras,*aux;
	int achou = 0,a;

	atras=NULL;
	comp = *Pc;

	while(comp != NULL)
	{
		achou = comparaPeca(comp,inicio,fim); // Chama a fun�o para identificar se ha ou n�o liga�o entre as pe�as
		if(achou==1) /* Encontrou uma pe�a que se encaixa no tabuleiro*/
		{
			if(atras == NULL)   /* Se a pe�a encontrada for a primeira da lista verifica-se onde pode se encaixar*/
			{
				if(comp->E == inicio->E) // 1? caso
				{
					*Pc = comp->prox;
					aux=invertePeca(comp);
					aux->prox=inicio;
					inicio = aux;
					tabuleiro=inicio;
					return 1;
				}
				if(comp->D == inicio->E) // 2? caso
				{
					*Pc = comp->prox;
					aux=comp;
					aux->prox=inicio;
					inicio=aux;
					tabuleiro=inicio;
					return 1;
				}
				if(comp->E == fim->D) // 3? caso
				{
					*Pc = comp->prox;
					aux=comp;
					aux->prox=fim->prox;
					fim->prox = aux;
					tabuleiro=inicio;
					return 1;
				}
				if(comp->D == fim->D) // 4? caso
				{
					*Pc = comp->prox;
					aux = invertePeca(comp);
					aux->prox=fim->prox;
					fim->prox=aux;
					tabuleiro=inicio;
					return 1;
				}

			}
			else /* Se a pe�a encontrada estiver no meio ou no final da lista verifica-se onde pode se encaixar*/
			{
				if(comp->E == inicio->E) // 1? caso
				{
					atras->prox = comp->prox;
					aux=invertePeca(comp);
					aux->prox = inicio;
					inicio = aux;
					tabuleiro = inicio;
					return 1;
				}
				if(comp->D == inicio->E) // 2? caso
				{
					atras->prox = comp->prox;
					aux=comp;
					aux->prox = inicio;
					inicio = aux;
					tabuleiro = inicio;
					return 1;
				}
				if(comp->E == fim->D) // 3? caso
				{
					atras->prox = comp->prox;
					aux = comp;
					aux->prox = fim->prox;
					fim->prox = aux;
					tabuleiro = inicio;
					return 1;
				}
				if(comp->D == fim->D) // 5? caso
				{
					atras->prox = comp->prox;
					aux = invertePeca(comp);
					aux->prox = fim->prox;
					fim->prox = aux;
					tabuleiro = inicio;
					return 1;
				}
			}
		}
		atras = comp;
		comp=comp->prox;
	}
	if(achou == 0 && monte != NULL) // 5? caso n�o ha liga�o portanto ele compra
	{
		PC = distribui(&PC,&monte,1,0);
		inicio = tabuleiro;
		fim = FimLista(tabuleiro);
		a = jogaPC(&PC,inicio,fim);
		return a;
	}
	if(achou==0 && monte==NULL) // 5? caso n�o ha liga�o e ele passa a vez retornando (0)
	{

		return 0;
	}
	if(comp==NULL) return 0;
}

int comparaPeca(P *ptr,P *comeco,P *final)
{
	P *comp;

	comp=ptr;
	while(comp != NULL)
	{
		if(comp->E == comeco->E || comp->D == comeco->E || comp->E == final->D || comp->D == final->D)
		{
			return(1);
		}
		comp=comp->prox;
	} return(0);
}
/* Fun�o que ser� chamada ao se achar um valor nas pe�as do computador que se encaixe e indicar� se esta dever� ser indserida no come�o ou no fim. */


int joga_jogador(P *inicio, P *fim)
{
	P *peca,*primeiro,*ultimo;
	int aux=0;
	primeiro = inicio;
	ultimo = fim;
	peca = escolheTira(); // chama a fun�o que indicar� qual pe�a que o usu�rio quer jogar

	if(peca==NULL) return(0);
	else // compara a pe�a jogada com as extremidades da mesa
	{
		while(aux==0){

		if(peca->E==primeiro->E) // 1? caso
		{
			peca = invertePeca(peca);
			tabuleiro = distribui(&primeiro,&peca,1,1);
			return(1);
		}
		else if(peca->D==primeiro->E) // 2? caso
		{
			tabuleiro = distribui(&primeiro,&peca,1,1);
			return(1);
		}
		else if(peca->E==ultimo->D) // 3? caso
		{
			tabuleiro = insereFinal(&tabuleiro, &peca);
			return(1);
		}
		else if(peca->D == ultimo->D) // 4? caso
		{
			peca = invertePeca(peca);
			tabuleiro = insereFinal(&tabuleiro, &peca);
			return(1);
		}
		else // 5? caso n�o ha encaixe retorna a pe�a escolhida de volta para m�o do jogador
		{
			jogador = distribui(&jogador,&peca,1,1);
			mostra_jogo();
			system("cls");
			mostra_jogo();
			aux=0;
			peca=escolheTira();
		}
		}
	}system("pause");
	return(1);
}
/* Fun�o que far� com que a pe�a escolhida pelo usurio por outra fun�o seja jogada na mesa , caso o usuario tenha se enganado ao escolher a pe�a esta mesma pe�a se n�o houver encaixse retornar� a sua "m�o". */


P * insereFinal(P **lista, P **inserido)
{
	P *L,*aux;

	aux = *lista;
	L = FimLista(aux);
	L->prox = (*inserido);
	(*inserido)->prox=NULL;

	return(*lista);
}
/* Fun�o simples. Faz com q a pe�a inserida (**inserido) va para o final da mesa (**lista)*/


P * invertePeca(P *peca)
{
	P *pecaAux;
	pecaAux = (P *)malloc(sizeof(P));;
	pecaAux->D=peca->E;
	pecaAux->E=peca->D;

	return(pecaAux);
}
/* Fun�o onde se precisar inverter� a pe�a ,ou seja, caso a parte Esquerda da pe�a desejada seja igual a parte Esquerda da mesa ou caso a parte Direita da pe�a seja igual a parte Direita da pe�a. */

P * escolheTira()
{
	P *aux,*aux1,*ult,*pri;
	int nPecas=0,p,i=1,a,cont;

	mostraIndice(jogador);
	nPecas = conta(jogador);
	cont=conta(monte);

	printf("\nEscolha a pe�a, -1 sai do jogo e 0 compra pe�a:");
		scanf("%i",&p);
	while(p>nPecas || (p<1 && p!=-1 && p!=0 ))
	{

		system("cls");
		mostra_jogo(); // Mostra como esta as pe�as no jogo
		mostraIndice(jogador); // Mostra as pe�as com os indices a serem escolhidos
		printf("\n");
		printf("Op�o Invalida Digite Novamente :"); // Caso digite valor fora dos indicados
		scanf("%i",&p);
		printf("\n");
	}
	if(p==-1) // Sai do jogo
	{
		exit(1);
	}
	if(p==0){ // Tenta comprar

		system("cls");
			if(cont==0){ // N�o comprar� e retornar� (0)
			return(NULL);
			}
			else{
			jogador=distribui(&jogador,&monte,1,0); // N�o joga nenhuma pe�a e compra uma aleat�riamente
			mostra_jogo();
			pri=tabuleiro;
			ult=FimLista(tabuleiro);
			a=joga_jogador(pri,ult); // Retorna para a jogada do usuario
			return(NULL);
			}
	}
	else
	{
		aux=aux1=jogador;

		while(i!=p) // Percorre at� o indice desejado
		{
			aux1=aux; // Otimiza os ponteiros para manter valores sem a pe�a escolhida
			aux=aux->prox;
			i++;
		}
		if(aux1 == aux) // Joga a primeira pe�a
		{
			jogador=jogador->prox;
			aux->prox=NULL;
		}
		if(aux1!=aux) // Joga uma pe�a a partir da segunda
		{
			aux1->prox=aux->prox;
			aux->prox=NULL;
		}
		return(aux);
	}

}
/* Fun�o que o usuario escolhera uma pe�a e esta pe�a ser� retornada para o uso de uma outra fun�o. Nela existe um pequeno menu onde o usuario deve escolher o indice da pe�a a ser jogada, 0 para comprar uma pe�a ou -1 para desistir do jogo atual. Um pequeno detalhe � que caso o usuario digite um indice inexistente em sua m�o o jogo lhe informar� e pedira q digite novamente. Caso o usuario queira comprar e o monte onde est�o as pe�as estiver zerados ent�o ele retornar� (0) indicando que ele passou de vez. */


P * FimLista(P *comeco)
{
	P *aux;

	aux = comeco;
    if (aux == NULL)
    return 0;
        else{
		while(aux!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo valor
		{
				 aux=aux->prox; // Indica qual ser� o proximo No
		}
        }
        return(aux); // Retorna o ultimo valor
}
/* Fun�o que percorre a lista desejada e guarda seu ultimo valor (seu ultimo NO).*/



void mostra(P *corre,int verifica)
{
	P *temp;
	temp = corre;

	if(temp==NULL)
		printf("Sem Pecas");

		while(temp!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo valor
		{
			if(verifica==1){
			printf("[%i|%i]",temp->E, temp->D);
			}
			if(verifica==2){
			printf("%s[%i|%i]%s ",FB,temp->E, temp->D,RS);
			}
			temp=temp->prox; // Indica qual ser� o proximo No
		}
		printf("\n");
}
/* Fun�o que percorre a lista desejada e mostra todos os valores contidos na mesma Lista. */


void mostraIndice(P *corre)
{
	P *temp;
	temp = corre;
	int cont=0;
	printf("Pe�as do Jogador:\n");
	if(temp==NULL)
		printf("Sem Pecas");

		while(temp!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo No
		{
			printf("%i-%s[%i|%i]%s  ",++cont,FB,temp->E, temp->D,RS); // Mostra um indice juntamente com a pe�a
			if(cont%8 == 0)
			printf("\n");
			temp=temp->prox; // Indica qual ser� o proximo No
		}
		printf("\n");
}
/* Fun�o que percorre a lista desejada e mostra todos os valores contidos na mesma Lista, indicando um indice em cada pe�a para que esta possa ser escolhida no decorrer do jogo. */


void esconde(P *corre,int verifica)
{
	P *temp;
	temp = corre;

	if(temp==NULL)
		printf("Sem Pecas");

		while(temp!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo No
		{
			if(verifica==1){
			printf("%s[#|#]%s ",FG,RS);
			}
			if(verifica==2){
			printf("%s[#|#]%s ",FR,RS);
			}
			temp=temp->prox; // Indica qual ser� o proximo No
		}
		printf("\n");
}
/* Fun�o que percorre a lista desejada e mostra as costas de todos os valores contidos na mesma Lista, mantendo seus valores para que estas pe�as possam ser escolhidas no decorrer do jogo pelo computador. Usada tamb�m para ocultar o valor da pe�a comprada do monte. */


P* distribui(P **inicio1, P **inicio2 , int qtd_pecas, int ver_pecas)
{
	P *aux,*aux1,*aux2;		/* Aux para percorrer a lista e aux1 para percorrer a lista atras de aux */
	int escolhido,contador;

	aux2 = *inicio1; // Ponteiro para onde a pe�a tem que ir
	srand(time(0));
	for(int i =0; i<qtd_pecas; i++)
	{
		aux = aux1 = *inicio2; // Ponteiro para de onde as pe�as tem que sair
		contador=conta(aux);

		if(ver_pecas==1){
		escolhido=(contador-1);
		}
		else
		{
		escolhido = rand()%contador;
		}
		for(int j = 0; j < escolhido; j++)		// Percorre a lista escolhida
		{
			aux1 = aux;
			aux = aux -> prox;
		}

			if(escolhido > 0 && escolhido < contador) // Meio da lista
			{
				if(aux2 == NULL)
				{				// Se o local n�o conter pe�a
					aux1 -> prox = aux -> prox;
					aux2 = aux;
					aux2->prox = NULL;      // � primeiro a ser inserido no local, ent�o sera o ultimo da lista
				}else
				{
					aux1 -> prox = aux -> prox;
					aux -> prox = aux2;
					aux2 = aux;
				}
			}
			if(escolhido == 0)
			{
				// Inicio da lista
				if(aux2 == NULL)
				{				// Se o local n�o conter pe�a
					aux2 = aux;
					(*inicio2) = (*inicio2)->prox;
					aux2->prox = NULL;     // � primeiro a ser inserido no local, ent�o sera o ultimo da lista
				}
				else
				{
					(*inicio2) = (*inicio2)->prox;
					aux->prox = aux2;
					aux2 = aux;
				}
			}
			if(escolhido == contador) // Fim da lista
			{
				if(aux2 == NULL)
				{				// Se o local n�o conter pe�a
					aux2 = aux;
					aux2 ->prox = NULL;     // � primeiro a ser inserido no local, ent�o sera o ultimo da lista
				}
				else
				{
					aux1 -> prox = aux -> prox;
					aux -> prox = aux2;
					aux2 = aux;
				}
			}
		contador--;
	}
	return(aux2);
}
/* Fun�o que � utilizada tanto para randomizar as pe�as quanto para que estas sejam inseridas no come�o da lista. O Primeiro parametro � para onde a pe�a retornada deve ir, O Segundo � de onde ela deve sair , O Terceiro indica quantas vezes a fun�o deve se repetir e o Quarto � onde ele verifica se ele deve randomizar as pe�as ou tira-la somente inserindo a pe�a no come�o da lista antes passada como parametro */


P * inicializa(P *ptr)
{
	int j;
	P *novo;
	P *p;

	p = ptr;

	for(int i=0; i<=6; i++)
	{
		j=i;
		for(; j<=6; j++) // La�o para pegar valores depois de seu carret�o. Exemplo ap�s o 0;0 parte para o 1;1
		{
			novo = (P *)malloc(sizeof(P));; // Cria um novo No para cada pe�a
			if(novo==NULL)
			{
				printf("Overflow\n"); //Caso a mem�ria seja sobrecarregada
				exit(0);
			}
			novo->E = i; // Valor do lado Esquerdo da pe�a
			novo->D = j; // Valor do lado Direito da pe�a
			novo->prox = p; // Quarda o endere�o da pr�xima pe�a
			p = novo;
		}
	}
	return(p);
}
/* Fun�o usada para iniciar o jogo, ou seja , cria as 28 pe�as que no futuro ser� usada no programa. */

int conta(P *corre)
{
	P *temp;
	temp = corre;
	int cont;
	cont=0;
		while(temp!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo No
		{
			temp=temp->prox; // Indica qual ser� o proximo No
			cont ++ ; // Contador que conta a quantidade de pe�as contidas na lista at� seu ultimo No
		}
	return (cont); //retorna a quantidade de pe�as
}
/* Fun�o para contar quantas pe�as ha em determinada lista. Usada tamb�m posteriormente para identificar se houve um ganhador indicando se a lista desejada possui ou n�o pe�as */


int soma(P *corre)
{
	P *temp;
	temp = corre;
	int soma;
	soma=0;
		while(temp!=NULL) // Enquanto para percorrer a lista at� chegar no ultimo No
		{
			soma+=temp->E+temp->D; // Somador que a cada passo soma as duas extremidades de cada pe�a
			temp=temp->prox; // Indica qual ser� o proximo No
		}
	return (soma); // retorna a soma de todas pe�as juntas
}
/* Fun�o para somar todas as pe�as de uma determinada lista. Usada tamb�m posteriormente para identificar se houve um ganhador indicando em compara�o com uma outra lista qual delas possui menor somat�rio dos lados de suas pe�as correspondentes. */


void comeco_jogo (P **corre, P **anda)
{
	P *usuario,*usuario1,*pc,*pc1,*valor,*valor1,*valor2,*valor3;
	P *auxJ,*auxPC;
	int tot1=-1;
	int tot2=-1;
	auxJ = jogador;
	auxPC = PC;
	usuario = usuario1 = valor = valor1 = *corre;
	pc = pc1 =  valor2 = valor3 =  *anda;

		while(usuario!=NULL) // Percorre as pe�as do Usuario
		{
			if(usuario->E == usuario->D && usuario->E>tot1) // Se este for um carret�o entra no la�o
			{
				valor = usuario1;
				valor1= usuario;
				tot1=usuario->D; // tot1 guarda o indice do carret�o do Jogador
			}
			usuario1 = usuario; // Quarda um No Anterior
			usuario = usuario->prox; // // Indica qual ser� o proximo No do Usuario
		}
		while(pc!=NULL) // Percorre as pe�as do Computador
		{
			if(pc->E==pc->D && pc->E>tot2) // Se este for um carret�o entra no la�o
			{
				valor3=pc1;
				valor2=pc;
				tot2=pc->D; // tot2 guarda o indice do carret�o do Computador
			}
			pc1 = pc; // Quarda um No Anterior
			pc=pc->prox; // // Indica qual ser� o proximo No do Computador
		}
		if(tot2>tot1)
		{
			printf("Valor do maior eh do: PC[%i|%i] \n",tot2,tot2);
			printf("O computador comeca...\n");
		}
		if(tot1>tot2)
		{
			printf("Valor do Maior eh do: Jogador[%i|%i] \n",tot1,tot1);
			printf("O Jogador comeca...\n");
		}
		if(tot1 == tot2)
		{
			printf("Valor igual !!! \n");
			printf("Entao Escolhendo a Maior Soma Entre as Pecas:\n");
		}
		printf("Pressione enter para continuar...\n");
		getchar();
		getchar();

		if(tot1>tot2) // Caso o maior carret�o seje o do Usuario
		{
				if(valor == valor1)
					auxJ = valor->prox;
				valor->prox = valor1->prox; // Far� a corre�o da lista de onde foi retirada uma pe�a
				valor1->prox=NULL; // Indica que a pe�a retirada ser� procedida do NULL
				*corre = auxJ;
				tabuleiro = distribui(&tabuleiro,&valor1,1,1);


		}
		if(tot2>tot1) // Caso o maior carret�o seje o do Computador
		{
				if(valor3 == valor2)
					auxPC = valor3->prox;
				valor3->prox = valor2->prox; // Far� a corre�o da lista de onde foi retirada uma pe�a
				valor2->prox = NULL; // Indica que a pe�a retirada ser� procedida do NULL
				*anda = auxPC;
				tabuleiro = distribui(&tabuleiro,&valor2,1,1);


		}
		if(tot1==tot2) // Caso n�o existir carret�o em ambos os Jogadores procura maior pe�a
				maior(&jogador,&PC);
}
/* Esta � a fun�o que iniciar� o jogo automaticamente. Assim ela varrer� o Usuario e o Computador antes de come�ar o jogo e dar� inicio jogando o maior carret�o dos dois e se n�o houver carret�es chamar� uma outra fun�o. */


void maior (P **corre, P **anda)
{
	P *usuario,*usuario1,*pc,*pc1,*valor,*valor1,*valor2,*valor3;
	P *auxJ,*auxPC;
	int soma1,soma2;

	soma1=0;
	soma2=0;
	auxJ = jogador;
	auxPC = PC;
	usuario = usuario1 = valor = valor1 = *corre;
	pc = pc1 =  valor2 = valor3 =  *anda;

		while(usuario!=NULL)
		{
		if(usuario->E!=usuario->D && (usuario->E+usuario->D) >soma1){
			valor = usuario1;
			valor1=usuario;
			soma1=usuario->D+usuario->E;
			}
			usuario1 = usuario;
			usuario=usuario->prox;
		}

		while(pc!=NULL)
		{
		if(pc->E!=pc->D && (pc->D+pc->E) >soma2) {
			valor3=pc1;
			valor2=pc;
			soma2=pc->D+pc->E;
			}
			pc1 = pc;
			pc=pc->prox;
		}

		if(soma2>soma1)
		{
			printf("Valor da Maior Soma � do: PC[%i] \n",soma2);
			printf("O computador come�a...\n");
		}
		if(soma1>soma2)
		{
			printf("Valor da Maior Soma � do: Jogador[%i] \n",soma1);
			printf("O Jogador come�a...\n");
		}
		if(soma1 == soma2)
		{
			printf("Valor da Maior Soma: Soma Iguais!!! \n");
			printf("Sem Pe�a Maior... Ent�o Preferencia do PC:\n");
		}
		printf("Pressione enter para continuar...\n");
		getchar();

		if(soma1 > soma2) // Caso a maior soma seje a do Usuario
		{
				if(valor == valor1) //
					auxJ = valor->prox;
				valor->prox = valor1->prox; // Far� a corre�o da lista de onde foi retirada uma pe�a
				valor1->prox=NULL; // Indica que a pe�a retirada ser� procedida do NULL
				*corre = auxJ;
				tabuleiro = distribui(&tabuleiro,&valor1,1,1);
		}
		if(soma2 > soma1) // Caso a maior soma seje a do Computador
		{
				if(valor3 == valor2)
					auxPC = valor3->prox;
				valor3->prox = valor2->prox; // Far� a corre�o da lista de onde foi retirada uma pe�a
				valor2->prox = NULL; // Indica que a pe�a retirada ser� procedida do NULL
				*anda = auxPC;
				tabuleiro = distribui(&tabuleiro,&valor2,1,1);
		}
		if(soma1 == soma2) // Caso exista duas pedras de valores maiores preferencia para o Computador
		{
				if(valor3 == valor2)
					auxPC = valor3->prox;
				valor3->prox = valor2->prox; // Far� a corre�o da lista de onde foi retirada uma pe�a
				valor2->prox = NULL; // Indica que a pe�a retirada ser� procedida do NULL
				*anda = auxPC;
				tabuleiro = distribui(&tabuleiro,&valor2,1,1);
		}
}
/* Continuidade da fun�o comeca_jogo ja que esta fun�o ser� chamada se nenhyum dos dois Jogadores possuirem carret�es esta fun�o far� com que o jogo come�e com a pe�a de maior valor absoluto se mesmo assim houver um empate o jogo dar� preferencia ao Computador */


void fim_jogo()
{

int cont1=0,cont2=0,soma1=0,soma2=0;

	cont1=conta(PC);
	cont2=conta(jogador);

	if (cont1==0 ) // Contador das pe�as do Usuario
		printf("%s%s Vencedor PC !! Valeu A Tentativa !! %s%s\n\n");
	if (cont2==0 ) // Contador das poe�as do Computador
		printf("%s%s Vencedor Jogador!! Meus Parab�ns !! %s%s\n\n",BR,FW,OL,RS);
	if (cont1!=0 && cont2!=0){
		soma1=soma(PC); // Soma das pe�as do Usuario
		soma2=soma(jogador); // Soma das pe�as do Computador
			if (soma1<soma2){
				printf("Soma das pe�as do PC: %i\n",soma1);
				printf("Soma das pe�as do jogador: %i\n\n",soma2);
				printf("%s%s Vencedor PC !! Valeu A Tentativa !! %s%s\n\n",BR,FW,OL,RS);
				}else{
			if (soma1>soma2){
				printf("Soma das pe�as do jogador: %i\n",soma2);
				printf("Soma das pe�as do PC: %i\n\n",soma1);
				printf("%s%s Vencedor Jogador !! Meus Parab�ns !! %s%s\n\n",BR,FW,OL,RS);
				}else{
				printf("%s%s Sem Vencedor !! Incrivel Deu Empate !! %s%s\n",BR,FW,OL,RS);
				}
				}
	}
	printf("Pressione enter para finalizar...\n");
}
/* Fun�o cuja finalidade unica � verificar qual � o jogador que venceu a partida. Nela s�o chamados outras duas fun�es para contar pe�as e ver se algumjogador chegou a zero e se caso precisar soma os valores de cada extremidade das pe�as restantes de cada jogador. */


void mostra_jogo()
{
	printf("\nMonte: %i\n",conta(monte));
		esconde(monte,1);
		printf("\n\n");
	printf("Pe�as do Computador:\n");
		esconde(PC,2);
		printf("\n\n");
	printf("Pe�as da Mesa:\n");
		mostra(tabuleiro,1);
		printf("\n\n");
	printf("Pe�as do Jogador:\n");
		mostra(jogador,2);
		printf("\n\n");
}
/* Fun�o que auxilia no processo do programa, � utilizado para encurtar o pr�prio programa e imprimir o estado do jogo atual. Indicando pe�as no monte, pe�as do computador e as pe�as do usuario antes de sua jogada. */


int menu () {

	int opc;

	system("cls");
	printf("\n%s%s	 ____________________________  Dominux  _______________________________ %s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                              ________                                |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                             |  menu  |                               |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                              --------                                |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|             __________________________________________               |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |                                          |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |                                          |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |                1 - Jogo                  |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |        2 - Informacoes Academicas        |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |                3 - Sair                  |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |                                          |              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|            |__________________________________________|              |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                                                                      |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                                                                      |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|______________________________________________________________________|%s%s\n",BB,FW,OL,RS);
	printf("escolha uma opcao:");
	scanf("%i",&opc);

	return(opc);
}
/* Fun�o somente para mostrar um menu de escolha. */


void sair () {

	system("cls");
	printf("\n%s%s	+---------------------------------------+%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                                       |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|  Obrigado Por UtilIzar Este Programa  |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	|                                       |%s%s\n",BB,FW,OL,RS);
	printf("%s%s	+---------------------------------------+%s%s\n\n",BB,FW,OL,RS);
}
/* Fun�o somente para mostrar um texto final. */


void info()
{
	system("cls");
	printf("\n	+-------------------- JOGO DOMINO (MODO TEXTO) -----------------------+\n");
	printf("	|                                                                     |\n");
	printf("	|               UNIVERSIDADE TECNOLOGICA                              |\n");
	printf("	|               FEDERAL DO PARANA                                     |\n");
	printf("	|               ENGENHARIA ELETRONICA                                 |\n");
	printf("	|               ESTRUTURA DE DADOS                                    |\n");
	printf("	|                                                                     |\n");
	printf("	|                                                                     |\n");
	printf("	|                                                                     |\n");
	printf("	|                                                                     |\n");
	printf("	+---------------------------------------------------------------------+\n");
	printf("Pressione enter para continuar...");
 	getchar();
}
/* Fun�o somente para mostrar um texto introdutivo para as informa�es dos academicos. */

