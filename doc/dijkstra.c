/*
Dr. Estell’s C Programs and Examples, Ver. 5.0

Page 94  (PDF Page 94)

dijkstra’s algorithm - finding the shortest path
*/

/*
*
* dijkstra.c - John K. Estell - 19 June 1995
* Implementation of Dijkstra's Algorithm.
* Vertices are v[0] through v[n-1], with n being the number of verice present in the
* graph. Will find shortest path from v[0] to v[z]. n will be obtained dynamically.
* For sets, index refers to vertex, value of zero implies not a member of the set,
* non-zero implies membership. For edges, enter weight if connected, "Inf" if not
* connected.
*/

#include <stdio.h>

/* function prototypes */

int get_integer(char *prompt);
float get_float(char *prompt);
void *make_array(int number_of_elements, int data_size);
int get_number_of_vertices(void);
int get_terminal_vertex(int number_of_vertices);

/* main */

int main(void)
{
    float *length,          /* length of path from v[0] to v[i] */
          **weight;         /* weight of edge {v[i], v[j]} */
    float min_value,        /* used for finding minimal length */
          Inf = 1.0 / 0.0;  /* need to define infinity */
    int   *set;             /* distinguished set of vertices */
    int   *path;            /* used to mark minimal length path */
    int   *stack;           /* used to backtrack the path */
    int   n,                /* number of vertices */
          u,                /* minimal vertex */
          z,                /* terminal vertex */
          i, j;             /* generic counting variables */
    char outbuf[132];

    /* get input */

    n = get_number_of_vertices();
    z = get_terminal_vertex(n);

    /* allocate memory for one-dimensional arrays */

    length = (float *) make_array(n, sizeof(float));
    set = (int *) make_array(n, sizeof(int));
    path = (int *) make_array(n, sizeof(int));
    stack = (int *) make_array(n, sizeof(int));

    /* allocate memory for two-dimensional array */

    weight = (float **) make_array(n, sizeof(float *));
    for (i = 0; i < n; i++)
        weight[i] = (float *) make_array(n, sizeof(float));

    /* get the weights */
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            sprintf(outbuf, "Enter weight w(%d,%d): ", i, j);
            weight[i][j] = get_float(outbuf);
        }
    
    /*
     *  initialize Dijkstra's Algorithm variables:
     *      the distinguished set of vertices is initially empty.
     *      length[0] = 0, all other lengths are set to infinity.
     */

    for (i = 0; i < n; i++)
    {
        set[i] = 0;
        path[i] = -1;
    }

    length[0] = 0.0;
    for (i = 1; i < n; i++)
        length[i] = Inf;

    /* Perform the iteration */

    while ( set[z] == 0 )
    {
        /* find minimal vertex u not in the set */

        min_value = Inf;
        for (i = 0; i < n; i++)
            if (set[i] == 0)
                if (length[i] < min_value)
                {
                    min_value = length[i];
                    u = i;
                }

        /* add the minimal vertex to the set (and print out info for demo) */

        printf("min vertex: %d\n", u);
        printf("min length: %f\n", length[u]);

        set[u] = 1;

        /* update the length values and the path back pointer */

        for (i = 0; i < n; i++)
            if (set[i] == 0)
            {
                if (length[u] + weight[u][i] < length[i])
                {
                    length[i] = length[u] + weight[u][i];
                    path[i] = u;
                }
            }
    }

    /* print out shortest path and its length */

    i = 0;
    while (u != 0)
    {
        stack[i++] = u;
        u = path[u];
    }
    stack[i] = 0;
    printf("Shortest path is: ");
    while (i >= 0)
        printf("%d ", stack[i--]);
    printf("\n");

    printf("Length of shortest path is %f\n", length[z]);
    return 0; /* program complete - successful termination */
}

/*
 *  get_integer - will obtain an integer from the standard input.
 *                function will not return until an integer is obtained.
 */

int get_integer(char *prompt)
{
    int value, valid_input = 0;
    char inbuf[133];

    while (valid_input < 1)
    {
        printf(prompt);
        fgets(inbuf, sizeof(inbuf), stdin);
        valid_input = sscanf(inbuf, "%d", &value);
        if (valid_input == -1)
            printf("Please enter an integer value.\n");
        if (valid_input == 0)
            printf("That is not an integer - please enter an integer value.\n");
    }

    return value;
}

/*
 *  get_float - will obtain a floating point value from the standard input.
 *              function will not return until a floating point value
 *              is obtained.
 */

float get_float(char *prompt)
{
    int valid_input = 0;
    float value;
    char inbuf[133];

    while (valid_input < 1)
    {
        printf(prompt);
        fgets(inbuf, sizeof(inbuf), stdin);
        valid_input = sscanf(inbuf, "%f", &value);
        if (valid_input == -1)
            printf("Please enter a floating point value.\n");
        if (valid_input == 0)
        {
            printf("That is not a floating point value.\n");
            printf("Please enter a floating point value.\n");
            }
    }

    return value;
}

/*
 *  make_array - will allocate memory for specified number of cells of
 *               specified size. Returns address of allocation, or
 *               forces program termination on insufficient memory.
 */

void *make_array(int number_of_elements, int data_size)
{
    void *address;

    address = (void *) malloc(number_of_elements * data_size);
    if (address == NULL)
    {
        fprintf(stderr, "Insufficient memory for array allocation.\n");
        exit(1);
    }

    return address;
}

/*
 *  get_number_of_vertices - get the number of vertices to be used.
 */

int get_number_of_vertices(void)
{
    int number = -1;

    while (number < 2)
    {
        number = get_integer("Enter number of vertices: ");
        if (number < 2)
            printf("The number of vertices must be 2 or greater.\n");
    }

    return number;
}

/*
 *  get_terminal_vertex - get vertex number of the terminal vertex.
 */

int get_terminal_vertex(int number_of_vertices)
{
    int number = -1;

    while ((number < 1) || (number > number_of_vertices - 1))
    {
        number = get_integer("Enter terminal vertex: ");
        if ((number < 1) || (number > number_of_vertices - 1))
            printf("The value of the terminal vertex must be between 1 and %d.\n",
                    number_of_vertices - 1);
    }

    return number;
}
