#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/*	Struct de status dos tipos Personagens e dos Inimigos.
	Definem todos os parâmetros que os personagens criados
	pelo jogador terão e também os inimigos. */
typedef struct {
	int id, vida, vidaMax, mana, manaMax, nivel, EXP, EXPMax, pontuacao, pontuacaoMax;
	char nome[20], classe[15], skills[4][15];
	float forcaFisica, forcaMagica, sorte, agilidade;
	float resistenciaMagica, armadura;
} Personagem;

typedef struct {
	int id, vida, vidaMax, mana, manaMax, nivel, pontos, EXP;
	char nome[20];
	float forcaFisica, forcaMagica, sorte, agilidade;
	float resistenciaMagica, armadura;
} Inimigo;

/*	Structs que simulam uma tabela para facilitar o controle
 	dos personagens e inimigos. Como atributo de ambos temos 
	uma variável de tamanho que representa quantos itens temos
	no vetor de Personagens/Inimigos. */
typedef struct {
	Personagem personagens[9];
	int tamanho;
} Tabela;

Tabela tab; 

typedef struct {
	Inimigo inimigos[11];
} TabelaInimigos;

TabelaInimigos tab1;

/*	Struct para duplo retorno após o uso de uma skill ou item.
	Dessa forma será possível retornar as alterações em dois
	chars ao mesmo tempo através das funções. */
typedef struct {
	Inimigo inimigo;
	Personagem personagem;
} Retorno;

//Inicialização das funções---------------------

int escudo ();

void start();

Personagem dungeon();

Personagem batalha();

void save ();

void read ();

void del ();

void update ();

Personagem incrementoEXP ();

Personagem definirAtributos();

Personagem subirNivel();

void definirInimigos();

Retorno skills();

//------------------------------------------------

int main () {
	definirInimigos();
	srand(time(NULL));
	
	tab.tamanho = 0;
	char opcao[2];
	int escolha;
	
	//	Menu inicial do jogo
	do{
		setbuf(stdin, NULL);
		printf("\n-------Kouhai no Kimyou na Bouken-------");
		printf("\n1- Jogar\n2- Criar Personagem\n3- Exibir Personagens\n4- Apagar Personagem\n5- Editar Personagem\n6- Sair");
		printf("\n-------------------------\n");
		scanf("%s", opcao);

		escolha = escudo(opcao, "Escolha uma opcao valida!\n");
		
		switch(escolha){
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
	} while(escolha != 6);
	
	return 0;
}

// Essa função é usada para evitar entradas indesejadas no programa.
int escudo(char entrada[2], char msgErro[100]){

	int chars = strlen(entrada), valor;

	while (chars > 1){
		printf("\n%s", msgErro);
		scanf("%s", entrada);

		chars = strlen(entrada);
	}

	if(!strcmp(entrada, "0")){
		return 0;
	} else {
		valor = atoi(entrada);
	}
	if(valor == 0){
		return -1;
	}

	return valor;
}

/*Sessão das funções que manipulam a campanha dentro da dungeon
--------------------------------------------------------*/

/*	A função start() pede ao jogador que identifique qual personagem
	durante a camapanha. No fim altera os personagem segundo suas ações
	na dungeons.*/
void start (){
	
	if(tab.tamanho != 0){
		read();
		int i;
		char entrada[2];
		printf("\nDigite o ID do personagem que deseja usar: ");
		scanf("%s", entrada);

		i = escudo(entrada, "Escolha um personagem existente: ");
		
		while (i > tab.tamanho-1 || i < 0){
			printf("Escolha um personagem existente: ");
			scanf("%s", entrada);

			i = escudo(entrada, "");
		}
		
		printf("Muito bem. Hora de entrar na dungeon!\n\n");
		tab.personagens[i] = dungeon(tab.personagens[i]);
		printf("\nA campanho encerrou. Sua pontucao foi %d.\n", tab.personagens[i].pontuacaoMax);
		
	} else {
		printf("Nao existem personagens cadastrados. Retorne ao menu e cadastre algum personagem.");
	}
	
}

/*	A função dungeon() seta os personagens que seram utilizados durante
	a luta. Os inimigos serão escolhidos aleatóriamente. Salva a pontuação
	do jogador e retorna o personagem modificado no fim da função. */
Personagem dungeon (Personagem personagem){

	personagem.vida = personagem.vidaMax;
	personagem.mana = personagem.manaMax;

	Inimigo inimigo;
	int inimigoRandom;

	while (personagem.vida > 0){
		inimigoRandom = rand()%10;
		inimigo = tab1.inimigos[inimigoRandom];
		printf("Um %s apareceu. Derrote-o!\n", inimigo.nome);
		personagem = batalha(personagem, inimigo);
	}

	if(personagem.pontuacao > personagem.pontuacaoMax){
		personagem.pontuacaoMax = personagem.pontuacao;
		return personagem;
	} else {
		return personagem;
	}
	
}
/*Fim da sessão da dungeon
--------------------------------------------------------*/

/*Sessão do sistema de batalha
--------------------------------------------------------*/

//	Função que gerencia a mecânica de batalha do jogo.
Personagem batalha (Personagem personagem, Inimigo inimigo) {

	inimigo.vida = inimigo.vidaMax;
	inimigo.mana = inimigo.manaMax;

	Personagem imagemP;
	strcpy(imagemP.classe, "Imagem");
	imagemP = definirAtributos(imagemP, "Imagem");

	Inimigo imagemI = tab1.inimigos[10];

	int opcao, esquiva, danoAtaque, index, cont = 0;
	float eficienciaAtk = 0, eficienciaDfs = 0;
	bool fuga = false;
	char entrada[2];

	do{
		cont++;
		eficienciaDfs = 0;
		setbuf(stdin, NULL);
		printf("\n---------Batalha---------");
		printf("\nVida: %d/%d	Mana: %d/%d\n", personagem.vida, personagem.vidaMax, personagem.mana, personagem.manaMax);
		printf("\nInimigo Vida: %d/%d	Mana: %d/%d\n", inimigo.vida, inimigo.vidaMax, inimigo.mana, inimigo.manaMax);
		printf("\n1- Atacar\n2- Defender\n3- Habilidades\n4- Fujir");
		printf("\n--------Turno %d---------\n", cont);
		scanf("%s", entrada);

		opcao = atoi(entrada);
		
		switch(opcao){
			case 1:
				esquiva = rand()%200+1;
				if(esquiva <= inimigo.agilidade+inimigo.sorte){
					printf("Inimigo Esquivou! \n");
				} else {
					eficienciaAtk = ((float)(rand()%40+80)/100)*(float)personagem.forcaFisica;
					if((int)eficienciaAtk < inimigo.armadura){
						eficienciaAtk = 1;
					} else {
						inimigo.vida -= ((int)eficienciaAtk-inimigo.armadura);
						eficienciaAtk = ((int)eficienciaAtk-inimigo.armadura);
					}
					printf("Inimigo recebeu %.0f de dano. ", eficienciaAtk);
				}
				break;
			case 2:
				eficienciaDfs = (personagem.armadura+personagem.resistenciaMagica)/(rand()%30+70);
				break;
			case 3:
				printf("\n------------------------\n");
				printf("|1.%15s| 2.%15s|\n", personagem.skills[0], personagem.skills[1]);
				printf("|3.%15s| 4.%15s|\n", personagem.skills[2], personagem.skills[3]);
				printf("\nQual Skill deseja usar? ");
				scanf("%s", entrada);

				index = escudo(entrada, "\nEntrada invalida. Escolha uma opcao valida: ");

				while(index > 4 || index < 1){
					printf("\nEntrada invalida. Escolha uma opcao valida: ");
					scanf("%s", entrada);
					index = escudo(entrada, "\nEntrada invalida. Escolha uma opcao valida: ");
				}

				Retorno retorno = skills(personagem, imagemP, inimigo, imagemI, personagem.skills[index-1]);

				personagem = retorno.personagem;
				inimigo = retorno.inimigo;
				
				break;
			case 4:
				fuga = true;
				printf("Voce fugiu! ");
				break;
			default:
				printf("\nVoce escorregou e perdeu o turno! \n");
				break;
		}

		if(!fuga){
			if(esquiva <= personagem.agilidade+personagem.sorte){
				printf("Voce Esquivou! \n");
			} else {
				eficienciaAtk = ((float)(rand()%40+80)/100)*(float)inimigo.forcaFisica;
				if((int)eficienciaAtk < personagem.armadura+eficienciaDfs){
					eficienciaAtk = 1;
				} else {
					personagem.vida -= ((int)eficienciaAtk-personagem.armadura-eficienciaDfs);
					eficienciaAtk = ((int)eficienciaAtk-personagem.armadura-eficienciaDfs);
				}
				printf("Voce recebeu %.0f de dano. \n", eficienciaAtk);
			}
		} else {
			personagem.vida = 0;
			break;
		}

		imagemP = definirAtributos(imagemP, "Imagem");

		imagemI = tab1.inimigos[10];

	} while(inimigo.vida > 0 && personagem.vida > 0);

	if(personagem.vida > 0){
		printf("Inimigo derrotado! Você recebeu %d EXP\n", inimigo.pontos);
		personagem.pontuacao += inimigo.pontos;
		personagem = incrementoEXP(personagem, inimigo.EXP);
	}

	return personagem;
}

Retorno skills (Personagem personagem, Personagem imagemP, Inimigo inimigo, Inimigo imagemI, char skill[15]){

	if(!strcmp(personagem.classe, "Guerreiro")){
		if(!strcmp(skill, "Deflect")){
			
		} else if(!strcmp(skill, "Castle of Stone")){
			imagemP.armadura = 9999;
			imagemP.resistenciaMagica = 9999;
			printf("\nDurante este turno %s fica imune a qualquer ataque. ", personagem.nome);
		} else if(!strcmp(skill, "Drain Resistance")){
			
		} else if(!strcmp(skill, "Armor Striker")){
			imagemP.forcaFisica += (personagem.armadura)*0.20;
			int eficienciaAtk = ((float)(rand()%40+80)/100)*(float)personagem.forcaFisica+(float)imagemI.forcaFisica;
			if((int)eficienciaAtk < inimigo.armadura){
				eficienciaAtk = 1;
			} else {
				inimigo.vida -= ((int)eficienciaAtk-inimigo.armadura);
				eficienciaAtk = ((int)eficienciaAtk-inimigo.armadura);
			}
			printf("Armor Strike! Voce inflingiu %d de dano no inimigo. \n", eficienciaAtk);
		} else {
			printf("Não acontece nada... ");
		}
	} else if(!strcmp(personagem.classe, "Mago Negro")){
		if(!strcmp(skill, "Thunder")){
			
		} else if(!strcmp(skill, "Fire")){

		} else if(!strcmp(skill, "Aqua")){
			
		} else if(!strcmp(skill, "Swamp Dust")){
			
		} else {
			printf("Não acontece nada... ");
		}
	} else if(!strcmp(personagem.classe, "Mago Branco")){
		if(!strcmp(skill, "Life Drain")){

		} else if(!strcmp(skill, "Regen")){

		} else if(!strcmp(skill, "Vitality Burst")){
			
		} else if(!strcmp(skill, "Unfair Exchange")){
			
		} else {
			printf("Não acontece nada... ");
		}
	} else if(!strcmp(personagem.classe, "Samurai")){
		if(!strcmp(skill, "Juuhazan")){

		} else if(!strcmp(skill, "Rekkyoudan")){

		} else if(!strcmp(skill, "Raikousen")){
			
		} else if(!strcmp(skill, "Hyouryuushou")){
			
		} else {
			printf("Não acontece nada... ");
		}
	} else if(!strcmp(personagem.classe, "Arqueiro")){
		if(!strcmp(skill, "Ultra Instinct")){

		} else if(!strcmp(skill, "Aim: Weak Point")){

		} else if(!strcmp(skill, "Focus")){
			
		} else if(!strcmp(skill, "Speed Shot")){
			
		} else {
			printf("Não acontece nada... ");
		}
	}	

	Retorno retorno;
	retorno.personagem = personagem;
	retorno.inimigo = inimigo;

	return retorno;

}
/*Fim da sessão do sistema de batalha
--------------------------------------------------------*/

/*Sessão das funções que manipulam o CRUD
--------------------------------------------------------*/
void save () {
	
	tab.personagens[tab.tamanho].id = tab.tamanho;
	tab.personagens[tab.tamanho].nivel =  1;
	printf("Digite o nome do Personagem: ");
	setbuf(stdin, NULL); scanf("%[^\n]s", tab.personagens[tab.tamanho].nome);
	
	int opcao;
	char entrada[2];
	printf("Escolha a classe do personagem: ");
	
	do{
		printf("\n1- Guerreiro\n2- Mago Negro\n3- Mago Branco\n4- Samurai\n5- Arqueiro\n");
		scanf("%s", entrada);

		opcao = escudo(entrada, "Isso não é uma classe. Escolha uma classe adequada: \n1- Guerreiro\n2- Mago Negro\n3- Mago Branco\n4- Samurai\n5- Arqueiro\n");

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
				printf("Isso não é uma classe. Escolha uma classe adequada: ");
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
		printf("%d	|%15s|%d	|%15s|	%d\n", tab.personagens[i].id, tab.personagens[i].nome, tab.personagens[i].nivel,
			tab.personagens[i].classe, tab.personagens[i].pontuacaoMax);
	}
}

void del () {
	
	if(tab.tamanho > 0){	
		read();
		
		int indice;
		char entrada[2];
		printf("\nQual o ID do item que deseja apagar?");
		scanf("%s", entrada);

		indice = escudo(entrada, "Esse personagem nao existe.");
		
		if(indice < tab.tamanho && indice >= 0){
			for(indice; indice <= tab.tamanho; indice++){
				tab.personagens[indice-1] = tab.personagens[indice]; 
				tab.personagens[indice-1].id--;
			}
			if(tab.tamanho >= 0){
				tab.tamanho--;
				printf("Personagem Apagado.");
			}		
		} else {
			printf("Esse personagem nao existe.");
		}	
	} else {
		printf("Nao existem personagens para apagar");
	}	
}

void update () {
	
	read ();
	
	int indice;
	char nome[20], entrada[2];
	printf("\nQual o ID do Personagem que deseja atualizar?");
	scanf("%s", entrada);

	indice = escudo(entrada, "\nValor invalido. Digite um valor valido: ");
	
	if(indice >= tab.tamanho || indice < 0){
		printf("\nPara atualizar voce deve escolher num indice que ja esteja em uso.");
	} else {
		printf("Qual o novo nome?");
		setbuf(stdin, NULL); scanf("%[^\n]s", tab.personagens[indice].nome);
		printf("\nPersongem Atualizado.");
	}
	
}
/*Fim da sessão de CRUD
--------------------------------------------------------*/

/*Sessão das funções de definição dos atributos de personagens, inimigos, skills e itens
--------------------------------------------------------*/

/*	Sempre que o personagem ganha EXP, essa função faz o acrescimo. Caso seja o suficiente,
	a função de aumentar o nível será chamado.*/
Personagem incrementoEXP (Personagem personagem, int exp){

	personagem.EXP += exp;
	int sobra = personagem.EXP-personagem.EXPMax;

	if(sobra >= 0){
		printf("Seu nivel aumentou! %d -> ", personagem.nivel);
		personagem = subirNivel(personagem);
		printf("%d\n", personagem.nivel);
		personagem.EXP = sobra;
	}

	return personagem;
}

/*	Ao criar um personagem, seus atributos base são definidos pela classe
	definida pelo jogador através dessa função. */
Personagem definirAtributos(Personagem personagem, char classe[15]){

	int i;
	for(i = 0; i < 4; i++){
		strcpy(personagem.skills[i], "Vazio");
	}

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
		strcpy(personagem.skills[0], "Armor Striker");
		strcpy(personagem.skills[1], "Castle of Stone");
	} else if(!strcmp(classe, "Mago Negro")){
		personagem.EXPMax = 65;
		personagem.vidaMax = 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 1.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	} else if(!strcmp(classe, "Mago Branco")){
		personagem.EXPMax = 60;
		personagem.vidaMax = 20; 
		personagem.manaMax = 30;
		personagem.forcaFisica = 2.0;
		personagem.forcaMagica = 30.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 1.0;
		personagem.armadura = 1.0;
		personagem.resistenciaMagica = 2.0;
	} else if(!strcmp(classe, "Samurai")){
		personagem.EXPMax = 55;
		personagem.vidaMax = 30; 
		personagem.manaMax = 15;
		personagem.forcaFisica = 20.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 5.0;
		personagem.agilidade = 10.0;
		personagem.armadura = 6.0;
		personagem.resistenciaMagica = 6.0;
	} else if(!strcmp(classe, "Arqueiro")){
		personagem.EXPMax = 55;
		personagem.vidaMax = 20; 
		personagem.manaMax = 10;
		personagem.forcaFisica = 15.0;
		personagem.forcaMagica = 10.0;
		personagem.sorte = 10.0;
		personagem.agilidade = 20.0;
		personagem.armadura = 2.0;
		personagem.resistenciaMagica = 2.0;
	} else if(!strcmp(classe, "Imagem")){
		personagem.EXPMax = 0;
		personagem.vidaMax = 0; 
		personagem.manaMax = 0;
		personagem.forcaFisica = 0;
		personagem.forcaMagica = 0;
		personagem.sorte = 0;
		personagem.agilidade = 0;
		personagem.armadura = 0;
		personagem.resistenciaMagica = 0;
	}

	return personagem;
}

/*	Cada classe possui uma curva de evolução diferente. Essa função aumenta
	os status do personagem de acordo com sua classe quando o mesmo sobe o nível.*/
Personagem subirNivel(Personagem personagem){
	
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
	} else if(!strcmp(classe, "Mago Negro")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 1.02; 
		personagem.manaMax *= 1.05;
		personagem.forcaFisica *= 1.01;
		personagem.forcaMagica *= 1.05;
		personagem.sorte *= 1.01;
		personagem.agilidade *= 1.01;
		personagem.armadura *= 1.01;
		personagem.resistenciaMagica *= 1.01;
	} else if(!strcmp(classe, "Mago Branco")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 1.02; 
		personagem.manaMax *= 1.05;
		personagem.forcaFisica *= 1.01;
		personagem.forcaMagica *= 1.07;
		personagem.sorte *= 1.01;
		personagem.agilidade *= 1.01;
		personagem.armadura *= 1.01;
		personagem.resistenciaMagica *= 1.01;
	} else if(!strcmp(classe, "Samurai")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 1.04; 
		personagem.manaMax *= 1.03;
		personagem.forcaFisica *= 1.06;
		personagem.forcaMagica *= 1.03;
		personagem.sorte *= 1.02;
		personagem.agilidade *= 1.02;
		personagem.armadura *= 1.03;
		personagem.resistenciaMagica *= 1.03;
	} else if(!strcmp(classe, "Arqueiro")){
		personagem.EXPMax *= 1.10;
		personagem.vidaMax *= 1.02; 
		personagem.manaMax *= 1.02;
		personagem.forcaFisica *= 1.07;
		personagem.forcaMagica *= 1.02;
		personagem.sorte *= 1.01;
		personagem.agilidade *= 1.01;
		personagem.armadura *= 1.01;
		personagem.resistenciaMagica *= 1.01;
	}

	return personagem;
}

// Os inimigos são criados por essa função.
void definirInimigos() {
	
	//1� Inimigo
	tab1.inimigos[0].id = 0;
	tab1.inimigos[0].nivel = 1;
	tab1.inimigos[0].EXP = 10;
	tab1.inimigos[0].pontos = 10;
	strcpy(tab1.inimigos[0].nome, "Slime");
	tab1.inimigos[0].vidaMax = 25;
	tab1.inimigos[0].manaMax = 25;
	tab1.inimigos[0].forcaFisica = 7;
	tab1.inimigos[0].forcaMagica = 20;	
	tab1.inimigos[0].sorte = 0;
	tab1.inimigos[0].agilidade = 1;
	tab1.inimigos[0].armadura = 10;
	tab1.inimigos[0].resistenciaMagica = 0;
	
	//2� Inimigo
	tab1.inimigos[1].id = 1;
	tab1.inimigos[1].nivel = 1;
	tab1.inimigos[1].pontos = 10;
	tab1.inimigos[1].EXP = 10;
	strcpy(tab1.inimigos[1].nome, "Rato");
	tab1.inimigos[1].vidaMax = 25;
	tab1.inimigos[1].manaMax = 25;
	tab1.inimigos[1].forcaFisica = 7;
	tab1.inimigos[1].forcaMagica = 4;	
	tab1.inimigos[1].sorte = 0;
	tab1.inimigos[1].agilidade = 5;
	tab1.inimigos[1].armadura = 4;
	tab1.inimigos[1].resistenciaMagica = 0;

	//3� Inimigo
	tab1.inimigos[2].id = 2;
	tab1.inimigos[2].nivel = 1;
	tab1.inimigos[2].pontos = 10;
	tab1.inimigos[2].EXP = 10;
	strcpy(tab1.inimigos[2].nome, "Morcego");
	tab1.inimigos[2].vidaMax = 25;
	tab1.inimigos[2].manaMax = 25;
	tab1.inimigos[2].forcaFisica = 7;
	tab1.inimigos[2].forcaMagica = 20;	
	tab1.inimigos[2].sorte = 0;
	tab1.inimigos[2].agilidade = 10;
	tab1.inimigos[2].armadura = 3;
	tab1.inimigos[2].resistenciaMagica = 0;
	
	//4� Inimigo
	tab1.inimigos[3].id = 3;
	tab1.inimigos[3].nivel = 1;
	tab1.inimigos[3].pontos = 10;
	tab1.inimigos[3].EXP = 10;
	strcpy(tab1.inimigos[3].nome, "Caveira");
	tab1.inimigos[3].vidaMax = 25;
	tab1.inimigos[3].manaMax = 25;
	tab1.inimigos[3].forcaFisica = 12;
	tab1.inimigos[3].forcaMagica = 0;	
	tab1.inimigos[3].sorte = 0;
	tab1.inimigos[3].agilidade = 4;
	tab1.inimigos[3].armadura = 7;
	tab1.inimigos[3].resistenciaMagica = 0;
	
	//5� Inimigo
	tab1.inimigos[4].id = 4;
	tab1.inimigos[4].nivel = 1;
	tab1.inimigos[4].pontos = 10;
	tab1.inimigos[4].EXP = 10;
	strcpy(tab1.inimigos[4].nome, "Zumbi Mizeravi");
	tab1.inimigos[4].vidaMax = 25;
	tab1.inimigos[4].manaMax = 25;
	tab1.inimigos[4].forcaFisica = 8;
	tab1.inimigos[4].forcaMagica = 0;	
	tab1.inimigos[4].sorte = 0;
	tab1.inimigos[4].agilidade = 0.5;
	tab1.inimigos[4].armadura = 5;
	tab1.inimigos[4].resistenciaMagica = 0;
	
	//6� Inimigo
	tab1.inimigos[5].id = 5;
	tab1.inimigos[5].nivel = 1;
	tab1.inimigos[5].pontos = 10;
	tab1.inimigos[5].EXP = 10;
	strcpy(tab1.inimigos[5].nome, "Minion Bosta");
	tab1.inimigos[5].vidaMax = 25;
	tab1.inimigos[5].manaMax = 25;
	tab1.inimigos[5].forcaFisica = 5;
	tab1.inimigos[5].forcaMagica = 5;	
	tab1.inimigos[5].sorte = 0;
	tab1.inimigos[5].agilidade = 1;
	tab1.inimigos[5].armadura = 3;
	tab1.inimigos[5].resistenciaMagica = 0;
	
	//7� Inimigo
	tab1.inimigos[6].id = 6;
	tab1.inimigos[6].nivel = 1;
	tab1.inimigos[6].pontos = 10;
	tab1.inimigos[6].EXP = 10;
	strcpy(tab1.inimigos[6].nome, "Lobo");
	tab1.inimigos[6].vidaMax = 25;
	tab1.inimigos[6].manaMax = 25;
	tab1.inimigos[6].forcaFisica = 14;
	tab1.inimigos[6].forcaMagica = 0;	
	tab1.inimigos[6].sorte = 0;
	tab1.inimigos[6].agilidade = 10;
	tab1.inimigos[6].armadura = 9;
	tab1.inimigos[6].resistenciaMagica = 0;
	
	//8� Inimigo
	tab1.inimigos[7].id = 7;
	tab1.inimigos[7].nivel = 1;
	tab1.inimigos[7].pontos = 10;
	tab1.inimigos[7].EXP = 10;
	strcpy(tab1.inimigos[7].nome, "OVNI");
	tab1.inimigos[7].vidaMax = 25;
	tab1.inimigos[7].manaMax = 25;
	tab1.inimigos[7].forcaFisica = 5;
	tab1.inimigos[7].forcaMagica = 15;	
	tab1.inimigos[7].sorte = 0;
	tab1.inimigos[7].agilidade = 4;
	tab1.inimigos[7].armadura = 10;
	tab1.inimigos[7].resistenciaMagica = 0;
	
	//9� Inimigo
	tab1.inimigos[8].id = 8;
	tab1.inimigos[8].nivel = 1;
	tab1.inimigos[8].pontos = 10;
	tab1.inimigos[8].EXP = 10;
	strcpy(tab1.inimigos[8].nome, "Dragao Bebe");
	tab1.inimigos[8].vidaMax = 30;
	tab1.inimigos[8].manaMax = 30;
	tab1.inimigos[8].forcaFisica = 18;
	tab1.inimigos[8].forcaMagica = 20;	
	tab1.inimigos[8].sorte = 0;
	tab1.inimigos[8].agilidade = 5;
	tab1.inimigos[8].armadura = 10;
	tab1.inimigos[8].resistenciaMagica = 5;
	
	//10� Inimigo
	tab1.inimigos[9].id = 9;
	tab1.inimigos[9].nivel = 1;
	tab1.inimigos[9].pontos = 10;
	tab1.inimigos[9].EXP = 10;
	strcpy(tab1.inimigos[9].nome, "Aronguejo");
	tab1.inimigos[9].vidaMax = 25;
	tab1.inimigos[9].manaMax = 25;
	tab1.inimigos[9].forcaFisica = 5;
	tab1.inimigos[9].forcaMagica = 5;	
	tab1.inimigos[9].sorte = 0;
	tab1.inimigos[9].agilidade = 1;
	tab1.inimigos[9].armadura = 10;
	tab1.inimigos[9].resistenciaMagica = 0;

	//11� Inimigo
	tab1.inimigos[10].id = 10;
	tab1.inimigos[10].nivel = 1;
	tab1.inimigos[10].pontos = 0;
	tab1.inimigos[10].EXP = 0;
	strcpy(tab1.inimigos[10].nome, "Imagem");
	tab1.inimigos[10].vidaMax = 0;
	tab1.inimigos[10].manaMax = 0;
	tab1.inimigos[10].forcaFisica = 0;
	tab1.inimigos[10].forcaMagica = 0;	
	tab1.inimigos[10].sorte = 0;
	tab1.inimigos[10].agilidade = 0;
	tab1.inimigos[10].armadura = 0;
	tab1.inimigos[10].resistenciaMagica = 0;
}

/*Fim da sessão de definição de atributos
--------------------------------------------------------*/