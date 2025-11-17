
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- 1. ESTRUTURAS DE DADOS ---

// Estrutura para um Território (conforme os requisitos)
typedef struct {
    char nome[30];
    char cor[10]; // Dono do território
    int tropas;
} Territorio;

// Estrutura para o Jogador
typedef struct {
    char nome[50];
    char cor[10];
    // Missão armazenada DINAMICAMENTE (ponteiro)
    char* missao; 
} Jogador;

// --- 2. FUNÇÕES DE UTILIDADE E MISSÃO ---

/**
 * Rola um dado e retorna um valor entre 1 e 6.
 */
int rolarDado() {
    return (rand() % 6) + 1;
}

/**
 * Atribui uma missão aleatória a um jogador.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // Usa strcpy para copiar a string da missão sorteada para o destino alocado
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * Verifica se o jogador cumpriu sua missão.
 * A lógica é simples: se o jogador dominar pelo menos 4 territórios,
 * ele é considerado vitorioso (simulando uma missão de domínio).
 *
 * @param missao A string da missão (não utilizada na lógica simplificada, mas exigida).
 * @param mapa O vetor de territórios.
 * @param tamanho O número total de territórios.
 * @return 1 se a missão for cumprida, 0 caso contrário.
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int territoriosDominados = 0;
    
    // Conta quantos territórios o jogador (pela sua cor) domina
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosDominados++;
        }
    }

    // Lógica de verificação simplificada (EXEMPLO): 
    // Se a missão for domínio, checa se dominou X territórios.
    if (territoriosDominados >= 4) { // Exemplo: Missão de dominar 4 ou mais territórios
        return 1; // Missão cumprida
    }
    
    return 0; // Missão ainda não cumprida
}

// --- 3. FUNÇÃO DE ATAQUE ---

/**
 * Simula a rolagem de dados para um ataque.
 * * @param atacante Ponteiro para o Território atacante.
 * @param defensor Ponteiro para o Território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Requisito: O jogador só pode atacar territórios INIMIGOS
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Você não pode atacar um território de sua própria cor (%s).\n", defensor->cor);
        return;
    }

    if (atacante->tropas <= 1) {
        printf("\n[ERRO] O ataque falhou. É necessário pelo menos 2 tropas no território atacante.\n");
        return;
    }
    
    // Rolagem de Dados (simplificado para uma única rolagem para cada)
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("\n--- SIMULAÇÃO DE ATAQUE ---\n");
    printf("Território Atacante (%s - %s) vs. Território Defensor (%s - %s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado Atacante: %d\n", dadoAtacante);
    printf("Dado Defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // ATACANTE VENCE
        printf(">>> Vitória do Atacante! <<<\n");
        
        // Requisito: Transfere a cor e metade das tropas
        int tropasTransferidas = atacante->tropas / 2;
        
        // 1. Transfere a cor (o atacante conquista o território)
        strcpy(defensor->cor, atacante->cor);
        
        // 2. Move as tropas
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;

        printf("Território %s CONQUISTADO! Agora pertence a %s.\n", defensor->nome, defensor->cor);
        printf("%d tropas transferidas. %s agora tem %d tropas.\n", 
               tropasTransferidas, atacante->nome, atacante->tropas);
        
    } else {
        // DEFENSOR VENCE OU EMPATE (Defensor sempre ganha no empate no War)
        printf(">>> Vitória do Defensor. <<<\n");
        
        // Requisito: Atacante perde uma tropa
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa. %s agora tem %d tropas.\n", atacante->nome, atacante->tropas);
    }
    printf("---------------------------\n");
}

// --- 4. FUNÇÃO DE LIBERAÇÃO ---

/**
 * Libera toda a memória alocada dinamicamente (missão e mapa).
 */
void liberarMemoria(Jogador* jogador, Territorio* mapa) {
    printf("\n[INFO] Liberando memória alocada dinamicamente...\n");
    
    // Libera a memória alocada para a missão do jogador (char*)
    if (jogador != NULL && jogador->missao != NULL) {
        free(jogador->missao);
        jogador->missao = NULL; 
        printf("-> Memória da missão liberada.\n");
    }
    
    // Libera a memória alocada para o mapa (vetor de Territorio)
    if (mapa != NULL) {
        free(mapa);
        printf("-> Memória do mapa de territórios liberada.\n");
    }
}


// --- 5. MAIN (FLUXO PRINCIPAL) ---

int main() {
    // Requisito: Usar srand(time(NULL)) para gerar números aleatórios
    srand(time(NULL));

    // A. CRIAÇÃO DO VETOR DE MISSÕES
    char* missoes[] = {
        "DOMINAR 4 TERRITÓRIOS E ELIMINAR O JOGADOR VERMELHO",
        "CONQUISTAR 5 TERRITÓRIOS E 2 CONTINENTES",
        "DOMINAR 6 TERRITÓRIOS SEGUIDOS",
        "DOMINAR 10 TERRITÓRIOS",
        "CONQUISTAR A AMÉRICA DO NORTE E A ÁSIA"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);
    
    // B. ALOCAÇÃO DINÂMICA DO MAPA (Territórios)
    int totalTerritorios = 5;
    // Requisito: Usar calloc/malloc para alocar vetores de territórios
    Territorio* mapa = (Territorio*)calloc(totalTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro na alocação de memória para o mapa!\n");
        return 1;
    }

    // Inicialização do Mapa (dados da tela de console da imagem a5600e8a-a2ea-46cb-a4a3-b6e8c309e4f0)
    strcpy(mapa[0].nome, "AMERICA"); strcpy(mapa[0].cor, "AZUL"); mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "EUROPA");  strcpy(mapa[1].cor, "VERDE"); mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "ÁSIA");    strcpy(mapa[2].cor, "VERMELHO"); mapa[2].tropas = 2;
    strcpy(mapa[3].nome, "ÁFRICA");  strcpy(mapa[3].cor, "AMARELO"); mapa[3].tropas = 4;
    strcpy(mapa[4].nome, "OCEANIA"); strcpy(mapa[4].cor, "BRANCO"); mapa[4].tropas = 1;


    // C. INICIALIZAÇÃO DO JOGADOR
    Jogador p1;
    strcpy(p1.nome, "Player 1");
    strcpy(p1.cor, "AZUL");

    // Requisito: Alocar memória DINAMICAMENTE para armazenar a missão
    p1.missao = (char*)malloc(100 * sizeof(char));
    if (p1.missao == NULL) {
        printf("Erro na alocação de memória para a missão!\n");
        liberarMemoria(NULL, mapa); // Libera o mapa antes de sair
        return 1;
    }

    // D. ATRIBUIÇÃO DA MISSÃO
    atribuirMissao(p1.missao, missoes, totalMissoes);

    // E. EXIBIÇÃO INICIAL DO ESTADO
    printf("--- WAR ESTRUTURADO (DESAFIO FINAL) ---\n");
    printf("Jogador: %s (%s)\n", p1.nome, p1.cor);
    printf("SUA MISSÃO: %s\n", p1.missao);
    printf("---------------------------------------\n\n");
    
    printf("--- ESTADO INICIAL DO MAPA ---\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("%d. %-10s (Exército: %-8s, Tropas: %d)\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------\n");

    // F. SIMULAÇÃO DE AÇÕES (Ataque)
    printf("\n** SIMULAÇÃO DE UM TURNO (JOGADOR AZUL) **\n");
    
    // O jogador AZUL ataca o território VERDE (mapa[1]) a partir da AMERICA (mapa[0])
    atacar(&mapa[0], &mapa[1]); 

    // G. VERIFICAÇÃO DO VENCEDOR E EXIBIÇÃO DO NOVO ESTADO
    printf("\n--- ESTADO DO MAPA APÓS ATAQUE ---\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("%d. %-10s (Exército: %-8s, Tropas: %d)\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("----------------------------------\n");

    // Requisito: Exibição condicional da vitória
    if (verificarMissao(p1.missao, mapa, totalTerritorios, p1.cor)) {
        printf("\n******************\n");
        printf("PARABÉNS %s! SUA MISSÃO FOI CUMPRIDA! VOCÊ VENCEU!\n", p1.nome);
        printf("****************\n");
    } else {
        printf("\n[INFO] A missão de %s ainda não foi cumprida. O jogo continua...\n", p1.nome);
    }

    // H. LIBERAÇÃO DA MEMÓRIA
    // Requisito: Usar free() para evitar vazamentos de memória.
    liberarMemoria(&p1, mapa); 
    
    return 0;
}