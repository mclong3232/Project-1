#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This program prompts the user for two input values to compute the average of the two values.
 *
 * @author Michael Long and Kirby Gagne
 * @date 8/29/2026
 * @info Course COP4634
 */


/* test program with more than this many tokens for input */
#define MAXARGS 32

/* the largest string the shell can accept */
#define MAXIN 256

/* definitions to simulate a boolean type */
#define true 1
#define false 0

/* structure to hold input data */
struct PARAM
{
    char *inputRedirect;            /* file name or NULL */
    char *outputRedirect;           /* file name or NULL */
    int  argumentCount;             /* number of tokens in argument vector */
    char *argumentVector[MAXARGS];  /* array of strings */
};

/* a typedef so we dont need to use "struct PARAM" all the time */
typedef struct PARAM Param_t;

/* a typedef simulating a boolean type */
typedef int bool;

/**
 * Print the contents of the structure when the shell is started with the debug option –Debug.
 * @param param the parameters from the user input
 */
void printParams(Param_t * param)
{

    int i;
    printf ("InputRedirect: [%s]\n", (param->inputRedirect != NULL) ? param->inputRedirect:"NULL");
    printf ("OutputRedirect: [%s]\n", (param->outputRedirect != NULL) ? param->outputRedirect:"NULL");
    printf ("ArgumentCount: [%d]\n", param->argumentCount);
    for (i = 0; i < param->argumentCount; i++)
        printf("ArgumentVector[%2d]: [%s]\n", i, param->argumentVector[i]);
}

/**
 * Allocate memory for the struct PARAM
 * @return PARAM a pointer to the newly allocated memory for the struct PARAM
 */
Param_t *createPARAM()
{
    // Allocate memory for the struct; exit if malloc() fails
    Param_t *PARAM = (Param_t *) malloc (sizeof (struct PARAM));
    if (!PARAM)
    {
        fprintf (stderr, "> Ran out of memory! \n");
        exit (1);
    }
    
    // Initialize variables in the struct
    PARAM->inputRedirect = NULL;
    PARAM->outputRedirect = NULL;
    PARAM->argumentCount = 0;

    
    return PARAM;
}

/**
 * Add a token to the argumentVector
 */
void addToken(Param_t *PARAM, char *tok, int i)
{
    PARAM->argumentVector[i] = tok;
}

/**
 * Free memory allocated for the struct PARAM
 * @param PARAM a pointer to memory allocated for the struct PARAM
 *
 * @return a NULL pointer
 */
Param_t *deletePARAM(Param_t *PARAM)
{
    free(PARAM);
    return NULL;
}

/**
 * Search for a specific char in a char array
 * @param search the char array to be searched for
 * @param target the char array to be searched
 *
 * @return result a flag that is true (1) if found and false (0) if not found
 */
bool searchChar(char *search, char **target)
{
    bool result = false;
    
    char *c = *target;
    while (*c)
    {
        if (strchr(search, *c))
        {
            result = true;
            (*target)++;
            break;
        }
        
        c++;
    }
    
    return result;
}

/**
 * This function starts the program.
 */
int main(int argc, const char * argv[])
{

    char input[MAXIN];
    char demin[] = " \n\t";
    char *token;
    bool flag = 0;
    
    // Prompt the user for input
    fprintf(stdout, "$$$ ");
    fflush(stdout);
    fgets(input, MAXIN, stdin);
    
    // Continues to prompt user for input until "exit" command is entered
    while (strcmp(input, "exit\n") != 0)
    {
        // Get the first token and add it to the struct
        Param_t *PARAM = createPARAM();
        int i = 0;
        token = strtok(input, demin);
        addToken(PARAM, token, i);
        PARAM->argumentCount++;
        
        // Get the rest of the tokens and add them to the struct
        while (token)
        {
            i++;
            token = strtok(NULL, demin);
            if (token)
            {
                // Test where the token will be added in the struct PARAM
                if      (searchChar("<", &token))        PARAM->inputRedirect = token;
                else if (searchChar(">", &token))        PARAM->outputRedirect = token;
                else if (strcmp(token, "-Debug") == 0)   flag = true;
                else
                {
                    flag = false;
                    addToken(PARAM, token, i);
                    PARAM->argumentCount++;
                }
            }
        }
        if (flag == true) printParams(PARAM);
        flag = false;
        fprintf(stdout, "$$$ ");
        fflush(stdout);
        deletePARAM(PARAM);
        fgets(input, MAXIN, stdin);
    }
    return 0;
}
