#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct para informaçoes da palavra
typedef struct Infor{
	int linha;
	int repeticoes;
	struct Infor *proxInfor;
}Infor;

//struct para as palavras
typedef struct Palavra{
	char palavra[20];
	Infor *infor;
	struct Palavra *proxPalavra;
}Palavra;

//funcoes
void limparegistro();
void registrarStr(char *str, int *linha);
int palavraExistente(char *str);
int contaCarac(char *str);
Infor *registrarInfor(int *linha);
void imprime();
void libera();

//variaveis globais
int registro[400];
Palavra *Patual = NULL;

int main(){
	limparegistro();
	char url[]="texto.txt";
	char chr;
	int linha=0, coluna = 0;
	int cont=0;
	FILE *arq;
	char aux[20];
	arq = fopen(url, "r");
	if(arq == NULL){
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}
	
	//contando caracteres ate encontrar um espaco ou \n, obtendo uma palavra
	while( (chr=fgetc(arq))!= EOF ){
		if(chr == 32 || chr == '\n'){
	   		aux[cont] = '\0';
	   		cont = 0;
	   		registrarStr(aux, &linha);
	   		if(chr == '\n'){
	   			linha += 1;
			}
			continue;
		}
		aux[cont++] = chr;
	}	
	aux[cont] = '\0';	
	registrarStr(aux, &linha);	
	fclose(arq);
	
	imprime();
	libera();
}

void registrarStr(char *str, int *linha){
	//verifica se a palavra ja foi registrada ou nao
	if(!palavraExistente(str)){
		Palavra *auxpal = Patual;
		Infor *auxInfor;
		while(strcmp(auxpal->palavra,str) != 0){
			auxpal = auxpal->proxPalavra;
		}
		auxInfor = auxpal->infor;
		while(auxInfor->linha != *linha){
			auxInfor = auxInfor->proxInfor;
			if(auxInfor == NULL){
				break;
			}
		}
		if(auxInfor != NULL){
			auxInfor->repeticoes += 1;
		}else{
			auxInfor = auxpal->infor;
			while(auxInfor->proxInfor != NULL){
				auxInfor = auxInfor->proxInfor;
			}
			auxInfor->proxInfor = registrarInfor(linha);
		}
	}else{
		Palavra *novapal = (Palavra*) malloc(sizeof(Palavra));
		novapal->proxPalavra = Patual;
		strcpy(novapal->palavra, str);
		Patual = novapal;
		novapal->infor = registrarInfor(linha);
		registro[contaCarac(str)] = contaCarac(str);
	}
}

Infor *registrarInfor(int *linha){
	Infor *novoInfor = (Infor*) malloc(sizeof(Infor));
	novoInfor->linha = *linha;
	novoInfor->repeticoes = 1;
	novoInfor->proxInfor = NULL;
	return novoInfor;
}

int contaCarac(char *str){
	int j,cont = 0;
	for(j=0;j<strlen(str);j++){
		cont += str[j];
		cont -= 97;
	}
	return cont;
}

int palavraExistente(char *str){
	int cont = contaCarac(str);
	if(registro[cont] != cont){
		return 1;
	}else{
		return 0;
	}
}

void imprime(){
	Palavra *auxpal = Patual;
	Infor *auxInfor;
	while(auxpal != NULL){
		auxInfor = auxpal->infor;
		printf("%s",auxpal->palavra);
		while(auxInfor != NULL){
			printf("->");
			printf("(%d,%d)",auxInfor->linha,auxInfor->repeticoes);
			auxInfor = auxInfor->proxInfor;
		}
		auxpal = auxpal->proxPalavra;
		printf("\n");
	}
}

void libera(){
	Palavra *ultimaPal, *penultimaPal;
	Infor *ultimaInfor, *penultimaInfor;
	ultimaPal = Patual;
	penultimaPal = NULL;
	while(ultimaPal != NULL){
		ultimaInfor = ultimaPal->infor;
		penultimaInfor = NULL;
		
		while(ultimaInfor != NULL){
			penultimaInfor = ultimaInfor;
			ultimaInfor = ultimaInfor->proxInfor;
			free(penultimaInfor);
		}
		free(ultimaInfor);
		
		penultimaPal = ultimaPal;
		ultimaPal = ultimaPal->proxPalavra;
		free(ultimaPal);
	}
	free(ultimaPal);
}

void limparegistro(){
	int i;
	for(i=0;i<400;i++){
		registro[i] = -1;
	}
}
