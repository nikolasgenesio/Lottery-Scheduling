#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"
#include "lottery.h"

// automatizar os processos
#define SCHED_ITERATIONS 1				  // iteracoes
#define PROCESS_CREATION_PROBABILITY 0.3  // probabilidade de criacao
#define PROCESS_DESTROY_PROBABILITY 0.05  // probabilidade de remocao
#define PROCESS_BLOCK_PROBABILITY 0.6	  // probabilidade de bloqueio
#define PROCESS_UNBLOCK_PROBABILITY 0.4	  // probabilidade de desbloqueio
#define PROCESS_TCKTRANSF_PROBABILITY 0.1 // probabilidade de transferencia de tickets

/**
 * @brief Funcao para inicializar parametros
 *
 * @param p processo
 */
void dumpSchedParams(Process *p)
{
	LotterySchedParams *lsp = processGetSchedParams(p);
	printf("Tickets: %d", lsp->num_tickets);
}

/**
 * @brief Funcao responsavel por criar um processo
 *
 * @param plist processo
 * @param ppid identificador do processo pai
 * @param num_tickets numero de tickets do processo
 * @return Process* processo criado
 */
Process *createProcess(Process *plist, int ppid, int num_tickets)
{
	LotterySchedParams *lsp;
	printf("Criando processo... ");
	// inicializa os parametros
	plist = processCreate(plist);
	lsp = malloc(sizeof(LotterySchedParams));
	lsp->num_tickets = num_tickets;
	lottInitSchedParams(plist, lsp);
	processSetStatus(plist, PROC_READY);
	processSetParentPid(plist, ppid);
	printf(" Criado PID %d!\n", processGetPid(plist));
	return plist; // retorna o processo
}

/**
 * @brief Funcao responsavel por destruir totalmente um processo
 *
 * @param plist processo
 * @param pid identificador do processo
 * @return Process* processo do inicio
 */
Process *destroyProcess(Process *plist, int pid)
{
	// processo de destruicao
	printf("Destruindo processo... ");
	plist = processDestroy(plist, pid);
	printf(" Destruido PID %d!\n", pid);
	return plist;
}

/**
 * @brief Funcao para contar a quantidade de processos prontos
 *
 * @param plist processo
 * @return int quantidade de processos prontos
 */
int countReady(Process *plist)
{
	Process *p, *next;
	int count = 0;

	// percorre a lista
	for (p = plist; p != NULL; p = next)
	{
		next = processGetNext(p);
		if (processGetStatus(p) == PROC_READY)
			count++;
	}
	return count;
}

/**
 * @brief Funcao para obter um processo pronto atraves de um numero aleatorio
 *
 * @param plist processo
 * @param n numero aleatorio
 * @return Process* processo
 */
Process *getNthReady(Process *plist, int n)
{
	Process *p = NULL, *next;
	int count = 0;

	// percorre a lista
	for (p = plist; p != NULL; p = next)
	{
		next = processGetNext(p);
		if (processGetStatus(p) == PROC_READY) // verifica os processos prontos
			count++;						   // incrementa
		if (count == n)						   // verifica se eh igual ao numero aleatorio
			return p;						   // retorna o processo
	}
	return NULL;
}

/**
 * @brief Funcao para realizar acoes aleatorias
 *
 * @param plist processo
 * @return Process* processo modificado
 */
Process *randomActions(Process *plist)
{
	Process *p, *next, *dst;			  // auxiliadores
	int pid, n, transfer, transferred;	  // auxiliadores
	int ready;							  // auxiliar processo pronto
	double r = rand() / (double)RAND_MAX; // sorteio de um numero aleatorio
	printf("===Acoes Aleatorias===\n");
	// se o numero aleatorio eh menor que a probabilidade de criacao do processo, cria o novo processo
	if (r < PROCESS_CREATION_PROBABILITY)
		plist = createProcess(plist, 1, (rand() % 100 + 1) * 100);

	// percorre a lista de processos
	for (p = plist; p != NULL; p = next)
	{
		next = processGetNext(p); // processo posterior
		pid = processGetPid(p);	  // identificador do processo
		if (pid == 1)
			continue;
		r = rand() / (double)RAND_MAX; // sorteio de um numero aleatorio

		// se o processo esta pronto e o numero aleatorio eh menor que a probabilidade de destruicao do processo
		if (processGetStatus(p) == PROC_READY &&
			r < PROCESS_DESTROY_PROBABILITY)
		{
			plist = destroyProcess(plist, processGetPid(p)); // destroi o processo
			continue;
		}
		r = rand() / (double)RAND_MAX; // sorteio de um numero aleatorio

		// se o processo esta executando e o numero aleatorio eh menor que a probabilidade de bloqueio do processo
		if (processGetStatus(p) == PROC_RUNNING &&
			r < PROCESS_BLOCK_PROBABILITY)
		{
			processSetStatus(p, PROC_WAITING);	   // altera o status para aguardando
			r = rand() / (double)RAND_MAX;		   // sorteio de um numero aleatorio
			if (r < PROCESS_TCKTRANSF_PROBABILITY) // se o numero aleatorio eh menor que a probabilidade de transferencia do processo
			{
				ready = countReady(plist) - 1; // quantidade de processos prontos
				if (ready > 0)				   // se a quantidade de processos prontos for maior que zero
				{
					n = rand() % ready + 1;				 // sorteia um numero aleatorio entre 1 e a quantidade de processos prontos
					transfer = (rand() % 100 + 1) * 100; // sorteio um numero aleatorio para a transferencia
					dst = getNthReady(plist, n);		 // pega um processo para ser transferido
					transferred = lottTransferTickets(p, dst,
													  transfer); // realiza a transferencia
					printf("Transferidos %d tickets do processo %d para processo %d, de %d solicitados\n",
						   transferred, pid,
						   processGetPid(dst), transfer);
				}
			}
			printf("Bloqueado processo %d\n", pid); // boqueia o processo
		}

		// se o processo esta aguardando e o numero aleatorio eh menor que sua probabilidade de desbloqueio
		else if (processGetStatus(p) == PROC_WAITING && r < PROCESS_UNBLOCK_PROBABILITY)
		{
			processSetStatus(p, PROC_READY); // desbloqueia o processo e coloca como pronto
			printf("Desbloqueado processo %d\n", pid);
		}
	}

	printf("======================\n");
	return plist;
}

int main(void)
{
	int i = 0, step = 0;
	char c = ' ';
	Process *plist = NULL, *p1 = NULL;

	srand(time(NULL));

	// inicializa escalonadores de processos
	schedInitSchedInfo();
	lottInitSchedInfo();

	//cria o primeiro processo com PPID e tickets 1
	plist = createProcess(plist, 1, 1);
	printf("\n");

	//realiza os casos para a iteracao com o usuario
	while (c != 'n')
	{
		switch (i)
		{
		case 0:
			printf("(Passo:%d)\n", step);
			plist = randomActions(plist);
			printProcess(plist, dumpSchedParams);
			printf("\n");
			i++;
			break;
		case SCHED_ITERATIONS + 1:
			printf("(Passo:%d/Iteracoes:%d)\n", step, i - 1);
			printProcess(plist, dumpSchedParams);
			step++;
			i = 0;
			printf("\nContinuar (s/n)? ");
			fflush(stdout);
			c = getchar();
			printf("\n");
			break;
		default:
			p1 = schedSchedule(plist);
			i++;
		}
	}
	return 0;
}
