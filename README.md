<h1 align="center">🎰 Escalonamento por Loteria 🎰 </h1>

<p align="center">Projeto desenvolvido em C durante o trabalho avaliativo da disciplina de Sistemas Operacionais (DCC062), ministrada pelo professor Eduardo Pagani Julio do DCC - UFJF.</p>



## 📌 Conceitos

<h3>Escalonador</h3>

<p align="justify">Quando um computador é multiprogramado, ele tem processos competindo pela CPU a todo momento. Isso ocorre quando dois processos ou mais estão em estado pronto. Se uma CPU estiver disponível, é preciso escolher qual processo vai ser executado em seguida. O escalonador é a parte do sistema operacional que realiza essa escolha, e o algoritmo que ele usa é chamado de algoritmo de escalonamento.</p>

<h3>Escalonamento por Loteria</h3>
<p align="justify">O escalonamento por loteria foi proposto em 1994 por <a href="https://www.usenix.org/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf"> Waldspurger e Weihl </a> e define baseado no número de bilhetes e de forma probabilística qual vai ser o próximo processo a rodar, ou seja, vai ser um sorteio para isso, e não de forma determinística. A cada "rodada" o sistema vai ver quantos bilhetes de processos prontos existem e sorteia um número aleatório desses N bilhetes.</p>

<h3>Algoritmo</h3>
<p align="justify">A figura abaixo ilustra como o algoritmo funciona.</p>

<p align="center">
  <img src="http://www.scielo.org.co/img/revistas/dyna/v82n189/v82n189a27fig01.gif" width="400" title="hover text">
</p>


<p align="justify">A figura abaixo mostra um exemplo de como esse algoritmo funciona. Para selecionar o processo vencedor, o algoritmo gera um número aleatório entre um e o total de tickets do sistema. Em seguida, ele percorre a fila de execução acumulando o número de tickets vistos até agora. Para cada processo na fila, ele verifica se o número acumulado de tickets é maior que o número aleatório. Se for, então este seria o processo vencedor que contém o bilhete de loteria sorteado aleatoriamente. Observe que o total de tickets no sistema é 40 e o número aleatório é 35. Para cada processo, verificamos se o número de tickets acumulados até o momento é maior que 35. Continuamos até chegar ao quarto processo, que é declarado para ser o vencedor e recebe a CPU.</p>
<p align="center">
  <img src="http://www.scielo.org.co/img/revistas/dyna/v82n189/v82n189a27fig02.gif" width="400" title="hover text">
</p>

<h3>Gerenciamento de Recursos</h3>

<h4>Transferência de Bilhetes</h4>
<p align="justify">São transferências explícitas de bilhetes de um cliente para  outro. As transferências de tickets podem ser usadas em qualquer situação em que um cliente bloqueie devido a alguma dependência. Um processo bloqueado pode transferir seus tickets para o processo que detém o desejado recurso.</p>

<h4>Inflação de Bilhetes</h4>
<p align="justify">Um cliente pode criar mais tickets para sí mesmo. Por um lado, esta abordagem pode apresentar problemas no sentido de que um cliente pode monopolizar um recurso criando um grande número de bilhetes de loteria.</p>

<h4>Bilhetes de Compensação</h4>
<p align="justify">Se um cliente usa apenas uma fração f de sua unidade de recurso alocada (por exemplo, ciclos/tempo de CPU), pode ser atribuída uma compensação que infla seu número de tickets por um fator de 1/f. Isso garante justiça e melhor capacidade de resposta em caso de escalonamento de processos. A compensação ajuda esses processos a obter sua parcela justa da CPU.</p>

## 🎮 Rodando aplicação

```bash
# Clone este repositório
git clone https://github.com/nikolasgenesio/Lottery-Scheduling.git

# Acesse a pasta do projeto no terminal/cmd
cd Lottery-Scheduling

# Compile o programa
gcc -o lottery main.c

# Execute o programa
.\lottery.exe

```
## 🛠 Tecnologias

As seguintes ferramentas foram usadas na construção do projeto:


- [Git](https://git-scm.com/)

## 👨‍💻 Autores
- Ágata Meireles Carvalho
- Matheus Luz Boaventura
- Nikolas Oliver Sales Genesio
- Thiago do Vale Cabral
