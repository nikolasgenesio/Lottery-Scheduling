#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "scheduler.h"

struct proc
{
	int pid;			// Identificador do Processo
	int ppid;			// Identificador do Processo Pai
	int status;			// Status do Processo
	int cpu_usage;		// Tempo total de uso da CPU
	int sched_slot;		// Slot do algoritmo de escalonamento associado
	void *sched_params; // Pont generico para parametros de escalonamento
	struct proc *prev;	// Encadeamento processo anterior
	struct proc *next;	// Encadeamento processo posterior
};

/**
 * @brief Funcao que retorna o PID (identificador do Processo) de um processo
 *
 * @param p processo
 * @return int PID
 */
int processGetPid(Process *p)
{
	return p->pid;
}

/**
 * @brief Funcao que retorna o PPID (identificador do Processo Pai) de um processo
 *
 * @param p processo
 * @return int PPID
 */
int processGetParentPid(Process *p)
{
	return p->ppid;
}

/**
 * @brief Funcao que retorna o status atual de um processo
 *
 * @param p processo
 * @return int status
 */
int processGetStatus(Process *p)
{
	return p->status;
}

/**
 * @brief Funcao que retorna o tempo de CPU usado por um processo
 *
 * @param p processo
 * @return int tempo de CPU usado
 */
int processGetCpuUsage(Process *p)
{
	return p->cpu_usage;
}

/**
 * @brief Funcao que retorna o ponteiro para os parametros de escalonamento
 *
 * @param p processo
 * @return void* ponteiro para os parametros
 */
void *processGetSchedParams(Process *p)
{
	return p->sched_params;
}

/**
 * @brief Funcao que retorna o slot ocupado pelo algoritmo de escalonamento
 *
 * @param p processo
 * @return int slot ocupado
 */
int processGetSchedSlot(Process *p)
{
	return p->sched_slot;
}

/**
 * @brief Funcao que aponta o ponteiro para o proximo processo da lista
 *
 * @param p processo
 * @return Process* ponteiro para o proximo processo
 */
Process *processGetNext(Process *p)
{
	return p->next;
}

/**
 * @brief Funcao que aponta o ponteiro para o processo anterior da lista
 *
 * @param p processo
 * @return Process* ponteiro para o processo anterior
 */
Process *processGetPrev(Process *p)
{
	return p->prev;
}

/**
 * @brief Funcao que altera o valor do identificador do processo pai de um processo
 *
 * @param p processo
 * @param ppid identificador do processo pai
 * @return int identificador do processo que foi alterado
 */
int processSetParentPid(Process *p, int ppid)
{
	Process *found = processGetByPid(p, ppid); // retorna o processo
	if (!found)								   // se nao existe
		return -1;							   // retorna negativo
	p->ppid = ppid;							   // altera o PPID
	return p->pid;							   // retorna o PID
}

/**
 * @brief Funcao que altera o status de um processo
 *
 * @param p processo
 * @param status status do processo
 * @return int identificador do processo
 */
int processSetStatus(Process *p, int status)
{
	int idProcess = p->pid; // identificador do processo
	switch (p->status)
	{
	case PROC_INITIALIZING:		  // inicializando
		if (status == PROC_READY) // passa para pronto
			p->status = status;	  // altera o status
		else
			idProcess = -1; // transicao invalida
		break;
	case PROC_READY:				// pronto
		if (status == PROC_RUNNING) // passa para executando
			p->status = status;		// altera o status
		else
			idProcess = -1; // transicao invalida
		break;
	case PROC_WAITING:			  // aguardando
		if (status == PROC_READY) // passa para pronto
			p->status = status;	  // altera o status
		else
			idProcess = -1; // transicao invalida
		break;
	case PROC_RUNNING:										// execurando
		if (status == PROC_READY || status == PROC_WAITING) // passa para pronto ou aguardando
			p->status = status;								// altera o status
		else
			idProcess = -1; // transicao invalida
		break;
	default:
		idProcess = -1; // transicao invalida
	}
	if (idProcess == p->pid)			// verifica se eh valido
		schedNotifyProcStatusChange(p); // notifica
	return idProcess;					// retorna o identificador do processo
}

/**
 * @brief Funcao que adiciona o tempo da CPU
 *
 * @param p processo
 * @param add tempo para ser adicionado
 * @return int tempo atualizado
 */
int processAddCpuUsage(Process *p, int add)
{
	return p->cpu_usage += add;
}

/**
 * @brief Funcao que redireciona ponteiro de parametros de escalonamento para uma estrutura
 *
 * @param p processo
 * @param sp estrutura
 */
void processSetSchedParams(Process *p, void *sp)
{
	p->sched_params = sp;
}

/**
 * @brief Funcao que altera o escalonamento por meio do numero do slot associado
 *
 * @param p processo
 * @param slot numero do slot
 */
void processSetSchedSlot(Process *p, int slot)
{
	p->sched_slot = slot;
}

/**
 * @brief Funcao que retorna um processo atraves do seu identificador
 *
 * @param plist processo
 * @param pid identificador do processo
 * @return Process* processo
 */
Process *processGetByPid(Process *plist, int pid)
{
	Process *current = plist;					   // recebe o processo
	while (current != NULL && current->pid != pid) // verifica
		current = current->next;				   // pega o processo
	return current;								   // retorna
}

/**
 * @brief Funcao que retorna um processo a partir do seu status
 *
 * @param plist processo
 * @param status status
 * @return Process* processo
 */
Process *processGetByStatus(Process *plist, int status)
{
	Process *current = plist;							 // recebe o processo
	while (current != NULL && current->status != status) // verifica
		current = current->next;						 // pega o processo
	return current;										 // retorna
}

/**
 * @brief Funcao que retorna um processo a partir de um algoritmo de escalonamento que eh identificado pelo seu slot
 *
 * @param plist processo
 * @param slot slot
 * @return Process* processo
 */
Process *processGetBySchedSlot(Process *plist, int slot)
{
	Process *current = plist;
	while (current != NULL && current->sched_slot != slot)
		current = current->next;
	return current;
}

/**
 * @brief Funcao que cria um processo no inicio da lista
 *
 * @param plist processo
 * @return Process* processo do inicio
 */
Process *processCreate(Process *plist)
{
	static int pidseed = 0;
	// inicializar os atributos do processo
	Process *newp = malloc(sizeof(Process));
	newp->pid = ++pidseed;
	newp->ppid = 0;
	newp->status = PROC_INITIALIZING;
	newp->cpu_usage = 0;
	newp->sched_params = NULL;
	newp->sched_slot = -1;
	if (plist) // Se lista de processos nao for vazia, encadear no inicio dela
	{
		// ajusta os ponteiros
		newp->next = plist;
		newp->prev = plist->prev;
		plist->prev = newp;
	}
	else // lista vazia, processo eh o unico existente
	{
		newp->next = NULL;
		newp->prev = newp;
	}
	return newp; // retorna o processo
}

/**
 * @brief Funcao que remover um processo atraves do PID
 *
 * @param plist processo
 * @param pid identificador do processo
 * @return Process* processo do inicio
 */
Process *processDestroy(Process *plist, int pid)
{
	Process *found = processGetByPid(plist, pid); // retorna o processo atraves do seu identificador

	if (found) // se foi encontrado
	{
		SchedInfo *sched;
		if (plist == found) // Processo a ser removido eh primeiro da lista
		{
			// ajusta os ponteiros
			plist = found->next;
			if (plist)
				plist->prev = found->prev;
		}
		else // processo nao eh o primeiro da lista
		{
			// ajusta os ponteiros
			if (found->next == NULL)
				plist->prev = found->prev;
			else
				found->next->prev = found->prev;
			found->prev->next = found->next;
		}

		//retorna as informacoes e remove
		sched = schedGetSchedInfo(found->sched_slot);
		if (sched)
			sched->releaseParamsFn(found);
		found->sched_params = NULL;
		found->prev = NULL;
		found->next = NULL;
		free(found);
	}
	return plist;
}

/**
 * @brief Funcao que imprime a lista de processo
 * 
 * @param plist processo
 * @param dumpSchedParamsFn parametros do processo
 */
void printProcess(Process *plist, void (*dumpSchedParamsFn)(Process *p))
{
	Process *current = plist;
	while (current != NULL)
	{
		printf("PID: %d; STATUS: %d; CPU: %d; ",
			   current->pid, current->status, current->cpu_usage);
		dumpSchedParamsFn(current);
		printf("\n");
		current = current->next;
	}
}
