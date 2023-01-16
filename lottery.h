#ifndef LOTTERY_H
#define LOTTERY_H

#include "scheduler.h"

typedef struct lottery_params {
        int num_tickets; //numero de tickets
        int num_minor_ticket; //numero de tickets antes da soma
        int num_greater_ticket; //numero de tickets apos a soma
} LotterySchedParams;

/**
 * @brief Funcao que realiza a inicializacao do escalonador
 * 
 */
void lottInitSchedInfo(void);



/**
 * @brief Funcao que inicializa os parametros de escalonamento de um processo
 *
 * @param p processo
 * @param params parametro
 */
void lottInitSchedParams(Process *p, void *params);

/**
 * @brief Funcao que recebe a notificacao que um processo mudou de estado
 *
 * @param p processo
 */
void lottNotifyProcStatusChange(Process* p);

/**
 * @brief Funcao que realiza o escalonamento por loteria
 *
 * @param plist processo
 * @return Process* processo sorteado
 */
Process* lottSchedule(Process *plist);

/**
 * @brief Funcao que libera os parametros de escalonamento de um processo
 * 
 * @param p processo
 * @return int numero do slot do processo que ele estava associado
 */
int lottReleaseParams(Process *p);

/**
 * @brief Funcao que realiza a transferencia de tickets entre dois processos
 * 
 * @param src processo origem
 * @param dst processo destino
 * @param tickets numero de tickets
 * @return int numero de tickets que foi transferido
 */
int lottTransferTickets(Process *src, Process *dst, int tickets);

#endif
