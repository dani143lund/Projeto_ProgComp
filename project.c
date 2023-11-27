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
#include <windows.h>
#include <time.h>
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
void getEmployeesData(FILE *file, tEmp *employees);
int findHigherID(tEmp *employees, int nrEmp);
int addEmployee(tEmp **employees, int nrEmp);
void editPersonalData(tEmp *employee);
void addPersonalData(tEmp *employee);
void addAddressData(tEmp *employee);
void logEmployeeData(tEmp *employee, FILE *file);
void displayEmployees(tEmp *employees, int nrEmp);
void findEmployee(tEmp *employee, int nrEmployees);
void editEmployee(tEmp *employee);
int removeEmployee(tEmp *employees, int nrEmployees);
void showEmployee(tEmp *employee, int nrEmployees);
void salaryAdj(tEmp* employee, int nrEmployees);
void writeData(tEmp *employees, FILE *file, int nrEmp);
void showMenu(int nrEmp);

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
    system("color F9");
    return nrEmp;
}

void getEmployeesData(FILE *file, tEmp *employees)
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
                    //printf("Emp %d name||%s||\n",empIdx,currentEmployee->name);
                    employeeDataField++;
                    break;

                case 1:
                    currentEmployee->employeeID = (int)atoi(strBuffer);
                    //printf("Emp %d employeeID||%d||\n",empIdx,currentEmployee->employeeID);
                    employeeDataField++;
                    break;

                case 2:
                    currentEmployee->salary = (float)atof(strBuffer);
                    //printf("Emp %d salary||%.2f||\n",empIdx,currentEmployee->salary);
                    employeeDataField++;
                    break;

                case 3:
                    strncpy(currentEmployee->role,strBuffer,sizeof(strBuffer));
                    //printf("Emp %d role||%s||\n",empIdx,currentEmployee->role);
                    employeeDataField++;
                    break;

                case 4:
                    strncpy(currentEmployee->address.strName,strBuffer,sizeof(strBuffer));
                    //printf("Emp %d address.strName||%s||\n",empIdx,currentEmployee->address.strName);
                    employeeDataField++;
                    break;

                case 5:
                    currentEmployee->address.addrNr = (int)atoi(strBuffer);
                    //printf("Emp %d address.addrNr||%d||\n",empIdx,currentEmployee->address.addrNr);
                    employeeDataField++;
                    break;

                case 6:
                    strncpy(currentEmployee->address.addrRegion,strBuffer,sizeof(strBuffer));
                    //printf("Emp %d address.addrRegion||%s||\n",empIdx,currentEmployee->address.addrRegion);
                    employeeDataField++;
                    break;

                case 7:
                    strncpy(currentEmployee->address.addrCity,strBuffer,sizeof(strBuffer));
                    //printf("Emp %d address.addrCity||%s||\n",empIdx,currentEmployee->address.addrCity);
                    employeeDataField++;
                    break;

                case 8:
                    strncpy(currentEmployee->address.addrState,strBuffer,sizeof(strBuffer));
                    //printf("Emp %d address.addrState||%s||\n",empIdx,currentEmployee->address.addrState);
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

int findHigherID(tEmp *employees, int nrEmp)
{
    int higherID = 0;

    for(int i = 0; i < nrEmp; i++)
    {
        if(employees[i].employeeID > higherID)
        {
            higherID = employees[i].employeeID;
        }
    }

    return higherID;
}

int addEmployee(tEmp **employees, int nrEmp)
{
    int quantity, empID;
    int currentSize = nrEmp;

    printf("******************************************************");
    printf("\n**                                                  **");
    printf("\n**             ADICIONAR FUNCIONARIOS               **");
    printf("\n**                                                  **");
    printf("\n******************************************************");
    printf("\nDigite a quantidade de funcionarios a cadastrar: ");
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
        system("cls");
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**             ADICIONAR FUNCIONARIOS               **");
        printf("\n**                                                  **");

        empID = findHigherID(*employees, nrEmp) + 1;

        for (int i = currentSize; i < currentSize + quantity; i++) 
        {
            //printf("%d < %d\n", currentSize, currentSize + quantity);
            (*employees)[i].employeeID = empID;
            addPersonalData(&(*employees)[i]);
            addAddressData(&(*employees)[i]);

            empID++;
        }

        printf("\n%d funcionario(s) adicionado(s) com sucesso!\n\n", quantity);
        system("pause");
        while(getchar() != '\n');
        return nrEmp;
    }
}

void editPersonalData(tEmp *employee)
{
    int dataCheck;

    printf("******************************************************");
    printf("\n**                                                  **");
    printf("\n**               EDITAR FUNCIONARIO                 **");
    printf("\n**                                                  **");
    printf("\n******************************************************\n");
    printf("**                 Dados Pessoais                   **");
    printf("\n******************************************************\n\n");

    do
    {
        printf("Salario do funcionario: ");
        dataCheck = scanf("%f", &employee->salary);
        if(dataCheck != 1)
        {
            printf("O formato dos dados inseridos esta incorreto!\n\n");
            while ( getchar() != '\n' );
        }
    } while (dataCheck != 1);
    
    printf("Cargo do funcionario: ");
    scanf(" %49[^\n]", employee->role);    
}

void addPersonalData(tEmp *employee)
{
    int dataCheck = 0;

    printf("\n******************************************************\n");
    printf("**                 Dados Pessoais                   **");
    printf("\n******************************************************\n");
    printf("\nNome do funcionario: ");
    scanf(" %49[^\n]", employee->name);
    
    do
    {
        printf("Salario do funcionario: ");
        dataCheck = scanf("%f", &employee->salary);
        if(dataCheck != 1)
        {
            printf("O formato dos dados inseridos esta incorreto!\n\n");
            while ( getchar() != '\n' );
        }
    } while (dataCheck != 1);

    printf("Cargo do funcionario: ");
    scanf(" %49[^\n]", employee->role);    
}

void addAddressData(tEmp *employee)
{
    int dataCheck = 0;

    printf("\n******************************************************\n");
    printf("**                    Endereco                      **");
    printf("\n******************************************************\n");
    printf("\nRua: ");
    scanf(" %49[^\n]", employee->address.strName);

    do
    {
        printf("Numero: ");
        dataCheck = scanf("%d", &employee->address.addrNr);
        if(dataCheck != 1)
        {
            printf("O formato dos dados inseridos esta incorreto!\n\n");
            while ( getchar() != '\n' );
        }
    } while (dataCheck != 1);

    printf("Bairro: ");
    scanf(" %49[^\n]", employee->address.addrRegion);

    printf("Cidade: ");
    scanf(" %49[^\n]", employee->address.addrCity);

    printf("Estado: ");
    scanf(" %2[^\n]", employee->address.addrState);
    while ( getchar() != '\n' );
}

void logEmployeeData(tEmp *employee, FILE *file)
{
    //printf("%s,%d,%.2f,%s,%s,%d,%s,%s,%s\n", employee->name, employee->employeeID, employee->salary,
                                                        //employee->role, employee->address.strName, employee->address.addrNr,
                                                        //employee->address.addrRegion, employee->address.addrCity, employee->address.addrState);
    fprintf(file, "%s,%d,%.2f,%s,%s,%d,%s,%s,%s\n", employee->name, employee->employeeID, employee->salary,
                                                        employee->role, employee->address.strName, employee->address.addrNr,
                                                        employee->address.addrRegion, employee->address.addrCity, employee->address.addrState);
}

void displayEmployees(tEmp *employees, int nrEmp) 
{
    int userChoice = -1;
    int i, j;

    tEmp bufEmployee;

    if(nrEmp < 1)
    {
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**               LISTAR FUNCIONARIOS                **");
        printf("\n**                                                  **");
        printf("\n******************************************************\n");
        printf("\nNao ha funcionarios a serem listados!\n");
    }
    else
    {
        do
        {
            printf("******************************************************");
            printf("\n**                                                  **");
            printf("\n**               LISTAR FUNCIONARIOS                **");
            printf("\n**                                                  **");
            printf("\n******************************************************");
            printf("\n**  Selecione qual o modo de listagem desejado:     **");
            printf("\n******************************************************\n");
            printf("\n1 - Listar por ID \n");
            printf("2 - Listar por salario \n");
            printf("3 - Listar por ordem alfabetica \n\n");
            printf("Modo: ");
            scanf("%d",&userChoice);

            if((userChoice != 1) && (userChoice != 2) && (userChoice != 3))
            {
                printf("Opcao invalida! Tente novamente! \n\n");
                system("pause");
                system("cls");
            }
        } while ((userChoice != 1) && (userChoice != 2) && (userChoice != 3));

        switch(userChoice)
        {
            case 1:
                for(i = nrEmp - 1; i > 0; i--)
                {
                    for(j = 0; j < i; j++)
                    {
                        if(employees[j].employeeID > employees[j+1].employeeID)
                        {
                            bufEmployee = employees[j];
                            employees[j] = employees[j+1];
                            employees[j+1] = bufEmployee;

                        }
                    }
                }
                break;

            case 2:
                for(i = nrEmp - 1; i > 0; i--)
                {
                    for(j = 0; j < i; j++)
                    {
                        if(employees[j].salary > employees[j+1].salary)
                        {
                            bufEmployee = employees[j];
                            employees[j] = employees[j+1];
                            employees[j+1] = bufEmployee;

                        }
                    }
                }
                break;

            case 3:
                for(i = nrEmp - 1; i > 0; i--)
                {
                    for(j = 0; j < i; j++)
                    {
                        if(employees[j].name[0] > employees[j+1].name[0])
                        {
                            bufEmployee = employees[j];
                            employees[j] = employees[j+1];
                            employees[j+1] = bufEmployee;

                        }
                    }
                }
                break;
        }

        system("cls");
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**               LISTAR FUNCIONARIOS                **");
        printf("\n**                                                  **");
        printf("\n******************************************************");
        printf("\n**                   Listagem                       **");
        printf("\n******************************************************\n\n");

        for(int empCntr = 0; empCntr < nrEmp; empCntr++)
        {
            printf("%d - %s,%d,%.2f,%s,%s,%d,%s,%s,%s\n", empCntr + 1, employees[empCntr].name, employees[empCntr].employeeID, employees[empCntr].salary,
                                                        employees[empCntr].role, employees[empCntr].address.strName, employees[empCntr].address.addrNr,
                                                        employees[empCntr].address.addrRegion, employees[empCntr].address.addrCity, employees[empCntr].address.addrState);
        }
    }

    printf("\n");
    system("pause");
}

void findEmployee(tEmp *employee, int nrEmployees) // Finds an employee through its ID
{
    // -> Adaptar funcao para ler diretamente das structs apos o parsing inicial
    int empId;
    bool isEmployeeFound = false;

    printf("******************************************************");
    printf("\n**                                                  **");
    printf("\n**               EDITAR FUNCIONARIO                 **");
    printf("\n**                                                  **");
    printf("\n******************************************************");
    printf("\n\nInsira o ID do funcionario a ser editado: ");
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
        printf("O ID indicado nao existe!\n\n");
        while ( getchar() != '\n' );
    }
    else
    {
        system("cls");
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**               EDITAR FUNCIONARIO                 **");
        printf("\n**                                                  **");
        printf("\n******************************************************\n");
        printf("**            Rotina de edicao executada!           **");
        printf("\n******************************************************\n\n");
        while ( getchar() != '\n' );
    }
    system("pause");
}

void editEmployee(tEmp *employee)
{
    char userChoice;
    system("cls");

    do
    {
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**               EDITAR FUNCIONARIO                 **");
        printf("\n**                                                  **");
        printf("\n******************************************************\n");
        printf(" Usuario selecionado: %s, %s",employee->name, employee->role);
        printf("\n******************************************************\n\n");
        printf("Deseja editar o cargo ou salario? (s/n): ");
        scanf(" %c",&userChoice);
        
        if((userChoice == 's') || (userChoice == 'S'))
        {
            system("cls");
            editPersonalData(employee);
        }
        else if((userChoice != 'n') && (userChoice != 'N'))
        {
            printf("\nOpcao Invalida! Tente novamente.\n\n");
            system("pause");
            system("cls");
            while(getchar() != '\n');
        }

    } while (userChoice != 's' && userChoice != 'S' &&
             userChoice != 'n' && userChoice != 'N');

    do
    {
        system("cls");
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**               EDITAR FUNCIONARIO                 **");
        printf("\n**                                                  **");
        printf("\n******************************************************\n");
        printf(" Usuario selecionado: %s, %s",employee->name, employee->role);
        printf("\n******************************************************\n");
        printf("\nDeseja editar o endereco? (s/n): ");
        scanf(" %c",&userChoice);

        if((userChoice == 's') || (userChoice == 'S'))
        {
            system("cls");
            printf("******************************************************");
            printf("\n**                                                  **");
            printf("\n**               EDITAR FUNCIONARIO                 **");
            printf("\n**                                                  **");

            addAddressData(employee);
        }
        else if((userChoice != 'n') && (userChoice != 'N'))
        {
            printf("Opcao Invalida! Tente novamente.\n\n");
            system("pause");
            while(getchar() != '\n');
        }
    } while (userChoice != 's' && userChoice != 'S' &&
             userChoice != 'n' && userChoice != 'N');
}

int removeEmployee(tEmp *employees, int nrEmployees) 
{
    int empId;
    char userChoice;
    bool isEmployeeFound = false, proceedRemoval = false;

    printf("******************************************************");
    printf("\n**                                                  **");
    printf("\n**               REMOVER FUNCIONARIO                **");
    printf("\n**                                                  **");
    printf("\n******************************************************");
    printf("\n\nInsira o ID do funcionario a ser removido: ");
    scanf("%d", &empId);

    int foundIndex = -1;

    for (int cntr = 0; cntr < nrEmployees; cntr++) 
    {
        if (employees[cntr].employeeID == empId) 
        {
            isEmployeeFound = true;
            foundIndex = cntr;
            break; // Não é necessário continuar procurando se já encontrou
        }
    }

    if (isEmployeeFound) 
    {
        do
        {
            system("cls");
            printf("******************************************************");
            printf("\n**                                                  **");
            printf("\n**               REMOVER FUNCIONARIO                **");
            printf("\n**                                                  **");
            printf("\n******************************************************\n");
            printf("    Usuario selecionado: %s, %s\n",employees[foundIndex].name, employees[foundIndex].role);
            printf("******************************************************\n\n");
            printf("Deseja mesmo prosseguir com a remocao? (s/n): ");
            scanf(" %c",&userChoice);

            if((userChoice == 's') || (userChoice == 'S'))
            {
                proceedRemoval = true;
            }
            else if((userChoice != 'n') && (userChoice != 'N'))
            {
                printf("Opcao Invalida! Tente novamente.\n\n");
                system("pause");
                system("cls");
                while(getchar() != '\n');
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

            printf("\nFuncionario removido com sucesso!\n");
        }
    } 
    else 
    {
        printf("O ID indicado nao existe!\n");
        while ( getchar() != '\n' );
    }

    printf("\n");
    system("pause");
    return nrEmployees;
}

void showEmployee(tEmp *employee, int nrEmployees) // Shows employee details through its ID
{
    int empId;
    bool isEmployeeFound = false;

    printf("******************************************************");
    printf("\n**                                                  **");
    printf("\n**              CONSULTAR FUNCIONARIO               **");
    printf("\n**                                                  **");
    printf("\n******************************************************\n");

    if(nrEmployees < 1)
    {
        printf("\nNao ha funcionarios a serem consultados!\n\n");
    }
    else
    {
        printf("\nInsira o ID do funcionario a ser consultado: ");
        scanf("%d",&empId);

        tEmp *currentEmployee = employee;

        for(int cntr = 0; cntr < nrEmployees; cntr++) // Sweep through the employees data
        {
            if(currentEmployee->employeeID == empId) // Employee found
            {
                isEmployeeFound = true;
                system("cls");
                printf("******************************************************");
                printf("\n**                                                  **");
                printf("\n**              CONSULTAR FUNCIONARIO               **");
                printf("\n**                                                  **");
                printf("\n******************************************************\n");
                printf("**             Dados do funcionario ID %d           **",empId);
                printf("\n******************************************************\n");
                printf("\nNome: %s",currentEmployee->name);
                printf("\nSalario: %.2f",currentEmployee->salary);
                printf("\nCargo: %s",currentEmployee->role);
                printf("\nEndereco: %s, %d, %s, %s, %s.\n\n",currentEmployee->address.strName, currentEmployee->address.addrNr,
                                                        currentEmployee->address.addrRegion, currentEmployee->address.addrCity, currentEmployee->address.addrState);
                break;
            }
            currentEmployee++;
        }

        if(!isEmployeeFound)
        {
            printf("O ID indicado nao existe!\n\n");
            while ( getchar() != '\n' );
        }
    }

    system("pause");
}

void salaryAdj(tEmp* employee, int nrEmployees)
{
    float percent;
    int dataCheck;

    if(nrEmployees < 1)
    {
        printf("******************************************************");
        printf("\n**                                                  **");
        printf("\n**                REAJUSTAR SALARIOS                **");
        printf("\n**                                                  **");
        printf("\n******************************************************\n");
        printf("\nNao ha funcionarios a serem reajustados!\n\n");
    }
    else
    {
        do
        {
            do
            {   
                printf("******************************************************");
                printf("\n**                                                  **");
                printf("\n**                REAJUSTAR SALARIOS                **");
                printf("\n**                                                  **");
                printf("\n******************************************************\n");
                printf("\nDigite o percentual de reajuste salarial (Max 20 %c): ",'%');
                dataCheck = scanf("%f",&percent);

                if(dataCheck != 1)
                {
                    printf("\nO formato dos dados inseridos esta incorreto!\n\n");
                    system("pause");
                    system("cls");
                    while ( getchar() != '\n' );
                }

            } while (dataCheck != 1);
            
            if(percent < 0)
            {
                printf("\nNao e possivel atribuir um reajuste negativo!\n\n");
                system("pause");
                system("cls");
            }
            else if(percent > 20)
            {
                printf("\nO reajuste inserido e maior do que o permitido!\n\n");
                system("pause");
                system("cls");
            }
        } while ((percent < 0) || (percent > 20));

        for(int cntr = 0; cntr < nrEmployees; cntr++)
        {
            employee[cntr].salary = employee[cntr].salary * (1.0 + (percent / 100.0));
        }

        printf("\nSalarios reajustados com sucesso!\n\n");
    }
    system("pause");
}

void writeData(tEmp *employees, FILE *file, int nrEmp)
{
    for(int i = 0; i < nrEmp; i++) {
        logEmployeeData(employees, file);
        employees++;
    }
}

void showMenu(int nrEmp) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    system("cls");
    printf("\n\n");
    printf("******************************************************\n");
    printf("**     DATA: %02d/%02d/%d             HORA: %02d:%02d     **", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    printf("\n**                                                  **");
    printf("\n**     SISTEMA DE GERENCIAMENTO DE FUNCIONARIOS     **");
    printf("\n**                                                  **");
    printf("\n******************************************************\n");
    printf("**         Numero de funcionarios atual: %02d         **",nrEmp);
    printf("\n******************************************************\n");
    printf(" ____________________________________________________\n");
    printf("|                                                    |\n");
    printf("|                 Opcoes do sistema                  |\n");
    printf("|____________________________________________________|\n");
    printf("| 1. Adicionar Funcionario                           |\n");
    printf("| 2. Listar cadastros                                |\n");
    printf("| 3. Editar Cadastro                                 |\n");
    printf("| 4. Remover Cadastro                                |\n");
    printf("| 5. Consultar funcionario                           |\n");
    printf("| 6. Reajuste coletivo                               |\n");
    printf("| 7. Sair                                            |\n");
    printf("|____________________________________________________|\n");                     
    printf(" Escolha uma opcao: ");
}

/*************************************************************
 * Main code
 ************************************************************/
int main() {
    FILE *file;
    tEmp *employeesDB = NULL;
    int choice, nrOfEmployees = 0;

    file = fopen("dados_rh.txt", "a+");

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
            
            getEmployeesData(file,employeesDB);
        }
        fclose(file);
    }

    do {
        showMenu(nrOfEmployees);
        scanf("%d", &choice);
        while(getchar() != '\n');
        switch (choice) {

            case 1:
                system("cls");
                nrOfEmployees = addEmployee(&employeesDB,nrOfEmployees);
                break;

            case 2:
                system("cls");
                displayEmployees(employeesDB,nrOfEmployees);
                break;

            case 3:
                system("cls");
                findEmployee(employeesDB, nrOfEmployees);
                break;

            case 4:
                system("cls");
                nrOfEmployees = removeEmployee(employeesDB, nrOfEmployees);
                break;

            case 5:
                system("cls");
                showEmployee(employeesDB, nrOfEmployees);
                break;

            case 6:
                system("cls");
                salaryAdj(employeesDB, nrOfEmployees);
                break;

            case 7:
                if(employeesDB != NULL)
                {
                    file = fopen("dados_rh.txt", "w");
                    writeData(employeesDB, file, nrOfEmployees);
                    fclose(file);
                }

                printf("\nSaindo...\n");
                sleep(2);
                free(employeesDB);
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                while ( getchar() != '\n' );
                break;
        }

    } while (choice != 7);

    return 0;
}
