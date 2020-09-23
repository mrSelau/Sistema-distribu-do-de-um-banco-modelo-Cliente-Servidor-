#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "const.h"

#define TAM_MAX_CPF 10
#define NUM_CONTAS 99999

typedef struct {
	char	cpf[TAM_MAX_CPF+1];
	double	saldo;
} CONTA;

typedef struct {
	char	op[TAM_MAX_CPF+1];
	double	qtd;
} OP;

CONTA contas[NUM_CONTAS] = {};
OP ops[NUM_CONTAS] = {};
	
int nvetor = 0; // zero elementos

double *autentica_1_svc(char **id)
{
	static double erro = -1.0;
	static double acerto = 1.0;
	int i;

	for	(i=0;i<=nvetor;++i){
		if	(strcmp(contas[i].cpf,*id)==0){
			fprintf( stderr, "Conta existente\n");
			return &acerto;
		}
	}
	fprintf( stderr, "Conta NAO existe\n");
	return &erro;
}

double *obtem_saldo_1_svc(char **id)
{
	static double erro = -1.0;
	int i;

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(contas[i].cpf,*id)==0)
			return &(contas[i].saldo);
	fprintf( stderr, "Informando Saldo");
	return &erro;
}

double *add_conta_1_svc(char **id)
{	
	CONTA new;
	strcpy(new.cpf, *id);
	new.saldo = 0;
	
	OP op;
	strcpy(op.op, "CREATE");
	op.qtd = 0;
	
	static double erro = -1.0;
	static double acerto = 1.0;
	
	int i;
	for	(i=0;i<=nvetor;++i){
		if	(strcmp(contas[i].cpf,*id)==0){
			return &erro;
		}
	}
	contas[nvetor++] = new;
	ops[nvetor++] = op; 
	fprintf( stderr, "Conta adicionada\n");
	return &acerto;
}

double *del_conta_1_svc(char **id)
{	

	static double erro = -1.0;
	static double acerto = 1.0;
	
	int i;
	for	(i=0;i<=nvetor;++i){
		if	(strcmp(contas[i].cpf,*id)==0){
			for (i;i<nvetor;++i){
				strcpy(contas[i].cpf, contas[i+1].cpf);
				contas[i].saldo = contas[i+1].saldo;
				strcpy(ops[i].op, ops[i+1].op);
				ops[i].qtd = ops[i+1].qtd;
			}
			nvetor--;
			fprintf( stderr, "Conta deletada\n");
			return &acerto;
		}
	}
	return &erro;
}

double *deposito_1_svc(char **id)
{
	static double erro = -1.0;
	static double acerto = 1.0;
	int i;
	
	char* token = strtok(*id, "*");
	char* next_token = token; // first token
	next_token = strtok(NULL, " ");
	double saldo = atof(next_token);
	
	OP op;
	strcpy(op.op, "DEPOSITO");
	op.qtd = saldo;

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(contas[i].cpf,token)==0){
			contas[i].saldo += saldo;
			fprintf( stderr, "%s Depositou %lf \n", token,saldo);
			return &acerto;
		}
	fprintf( stderr, "%s Não possui conta \n", token);
	return &erro;
}

double *deposito_erro_1_svc(char **id)
{
	static double erro1 = -1.0;
	static double erro2 = -2.0;
	static double erro3 = -3.0;
	static double acerto = 1.0;
	int i;
	
	char* token = strtok(*id, "*");
	char* next_token = token; // first token
	next_token = strtok(NULL, " ");
	double saldo = atof(next_token);
	
	OP op;
	strcpy(op.op, "DEPOSITO");
	op.qtd = saldo;

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(ops[i].op,op.op)==0){
			if(ops[i].qtd == op.qtd){
				fprintf( stderr, "Operacao ja realizada\n");
				strcpy(ops[i].op, "CREATE");
				ops[i].qtd = 0.0;
				return &erro1;
			}
		}

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(contas[i].cpf,token)==0){
			contas[i].saldo += saldo;
			fprintf( stderr, "%s Depositou %lf \n", token,saldo);
			strcpy(ops[i].op, op.op);
			ops[i].qtd = op.qtd;
			return &erro2;
		}
	fprintf( stderr, "%s Não possui conta \n", token);
	return &erro3;
}

double *saque_1_svc(char **id)
{
	static double erro = -1.0;
	static double acerto = 1.0;
	int i;
	
	char* token = strtok(*id, "*");
	char* next_token = token; // first token
	next_token = strtok(NULL, " ");
	double saldo = atof(next_token);
	
	OP op;
	strcpy(op.op, "SAQUE");
	op.qtd = saldo;

	for	(i=0;i<=nvetor;++i){
		if	(strcmp(contas[i].cpf,token)==0){
			if(contas[i].saldo > saldo){
				contas[i].saldo -= saldo;
				fprintf( stderr, "%s sacou %lf \n", token,saldo);
				return &acerto;
			}
			fprintf( stderr, "%s Não possui saldo suficiente \n", token);
			return &erro;
			
		}
	}
	fprintf( stderr, "%s Não possui conta \n", token);
	return &erro;
}

double *saque_erro_1_svc(char **id)
{
	static double erro1 = -1.0;
	static double erro2 = -2.0;
	static double erro3 = -3.0;
	static double acerto = 1.0;
	int i;
	
	char* token = strtok(*id, "*");
	char* next_token = token; // first token
	next_token = strtok(NULL, " ");
	double saldo = atof(next_token);
	
	OP op;
	strcpy(op.op, "SAQUE");
	op.qtd = saldo;

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(ops[i].op,op.op)==0){
			if(ops[i].qtd == op.qtd){
				fprintf( stderr, "Operacao ja realizada\n");
				strcpy(ops[i].op, "CREATE");
				ops[i].qtd = 0.0;
				return &erro1;
			}
		}

	for	(i=0;i<=nvetor;++i)
		if	(strcmp(contas[i].cpf,token)==0){
			contas[i].saldo -= saldo;
			fprintf( stderr, "%s sacou %lf \n", token,saldo);
			strcpy(ops[i].op, op.op);
			ops[i].qtd = op.qtd;
			return &erro2;
		}
	fprintf( stderr, "%s Não possui conta \n", token);
	return &erro3;
}


int main()
{
	if	(registerrpc(AUTENTICAPROG,AUTENTICAVERS,AUTENTICA,autentica_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(SALDOPROG,SALDOVERS,OBTEM_SALDO,obtem_saldo_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(ADDCONTAPROG,ADDCONTAVERS,ADD_CONTA,add_conta_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(DELCONTAPROG,DELCONTAVERS,DEL_CONTA,del_conta_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(DEPOSITOPROG,DEPOSITOVERS,DEPOSITO,deposito_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(DEPOSITOERROPROG,DEPOSITOERROVERS,DEPOSITOERRO,deposito_erro_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(SAQUEPROG,SAQUEVERS,SAQUE,saque_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	if	(registerrpc(SAQUEERROPROG,SAQUEERROVERS,SAQUEERRO,saque_erro_1_svc,
			     (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double ) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}
	svc_run();
	fprintf(stderr,"Erro em svc_run()!\n");
	return 1;
}

