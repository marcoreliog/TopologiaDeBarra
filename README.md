Utilizei um código em c++ que deve ser usado junto ao simulador discreto de redes Ns-3. Esse código em específico cria uma rede que usa a topologia em barra (bus topology) com 5 nós, e cada nó possuindo dois sockets, um para enviar e outro para receber.
O primeiro nó deve enviar um inteiro de 0 a 100 para o segundo, e ir repassando até chegar no quinto, este que irá atualizar o inteiro que está na rede e repassar pela rede fazendo o caminho inverso até chegar no segundo nó, e a partir daí, segue um ciclo do segundo nó ao quinto.

- Usei o protocolo de internet TCP/Ipv4
- A simulação dura 30 segundos
- conexão sem fio wifi

No final ao rodar o código c++ se espera um arquivo .xml para ser usado no simulador visual de redes Netanim, mas talvez não dê para entender direito o que está acontecendo por causa da qualidade do programa.

*Pretendo criar um simulador visual no cisco packet tracer que simule esse mesmo processo com a ajuda da linguagem python*
