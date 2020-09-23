#include <stdio.h>
#include <rpc/rpc.h>
#include "const.h"

int main(int argc, char *argv[])
{
	char *id;
	char valor[10];
	int stat, op, autentica;
	double saldo;

	if	(argc!=3)  {
		fprintf(stderr,"Uso:\n%s <nome_do_servidor> <nome>\n\n",argv[0]);
		return 1;
	}
	
	
	id = argv[2];
	autentica = callrpc(argv[1], AUTENTICAPROG, AUTENTICAVERS, AUTENTICA,
			       (xdrproc_t)xdr_wrapstring, (char *)&id, 
			       (xdrproc_t)xdr_double, (char *)&saldo );
	
	if	(autentica!= 0)  {
			clnt_perrno(stat);
			printf("Conta não existe\n");
			return 1;
		}
	if	(saldo<0.0){
		printf("Resultado: conta nao encontrada!\n");
	}
	//-----------------------------------------------------------
	else{
	
		printf("CAIXA AUTOMATICO:\n");
		printf("Escolha uma operacao:\n");
		printf("1 - Ver Saldo\n");
		printf("2 - Deposito\n");
		printf("3 - Saque\n");
		printf("4 - Deposito erro\n");
		printf("5 - Saque erro\n");
		printf("----------------------\n");
		scanf("%d",&op);
		printf("----------------------");
		
		id = argv[2];
		switch(op){
		case 1:
			stat = callrpc(argv[1], SALDOPROG, SALDOVERS, OBTEM_SALDO,
				       (xdrproc_t)xdr_wrapstring, (char *)&id, 
				       (xdrproc_t)xdr_double, (char *)&saldo );
			if	(stat!= 0)  {
				clnt_perrno(stat);
				printf("\n");
				return 1;
			}
			printf("\nID: %s\n",argv[2]);
			if	(saldo<0.0) 
				printf("Resultado: nome nao encontrado!\n");
			else
				printf("Saldo: %.1lf\n",saldo);
			break;
		case 2:
			strcat(id, "*");
			printf("----------------------\n");
			printf("Informe o valor:\n");
			scanf("%s",&valor);
			strcat(id, valor);
			
			stat = callrpc(argv[1], DEPOSITOPROG,DEPOSITOVERS,DEPOSITO,
				       (xdrproc_t)xdr_wrapstring, (char *)&id, 
				       (xdrproc_t)xdr_double, (char *)&saldo);
			if	(stat!= 0)  {
				clnt_perrno(stat);
				printf("\n");
				return 1;
			}
			printf("\nID: %s\n",argv[2]);
			if	(saldo<0.0) 
				printf("Conta não existe!\n");
			else
				printf("DEPOSITADO O VALOR: %.1lf\n",saldo);
			break;
		case 3:
			strcat(id, "*");
			printf("----------------------\n");
			printf("Informe o valor:\n");
			scanf("%s",&valor);
			strcat(id, valor);
			
			stat = callrpc(argv[1], SAQUEPROG,SAQUEVERS,SAQUE,
				       (xdrproc_t)xdr_wrapstring, (char *)&id, 
				       (xdrproc_t)xdr_double, (char *)&saldo);
			if	(stat!= 0)  {
				clnt_perrno(stat);
				printf("\n");
				return 1;
			}
			printf("\nID: %s\n",argv[2]);
			if	(saldo<0.0) 
				printf("Não fó possível!\n");
			else
				printf("Retirado O VALOR: %.1lf\n",saldo);
			break;
		case 4:
			strcat(id, "*");
			printf("----------------------\n");
			printf("Informe o valor:\n");
			scanf("%s",&valor);
			strcat(id, valor);
			
			stat = callrpc(argv[1], DEPOSITOERROPROG,DEPOSITOERROVERS,DEPOSITOERRO,
				       (xdrproc_t)xdr_wrapstring, (char *)&id, 
				       (xdrproc_t)xdr_double, (char *)&saldo);
			if	(stat!= 0)  {
				clnt_perrno(stat);
				printf("\n");
				return 1;
			}
			printf("\nID: %s\n",argv[2]);
			if	(saldo==-1.0) 
				printf("Operação ja realizada!\n");
			if	(saldo==-2.0) 
				printf("Falha!\n");
			if	(saldo==-3.0) 
				printf("Conta não existe!\n");
			break;
		case 5:
			strcat(id, "*");
			printf("----------------------\n");
			printf("Informe o valor:\n");
			scanf("%s",&valor);
			strcat(id, valor);
			
			stat = callrpc(argv[1], SAQUEERROPROG,SAQUEERROVERS,SAQUEERRO,
				       (xdrproc_t)xdr_wrapstring, (char *)&id, 
				       (xdrproc_t)xdr_double, (char *)&saldo);
			if	(stat!= 0)  {
				clnt_perrno(stat);
				printf("\n");
				return 1;
			}
			printf("\nID: %s\n",argv[2]);
			if	(saldo==-1.0) 
				printf("Operação ja realizada!\n");
			if	(saldo==-2.0) 
				printf("Falha!\n");
			if	(saldo==-3.0) 
				printf("Conta não existe!\n");

			break;
		default:
			printf("eRRRRou: \n");
			break;
		
		}
	}
	return(0);
	
}

