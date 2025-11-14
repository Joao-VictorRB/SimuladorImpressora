# Projeto Final - Sistema de Impressora Elgin

## 🎯 Objetivo
Desenvolver um programa em linguagem C que simule um sistema de comunicação com impressoras de cupom fiscal da Elgin, utilizando funções específicas da biblioteca da impressora, laços de repetição, estruturas condicionais e funções personalizadas. O foco é integrar lógica de programação com um cenário prático de automação comercial.

## 🧩 Descrição do Projeto
Este projeto implementa um sistema de atendimento de caixa (PDV simplificado), permitindo ao usuário selecionar operações básicas por meio de um menu interativo. O sistema se comunica com a impressora utilizando funções simuladas da biblioteca oficial da Elgin.

### Funcionalidades:
- Estabelecer e encerrar a conexão com a impressora.
- Imprimir texto no cupom.
- Imprimir QR Code.
- Imprimir código de barras.
- Avançar papel.
- Realizar corte do papel.
- Emitir sinal sonoro.
- Abrir gaveta de dinheiro.
- Imprimir XML de venda SAT ou de cancelamento.

### Estrutura do Menu:
```
1 - Configurar Conexao
2 - Abrir Conexao
3 - Impressao Texto
4 - Impressao QRCode
5 - Impressao Cod Barras
6 - Impressao XML SAT
7 - Impressao XML Canc SAT
8 - Abrir Gaveta Elgin
9 - Abrir Gaveta
10 - Sinal Sonoro
11 - Avancar Papel
12 - Corte do Papel
0 - Fechar Conexao e Sair
```

## 🔧 Funções da biblioteca Elgin utilizadas
- AbreConexaoImpressora()
- FechaConexaoImpressora()
- ImpressaoTexto()
- Corte()
- ImpressaoQRCode()
- ImpressaoCodigoBarras()
- AvancaPapel()
- AbreGavetaElgin()
- AbreGaveta()
- SinalSonoro()
- ImprimeXMLSAT()
- ImprimeXMLCancelamentoSAT()

## 🚀 Como executar o projeto
### Pré-requisitos:
- GCC instalado (Linux/Mac) ou MinGW (Windows).
- VS Code com extensão C/C++ (opcional).

### Passos:
1. Clone o repositório:
   ```bash
   git clone https://github.com/seuusuario/projeto-impressora-elgin.git
   ```
2. Acesse a pasta do projeto:
   ```bash
   cd projeto-impressora-elgin
   ```
3. Compile o código:
   ```bash
   gcc projeto_final.c -o projeto_final
   ```
4. Execute o programa:
   ```bash
   ./projeto_final
   ```
   (No Windows: `projeto_final.exe`)

## 📂 Estrutura do Repositório
```
projeto-impressora-elgin/
├── projeto_final.c      # Código-fonte principal
├── README.md            # Documentação do projeto
```

## ✅ Demonstração
O sistema exibe um menu interativo, permitindo ao usuário escolher operações. Caso a conexão não esteja aberta, o sistema alerta para abrir a conexão antes de executar funções.

Exemplo:
```
=== SISTEMA DE IMPRESSORA ELGIN ===
Escolha uma opcao: 2
✅ Conexao com a impressora aberta com sucesso!
Escolha uma opcao: 3
🖨️ Imprimindo texto no cupom...
Escolha uma opcao: 0
🔒 Conexao com a impressora encerrada.
Saindo do sistema...
```

## 📌 Observações
Este projeto é uma simulação. Para uso real, substitua as funções simuladas pelas funções da biblioteca oficial Elgin.
