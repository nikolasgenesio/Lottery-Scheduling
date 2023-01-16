#include "lottery.h"
#include <stdio.h>
#include <string.h>

// variaveis auxiliares
const char nameLottery[] = "LOTT";
int indexLottery = -1;
int distributedTickets = 0;
int totalTickets;

/**
 * @brief funcao que realiza a soma dos tickets dos processos prontos
 *
 * @param p processo
 */
void addTickProcess(Process *p)
{
	LotterySchedParams *params;				   // cria o ponteiro
	params = processGetSchedParams(p);		   // pega os parametros
	params->num_minor_ticket = totalTickets;   // recebe os tickets
	totalTickets += params->num_tickets;	   // soma os tickets
	params->num_greater_ticket = totalTickets; // recebe os tickets
}

/**
 * @brief Funcao que realiza a inicializacao do escalonador
 *
 */
void lottInitSchedInfo()
{
	SchedInfo *sched = malloc(sizeof(SchedInfo)); // cria o ponteiro

	totalTickets = 0; // inicializa o total de tickets

	// nome do escalonador
	for (int i = 0; i < 4; i++)
		sched->name[i] = nameLottery[i];

	// funcoes necessarias para o escalonador funcionar
	sched->initParamsFn = &lottInitSchedParams;
	sched->notifyProcStatusChangeFn = &lottNotifyProcStatusChange;
	sched->scheduleFn = &lottSchedule;
	sched->releaseParamsFn = &lottReleaseParams;

	indexLottery = schedRegisterScheduler(sched); // registra o algoritmo de escalonamento
}

/**
 * @brief Funcao que inicializa os parametros de escalonamento de um processo
 *
 * @param p processo
 * @param params parametro
 */
void lottInitSchedParams(Process *p, void *params)
{
	schedSetScheduler(p, params, indexLottery);
}

/**
 * @brief Funcao que recebe a notificacao que um processo mudou de estado
 *
 * @param p processo
 */
void lottNotifyProcStatusChange(Process *p)
{
	LotterySchedParams *params; // ponteiro para os parametros

	int status = processGetStatus(p); // retorna o estatus do processo

	if (status == PROC_READY) // se o processo estiver pronto
		addTickProcess(p);	  // chama a funcao que adiciona os tickets
	else
		distributedTickets = 1;
}

/**
 * @brief Funcao que realiza o escalonamento por loteria
 *
 * @param plist processo
 * @return Process* processo sorteado
 */
Process *lottSchedule(Process *plist)
{
	Process *p = plist;			  // ponteiro para o processo atual
	Process *drawnProcess = NULL; // ponteiro para o processo sorteado

	LotterySchedParams *params; // ponteiro para os parametros

	int drawn_ticket = -1; // bilhete sorteado

	// se ja distribuiu os tickets
	if (distributedTickets == 1)
	{
		Process *proc;					// cria o ponteiro para o processo
		LotterySchedParams *procParams; // cria o ponteiro para os parametros
		totalTickets = 0;

		// laco para rodar por toda a lista de processos
		for (proc = plist; proc != NULL; proc = processGetNext(proc))
		{
			if (processGetStatus(proc) == PROC_READY) // se o processo estiver pronto
				addTickProcess(proc);				  // adiciona os tickets
		}

		distributedTickets = 0;
	}

	drawn_ticket = rand() % totalTickets; // sorteia o numero aleatorio entre 0 e o total de tickets

	printf("Numero aleatorio: %d\n", drawn_ticket); // imprime na tela

	// laco para rodar por toda a lista de processos
	for (p = plist; p != NULL; p = processGetNext(p))
	{
		if (processGetStatus(p) == PROC_READY) // se o processo estiver pronto
		{
			params = processGetSchedParams(p); // pega seus parametros

			if (drawn_ticket >= params->num_minor_ticket &&
				drawn_ticket < params->num_greater_ticket) // verifica a soma e caso ele for o sorteado
			{
				drawnProcess = p; // ele eh o processo sorteado
				break;
			}
		}
	}

	return drawnProcess; // retorna o processo sorteado
}

/**
 * @brief Funcao que libera os parametros de escalonamento de um processo
 *
 * @param p processo
 * @return int numero do slot do processo que ele estava associado
 */
int lottReleaseParams(Process *p)
{
	int slot = processGetSchedSlot(p); // inicializa o slot

	LotterySchedParams *params = processGetSchedParams(p); // inicializa os parametros
	free(params);										   // desaloca

	return slot;
}

/**
 * @brief Funcao que realiza a transferencia de tickets entre dois processos
 *
 * @param src processo origem
 * @param dst processo destino
 * @param tickets numero de tickets
 * @return int numero de tickets que foi transferido
 */
int lottTransferTickets(Process *src, Process *dst, int tickets)
{
	int transfer; // tickets transferidos

	// pega os paramentros dos dois processos
	LotterySchedParams *proc1 = processGetSchedParams(src);
	LotterySchedParams *proc2 = processGetSchedParams(dst);

	// realiza a verificacao para a transferencia
	if (proc1->num_tickets < tickets)
		transfer = proc1->num_tickets;
	else
		transfer = tickets;

	// tira de um processo e adiciona no outro
	proc1->num_tickets -= transfer;
	proc2->num_tickets += transfer;

	distributedTickets = 1;
	return transfer;
}
