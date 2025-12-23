# Sistemas-operacionais
Trabalhos relacionados a disciplina de sistemas operacionais 

Trabalho 1: Sinais - Escreva um programa que realiza tratamento de sinais POSIX. O programa deve:
1 - Capturar os sinais SIGINT, SIGTSTP e SIGPIPE mudando suas operações para
O sinal SIGING deve ser ignorado
Contar quantas vezes o sinal SIGTSTP é gerado pelo usuário
O SIGPIPE deve zera o contador do SIGTSTP
2 - Utilize um for(EVER) para manter o processo vivo enquanto gera sinais
3 - Gerar os seguintes sinais de teste
CTRL+C, no terminal (número indefinido)
SIGPIPE pelo comando kill no terminal
SIGTSTP a cada 10 segundos
4 - Se o contador de SIGTSTP chegar a 12, o processo deve gerar um sinal a ele mesmo cuja ação default seja seu término

Trabalho 2: Fork/wait - Faça um programa em que três processos executam paralelamente as seguintes ações:
Pai - Imprime os números de 1 a 50, com um intervalo de 2 segundos entre cada número.
Após imprimir todos os números, imprime a frase “Processo pai vai morrer”.
Filho1 - Imprime os números de 100 a 200, com um intervalo de 1 segundo entre cada número.
Antes de imprimir os números, imprime a frase “Filho 1 foi criado”.
Após imprimir todos os números, imprime a frase “Filho 1 vai morrer”.
Filho 2 - Inicia apeas após o pai morrer, imprime os números de 51 a 99
Importante, em cada printf os processos devem imprimir o seu pid e o pid do seu pai.

Trabalho 3: Fork/wait/exec - Faça um programa em 2 processos são iniciados utilizando fork cada um com as seguintes ações:
Os filho 1 irá executar cálculo 2*3, e retornar ao pai, via exit() o resultado da equação (utilize exec() para fazer isso)
O filho 2 irá entrar em looping infinito
O pai, após receber o resultado do filho 1, irá terminar o looping infinito do filho 2. Para isso o pai deve enviar um sinal ao filho 2, esse deve capturar e mudar o valor de iteração do while() ou for())
Agora com o filho 2 acordado, o pai deve imprimir o resultado
Filho 2 deve virar zoobies

Trabalho 4: Escolha duas séries quaisquer para aproximar o número pi do site Pi series 1 ou Pi series 2 ou outro site qualquer. Veja exemplos sequenciais em Pi sequencial. Implemente a versão paralela dessas séries, u lizando pthreads, sequindo os seguintes requisitos:
Devem ser calculados pelo menos 1 bilhão (10^9) de termos de cada série.
Use variáveis reais de dupla precisão (double) nos cálculos;
O programa deve dividir o espaço de cálculo uniformemente entre as N threads
e.x. 1bilhão de termos com 2 threads = 500milhões de termos em cada thread;
cada thread efetua uma soma parcial de forma autônoma;
para evitar o uso de mecanismos de sincronização, cada thread T[i] deve depositar seu resultado parcial na posição result[i] de um vetor de resultados parciais.
Após o término das threads de cálculo, o programa principal soma os resultados parciais ob dos por elas e apresenta o resultado ﬁnal na tela;
Execute as threads no seu computador pessoal
execute as soluções com N = {1, 2, 4, 8 e 16} threads
Marque o tempo necessário para calcular Pi para cada N em cada máquina e faça um gráﬁco de linhas (NxTempo) apresentado os resultados ob dos em ambas as máquinas
Compare o resultado das duas soluções (series) escolhidas, indicando qual série é mais eﬁciente em termos de tempo e qualidade da solução (i.e. valor mais exato de pi)

Trabalho 5: Jantar de Gauleses - Uma tribo gaulesa janta em comunidade a par de uma mesa enorme com espaço para M javalis grelhados. Quando um gaulês quer comer, serve-se de um javali da mesa a menos que esta já esteja vazia. Nesse caso o gaulês acorda o cozinheiro e aguarda que este reponha javalis na mesa. O código seguinte representa o código que implementa o gaulês e o cozinheiro.
void Gaules()
{
  while(true)
  {
    Javali j = RetiraJavali();
    ComeJavali(j);
  }
}
void Cozinheiro(){
  while(true)
  {
    ColocaJavalis(M);
  }
}
Implemente o código das funções RetiraJavali() e ColocaJavalis() incluindo código de sincronização que previna deadlock e acorde o cozinheiro apenas quando a mesa está vazia.
Lembre que existem muitos gauleses e apenas um cozinheiro.
Identiﬁque regiões críticas na vida do gaules e do cozinheiro.
A solução deve aceitar um numero N de gauleses igual ao número de letras de seu primeiro nome e 1 único cozinheiro.
Cada gaules terá um nome, dado pela letra correspondente 
  – Ex: Lucas = 5 gauleses
Cada gaules deve imprimir na tela seu nome (dado pela letra) quando come e quando acorda o cozinheiro.
– Ex: Gaules L(0) comendo
– Ex: Gaules U(1) acordou cozinheiro
A quantidade javalis grelhados M deve ser igual ao número dos dois primeiros dígitos de seu RA (2157217)

Trabalho 6 - Jantar de Gauleses - Implemente o problema do Jantar de Gauleses usando shared memory 
Pode usar funções da shmem (system V) ou mmap (POSIX), escolha livre. Recomendo padrão POSIX
Deve, obrigatóriamente ter um executável, código fonte separado para o produtor e outro para o consumidor
A sincronização deve acontecer por semáforos em memória compartilhada, obrigatóriamente
Deve, obrigatóriamente ter um executável, código fonte separado para o produtor e outro para o consumidor
Os programas podem ser lançados em background (&) ou u lizando fork/exec

Trabalho 7 - Implemente o problema do Jantar de Gauleses usando MPI (família mpi_send) ou POSIX (família mk_send) escolha livre.
Deve, obrigatóriamente ter um executável, código fonte separado para o produtor e outro para o consumidor
A sincronização deve acontecer por passagem de mensagem, obrigatóriamente
Recomendo padrão POSIX, pois foi a ensinada nos vídeos. Para quem quiser usar MPI tem slides, e vc irá precisar instalar o pacote.

Trabalho 8 - Jantar de Gauleses - implemente o problema do Jantar de Gauleses usando pipes 
Usar pipes ou named pipes
Deve, obrigatóriamente ter um executável, código fonte separado para o produtor e outro para o consumidor
A sincronização deve acontecer por pipes, obrigatóriamente
Controlar o tamanho do buﬀer usando pipes é problemá co, já que o pipe tem um tamanho ﬁxo dado pelo sistema operacional. Sugestões de solução para ‘emular’ um buﬀer usando pipes 
não fazer nada (terá um pequeno desconto na nota) 
descobrir o tamanho do pipe e mandar dados de tamanho correto. Ex, pipe 100kb total, buﬀer de 5 elementos, cada ‘write’ ou ‘read’ no pipe feito com 20kb (100/5)
utilizar um esquema de passagem de mensagens (consumidor envia 5 mensagens vazias para produtor)
Desenvolva uma estratégia para comunicar 1 produtor (cozinheiro) e N consumidores (gauleses). Sugestão u lizar threads no processo gaules.
