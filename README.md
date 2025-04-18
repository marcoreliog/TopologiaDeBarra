# Topologia de Barra com Ciclo de Envio no NS-3

Este projeto simula uma topologia de barra no simulador de redes NS-3 (versão 3.35), onde um arquivo (representado por um valor aleatório) é transmitido de nó em nó até chegar ao nó 4. Ao chegar no nó 4, a mensagem retorna para o nó 1, reiniciando o ciclo de comunicação.

## Visão Geral
- 5 nós estão dispostos em uma topologia linear (barra).
- A comunicação entre os nós ocorre através de conexões TCP sobre uma rede Wi-Fi AdHoc.
- A cada chegada de pacote no nó 4, é gerado um novo valor aleatório que é enviado de volta até o nó 1.
- A lógica de direção dos pacotes depende do remetente: se veio do nó anterior, segue para o próximo, caso contrário, segue para o anterior.
- A simulação tem duração total de 30 segundos.

## Estrutura do Código

### Arquivos Incluídos
- Módulos do NS-3 necessários: `core`, `network`, `internet`, `mobility`, `wifi`, `applications`, `netanim`

### Principais Funções
- `IniciarEnvio`: inicia a transmissão a partir do nó 0 para o nó 1.
- `EnviarParaVizinho`: envia um valor para um nó vizinho.
- `ReceberPacote`: lógica de recebimento e encaminhamento de pacotes, com geração de novo valor nos nós 4 e 1.

### Configurações de Mobilidade
- Todos os nós usam o modelo `ConstantPositionMobilityModel`, mantendo posições fixas em linha reta.

### Rede e Sockets
- Configuração Wi-Fi AdHoc (802.11b)
- Cada nó tem um socket de envio e um de recepção

### Animação
- Geração de arquivo `topologia-barra.xml` para visualização no NetAnim
- Atribuição de descrições e ícones aos nós (ex: `pc.png`, `server.png`)

## Requisitos
- **NS-3.35** (ou compatível)
- `pc.png` e `server.png` (se desejar uma visualização personalizada no NetAnim)
- NetAnim (opcional, para visualização da simulação)

## Observações
- O código foi escrito e testado com a versão 3.35 do NS-3.
- Pode não funcionar corretamente em versões anteriores ou posteriores devido às mudanças na API.
- A sincronização exata de 30 segundos pode variar dependendo da execução, mas é garantida no NetAnim.

## Execução
Compile e execute este código com as ferramentas padrões do NS-3. Após a execução, o arquivo XML gerado pode ser carregado no NetAnim para observação visual da topologia em funcionamento.

---

Se quiser expandir esse exemplo ou adaptar para outros modelos de topologia (anel, malha, árvore), basta ajustar a lógica de encaminhamento nos nós.

