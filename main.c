// Importação de bibliotecas
#include <stdio.h>
#include <locale.h>
#include <string.h>

// Declaração de constantes
#define QUANTIDADE_LINHAS_PULAR 3
#define CODIGO_CARACTERE_SUBSTITUTO 42
#define ARQUIVO_USER "lista_usuarios.txt"

// Declaração de procedimentos
void exibeMenuInicial(int admin);
void pularLinhas(int quantidadeLinhas);
void criptografaSenha(char* senhaResgatada);
void incluirUsuario();
void alterarUsuario();
void excluirUsuario();
void listagemUsuarios();

// Declaração de funções
int resgataOpcao(int admin);
int verificaIntegridadeSenha(char* senhaResgatada);
int verificaUsuarioCadastrado(char* nomeUsuario);
int cargo();

// Execução do programa
int main() {
	setlocale(LC_ALL, "Portuguese");

    // Declaração de variáveis
    int continuarPrograma = 1, opcaoEscolhida, admin;
    admin = cargo();

    // Iniciando o programa
    while (continuarPrograma) {

        // Exibindo as opções
        exibeMenuInicial(admin);
        opcaoEscolhida = resgataOpcao(admin);

        // Direcionando o usuário
        if (opcaoEscolhida != 0) {

            // Redirecionando o usuário
            switch (opcaoEscolhida) {
                case 1:
                    incluirUsuario();
                    break;

                case 2:
                	if(admin){
                    	alterarUsuario();
                    }else{
                    	printf("Apenas administradores podem alterar usuários");	
					}
                    break;
                case 3:
                	if(admin){
                		excluirUsuario();
					}else{
						printf("Apenas administradores podem excluir usuários");
					}
                    break;
                case 4:
                    listagemUsuarios();
                    break;
                case 5:
                    printf("Encerrando o programa\n");
                    continuarPrograma = 0;
                    break;
                default:
                    printf("Opcao invalida\n");
            }
            pularLinhas(QUANTIDADE_LINHAS_PULAR);
            
        } else {
            printf("Opcao invalida\n");
        }
        pularLinhas(QUANTIDADE_LINHAS_PULAR);
    }
    return 0;
}

// Implementação de procedimentos
void exibeMenuInicial(int admin) {
    // Mostrando as opções disponíveis
    printf("===== Menu Inicial =====");
    if(admin){
    	printf("\n\n1. Cadastro de usuario\n2. Alteracao de usuario\n3. Exclusao de usuario\n4. Listagem de usuarios\n5. Sair\n\n");	
	}else{
		printf("\n\n1. Cadastro de usuario\n4. Listagem de usuarios\n5. Sair\n\n");
	}
}

void pularLinhas(int quantidadeLinhas) {
    // Pulando uma quantidade determinada de linhas
    for (int i = 0; i < quantidadeLinhas; i++) {
        printf("\n");
    }
}

void criptografaSenha(char* senhaResgatada) {
    // Declaração de variáveis
    int tamanhoSenha, somador, codigoNovoCaractere; 
    somador = tamanhoSenha = strlen(senhaResgatada);

    // Alterando todos os caracteres da senha
    for (int i = 0; i < tamanhoSenha; i++) {
        // Atribuições
        codigoNovoCaractere = (int) senhaResgatada[i] + somador;

        // Verificando se o caractere está fora da tabela ASCII
        if (codigoNovoCaractere > 126) {
            codigoNovoCaractere = CODIGO_CARACTERE_SUBSTITUTO;
        } 
        
        // Aplicando a mudança
        senhaResgatada[i] = (char) codigoNovoCaractere;    
        somador--;
    }    
}

int resgataOpcao(int admin) {
    // Declaração de variáveis
    int opcao; 

    // Entradas e saídas
    printf("Informe a opcao desejada: ");
    scanf("%d", &opcao);

    // Realizando a verificação
    if (opcao < 1 || opcao > 5 || (admin == 0 && (opcao == 2 || opcao == 3))) {
        return 0;
    }
    return opcao;
}

int cargo(){
	char cargo[20];
	
	printf("Informe o cargo (admin/funcionario): ");
	scanf("%s", cargo);
	
	if(strcmp(cargo, "admin") == 0){
		return 1;
	}else if(strcmp(cargo, "funcionario") == 0){
		return 0;
	}else{
		printf("Cargo inválido");
	}
}

int verificaUsuarioCadastrado(char* nomeUsuario){ // função para verificar se existe algum usuário com o nome inserido
    FILE *arquivo;
	arquivo = fopen(ARQUIVO_USER, "r");
    if(arquivo == NULL){
        printf("Criando o arquivo.\n");
        return 0;
    }
    
    char usuario[30], linha[100];
    while(fgets(linha, sizeof(linha), arquivo)){
        sscanf(linha, "%[^,]", usuario); // Lê a linha até encontrar a virgula
        if(strcmp(usuario, nomeUsuario) == 0){
            fclose(arquivo);
            return 1; // retornando 1 se o usuario for encontrado
        }
    }

    fclose(arquivo);
    return 0; // retorna 0 se o usuario nao for encontrado
}

void incluirUsuario(){ // função para adicionar usuarios em "lista_usuarios.txt"
    char nomeUsuario[30], senha[20];

    printf("Informe um nome de usuario (apelido): ");
    scanf("%s", nomeUsuario);

    if(verificaUsuarioCadastrado(nomeUsuario)){
        printf("Usuario já cadastrado\n");
        return;
    }

    printf("Informe uma senha: ");
    scanf("%s", senha);

    if(verificaIntegridadeSenha(senha) == 0){ // verificando se a senha atende os requisitos minimos especificados na função "verificaIntegridadeSenha"
        printf("A senha deve ter pelo menos:\n6 caracteres\n1 letra maiuscula\n1 letra minuscula\n1 numero\n");
        return;
    }

    criptografaSenha(senha);
    
    FILE* arquivo;
	arquivo = fopen(ARQUIVO_USER, "a");
	
    if(arquivo == NULL){
        printf("Erro ao encontrar o arquivo. Criando arquivo.\n");
        arquivo = fopen(ARQUIVO_USER, "w");
    }
    
    fprintf(arquivo, "%s, %s\n", nomeUsuario, senha);
    fclose(arquivo);
    printf("Usuario cadastrado.\n");
}

void alterarUsuario(){ // função para alterar algum usuário em "lista_usuarios.txt"
    char nomeUsuario[30], novaSenha[20], linha[100];
    int encontrado = 0;

    printf("Informe o nome do usuario para alterar: ");
    scanf("%s", nomeUsuario);

    if(verificaUsuarioCadastrado(nomeUsuario) == 0){ // verificando se o usuario existe
        printf("Usuario nao encontrado\n");
        return;
    }

    printf("Informe uma nova senha: ");
    scanf("%s", novaSenha);

    if(verificaIntegridadeSenha(novaSenha) == 0){ // verificando se a senha atende os requisitos minimos especificados na função "verificaIntegridadeSenha"
        printf("A senha deve ter pelo menos:\n6 caracteres\n1 letra maiuscula\n1 letra minuscula\n1 numero.\n");
        return;
    }
    criptografaSenha(novaSenha); // chama a função para criptografar a senha
    
    FILE *arquivo;
	arquivo = fopen(ARQUIVO_USER, "r");
    FILE *arq_temporario;
	arq_temporario = fopen("arq_temporario.txt", "w");

    while(fgets(linha, sizeof(linha), arquivo)){ // lê o arquivo linha por linha 
        char usuario[30], senha[20];
        sscanf(linha, "%[^,], %s", usuario, senha); // separa a linha em duas partes, antes da primeira virgula é o usuario, após ela é a senha

        if(strcmp(usuario, nomeUsuario) == 0){ // compara o nome lido na linha com o nome inserido pelo usuário
            fprintf(arq_temporario, "%s, %s\n", usuario, novaSenha); // se for igual, a senha é substituída e a linha é copiada para o arquivo temporário
            encontrado = 1;
        }else{
            fprintf(arq_temporario, "%s", linha); // se não for igual, a linha é copiado para o arquivo temporário sem alterações
        }
    }
    fclose(arquivo);
    fclose(arq_temporario);

    remove(ARQUIVO_USER); // exclui o arquivo original 
    rename("arq_temporario.txt", ARQUIVO_USER); // arquivo temporário passa a ser o arquivo original
    
    if(encontrado){
        printf("Senha alterada\n");
    }
}

void excluirUsuario(){ // função para excluir usuário do arquivo "lista_usuarios.txt"
    char nomeUsuario[30], linha[100];
    int encontrado = 0;

    printf("Informe o nome do usuario que deseja excluir: ");
    scanf("%s", nomeUsuario);

    FILE *arquivo;
	arquivo = fopen(ARQUIVO_USER, "r");
    FILE *arq_temporario;
	arq_temporario = fopen("arq_temporario.txt", "w");

    while(fgets(linha, sizeof(linha), arquivo)){ // ler linha por linha
        char usuario[30];
        sscanf(linha, "%[^,]", usuario); // ler todos os caracteres da linha até encontrar a virgula e armazenar em "usuario"
        if(strcmp(usuario, nomeUsuario) != 0){ // comparando o nome da linha com o nome digitado para ser excluído
            fprintf(arq_temporario, "%s", linha); // caso nao seja o mesmo nome, será passado para o arquivo temporário
        }else{
            encontrado = 1; // o nome foi encontrado e nao será copiado para o arquivo temporário
        }
    }
    fclose(arquivo);
    fclose(arq_temporario);

    remove(ARQUIVO_USER); // deleta o arquivo original
    rename("arq_temporario.txt", ARQUIVO_USER); // o arquivo temporário passa a ser o arquivo original

    if(encontrado){
        printf("Usuario excluido\n");
    }else{
        printf("Usuario nao encontrado\n");
    }
}

void listagemUsuarios(){ // função para listar todos os usuários cadastrados no arquivo "lista_usuarios.txt"
    FILE *arquivo;
	arquivo = fopen(ARQUIVO_USER, "r");
    char linha[100], usuario[30];

    if(arquivo == NULL){
        printf("Nenhum usuário cadastrado\n");
        return;
    }

    printf("===== Lista de Usuários =====\n");
    while(fgets(linha, sizeof(linha), arquivo)){
        sscanf(linha, "%[^,]", usuario); // ler ate encontrar a virgula e armazena na variavel usuario
        printf("- %s\n", usuario); // imprimindo todos os usuarios linha por linha, com um travessao na frente
    }
    fclose(arquivo);
}

int verificaIntegridadeSenha(char* senhaResgatada) {

    // Declaração de variáveis
    int tamanhoSenha = strlen(senhaResgatada), tamanhoValido, senhaValida;

    // Atribuições
    tamanhoValido = tamanhoSenha >= 6;

    // Verificando se a senha possui um tamanho válido
    if (tamanhoValido) {

        // Declaração de variáveis
        int quantidadeLetrasMaiusculas, quantidadeLetrasMinusculas, quantidadeNumeros, 
        letraMaiuscula, letraMinuscula, numero, codigoCaractere; 

        // Atribuições
        quantidadeLetrasMaiusculas = quantidadeLetrasMinusculas = quantidadeNumeros = 0;

        // Verificando se a senha atende aos requisitos
        for (int i = 0; i < tamanhoSenha; i++) {

            // Atribuições
            codigoCaractere = (int) senhaResgatada[i];
            letraMaiuscula = codigoCaractere >= 65 && codigoCaractere <= 90;
            letraMinuscula = codigoCaractere >= 97 && codigoCaractere <= 122;
            numero = codigoCaractere >= 48 && codigoCaractere <= 57;

            // Realizando as verificações
            if (letraMaiuscula) {
                quantidadeLetrasMaiusculas++;
            } else if (letraMinuscula) {
                quantidadeLetrasMinusculas++;
            } else if (numero) {
                quantidadeNumeros++;
            }

        } 
        
        // Atribuições
        senhaValida = quantidadeLetrasMaiusculas >= 1 && quantidadeLetrasMinusculas >= 1 && quantidadeNumeros >= 1;

    } if (senhaValida) {
        return 1;
    } return 0;

}
