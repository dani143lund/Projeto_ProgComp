/**
 * @file project.c
 *
 * Projeto semestral da disciplina Programacao de computadores
 * 
 * Sistema de RH para cadastro e manutencao de funcionarios
 **/



/*************************************************************
 * Includes
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*************************************************************
 * Structs definition
 ************************************************************/
typedef struct Address {
	char strName [50];
	int addrNr;
	char addrRegion [50];
	char addrCity [50];
	char addrState [3];
}tAddr;

typedef struct Employee {
    char name[50];
    int employeeID;
    float salary;
    char role[50];
    tAddr address;
}tEmp;

/*************************************************************
 * Prototypes
 ************************************************************/
int getEmployeesQty(FILE *file);
void getEmployeesData(FILE *file, tEmp *employees, int nrEmp);
int addEmployee(tEmp **employees, int nrEmp);
void editPersonalData(tEmp *employee);
void addPersonalData(tEmp *employee);
void addAddressData(tEmp *employee);
void logEmployeeData(tEmp *employee, FILE *file);
void displayEmployees(FILE *file);
void editEmployee(tEmp *employee);
void writeData(tEmp *employees, FILE *file, int nrEmp);
void showMenu();

/*************************************************************
 * Function definitions
 ************************************************************/
int getEmployeesQty(FILE *file)
{
    int nrEmp = 0, d;

    while((d = fgetc(file)) != EOF)
    { // Counts how many lines the .txt has (Number of lines = Number of employees)
        if (d == '\n') // Increment count if this character is newline
        {
            nrEmp = nrEmp + 1;
        }
    }
    printf("Numero de func: %d\n",nrEmp);
    return nrEmp;
}

void getEmployeesData(FILE *file, tEmp *employees, int nrEmp)
{
    int employeeDataField = 0;
    int strIdx = 0;
    int empIdx = 0;
    int c;
    char strBuffer[50];

    for(int idx = 0; idx < 50; idx++)
    {
        strBuffer[idx] = 0; // Cleans string buffer for avoiding memory garbage
    }

    tEmp *currentEmployee = employees;
    rewind(file);

    while((c = fgetc(file)) != EOF)
    {
        if((c != ',') && (c != '\n') && (c != '\r'))
        {
            strBuffer[strIdx] = c; // buffer string receives current txt char
            strIdx++;
        }
        else if(c != '\r')
        {
            switch(employeeDataField) // Copies the buffer string into employees struct avoiding string copy overflow
            {
                case 0:
                    strncpy(currentEmployee->name,strBuffer,sizeof(strBuffer));
                    printf("Emp %d name||%s||\n",empIdx,currentEmployee->name);
                    employeeDataField++;
                    break;

                case 1:
                    currentEmployee->employeeID = (int)atoi(strBuffer);
                    printf("Emp %d employeeID||%d||\n",empIdx,currentEmployee->employeeID);
                    employeeDataField++;
                    break;

                case 2:
                    currentEmployee->salary = (float)atof(strBuffer);
                    printf("Emp %d salary||%.2f||\n",empIdx,currentEmployee->salary);
                    employeeDataField++;
                    break;

                case 3:
                    strncpy(currentEmployee->role,strBuffer,sizeof(strBuffer));
                    printf("Emp %d role||%s||\n",empIdx,currentEmployee->role);
                    employeeDataField++;
                    break;

                case 4:
                    strncpy(currentEmployee->address.strName,strBuffer,sizeof(strBuffer));
                    printf("Emp %d address.strName||%s||\n",empIdx,currentEmployee->address.strName);
                    employeeDataField++;
                    break;

                case 5:
                    currentEmployee->address.addrNr = (int)atoi(strBuffer);
                    printf("Emp %d address.addrNr||%d||\n",empIdx,currentEmployee->address.addrNr);
                    employeeDataField++;
                    break;

                case 6:
                    strncpy(currentEmployee->address.addrRegion,strBuffer,sizeof(strBuffer));
                    printf("Emp %d address.addrRegion||%s||\n",empIdx,currentEmployee->address.addrRegion);
                    employeeDataField++;
                    break;

                case 7:
                    strncpy(currentEmployee->address.addrCity,strBuffer,sizeof(strBuffer));
                    printf("Emp %d address.addrCity||%s||\n",empIdx,currentEmployee->address.addrCity);
                    employeeDataField++;
                    break;

                case 8:
                    strncpy(currentEmployee->address.addrState,strBuffer,sizeof(strBuffer));
                    printf("Emp %d address.addrState||%s||\n",empIdx,currentEmployee->address.addrState);
                    employeeDataField = 0;
                    empIdx++;
                    currentEmployee++; // avoid memory overflow
                    break;
            }
            for(int idx = 0; idx < 50; idx++)
            {
                strBuffer[idx] = 0; // Cleans string buffer for next iteration -> Avoid memory garbage
            }
            strIdx = 0;
        }
    }
}

int addEmployee(tEmp **employees, int nrEmp)
{
    int quantity;
    int currentSize = nrEmp;

    printf("Digite a quantidade de funcionarios a cadastrar: ");
    scanf("%d", &quantity);

    nrEmp += quantity;
    
    if(nrEmp > 0)
    {
        *employees = (tEmp *)realloc(*employees, (nrEmp + 1) * sizeof(tEmp));
    }
    else
    {
        *employees = (tEmp *)malloc(quantity * sizeof(tEmp));
    }

    if (*employees == NULL) 
    {
        // Realloc fault
        fprintf(stderr, "Erro na realocacao de memoria.\n");
        return currentSize;
    } 
    else 
    {
        for (int i = currentSize; i < currentSize + quantity; i++) 
        {
            printf("%d < %d\n", currentSize, currentSize + quantity);
            addPersonalData(&(*employees)[i]);
            addAddressData(&(*employees)[i]);
        }

        printf("%d funcionario(s) adicionado(s) com sucesso!\n", quantity);
        return nrEmp;
    }
}

void editPersonalData(tEmp *employee)
{
    printf("\n---------- Dados Pessoais ----------\n");

    printf("Salario do funcionario: ");
    scanf("%f", &employee->salary);

    printf("Cargo do funcionario: ");
    scanf(" %49[^\n]", employee->role);    
}

void addPersonalData(tEmp *employee)
{
    printf("\n---------- Dados Pessoais ----------\n");

    printf("Nome do funcionario: ");
    scanf(" %49[^\n]", employee->name);

    printf("ID do funcionario: ");
    scanf("%d", &employee->employeeID);

    printf("Salario do funcionario: ");
    scanf("%f", &employee->salary);

    printf("Cargo do funcionario: ");
    scanf(" %49[^\n]", employee->role);    
}

void addAddressData(tEmp *employee)
{
    printf("\n---------- Endereco ----------\n");

    printf("Rua: ");
    scanf(" %49[^\n]", employee->address.strName);

    printf("Numero: ");
    scanf("%d", &employee->address.addrNr);

    printf("Bairro: ");
    scanf(" %49[^\n]", employee->address.addrRegion);

    printf("Cidade: ");
    scanf(" %49[^\n]", employee->address.addrCity);

    printf("Estado: ");
    scanf(" %2[^\n]", employee->address.addrState);
}

void logEmployeeData(tEmp *employee, FILE *file)
{
    printf("%s,%d,%.2f,%s,%s,%d,%s,%s,%s\n", employee->name, employee->employeeID, employee->salary,
                                                        employee->role, employee->address.strName, employee->address.addrNr,
                                                        employee->address.addrRegion, employee->address.addrCity, employee->address.addrState);
    fprintf(file, "%s,%d,%.2f,%s,%s,%d,%s,%s,%s\n", employee->name, employee->employeeID, employee->salary,
                                                        employee->role, employee->address.strName, employee->address.addrNr,
                                                        employee->address.addrRegion, employee->address.addrCity, employee->address.addrState);
}

void displayEmployees(FILE *file) {
/*     struct Employee currentEmployee;

    printf("\nLista de Funcionarios:\n");

    rewind(file); // Coloca o cursor no início do arquivo

    while (fscanf(file, " %49s %d %f", currentEmployee.name, &currentEmployee.employeeID, &currentEmployee.salary) == 3) {
        printf("Nome: %s\nID: %d\nSalario: %.2f\n\n", currentEmployee.name, currentEmployee.employeeID, currentEmployee.salary);
    } */
}

void findEmployee(tEmp *employee, int nrEmployees) // Finds an employee through its ID
{
    // -> Adaptar funcao para ler diretamente das structs apos o parsing inicial
    int empId;
    bool isEmployeeFound = false;

    printf("Insira o ID do funcionario a ser editado: ");
    scanf("%d",&empId);

    tEmp *currentEmployee = employee;

    for(int cntr = 0; cntr < nrEmployees; cntr++) // Sweep through the employees data
    {
        if(currentEmployee->employeeID == empId) // Employee found
        {
            editEmployee(currentEmployee);
            isEmployeeFound = true;
        }
        currentEmployee++;
    }

    if(!isEmployeeFound)
    {
        printf("O ID indicado nao existe!\n");
    }
}

void editEmployee(tEmp *employee)
{
    char userChoice;

    printf("Usuario selecionado: %s, %s\n",employee->name, employee->role);

    do
    {
        printf("Deseja editar o cargo ou salario? (s/n): ");
        scanf(" %c",&userChoice);

        if((userChoice == 's') || (userChoice == 'S'))
        {
            editPersonalData(employee);
        }
        else if((userChoice != 'n') && (userChoice != 'N'))
        {
            printf("Opcao Invalida! Tente novamente.\n\n");
        }

    } while (userChoice != 's' && userChoice != 'S' &&
             userChoice != 'n' && userChoice != 'N');

    do
    {
        printf("Deseja editar o endereco? (s/n): ");
        scanf(" %c",&userChoice);

        if((userChoice == 's') || (userChoice == 'S'))
        {
            addAddressData(employee);
        }
        else if((userChoice != 'n') && (userChoice != 'N'))
        {
            printf("Opcao Invalida! Tente novamente.\n\n");
        }

    } while (userChoice != 's' && userChoice != 'S' &&
             userChoice != 'n' && userChoice != 'N');
}

int removeEmployee(tEmp *employees, int nrEmployees) 
{
    int empId;
    char userChoice;
    bool isEmployeeFound = false, proceedRemoval = false;

    printf("Insira o ID do funcionario a ser removido: ");
    scanf("%d", &empId);

    int foundIndex = -1;

    for (int cntr = 0; cntr < nrEmployees; cntr++) 
    {
        if (employees[cntr].employeeID == empId) 
        {
            printf("Usuario selecionado: %s, %s\n",employees[cntr].name, employees[cntr].role);
            isEmployeeFound = true;
            foundIndex = cntr;
            break; // Não é necessário continuar procurando se já encontrou
        }
    }

    if (isEmployeeFound) 
    {
        do
        {
            printf("Deseja mesmo prosseguir com a remocao? (s/n): ");
            scanf(" %c",&userChoice);

            if((userChoice == 's') || (userChoice == 'S'))
            {
                proceedRemoval = true;
            }
            else if((userChoice != 'n') && (userChoice != 'N'))
            {
                printf("Opcao Invalida! Tente novamente.\n\n");
            }

        } while (userChoice != 's' && userChoice != 'S' &&
                userChoice != 'n' && userChoice != 'N');
        
        if(proceedRemoval)
        {
            // Mover os funcionários para trás a partir da posição encontrada
            for (int i = foundIndex; i < nrEmployees - 1; i++) {
                employees[i] = employees[i + 1];
            }

            // Limpar a última posição
            memset(&employees[nrEmployees - 1], 0, sizeof(tEmp));

            // Reduzir o número total de funcionários
            nrEmployees--;

            printf("Funcionario removido com sucesso!\n");
        }
    } 
    else 
    {
        printf("O ID indicado nao existe!\n");
    }

    return nrEmployees;
}

void writeData(tEmp *employees, FILE *file, int nrEmp)
{
    for(int i = 0; i < nrEmp; i++) {
        printf("i = %d",i);
        logEmployeeData(employees, file);
        employees++;
    }
}

void showMenu(int nrEmp) {
    printf("\n\n");
    printf(" Nr de funcionarios atual: %d\n",nrEmp);
    printf(" ____________________________\n");
    printf("|           Menu             |\n");
    printf("|____________________________|\n");
    printf("| 1. Adicionar Funcionario   |\n");
    printf("| 2. Listar cadastros        |\n");
    printf("| 3. Editar Cadastro         |\n");
    printf("| 4. Remover Cadastro        |\n");
    printf("| 5. Reajuste coletivo       |\n");
    printf("| 6. Sair                    |\n");
    printf("|____________________________|\n");
    printf("Escolha uma opcao: ");
}

/*************************************************************
 * Main code
 ************************************************************/
int main() {
    FILE *file;
    tEmp *employeesDB = NULL;
    int choice, nrOfEmployees = 0;

    file = fopen("dados_rh.txt", "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    else
    {
        nrOfEmployees = getEmployeesQty(file);

        if(nrOfEmployees > 0)
        {
            do
            {
                employeesDB = (tEmp*)malloc((nrOfEmployees + 1) * sizeof(tEmp));
                if (employeesDB == NULL) 
                {
                    // Malloc fault
                    fprintf(stderr, "Erro na realocacao de memoria.\n");
                }
            } while (employeesDB == NULL);
            
            getEmployeesData(file,employeesDB,nrOfEmployees);
        }
        fclose(file);
    }

    do {
        showMenu(nrOfEmployees);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                nrOfEmployees = addEmployee(&employeesDB,nrOfEmployees);
                break;
            case 2:
                displayEmployees(file);
                break;
            case 3:
                findEmployee(employeesDB, nrOfEmployees);
                break;
            case 4:
                nrOfEmployees = removeEmployee(employeesDB, nrOfEmployees);
                break;
            case 6:
                if(employeesDB != NULL)
                {
                    file = fopen("dados_rh.txt", "w");
                    writeData(employeesDB, file, nrOfEmployees);
                    fclose(file);
                }
                printf("Programa encerrado.\n");
                free(employeesDB);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (choice != 6);

    return 0;
}