#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
/* Handle global da DLL que será carregada dinamicamente */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
# define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
/* Aqui são definidas as assinaturas das funções exportadas pela DLL.
   Cada typedef define um ponteiro para função com o nome correspondente */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
/* Declaração dos ponteiros que receberão os endereços das funções da DLL */
static AbreConexaoImpressora_t AbreConexaoImpressora = NULL;
static FechaConexaoImpressora_t FechaConexaoImpressora = NULL;
static ImpressaoTexto_t ImpressaoTexto = NULL;
static Corte_t Corte = NULL;
static ImpressaoQRCode_t ImpressaoQRCode = NULL;
static ImpressaoCodigoBarras_t ImpressaoCodigoBarras = NULL;
static AvancaPapel_t AvancaPapel = NULL;
static AbreGavetaElgin_t AbreGavetaElgin = NULL;
static AbreGaveta_t AbreGaveta = NULL;
static SinalSonoro_t SinalSonoro = NULL;
static ImprimeXMLSAT_t ImprimeXMLSAT = NULL;
static ImprimeXMLCancelamentoSAT_t ImprimeXMLCancelamentoSAT = NULL;
static InicializaImpressora_t InicializaImpressora = NULL;

/* ======================= Configuração ======================= */
/* Variáveis globais para armazenar dados de configuração da impressora */
static int g_tipo = 1;
static char g_modelo[64] = "i9";
static char g_conexao[128] = "USB";
static int g_parametro = 0;
static int g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name) \
    do { \
        /* Obtém o endereço da função dentro da DLL */ \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name); \
        if (!(name)) { \
            /* Caso não encontre, exibe erro e cancela carregamento */ \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n", \
                    #name, GetLastError()); \
            return 0; \
        } \
    } while (0)

/* Função para limpar buffer do teclado antes de usar fgets */
static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funções para manipular a DLL ======================= */
/* Carrega a DLL e mapeia todas as funções necessárias */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    /* Resolve todas as funções exportadas */
    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

/* Libera a DLL carregada da memória */
static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ========================================================= */
/*             Funções implementadas pelos alunos            */
/* ========================================================= */

/* Função de validação de entrada (faixa, livre ou valores específicos) */
int validacao(int min, int max, int tipo){
    int opcao;
    if(tipo == 0 ){
        scanf("%d", &opcao);
        return opcao;
    } else if (tipo == 1) { /* Validação min/max */
        scanf("%d", &opcao);
        while (opcao < min || opcao > max) {
            printf("Opcao invalida! Tente novamente: ");
            scanf("%d", &opcao);
        }
        return opcao;
    } else if (tipo == 2) { /* Validação para estilos do texto (0,1,2,4,8) */
        scanf("%d", &opcao);
        while (opcao != 0 && opcao != 1 && opcao != 2 &&
               opcao != 4 && opcao != 8) {
            printf("Opcao invalida! Tente novamente: ");
            scanf("%d", &opcao);
        }
        return opcao;
    }
}

/* Mostra menu principal */
int exibirMenu(void)
{
    printf("\n========== MENU ==========\n");
    printf(" [1] - Configurar Conexao\n");
    printf(" [2] - Abrir Conexao\n");
    printf(" [3] - Impressao Texto\n");
    printf(" [4] - Impressao QRCode\n");
    printf(" [5] - Impressao Cod Barras\n");
    printf(" [6] - Impressao XML SAT\n");
    printf(" [7] - Impressao XML Cancel SAT\n");
    printf(" [8] - Abrir Gaveta Elgin\n");
    printf(" [9] - Abrir Gaveta\n");
    printf(" [10] - Sinal Sonoro\n");
    printf(" [0] - Fechar Conexao e Sair\n");
    printf("==========================\n");
    printf("Escolha uma opcao: ");
    int opcao_menu = validacao(0, 10, 1);
    return opcao_menu;
}

/* Configuração de conexão */
static void configurarConexao(void)
{
    printf("\nInforme os dados da impressora (tipo, modelo, conexao e parametro):\n");
    
    printf("===== TIPO =====\n");
    printf("[1] - USB\n");
    printf("[2] - RS232\n");
    printf("[3] - TCP/IP\n");
    printf("[4] - Bluetooth\n");
    printf("[5] - Android\n");
    printf("================\n");
    printf("Escolha um TIPO de conexao: ");
    int tipo = validacao(1,5,1);
    g_tipo = tipo;
    
    if(g_tipo == 5){
        /* Android não requer modelo e parâmetro */
        strcpy(g_modelo,"");
        strcpy(g_conexao, "");
        g_parametro = 0;
    } else {
        printf("=== MODELO ===\n");
        printf("[1] - Impressora i7\n");
        printf("[2] - Impressora i8\n");
        printf("[3] - Impressora i9\n");
        printf("=============================\n");
        printf("Escolha o MODELO da sua impressora: ");
        int opcao_modelo = validacao(1,3,1);

        switch(opcao_modelo){
            case 1: strcpy(g_modelo,"i7"); break;
            case 2: strcpy(g_modelo,"i8"); break;
            case 3: strcpy(g_modelo,"i9"); break;  
            default: strcpy(g_modelo,"i9"); break;
        }

        printf("===== CONEXAO =====\n");
        printf("[1] - USB\n");
        printf("[2] - RS232\n");
        printf("[3] - TCP/IP\n");
        printf("[4] - Bluetooth\n");
        printf("=====================\n");
        printf("Escolha a CONEXAO: ");
        int opcao_conexao = validacao(1,4,1);

        switch (opcao_conexao)
        {
        case 1: strcpy(g_conexao, "USB"); break;
        case 2: strcpy(g_conexao, "COM2"); break;
        case 3: strcpy(g_conexao, "192.168.0.20"); break;
        case 4: strcpy(g_conexao, "AA:BB:CC:DD:EE:FF"); break;
        default: strcpy(g_conexao, "USB"); break;
        }

        if (opcao_conexao == 1 || opcao_conexao == 4){
            g_parametro = 0;
        } else {
            g_parametro = 1; /* Parâmetro adicional */
        }
    }

    printf("Configuracao gravada: tipo=%d modelo=%s conexao=%s parametro=%d\n",
        g_tipo, g_modelo, g_conexao, g_parametro);
}

/* Abre conexão com a impressora */
static void abrirConexao(void)
{
    if(g_conectada == 0){
        int retorno_conexao = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
        if(retorno_conexao == 0){
            g_conectada = 1;
            printf("Conexao realizada com sucesso !!!\n");
        } else {
            printf("Erro ao abrir conexao, retorno %d\n", retorno_conexao);
        }
    } else {
        printf("Ja existe conexao aberta.\n");
    }
}

/* Fecha conexão */
static void fecharConexao(void)
{
    if(g_conectada == 1){
        int retorno_conexao = FechaConexaoImpressora();
        if(retorno_conexao == 0){
            g_conectada = 0;
            printf("Conexao FECHADA com sucesso !!!\n");
        } else {
            printf("Erro ao fechar conexao, retorno %d\n", retorno_conexao);
        }
    } else {
        printf("Nao ha conexao aberta.\n");
    }
}

/* Impressão de texto */
static void imprimirTexto(void)
{
    char texto[1024];

    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }
        
    /* limpar buffer antes de usar fgets */
    flush_entrada();
    printf("Digite o texto que deseja imprimir: ");
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        texto[0] = '\0';
    }

    /* Remove \n do final */
    texto[strcspn(texto, "\n")] = '\0';

    printf("=== Posicionamento do texto ===\n");
    printf("[0] - Alinhar a Esquerda\n");
    printf("[1] - Centralizado\n"); 
    printf("[2] - Alinhar a Direita\n");
    printf("=============================\n");
    int posicionamento = validacao(0,2,1);

    printf("=== Estilo do texto (valores aceitos: 0,1,2,4,8) ===\n");
    printf("[0] - Fonte A\n");
    printf("[1] - Fonte B\n");
    printf("[2] - Sublinhado\n");
    printf("[4] - Modo reverso\n");
    printf("[8] - Negrito\n");
    printf("=============================\n");
    int estilo = validacao(0,8,2);

    printf("=== Tamanho do texto ===\n");
    printf("[0] - 1x (normal)\n");
    printf("[1] - 2x altura\n");
    printf("[2] - 3x altura\n");
    printf("[3] - 4x altura\n");
    printf("=============================\n");
    int tamanho = validacao(0,3,1);

    int retorno_conexao = ImpressaoTexto(texto, posicionamento, estilo, tamanho);
    AvancaPapel(3);
    Corte(1); 
    if(retorno_conexao == 0){
        printf("Impressao de texto enviada com sucesso !!!\n");
    } else {
        printf("Erro na impressao de texto, retorno %d\n", retorno_conexao);
    }
}

/* Impressão de QRCode */
static void imprimirQRCode(void)
{
    if (g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }

    char texto[512];
    flush_entrada();
    printf("Digite o texto que deseja imprimir no QRCode: ");
    if (fgets(texto, sizeof(texto), stdin) == NULL) texto[0] = '\0';
    texto[strcspn(texto, "\n")] = '\0';

    int retorno_conexao = ImpressaoQRCode(texto, 6, 4);
    AvancaPapel(3);
    Corte(1);

    if(retorno_conexao == 0){
        printf("QRCode impresso com sucesso !!!\n");
    }else{
        printf("Erro ao imprimir QRCode, retorno %d\n", retorno_conexao);
    }
}

/* Impressão de código de barras */
static void imprimirCodigoBarras(void)
{
    if (g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }
    flush_entrada();

    int retorno_conexao = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
    AvancaPapel(3);
    Corte(1);
    if(retorno_conexao == 0){
        AvancaPapel(3);
        Corte(1);
        printf("Codigo de barras impresso com sucesso !!!\n");
    }else{
        printf("Erro ao imprimir codigo de barras, retorno %d\n", retorno_conexao);
    }
}
/* ======================================================================== */
/* ========================== imprimirXMLSAT ============================== */
/* ======================================================================== */
static void imprimirXMLSAT(void)
{
    /* Antes de tudo, verifica se a impressora está conectada */
    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }

    /* 
       Aqui abrimos o arquivo XMLSAT.xml no modo binário ("rb").
       Esse arquivo deve conter o XML completo emitido pelo SAT.
    */
    FILE *file = fopen("./XMLSAT.xml", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo XMLSAT.xml");
        return;
    }

    /* Move o ponteiro do arquivo para o final para descobrir o tamanho */
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Erro no fseek");
        fclose(file);
        return;
    }

    /* Captura o tamanho total do arquivo */
    long tamanho = ftell(file);
    if (tamanho <= 0) {
        printf("Arquivo vazio ou erro ao obter tamanho.\n");
        fclose(file);
        return;
    }

    /* Volta o ponteiro ao início para poder ler o arquivo */
    rewind(file);

    /* Aloca memória para armazenar o conteúdo do XML (mais 1 para '\0') */
    char *conteudo = (char *)malloc((size_t)tamanho + 1);
    if (conteudo == NULL) {
        printf("Erro ao alocar memoria.\n");
        fclose(file);
        return;
    }

    /* Lê o arquivo inteiro para o buffer */
    size_t lidos = fread(conteudo, 1, (size_t)tamanho, file);
    if (lidos != (size_t)tamanho) {
        printf("Erro ao ler o arquivo XMLSAT. Lidos=%zu Esperado=%ld\n", lidos, tamanho);
        free(conteudo);
        fclose(file);
        return;
    }

    /* Finaliza string com terminador nulo */
    conteudo[tamanho] = '\0';
    fclose(file);

    /*
        Chamada para a função da DLL:

        ImprimeXMLSAT(const char* xml, int param)

        - xml  → conteúdo completo do XML convertido para string
        - param → geralmente 0 (modo padrão de impressão)

        A DLL irá interpretar o XML e montar automaticamente o DANFE-SAT 
        em formato de impressão típica (cupom).
    */
    int retorno_conexao = ImprimeXMLSAT(conteudo, 0);

    /* Avança papel e realiza corte após impressão */
    AvancaPapel(3);
    Corte(1);

    /* Verifica retorno da DLL */
    if(retorno_conexao == 0){
        printf("XML SAT impresso com sucesso !!!\n");
    } else {
        printf("Erro ao imprimir XML SAT, retorno %d\n", retorno_conexao);
    }

    /* Libera memória alocada */
    free(conteudo);
}
/* =============================================================================== */
/* ===================== imprimirXMLCancelamentoSAT ============================== */
/* =============================================================================== */
static void imprimirXMLCancelamentoSAT(void)
{
    /* Verifica se a impressora está conectada */
    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }

    /*
        Abre o XML de cancelamento do SAT.
        Esse arquivo deve conter o XML retornado pelo SAT quando um cupom é cancelado.
    */
    FILE *file = fopen("./CANC_SAT.xml", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo CANC_SAT.xml");
        return;
    }

    /* Move ponteiro para o final para obter tamanho total */
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Erro no fseek");
        fclose(file);
        return;
    }

    /* Tamanho do XML */
    long tamanho = ftell(file);
    if (tamanho <= 0) {
        printf("Arquivo vazio ou erro ao obter tamanho.\n");
        fclose(file);
        return;
    }

    /* Volta ao início do arquivo */
    rewind(file);

    /* Buffer para o conteúdo do XML */
    char *conteudo = (char *)malloc((size_t)tamanho + 1);
    if (conteudo == NULL) {
        printf("Erro ao alocar memoria.\n");
        fclose(file);
        return;
    }

    /* Lê o conteúdo do arquivo */
    size_t lidos = fread(conteudo, 1, (size_t)tamanho, file);
    if (lidos != (size_t)tamanho) {
        printf("Erro ao ler o arquivo de cancelamento. Lidos=%zu Esperado=%ld\n", lidos, tamanho);
        free(conteudo);
        fclose(file);
        return;
    }
    conteudo[tamanho] = '\0';
    fclose(file);
    /*
        ASSINATURA DIGITAL DO CANCELAMENTO

        Essa string contém a assinatura base64 da operação de cancelamento do SAT.
        Ela é obrigatória para que a DLL valide e imprima o cancelamento corretamente.

        O primeiro parâmetro → conteúdo do XML  
        O segundo parâmetro → assinatura digital  
        O terceiro parâmetro → modo de impressão (normalmente 0)
    */
    const char *assinatura = 
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
    /*
        Chamada principal:

        ImprimeXMLCancelamentoSAT(xml_cancelado, assinatura, param)

        A DLL usará:
            - o XML de cancelamento
            - a assinatura digital
        para reconstruir o DANFE de cancelamento e mandá-lo para impressão.
    */
    int retorno_conexao = ImprimeXMLCancelamentoSAT(conteudo, assinatura, 0);

    AvancaPapel(3);
    Corte(1);

    if(retorno_conexao == 0){
        printf("XML de cancelamento impresso com sucesso !!!\n");
    } else {
        printf("Erro ao imprimir XML de cancelamento, retorno %d\n", retorno_conexao);
    }

    free(conteudo);
}
/* Abre gaveta Elgin */
static void abrirGavetaElginOpc(void)
{
    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }
    int retorno_conexao = AbreGavetaElgin(1, 50, 50);
    if(retorno_conexao == 0){
        printf("Gaveta Elgin disparada com sucesso !!!\n");
    } else {
        printf("Erro ao abrir gaveta Elgin, retorno %d\n", retorno_conexao);
    }
}
/* Abre gaveta padrão */
static void abrirGavetaOpc(void)
{
    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }
    int retorno_conexao = AbreGaveta(1, 5, 10);
    if(retorno_conexao == 0){
        printf("Gaveta padrï¿½o disparada com sucesso !!!\n");
    } else {
        printf("Erro ao abrir gaveta padrï¿½o, retorno %d\n", retorno_conexao);
    }
}
/* Emite sinal sonoro da impressora */
static void emitirSinalSonoro(void)
{
    if(g_conectada != 1){
        printf("Abra a conexao primeiro!!!\n");
        return;
    }
    int retorno_conexao = SinalSonoro(4, 50, 5);
    if(retorno_conexao == 0){
        printf("Sinal sonoro emitido com sucesso !!!\n");
    } else {
        printf("Erro ao emitir sinal sonoro, retorno %d\n", retorno_conexao);
    }
}
/* ======================= Função principal ======================= */
int main(void) {

    /* Primeiro carrega função e mapeia todas as funções da DLL */
    if (!carregarFuncoes()) return 1;

    int opcao;

    while (1) {
        opcao = exibirMenu();
        switch (opcao) {

            case 1:
               configurarConexao(); break;

            case 2:
               abrirConexao(); break;

            case 3:
                imprimirTexto();
                break;

            case 4:
                imprimirQRCode();
                break;

            case 5:
                imprimirCodigoBarras();
                break;

            case 6:
                imprimirXMLSAT();
                break;

            case 7:
                imprimirXMLCancelamentoSAT();
                break;

            case 8:
                abrirGavetaElginOpc();
                break;

            case 9:
                abrirGavetaOpc();
                break;

            case 10:
                emitirSinalSonoro();
                break;

            case 0:
                fecharConexao();
                liberarBiblioteca();
                printf("Encerrando o sistema...\n");
                return 0;
        }
    }
}