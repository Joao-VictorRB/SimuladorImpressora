## **Sistema de Impressão Elgin em C**

## **Visão Geral**

Este projeto implementa um sistema em C que simula a comunicação com uma impressora térmica da Elgin. A ideia é aproximar o funcionamento de um PDV real (ponto de venda), permitindo que o usuário teste operações típicas de impressão, como texto, QR Code, código de barras, abertura de gaveta e impressão de XML SAT.

O programa carrega dinamicamente a DLL da Elgin, inicializa ponteiros de função e executa chamadas reais da API da impressora.

---

# **Como o sistema funciona**

## **1 - Carregamento da DLL**

Logo no início da execução, o programa chama:

```
\\carregarFuncoes();
```

Essa função usa `LoadLibraryA()` para carregar a DLL **E1_Impressora01.dll**.
Logo após, cada função da impressora é vinculada ao seu ponteiro, usando `GetProcAddress()`.

Isso permite que o programa chame as funções reais da DLL sem precisar incluí-las no código.

Caso alguma função não seja encontrada, o sistema exibe o erro e não inicia.

---

## **2 - Configurações da impressora**

Antes de abrir a conexão, o sistema permite configurar:

* Tipo da impressora (USB, RS232, TCP/IP etc.)
* Modelo
* Tipo de conexão
* Parâmetro numérico

Esses valores são guardados nas variáveis globais:

```
g_tipo
g_modelo
g_conexao
g_parametro
```

Esses dados só poderão ser usados quando o usuário escolhe **abrir a conexão**.

---

## **3 - Abertura e fechamento da conexão**

A função responsável é:

```
AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
```

Se retornar **1**, significa sucesso.
O programa guarda essa informação em:

```
g_conectada = 1;
```

Da mesma forma, para fechar a conexão:

```
FechaConexaoImpressora();
```

**OBS:** O sistema só permite imprimir se a conexão estiver aberta.

---

## **4 - Impressão de texto**

Quando o usuário escolhe a opção 3, o sistema executa:

```
ImpressaoTexto("teste", 1, 4, 0);
```

Esses parâmetros indicam:

* Texto
* Alinhamento
* Estilo
* Tamanho da fonte

Após imprimir, o programa ainda faz:

```
Corte(4);
```

---

## **5 - Impressão de QR Code**

O usuário digita um texto, e o sistema executa:

```
ImpressaoQRCode(Texto, 6, 4);
```

Depois avança o papel e corta:

```
AvancaPapel(1);
Corte(1);
```

---

## **6 - Impressão de código de barras**

Primeiro, imprime um código padrão:

```
ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
```

Depois, imprime o texto que o usuário digitar.

---

## **7 -  Impressão de XML SAT**

O sistema abre o arquivo **XMLSAT.xml**, lê todo o conteúdo e envia para impressão usando:

```
ImprimeXMLSAT(conteudo, 0);
```

Esse recurso simula a impressão de um cupom fiscal completo.

---

## **8 - Abertura da gaveta**

O sistema possui a forma:

```
AbreGavetaElgin(1, 50, 50);

```

  Simula a abertura da gaveta do caixa.

---

## **9 - Sinal sonoro**

Usa:

```
SinalSonoro(4, 50, 5);
```

Simula o famoso “beep” da impressora.

---

# **Menu do Sistema**

O programa utiliza um laço `while(1)` que exibe o menu continuamente:

```
1 – Configurar Conexao
2 – Abrir Conexao
3 – Impressao Texto
4 – Impressao QRCode
5 – Impressao Cod Barras
6 – Impressao XML SAT
7 – Impressao XML Canc SAT
8 – Abrir Gaveta Elgin
9 – Abrir Gaveta Normal
10 – Sinal Sonoro
0 – Fechar Conexao e Sair
```

Cada opção chama uma função correspondente.

O menu só para quando o usuário escolhe **0**.

---

# **Funções da DLL utilizadas**

Essas são todas as funções carregadas da DLL:

* `AbreConexaoImpressora()`
* `FechaConexaoImpressora()`
* `ImpressaoTexto()`
* `Corte()`
* `ImpressaoQRCode()`
* `ImpressaoCodigoBarras()`
* `AvancaPapel()`
* `AbreGavetaElgin()`
* `AbreGaveta()`
* `SinalSonoro()`
* `ImprimeXMLSAT()`
* `ImprimeXMLCancelamentoSAT()`
* `InicializaImpressora()`

---

# **Conclusão**

O projeto integra conceitos importantes, como:

* Manipulação de DLLs
* Ponteiros para funções
* Estruturas de decisão
* Laços de repetição
* Leitura de arquivos
* Interação com o usuário
* Implementação de um menu dinâmico

O resultado é um sistema funcional que simula o uso real de uma impressora fiscal que pode ser usada em lojas fiscais, departamentos financeiros e também em situações reais de impressão direta. 

# 🔧 Devs

| Nome                   | GitHub                                       | LinkedIn                                                                       |
| ---------------------- | -------------------------------------------- | ------------------------------------------------------------------------------ |
| 👤 João Victor Batista | [GitHub](https://github.com/Joao-VictorRB)   | [LinkedIn](https://www.linkedin.com/in/jo%C3%A3ovictorbatista/)                |
| 👤 Isabelle Pimenta    | [GitHub](https://github.com/isabellepimenta) | [LinkedIn](https://www.linkedin.com/in/isabelle-pimenta-santos/)               |
| 👤 Rafael Rodrigues    | [GitHub](https://github.com/RafinhaAlmeida)  | [LinkedIn](https://www.linkedin.com/in/rafael-rodrigues-de-almeida-a45ba2300/) |
| 👤 Eduardo             | [GitHub](https://github.com/Eduardo0711)     | [LinkedIn](https://www.linkedin.com/in/eduardo-reis-221a0939a/)                |
| 👤 Samuel Sena         | [GitHub](https://github.com/Samuelsena9)     | [LinkedIn](https://www.linkedin.com/in/samuel-sena-036a3a257/)                 |
