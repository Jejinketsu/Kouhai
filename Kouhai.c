#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//	Struct de status dos personagens e dos inimigos
typedef struct {
	int id, vida, vidaMax, mana, manaMax, nivel, EXP, EXPMax, pontuacao;
	char nome[20], classe[15];
	float forcaFisica, forcaMagica, sorte, agilidade;
	float resistenciaMagica, armadura;
} Personagem;

typedef struct {
	int id, vida, vidaMax, mana, manaMax, nivel, pontos;
	char nome[20];
	float forcaFisica, forcaMagica, sorte, agilidade;
	float resistenciaMagica, armadura;
} Inimigo;

/*	Structs que simulam uma tabela para facilitar
	o controle dos personagens e inimigos*/
typedef struct {
	Personagem personagens[10];
	int tamanho;
} Tabela;

Tabela tab; 

typedef struct {
	Inimigo inimigos[10];
} TabelaInimigos;

TabelaInimigos tab1;

void start();

void dungeon();

void save ();

void read ();

void del ();

void update ();

Personagem definirAtributos();

void subirNivel();

void definirInimigos();

int main () {
	definirInimigos();
	srand(time(NULL));
	
	tab.tamanho = 0;
	int opcao;
	
	//	Menu inicial do jogo
	do{
		setbuf(stdin, NULL);
		printf("\n-------Kouhai no Kimyou na Bouken-------");
		printf("\n1- Jogar\n2- Criar Personagem\n3- Exibir Personagens\n4- Apagar Personagem\n5- Editar Personagem\n6- Sair");
		printf("\n-------------------------\n");
		scanf("%d", &opcao);
		
		switch(opcao){
			case 1:
				start();
				break;
			case 2:
				save();
				break;
			case 3:
				read();
				break;
			case 4:
				del();
				break;
			case 5:
				update();
				break;
			case 6:
				printf("\nFim do programa.\n");
				return 0;
			default:
				printf("\nEscolha uma opcao valida!\n");
				break;
		}
	} while(opcao != 6);
	
	return 0;
}

void start (){
	
	if(tab.tamanho != 0){
		read();
		int i;
		printf("\nDigite o ID do personagem que deseja usar: ");
		scanf("%d", &i);
		
		while (i > tab.tamanho-1){
			printf("Escolha um personagem existente: ");
			scanf("%d", &i);
		}
		
		printf("Muito bem. Hora de entrar na dungeon!\n");
		dungeon(tab.personagens[i]);
		printf("\nA campanho encerrou. Sua pontucao foi %d.\n", tab.personagens[i].pontuacao);
		
	} else {
		printf("Nao existem personagens cadastrados. Retorne ao menu e cadastre algum personagem.");
	}
	
}

void dungeon (Personagem personagem){
	personagem.vida = personagem.vidaMax;
	personagem.mana = personagem.manaMax;
	Inimigo inimigo;
	int inimigoRandom;

	while (personagem.vida > 0){
		inimigoRandom = rand()%10;
		inimigo = tab1.inimigos[inimigoRandom];
		printf("Um %s apareceu. Derrote-o!\n", inimigo.nome);
		personagem.vida -= 10;
	}
}

void save () {
	
	tab.personagens[tab.tamanho].id = tab.tamanho;
	tab.personagens[tab.tamanho].nivel =  1;
	printf("Digite o nome do Personagem: ");
	setbuf(stdin, NULL); scanf("%[^\n]s", tab.personagens[tab.tamanho].nome);
	
	int opcao;
	printf("Escolha a classe do personagem: ");
	printf("\n1- Guerreiro\n2- Mago Negro\n3- Mago Branco\n4- Samurai\n5- Arqueiro\n");
	scanf("%d", &opcao);
	
	do{
		switch(opcao){
			case 1:
				tab.personagens[tab.tamanho] = definirAtributos(tab.personagens[tab.tamanho], "Guerreiro");
				strcpy(tab.personagens[tab.tamanho].classe, "Guerreiro");
				break;
			case 2:
				tab.personagens[tab.tamanho] = definirAtributos(tab.personagens[tab.tamanho], "Mago Negro");
				strcpy(tab.personagens[tab.tamanho].classe, "Mago Negro");
				break;
			case 3:
				tab.personagens[tab.tamanho] = definirAtributos(tab.personagens[tab.tamanho], "Mago Branco");
				strcpy(tab.personagens[tab.tamanho].classe, "Mago Branco");
				break;
			case 4:
				tab.personagens[tab.tamanho] = definirAtributos(tab.personagens[tab.tamanho], "Samurai");
				strcpy(tab.personagens[tab.tamanho].classe, "Samurai");
				break;
			case 5:
				tab.personagens[tab.tamanho] = definirAtributos(tab.personagens[tab.tamanho], "Arqueiro");
				strcpy(tab.personagens[tab.tamanho].classe, "Arqueiro");
				break;
			default:
				printf("Isso n�o � uma classe. Escolha uma classe adequada.");
			break;
		}
	} while (opcao < 1 || opcao > 5);
	
	tab.tamanho++;
}

void read () {
	
	printf("\nPersonagens--------------------\n");
	printf("ID	|	NOME	|Nv	|	Classe	|	Pontuacao Maxima\n");
		
	int i = 0;
	for (i; i < tab.tamanho;i++){	
		printf("%d	|%s	|%d	|%s	|	%d\n", tab.personagens[i].id, tab.personagens[i].nome, tab.personagens[i].nivel,
			tab.personagens[i].classe, tab.personagens[i].pontuacao);
	}
}

void del () {
	
	if(tab.tamanho > 0){	
		read();
		
		int indice;
		printf("\nQual o ID do item que deseja apagar?");
		scanf("%d", &indice);
		
		if(indice < tab.tamanho && indice >= 0){
			for(indice; indice <= tab.tamanho; indice++){
				tab.personagens[indice-1] = tab.personagens[indice];
				tab.personagens[indice-1].id--;
			}
			if(tab.tamanho >= 0){
				tab.tamanho--;
				printf("Item Apagado.");
			}		
		} else {
			printf("Esse item nao existe.");
		}	
	} else {
		printf("Nao existem itens para apagar");
	}	
}

void update () {
	
	read ();
	
	int indice;
	char nome[20];
	printf("\nQual o ID do Personagem que deseja atualizar?");
	scanf("%d", &indice);
	
	if(indice >= tab.tamanho){
		printf("\nPara atualizar voce deve escolher num indice que ja esteja em uso.");
	} else {
		printf("Qual o novo nome?");
		setbuf(stdin, NULL); scanf("%[^\n]s", tab.personagens[indice].nome);
		printf("\nPersongem Atualizado.");
	}
	
}

void subirNivel(Personagem personagem){
	
	personagem.nivel += 1;
	
	char classe[15]; 
	strcpy(classe, personagem.classe);
	
	if(!strcmp(classe, "Guerreiro")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 1.06; 
		personagem.manaMax *= 0;
		personagem.forcaFisica *= 1.02;
		personagem.forcaMagica *= 0;
		personagem.sorte *= 1.02;
		personagem.agilidade *= 1.01;
		personagem.armadura *= 1.05;
		personagem.resistenciaMagica *= 1.05;
	}
	if(!strcmp(classe, "Mago Negro")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 1.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	}
	if(!strcmp(classe, "Mago Branco")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax = 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	}
	if(!strcmp(classe, "Samurai")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax = 30; 
		personagem.manaMax = 15;
		personagem.forcaFisica = 20.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 5.0;
		personagem.agilidade = 10.0;
		personagem.armadura = 6.0;
		personagem.resistenciaMagica = 6.0;
	}
	if(!strcmp(classe, "Arqueiro")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax = 20; 
		personagem.manaMax = 10;
		personagem.forcaFisica = 15.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 20.0;
		personagem.armadura = 2.0;
		personagem.resistenciaMagica = 2.0;
	}
}

Personagem definirAtributos(Personagem personagem, char classe[15]){
	if(!strcmp(classe, "Guerreiro")){
		personagem.EXPMax = 50;
		personagem.vidaMax = 45; 
		personagem.manaMax = 0;
		personagem.forcaFisica = 10.0;
		personagem.forcaMagica = 0.0;
		personagem.sorte = 2.0;
		personagem.agilidade = 2.0;
		personagem.armadura = 15.0;
		personagem.resistenciaMagica = 10.0;
	}
	if(!strcmp(classe, "Mago Negro")){
		personagem.EXPMax = 65;
		personagem.vidaMax = 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 1.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	}
	if(!strcmp(classe, "Mago Branco")){
		personagem.EXPMax = 60;
		personagem.vidaMax = 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	}
	if(!strcmp(classe, "Samurai")){
		personagem.EXPMax = 55;
		personagem.vidaMax = 30; 
		personagem.manaMax = 15;
		personagem.forcaFisica = 20.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 5.0;
		personagem.agilidade = 10.0;
		personagem.armadura = 6.0;
		personagem.resistenciaMagica = 6.0;
	}
	if(!strcmp(classe, "Arqueiro")){
		personagem.EXPMax = 55;
		personagem.vidaMax = 20; 
		personagem.manaMax = 10;
		personagem.forcaFisica = 15.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 20.0;
		personagem.armadura = 2.0;
		personagem.resistenciaMagica = 2.0;
	}
	return personagem;
}

void definirInimigos() {
	
	//1� Inimigo
	tab1.inimigos[0].id = 0;
	tab1.inimigos[0].nivel = 1;
	tab1.inimigos[0].pontos = 10;
	strcpy(tab1.inimigos[0].nome, "Slime");
	tab1.inimigos[0].vidaMax = 25;
	tab1.inimigos[0].manaMax = 25;
	tab1.inimigos[0].forcaFisica = 0;
	tab1.inimigos[0].forcaMagica = 20;	
	tab1.inimigos[0].sorte = 0;
	tab1.inimigos[0].agilidade = 1;
	tab1.inimigos[0].armadura = 20;
	tab1.inimigos[0].resistenciaMagica = 0;
	
	//2� Inimigo
	tab1.inimigos[1].id = 1;
	tab1.inimigos[1].nivel = 1;
	tab1.inimigos[1].pontos = 10;
	strcpy(tab1.inimigos[1].nome, "Rato");
	tab1.inimigos[1].vidaMax = 25;
	tab1.inimigos[1].manaMax = 25;
	tab1.inimigos[1].forcaFisica = 0;
	tab1.inimigos[1].forcaMagica = 20;	
	tab1.inimigos[1].sorte = 0;
	tab1.inimigos[1].agilidade = 1;
	tab1.inimigos[1].armadura = 20;
	tab1.inimigos[1].resistenciaMagica = 0;

	//3� Inimigo
	tab1.inimigos[2].id = 2;
	tab1.inimigos[2].nivel = 1;
	tab1.inimigos[2].pontos = 10;
	strcpy(tab1.inimigos[2].nome, "Morcego");
	tab1.inimigos[2].vidaMax = 25;
	tab1.inimigos[2].manaMax = 25;
	tab1.inimigos[2].forcaFisica = 0;
	tab1.inimigos[2].forcaMagica = 20;	
	tab1.inimigos[2].sorte = 0;
	tab1.inimigos[2].agilidade = 1;
	tab1.inimigos[2].armadura = 20;
	tab1.inimigos[2].resistenciaMagica = 0;
	
	//4� Inimigo
	tab1.inimigos[3].id = 3;
	tab1.inimigos[3].nivel = 1;
	tab1.inimigos[3].pontos = 10;
	strcpy(tab1.inimigos[3].nome, "Caveira");
	tab1.inimigos[3].vidaMax = 25;
	tab1.inimigos[3].manaMax = 25;
	tab1.inimigos[3].forcaFisica = 0;
	tab1.inimigos[3].forcaMagica = 20;	
	tab1.inimigos[3].sorte = 0;
	tab1.inimigos[3].agilidade = 1;
	tab1.inimigos[3].armadura = 20;
	tab1.inimigos[3].resistenciaMagica = 0;
	
	//5� Inimigo
	tab1.inimigos[4].id = 4;
	tab1.inimigos[4].nivel = 1;
	tab1.inimigos[4].pontos = 10;
	strcpy(tab1.inimigos[4].nome, "Zumbi Mizeravi");
	tab1.inimigos[4].vidaMax = 25;
	tab1.inimigos[4].manaMax = 25;
	tab1.inimigos[4].forcaFisica = 0;
	tab1.inimigos[4].forcaMagica = 20;	
	tab1.inimigos[4].sorte = 0;
	tab1.inimigos[4].agilidade = 1;
	tab1.inimigos[4].armadura = 20;
	tab1.inimigos[4].resistenciaMagica = 0;
	
	//6� Inimigo
	tab1.inimigos[5].id = 5;
	tab1.inimigos[5].nivel = 1;
	tab1.inimigos[5].pontos = 10;
	strcpy(tab1.inimigos[5].nome, "Minion Bosta");
	tab1.inimigos[5].vidaMax = 25;
	tab1.inimigos[5].manaMax = 25;
	tab1.inimigos[5].forcaFisica = 0;
	tab1.inimigos[5].forcaMagica = 20;	
	tab1.inimigos[5].sorte = 0;
	tab1.inimigos[5].agilidade = 1;
	tab1.inimigos[5].armadura = 20;
	tab1.inimigos[5].resistenciaMagica = 0;
	
	//7� Inimigo
	tab1.inimigos[6].id = 6;
	tab1.inimigos[6].nivel = 1;
	tab1.inimigos[6].pontos = 10;
	strcpy(tab1.inimigos[6].nome, "Lobo");
	tab1.inimigos[6].vidaMax = 25;
	tab1.inimigos[6].manaMax = 25;
	tab1.inimigos[6].forcaFisica = 0;
	tab1.inimigos[6].forcaMagica = 20;	
	tab1.inimigos[6].sorte = 0;
	tab1.inimigos[6].agilidade = 1;
	tab1.inimigos[6].armadura = 20;
	tab1.inimigos[6].resistenciaMagica = 0;
	
	//8� Inimigo
	tab1.inimigos[7].id = 7;
	tab1.inimigos[7].nivel = 1;
	tab1.inimigos[7].pontos = 10;
	strcpy(tab1.inimigos[7].nome, "OVNI");
	tab1.inimigos[7].vidaMax = 25;
	tab1.inimigos[7].manaMax = 25;
	tab1.inimigos[7].forcaFisica = 0;
	tab1.inimigos[7].forcaMagica = 20;	
	tab1.inimigos[7].sorte = 0;
	tab1.inimigos[7].agilidade = 1;
	tab1.inimigos[7].armadura = 20;
	tab1.inimigos[7].resistenciaMagica = 0;
	
	//9� Inimigo
	tab1.inimigos[8].id = 8;
	tab1.inimigos[8].nivel = 1;
	tab1.inimigos[8].pontos = 10;
	strcpy(tab1.inimigos[8].nome, "Dragao Bebe");
	tab1.inimigos[8].vidaMax = 25;
	tab1.inimigos[8].manaMax = 25;
	tab1.inimigos[8].forcaFisica = 0;
	tab1.inimigos[8].forcaMagica = 20;	
	tab1.inimigos[8].sorte = 0;
	tab1.inimigos[8].agilidade = 1;
	tab1.inimigos[8].armadura = 20;
	tab1.inimigos[8].resistenciaMagica = 0;
	
	//10� Inimigo
	tab1.inimigos[9].id = 9;
	tab1.inimigos[9].nivel = 1;
	tab1.inimigos[9].pontos = 10;
	strcpy(tab1.inimigos[9].nome, "Aronguejo");
	tab1.inimigos[9].vidaMax = 25;
	tab1.inimigos[9].manaMax = 25;
	tab1.inimigos[9].forcaFisica = 0;
	tab1.inimigos[9].forcaMagica = 20;	
	tab1.inimigos[9].sorte = 0;
	tab1.inimigos[9].agilidade = 1;
	tab1.inimigos[9].armadura = 20;
	tab1.inimigos[9].resistenciaMagica = 0;
}



